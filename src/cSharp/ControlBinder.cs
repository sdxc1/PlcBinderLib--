using System;
using System.Windows.Forms;

namespace InovancePlcLib
{
    //WinForm控件绑定器-一行代码绑定Button+TextBox+PLC地址
    //用法:
    //  var plc = new InovanceModbusTcp();
    //  plc.Init("127.0.0.1", 502, 1);
    //  var binder = new ControlBinder(plc);
    //  binder.ToggleM(btnM100, 100);          //点btnM100切换M100
    //  binder.ReadD(btnRd, txtD100, 100);      //点btnRd读D100显示
    //  binder.WriteD(btnWd, txtD100, 100);     //点btnWd从txtD100写D100
    public class ControlBinder
    {
        private readonly InovanceModbusTcp plc;

        public ControlBinder(InovanceModbusTcp plc)
        {
            this.plc = plc;
        }

        //=====M区(线圈) 功能码01读/05写
        //点击写M固定值
        public void WriteM(Button btn, int addr, bool value)
        {
            btn.Click += (s, e) => plc.WriteM(addr, value);
        }

        //点击切换M-读当前值取反写回
        public void ToggleM(Button btn, int addr)
        {
            btn.Click += (s, e) => plc.WriteM(addr, !plc.ReadM(addr));
        }

        //点击读M显示ON/OFF
        public void ReadM(Button btn, TextBox txt, int addr)
        {
            btn.Click += (s, e) => txt.Text = plc.ReadM(addr) ? "ON" : "OFF";
        }

        //=====D区(int16) 功能码03读/06写
        //点击从输入框读数值写入D
        public void WriteD(Button btn, TextBox txt, int addr)
        {
            btn.Click += (s, e) =>
            {
                if (int.TryParse(txt.Text, out int val))
                    plc.WriteD(addr, (ushort)val);
            };
        }

        //点击读D显示到输入框
        public void ReadD(Button btn, TextBox txt, int addr)
        {
            btn.Click += (s, e) => txt.Text = plc.ReadD(addr).ToString();
        }

        //=====D区(float) 功能码03读/10写
        //点击从输入框读浮点写入D
        public void WriteDF(Button btn, TextBox txt, int addr)
        {
            btn.Click += (s, e) =>
            {
                if (float.TryParse(txt.Text, out float val))
                    plc.WriteDFloat(addr, val);
            };
        }

        //点击读D浮点显示到输入框
        public void ReadDF(Button btn, TextBox txt, int addr)
        {
            btn.Click += (s, e) => txt.Text = plc.ReadDFloat(addr).ToString();
        }
    }
}
