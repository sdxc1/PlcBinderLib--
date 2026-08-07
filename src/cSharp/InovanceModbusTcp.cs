using System;
using System.Net.Sockets;
using System.Threading;

namespace InovancePlcLib
{
    //汇川PLC-ModbusTCP通信类-原生TcpClient实现
    public class InovanceModbusTcp
    {
        private TcpClient tcpClient;
        private NetworkStream stream;
        private readonly object lockObj = new object();

        private string ipAddr;
        private int port = 502;
        private int slaveStation = 1;
        private ushort transactionId = 0;

        private volatile bool isConnected = false;
        private volatile bool autoReconnect = false;
        private Thread reconnectThread;

        public bool IsConnected => isConnected;

        //=====连接部分
        public bool Init(string ip, int port, int slaveId)
        {
            //允许初始化开启
            //StartReconnectThread();
            return Connect(ip, port, slaveId);
        }

        private bool Connect(string ip, int port, int slaveId)
        {
            if (isConnected) return true;
            Disconnect();

            try
            {
                tcpClient = new TcpClient();
                //连接超时3秒-PLC不响应时不卡死
                var ar = tcpClient.BeginConnect(ip, port, null, null);
                if (!ar.AsyncWaitHandle.WaitOne(3000))
                {
                    tcpClient.Close();
                    isConnected = false;
                    return false;
                }
                tcpClient.EndConnect(ar);

                tcpClient.SendTimeout = 500;
                tcpClient.ReceiveTimeout = 500;
                stream = tcpClient.GetStream();

                ipAddr = ip;
                this.port = port;
                slaveStation = slaveId;
                isConnected = true;
                return true;
            }
            catch
            {
                isConnected = false;
                return false;
            }
        }

        //关闭底层连接-不获取锁-锁内锁外都能调
        private void CloseConnection()
        {
            isConnected = false;
            try { stream?.Close(); } catch { }
            stream = null;
            try { tcpClient?.Close(); } catch { }
            tcpClient = null;
        }

        //断开连接
        public void Disconnect()
        {
            lock (lockObj)
            {
                stream?.Close();
                stream = null;
                tcpClient?.Close();
                tcpClient = null;
                isConnected = false;
            }
        }

        //启动自动重连线程
        public void StartReconnectThread()
        {
            autoReconnect = true;
            reconnectThread = new Thread(ReconnectLoop)
            {
                IsBackground = true
            };
            reconnectThread.Start();
        }

        private void ReconnectLoop()
        {
            while (autoReconnect)
            {
                if (!isConnected)
                    Connect(ipAddr, port, slaveStation);
                Thread.Sleep(1000);
            }
        }

        //=====报文构建
        //Modbus大端写入-高字节在前
        private static void WriteU16(byte[] buf, int offset, ushort val)
        {
            buf[offset] = (byte)(val >> 8);
            buf[offset + 1] = (byte)val;
        }

        private ushort GetNextTid()
        {
            return ++transactionId;
        }

        //标准请求帧:-TID(2)+协议ID(2)+长度(2)+从站(1)+功能码(1)+地址(2)+数量(2)
        private byte[] BuildRequest(byte funcCode, ushort addr, ushort count)
        {
            byte[] frame = new byte[12];
            WriteU16(frame, 0, GetNextTid());
            WriteU16(frame, 2, 0);
            WriteU16(frame, 4, 6);
            frame[6] = (byte)slaveStation;
            frame[7] = funcCode;
            WriteU16(frame, 8, addr);
            WriteU16(frame, 10, count);
            return frame;
        }

        //=====收发处理
        //TCP流式发送-阻塞写完-异常即失败
        private bool SendFull(byte[] data, int offset, int length)
        {
            try
            {
                stream.Write(data, offset, length);
                return true;
            }
            catch
            {
                return false;
            }
        }

        //TCP流式接收-循环读完
        private bool RecvFull(byte[] buffer, int offset, int length)
        {
            int received = 0;
            while (received < length)
            {
                int n = stream.Read(buffer, offset + received, length - received);
                if (n <= 0) return false;
                received += n;
            }
            return true;
        }

