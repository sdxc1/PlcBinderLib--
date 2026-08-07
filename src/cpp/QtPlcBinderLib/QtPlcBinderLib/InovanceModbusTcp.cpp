#include "pch.h"

#include "InovanceModbusTcp.h"
#include <cstring>
#include <chrono>

namespace
{
    //wsa初始化
    bool wsaLoaded = false;

    //初始化Windows的socketAPI接口
    void initWsa()
    {
        if (wsaLoaded)
            return;
        WSADATA wsaData;
        //加载2.2版本
        if(WSAStartup(MAKEWORD(2, 2), &wsaData))return;
        wsaLoaded = true;
    }

    //关闭连接
    void CloseSocket(SOCKET sock)
    {
        if (sock != InvalidSocket)
        {
            ::closesocket(sock);
        }
    }

    //=====报文编码作用与处理=====
    //Modbus协议规定-16位数字高位字节先发,低位后发
    //假设发数字5,它完整的二进制-00000000-00000101(位权-左高右低)
    //高八位处理:(static_cast<uint8_t>(val >> 8))
    //根据要求先发高字节也就是前八位,(val >> 8),就是右移八位,
    //以数字5为例就变成了-00000000-00000000,而此时转成 uint8_t单字节,才能存入报文容器,强制类型转换时自动截断高位
    //uint8_t为无符号单字节类型,只能存储8位,上面移动完也就变成了-00000000-00000000,然后只取后面部分存储,写入报文
    //低八位处理:(static_cast<uint8_t>(val))
    //根据数据存储从低位开始存储,这时候刚好存储低八位-因为uint16_t直接强转uint8_t会自动丢弃高8bit
    void messageEncodingProcessing(std::vector<uint8_t>& message, uint16_t val)
    {
        message.push_back(static_cast<uint8_t>(val >> 8));//高八位处理-先发
        message.push_back(static_cast<uint8_t>(val));//低八位处理-后发
    }
    //函数作用:
    //假设我要发43981-二进制为-10101011-11001101
    //我该方法可以转成[171,205]-先发高位171后发低位205
}

InovanceModbusTcp::InovanceModbusTcp()
{
    initWsa();
}

InovanceModbusTcp::~InovanceModbusTcp()
{
    autoReconnectEnable = false;
    threadRun = false;

    if (reconnectWorker.joinable())
        reconnectWorker.join();

    disconnect();
}
//=====通信连接部分
int InovanceModbusTcp::init(const std::string& ip, int port, int slaveId)
{
    if (!connect(ip, port, slaveId)) return 1;//弹出1是连接失败
    return 0;//弹出0是成功
}

bool InovanceModbusTcp::connect(const std::string& ip, int ports, int slaveId)
{
    if (m_bConnected) return true;

    disconnect();
    initWsa();

    //设置通讯道
    m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == InvalidSocket)
    {
        m_bConnected = false;
        return false;
    }

    //设置500ms超时
    DWORD recvTimeout = 500;
    setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&recvTimeout, sizeof(recvTimeout));

    //设置协议与端口号
    sockaddr_in target{};
    target.sin_family = AF_INET;
    target.sin_port = htons(static_cast<uint16_t>(ports));

    //校验ip
    if (inet_pton(AF_INET, ip.c_str(), &target.sin_addr) <= 0)
    {
        CloseSocket(m_socket);
        m_socket = InvalidSocket;
        m_bConnected = false;
        return false;
    }

    //握手通讯测试-关联后台线程
    if (::connect(m_socket, (sockaddr*)&target, sizeof(target)) < 0)
    {
        CloseSocket(m_socket);
        m_socket = InvalidSocket;
        m_bConnected = false;
        return false;
    }

    //流程应当先设置通信通道与超时配置和协议与端口号然后校准ip后测试连接
    ipAddr = ip;
    port = ports;
    slaveStation = slaveId;
    m_bConnected = true;
    return true;
}

void InovanceModbusTcp::disconnect()
{
    std::lock_guard<std::mutex> lock(m_commMtx);
    CloseSocket(m_socket);
    m_socket = InvalidSocket;
    m_bConnected = false;
}

