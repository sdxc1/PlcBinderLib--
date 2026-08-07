using System;
using System.Threading.Tasks;
using System.Windows.Forms;
using InovancePlcLib;

namespace InovancePlcTest
{
    public partial class Form1 : Form
    {
        private InovanceModbusTcp plc;
        private ControlBinder binder;

        public Form1()
        {
            InitializeComponent();
            btnConnect.Click += btnConnect_Click;
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private async void btnConnect_Click(object sender, EventArgs e)
        {
            //已连接就断开
            if (plc != null && plc.IsConnected)
            {
                plc.Disconnect();
                btnConnect.Text = "连接";
                lblStatus.Text = "已断开";
                return;
            }

            //连接
            if (plc == null) plc = new InovanceModbusTcp();
            int port = int.TryParse(txtPort.Text, out var p) ? p : 502;

            btnConnect.Enabled = false;
            lblStatus.Text = "连接中";

            bool ok = await Task.Run(() => plc.Init(txtIp.Text, port, 1));
            btnConnect.Enabled = true;

            if (!ok) { lblStatus.Text = "连接失败"; return; }


            //首次连接才绑定-后续重连不用重复绑
            if (binder == null)
            {
                binder = new ControlBinder(plc);
                binder.ToggleM(btnToggleM, 100);
                binder.ReadM(btnReadM, txtMResult, 100);
                binder.WriteD(btnWriteD, txtDInput, 100);
                binder.ReadD(btnReadD, txtDResult, 100);
                binder.WriteDF(btnWriteDF, txtDFInput, 200);
                binder.ReadDF(btnReadDF, txtDFResult, 200);
            }

            btnConnect.Text = "断开";
            lblStatus.Text = "已连接";
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            plc?.Disconnect();
            base.OnFormClosing(e);
        }

        
    }
}