        //收发事务-发请求 收MBAP头 收PDU体 返回完整响应
        private byte[] Transact(byte[] request)
        {
            lock (lockObj)
            {
                if (stream == null || !isConnected)
                    return null;

                if (!SendFull(request, 0, request.Length))
                {
                    CloseConnection();
                    return null;
                }

                //收MBAP头7字节
                byte[] mbapHeader = new byte[7];
                if (!RecvFull(mbapHeader, 0, 7))
                {
                    CloseConnection();
                    return null;
                }

                //解析PDU长度 Length字段含UID 减1得PDU体
                ushort pduLen = (ushort)((mbapHeader[4] << 8) | mbapHeader[5]);
                int bodyLen = pduLen - 1;
                if (bodyLen < 0) bodyLen = 0;

                byte[] response = new byte[7 + bodyLen];
                Array.Copy(mbapHeader, response, 7);

                if (bodyLen > 0 && !RecvFull(response, 7, bodyLen))
                {
                    CloseConnection();
                    return null;
                }

                //异常响应 功能码最高位置1
                if (response.Length >= 9 && (response[7] & 0x80) != 0)
                {
                    CloseConnection();
                    return null;
                }

                return response;
            }
        }

        //=====M区(线圈) 功能码01读/05写
        public bool ReadM(int addr)
        {
            if (!isConnected) return false;
            var resp = Transact(BuildRequest(0x01, (ushort)addr, 1));
            //MBAP(7)+UID(1)+FC(1)+byteCount(1)+data(1)
            if (resp == null || resp.Length < 10)
            {
                CloseConnection();
                return false;
            }
            return resp[9] != 0;
        }

        public bool WriteM(int addr, bool value)
        {
            if (!isConnected) return false;
            byte[] frame = new byte[12];
            WriteU16(frame, 0, GetNextTid());
            WriteU16(frame, 2, 0);
            WriteU16(frame, 4, 6);
            frame[6] = (byte)slaveStation;
            frame[7] = 0x05; //写单线圈
            WriteU16(frame, 8, (ushort)addr);
            WriteU16(frame, 10, value ? (ushort)0xFF00 : (ushort)0x0000);
            return Transact(frame)?.Length >= 12;
        }

        //=====D区(int16) 功能码03读/06写
        public ushort ReadD(int addr)
        {
            if (!isConnected) return 0;
            var resp = Transact(BuildRequest(0x03, (ushort)addr, 1));
            //MBAP(7)+UID(1)+FC(1)+byteCount(1)+data(2) = 11字节
            if (resp == null || resp.Length < 11)
            {
                CloseConnection();
                return 0;
            }
            return (ushort)((resp[9] << 8) | resp[10]);
        }

        public bool WriteD(int addr, ushort value)
        {
            if (!isConnected) return false;
            byte[] frame = new byte[12];
            WriteU16(frame, 0, GetNextTid());
            WriteU16(frame, 2, 0);
            WriteU16(frame, 4, 6);
            frame[6] = (byte)slaveStation;
            frame[7] = 0x06; //写单寄存器
            WriteU16(frame, 8, (ushort)addr);
            WriteU16(frame, 10, value);
            return Transact(frame)?.Length >= 12;
        }

        //=====D区(float) 功能码03读/10写
        //汇川实数低字在前
        public float ReadDFloat(int addr)
        {
            if (!isConnected) return 0f;
            var resp = Transact(BuildRequest(0x03, (ushort)addr, 2));
            //MBAP(7)+UID(1)+FC(1)+byteCount(1)+data(4) = 13字节
            if (resp == null || resp.Length < 13)
            {
                CloseConnection();
                return 0f;
            }
            //低字在前 拼小端后BitConverter解析
            byte[] buf = { resp[10], resp[9], resp[12], resp[11] };
            return BitConverter.ToSingle(buf, 0);
        }

        public bool WriteDFloat(int addr, float value)
        {
            if (!isConnected) return false;
            //小端内存 字内字节交换 汇川低字在前
            byte[] fb = BitConverter.GetBytes(value);
            byte[] data = { fb[1], fb[0], fb[3], fb[2] };

            byte[] frame = new byte[17];
            WriteU16(frame, 0, GetNextTid());
            WriteU16(frame, 2, 0);
            WriteU16(frame, 4, 11);
            frame[6] = (byte)slaveStation;
            frame[7] = 0x10; //写多寄存器
            WriteU16(frame, 8, (ushort)addr);
            WriteU16(frame, 10, 2);
            frame[12] = 4;
            Array.Copy(data, 0, frame, 13, 4);

            return Transact(frame)?.Length >= 12;
        }
    }
}