void InovanceModbusTcp::reconnectLoop()
{
    while (autoReconnectEnable)
    {
        if (!m_bConnected)
        {
            connect(ipAddr, port, slaveStation);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void InovanceModbusTcp::startReconnectThread()
{
    autoReconnectEnable = true;
    reconnectWorker = std::thread(&InovanceModbusTcp::reconnectLoop, this);
}

//=====数据处理部分
//由于TCP是流式缓冲区协议,无法一次性发完所有数据
//假设现在有2000个字节,看看第一个能发多少.假设发了1600,第二次我们对输入的数据,从第1601开始发送直到发完2000个字节
bool InovanceModbusTcp::sendFull(const uint8_t* data, int dataLen)
{
    int sendOffset = 0;
    while (sendOffset < dataLen)
    {
        int ret = send(m_socket, (const char*)(data + sendOffset), dataLen - sendOffset, 0);
        //断开连接或网络中断返回
        if (ret <= 0) return false;
        //跳转最新字节地址
        sendOffset += ret;
    }
    return true;
}
//收同理
bool InovanceModbusTcp::recvFull(uint8_t* dstBuf, int readLen)
{
    int recvOffset = 0;
    while (recvOffset < readLen)
    {
        int ret = recv(m_socket, (char*)(dstBuf + recvOffset), readLen - recvOffset, 0);
        if (ret <= 0) return false;
        recvOffset += ret;
    }
    return true;
}

//ModbusTCP的事务id-请求plc会带着TID返回
uint16_t InovanceModbusTcp::getNextTid()
{
    return ++transactionId;
}

//建立请求方法
std::vector<uint8_t> InovanceModbusTcp::buildRequest(uint8_t funcCode, uint16_t addr, uint16_t count)
{
    //存储空间
    std::vector<uint8_t> frame;
    frame.reserve(12);

    //标准报文协议:TID(2)+协议ID(2)+长度(2)+从站(1)+功能码(1)+ 地址(2)+数量(2)-遵循先高后低的发送顺序
    //TID
    messageEncodingProcessing(frame, getNextTid());
    //协议ID-默认0
    messageEncodingProcessing(frame, 0);
    //设置长度
    messageEncodingProcessing(frame, 6);
    //设置从站-单字节不用处理
    frame.push_back(static_cast<uint8_t>(slaveStation));
    //设置功能码
    frame.push_back(funcCode);
    //设置地址-同上要求利用messageEncodingProcessing
    messageEncodingProcessing(frame, addr);
    //设置数量
    messageEncodingProcessing(frame, count);
    
    //返回标准报文
    return frame;
}

//先发-再收-客户端主动发指令,从站收到后才回复数据-重点:TCP是连续字节流你不读它就一直留在里面堆积
std::vector<uint8_t> InovanceModbusTcp::transact(const std::vector<uint8_t>& reqFrame)
{
    std::lock_guard<std::mutex> lock(m_commMtx);

    //无效返回
    if (m_socket == InvalidSocket)
        return {};

    //发送数据
    if (!sendFull(reqFrame.data(), static_cast<int>(reqFrame.size())))
    {
        m_bConnected = false;
        return {};
    }

    //存储MBAP数组
    uint8_t mbapHeader[7] = { 0 };
    //收前七字节存储MBAP
    if (!recvFull(mbapHeader, 7))
    {
        m_bConnected = false;
        return {};
    }

    //PDU内容接收应答解析长度-用来知道后面要加什么
    //根据TID(2)+协议ID(2)+长度(2)+从站(1)+功能码(1)+ 数据地址(2)+数量(2)
    //取第四个字节左移八位
    //首先我们找到长度的字节,因为长度是两个字节组成的,我们先获取高位字节,但这个数据类型是一个字节,于是我们左移八位,让他变成了,两个字节,他占据了高位
    //相当于10101111变成了10101111-00000000然后在通过或的作用,把第五位的假设11110000 跟前面或对比,就变成了10101111-11110000
    uint16_t pduLen = (static_cast<uint16_t>(mbapHeader[4]) << 8) | mbapHeader[5];
    //MBAP+PDU组成真正长度的报文
    int totalRespLen = 7 + pduLen;

    //创建符合长度的动态数组
    std::vector<uint8_t> response;
    //设置数组长度
    response.reserve(totalRespLen);
    //把功能地址数量插入到最后面
    response.insert(response.end(), mbapHeader, mbapHeader + 7);

    //收数据-为什么这里还能收取-因为TCP是连续字节流你不读它就一直留在里面堆积
    std::vector<uint8_t> pduBody(pduLen - 1);
    if (pduLen > 1 && !recvFull(pduBody.data(), pduLen - 1))
    {
        m_bConnected = false;
        return {};
    }
    response.insert(response.end(), pduBody.begin(), pduBody.end());

    //功能码最高位置1 = Modbus异常响应
    if (response.size() >= 9 && (response[7] & 0x80))
    {
        m_bConnected = false;
        return {};
    }

    return response;

}

//=====PLC方法操作
//建议双字节接收原始数据,外部直接用int强转换就可以
uint16_t InovanceModbusTcp::readD(int addr)
{
    if (!m_bConnected)
        return 0;
    //功能码0x03为读取保持寄存器-读取D操作
    auto resp = transact(buildRequest(0x03, static_cast<uint16_t>(addr), 1));
    //报文异常直接弹出-MBAP(7)+UID(1)+FC(1)+byteCount(1)+data(2)
    if (resp.size() < 11)
    {
        m_bConnected = false;
        return 0;
    }

    //数据在resp[9]高位和resp[10]低位
    return (static_cast<uint16_t>(resp[9]) << 8) | resp[10];
}

bool InovanceModbusTcp::writeD(int addr, uint16_t value)
{
    if (!m_bConnected)
        return false;
    //建立报文
    std::vector<uint8_t> frame;
    frame.reserve(12);
    messageEncodingProcessing(frame, getNextTid());
    messageEncodingProcessing(frame, 0x0000);
    messageEncodingProcessing(frame, 6);
    frame.push_back(static_cast<uint8_t>(slaveStation));
    //0x06写读取保持寄存器-写D操作
    frame.push_back(0x06);
    messageEncodingProcessing(frame, static_cast<uint16_t>(addr));
    messageEncodingProcessing(frame, value);
    //校验报文
    return transact(frame).size() >= 12;
}

float InovanceModbusTcp::readDFloat(int addr)
{
    if (!m_bConnected)
        return 0.0f;
    //0x03读两个D后拼接成实数
    auto resp = transact(buildRequest(0x03, static_cast<uint16_t>(addr), 2));
    //MBAP(7)+UID(1)+FC(1)+byteCount(1)+data(4)
    if (resp.size() < 13)
    {
        m_bConnected = false;
        return 0.0f;
    }

    //resp[9]是byteCount(=4),数据在resp[9..12]
    //汇川实数高低置换原则
    uint8_t buf[4];
    buf[0] = resp[10];
    buf[1] = resp[9];
    buf[2] = resp[12];
    buf[3] = resp[11];

    float val;
    //copy数据-从数据地址开始拷贝四个byte
    memcpy(&val, buf, 4);
    return val;
}

bool InovanceModbusTcp::writeDFloat(int addr, float value)
{
    if (!m_bConnected)
        return false;

    std::vector<uint8_t> frame;
    frame.reserve(15);
    messageEncodingProcessing(frame, getNextTid());
    messageEncodingProcessing(frame, 0x0000);
    messageEncodingProcessing(frame, 11);
    frame.push_back(static_cast<uint8_t>(slaveStation));
    //0x10批量写入多个保持寄存器,也就是写实数D
    frame.push_back(0x10);
    messageEncodingProcessing(frame, static_cast<uint16_t>(addr));
    messageEncodingProcessing(frame, 2);
    frame.push_back(4);

    uint8_t floatBuf[4];
    memcpy(floatBuf, &value, 4);
    frame.insert(frame.end(), floatBuf, floatBuf + 4);
    
    //发送报文是否正确
    return transact(frame).size() >= 12;
}

bool InovanceModbusTcp::readM(int addr)
{
    if (!m_bConnected)
        return false;
    //读操作一定要记住先发再读操作-0x01读线圈-读M操作
    auto resp = transact(buildRequest(0x01, static_cast<uint16_t>(addr), 1));
    if (resp.size() < 10)
    {
        m_bConnected = false;
        return false;
    }

    return resp[9] != 0;
}

bool InovanceModbusTcp::writeM(int addr, uint16_t value)
{
    if (!m_bConnected)
        return false;
    //同理
    std::vector<uint8_t> frame;
    frame.reserve(12);
    messageEncodingProcessing(frame, getNextTid());
    messageEncodingProcessing(frame, 0x0000);
    messageEncodingProcessing(frame, 6);
    frame.push_back(static_cast<uint8_t>(slaveStation));
    //0x05强制线圈-写入线圈-写M操作
    frame.push_back(0x05);
    messageEncodingProcessing(frame, static_cast<uint16_t>(addr));
    messageEncodingProcessing(frame, value ? 0xFF00 : 0x0000);

    return transact(frame).size() >= 12;
}
