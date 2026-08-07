# PlcBinderLib

适用于cpp与c#双语言,qt与winform双框架
更简便地控制PLC。一行代码绑定UI控件与PLC地址，点击按钮自动读写，不用手写通信逻辑。

## 能干什么

- 原生Socket实现ModbusTCP通信，不依赖libmodbus等第三方库
- 一行代码把Button/TextBox绑定到PLC的M/D地址，点击自动执行读写
- 支持汇川PLC的M区线圈(01/05)、D区整数(03/06)、D区浮点(03/10)
- 浮点读写自动处理汇川低字在前的字节序
- 通信层和绑定层分离，拿来就能用，改源码也方便

## 项目结构

```
c++版本与源代码
src/cpp/QtPlcBinderLib/          # C++ 静态库
├── InovanceModbusTcp.h/.cpp     # ModbusTCP通信-Winsock实现
├── qtBinde.h/.cpp               # Qt UI绑定层
src/cpp/libTest/                 # Qt测试程序
├── libTest.h/.cpp               # 用qtBinde一行绑定按钮和PLC地址

c#版本与源代码
src/cSharp/                      # C# WinForm版
├── InovanceModbusTcp.cs         # ModbusTCP通信-TcpClient实现
├── ControlBinder.cs             # WinForm UI绑定层
├── Form1.cs                     # 测试程序
```

## 用法

### C++ / Qt

```cpp
InovanceModbusTcp plc;
plc.init("127.0.0.1", 502, 1);

auto binder = new qtBinde(&plc, this);
binder->toggleM(ui.m100, 100);              //点按钮切换M100
binder->writeD(ui.wd100, ui.txtD100, 100);  //点按钮从输入框写D100
binder->readDF(ui.rd200, ui.txtDF200, 200); //点按钮读D200浮点显示
```

### C# / WinForm

```csharp
var plc = new InovanceModbusTcp();
plc.Init("127.0.0.1", 502, 1);

var binder = new ControlBinder(plc);
binder.ToggleM(btnM100, 100);              //点按钮切换M100
binder.WriteD(btnWd, txtD100, 100);        //点按钮从输入框写D100
binder.ReadDF(btnRd, txtDF200, 200);       //点按钮读D200浮点显示
```

## 绑定方法一览

| 方法 | 功能 | 功能码 |
|------|------|--------|
| ToggleM | 点击切换M线圈(读当前值取反写回) | 01读/05写 |
| ReadM | 点击读M显示ON/OFF | 01 |
| WriteD | 点击从输入框写D整数 | 06 |
| ReadD | 点击读D整数显示 | 03 |
| WriteDF | 点击从输入框写D浮点 | 10 |
| ReadDF | 点击读D浮点显示 | 03 |

## 通信层API

```cpp
// C++
plc.readM(100);          //读M100
plc.writeM(100, true);   //写M100
plc.readD(100);          //读D100
plc.writeD(100, 666);    //写D100
plc.readDFloat(200);     //读D200浮点
plc.writeDFloat(200, 3.14f); //写D200浮点
```

```csharp
// C#
plc.ReadM(100);
plc.WriteM(100, true);
plc.ReadD(100);
plc.WriteD(100, 666);
plc.ReadDFloat(200);
plc.WriteDFloat(200, 3.14f);
```

## 构建

C++版需要Visual Studio 2022 + Qt 5.14.2，打开`QtPlcBinderLib.sln`编译。

C#版需要.NET Framework 4.8，打开`CSharp/InovancePlcTest.sln`或`dotnet build`。

## 支持的PLC

汇川H3U/H5U，通过ModbusTCP协议通信。其他支持ModbusTCP的PLC改从站号和地址映射也能用。

## License

MIT
