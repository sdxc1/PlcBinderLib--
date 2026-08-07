#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <atomic>
#include <thread>
#include <cstdint>

//Windows Socket 依赖
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

//无效标志
constexpr SOCKET InvalidSocket = INVALID_SOCKET;

//汇川PLCModbusTCP通信类
//适用于汇川系列PLC直接操控读写操作
//采用原生socket

class InovanceModbusTcp
{
public:
    InovanceModbusTcp();
    ~InovanceModbusTcp();

    //===禁用拷贝构造与赋值
    InovanceModbusTcp(const InovanceModbusTcp&) = delete;
    InovanceModbusTcp& operator=(const InovanceModbusTcp&) = delete;

    //===D保持寄存器读写
    uint16_t readD(int addr);
    bool writeD(int addr, uint16_t value);
    float readDFloat(int addr);
    bool writeDFloat(int addr, float value);

    //===M线圈读写
    bool readM(int addr);
    bool writeM(int addr, uint16_t value);

    //===初始化与线程连接
    //初始化+连接
    int init(const std::string& ip, int port, int slaveId);
    //断连
    void disconnect();
    //开启重连线程
    void startReconnectThread();

    //在线标志
    bool isConnected() const { return m_bConnected; }

private:
    //请求报文
    std::vector<uint8_t> buildRequest(uint8_t funcCode, uint16_t addr, uint16_t count);
    //收发处理
    std::vector<uint8_t> transact(const std::vector<uint8_t>& reqFrame);
    //获取事务ID
    uint16_t getNextTid();

    //自动重连线程循环
    void reconnectLoop();
    //连接操作
    bool connect(const std::string& ip, int port, int slaveId);

    //发送
    bool sendFull(const uint8_t* data, int dataLen);
    //接收
    bool recvFull(uint8_t* dstBuf, int readLen);

    //Socket句柄
    SOCKET m_socket = InvalidSocket;

    //通信锁
    std::mutex m_commMtx;
    uint16_t transactionId = 0;

    //PLC连接参数
    std::string ipAddr;
    int port = 502;
    int slaveStation = 1;

    //状态标记
    std::atomic<bool> m_bConnected{ false };
    std::atomic<bool> autoReconnectEnable{ false };
    std::atomic<bool> threadRun{ true };
    std::thread reconnectWorker;
};