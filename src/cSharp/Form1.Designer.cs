using System.Drawing;
using System.Windows.Forms;

namespace InovancePlcTest
{
    partial class Form1
    {
        private System.ComponentModel.IContainer components = null;

        // 连接区
        private Label lblIp;
        private TextBox txtIp;
        private Label lblPort;
        private TextBox txtPort;
        private Button btnConnect;

        // M区
        private Label lblMSep;
        private Button btnToggleM;
        private Button btnReadM;
        private TextBox txtMResult;

        // D区 int16
        private Label lblDSep;
        private Label lblDInput;
        private TextBox txtDInput;
        private Button btnWriteD;
        private Button btnReadD;
        private TextBox txtDResult;

        // D区 float
        private Label lblDFSep;
        private Label lblDFInput;
        private TextBox txtDFInput;
        private Button btnWriteDF;
        private Button btnReadDF;
        private TextBox txtDFResult;

        // 状态
        private Label lblStatus;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {
            this.lblIp = new System.Windows.Forms.Label();
            this.txtIp = new System.Windows.Forms.TextBox();
            this.lblPort = new System.Windows.Forms.Label();
            this.txtPort = new System.Windows.Forms.TextBox();
            this.btnConnect = new System.Windows.Forms.Button();
            this.lblMSep = new System.Windows.Forms.Label();
            this.btnToggleM = new System.Windows.Forms.Button();
            this.btnReadM = new System.Windows.Forms.Button();
            this.txtMResult = new System.Windows.Forms.TextBox();
            this.lblDSep = new System.Windows.Forms.Label();
            this.lblDInput = new System.Windows.Forms.Label();
            this.txtDInput = new System.Windows.Forms.TextBox();
            this.btnWriteD = new System.Windows.Forms.Button();
            this.btnReadD = new System.Windows.Forms.Button();
            this.txtDResult = new System.Windows.Forms.TextBox();
            this.lblDFSep = new System.Windows.Forms.Label();
            this.lblDFInput = new System.Windows.Forms.Label();
            this.txtDFInput = new System.Windows.Forms.TextBox();
            this.btnWriteDF = new System.Windows.Forms.Button();
            this.btnReadDF = new System.Windows.Forms.Button();
            this.txtDFResult = new System.Windows.Forms.TextBox();
            this.lblStatus = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // lblIp
            // 
            this.lblIp.Location = new System.Drawing.Point(15, 18);
            this.lblIp.Name = "lblIp";
            this.lblIp.Size = new System.Drawing.Size(25, 15);
            this.lblIp.TabIndex = 0;
            this.lblIp.Text = "IP:";
            // 
            // txtIp
            // 
            this.txtIp.Location = new System.Drawing.Point(40, 15);
            this.txtIp.Name = "txtIp";
            this.txtIp.Size = new System.Drawing.Size(120, 21);
            this.txtIp.TabIndex = 1;
            this.txtIp.Text = "127.0.0.1";
            // 
            // lblPort
            // 
            this.lblPort.Location = new System.Drawing.Point(170, 18);
            this.lblPort.Name = "lblPort";
            this.lblPort.Size = new System.Drawing.Size(35, 15);
            this.lblPort.TabIndex = 2;
            this.lblPort.Text = "Port:";
            // 
            // txtPort
            // 
            this.txtPort.Location = new System.Drawing.Point(205, 15);
            this.txtPort.Name = "txtPort";
            this.txtPort.Size = new System.Drawing.Size(45, 21);
            this.txtPort.TabIndex = 3;
            this.txtPort.Text = "502";
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(260, 13);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(75, 25);
            this.btnConnect.TabIndex = 4;
            this.btnConnect.Text = "连接";
            // 
            // lblMSep
            // 
            this.lblMSep.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold);
            this.lblMSep.Location = new System.Drawing.Point(15, 55);
            this.lblMSep.Name = "lblMSep";
            this.lblMSep.Size = new System.Drawing.Size(100, 15);
            this.lblMSep.TabIndex = 5;
            this.lblMSep.Text = "M区(线圈)";
            // 
            // btnToggleM
            // 
            this.btnToggleM.Location = new System.Drawing.Point(15, 80);
            this.btnToggleM.Name = "btnToggleM";
            this.btnToggleM.Size = new System.Drawing.Size(100, 28);
            this.btnToggleM.TabIndex = 6;
            this.btnToggleM.Text = "Toggle M100";
            // 
            // btnReadM
            // 
            this.btnReadM.Location = new System.Drawing.Point(125, 80);
            this.btnReadM.Name = "btnReadM";
            this.btnReadM.Size = new System.Drawing.Size(100, 28);
            this.btnReadM.TabIndex = 7;
            this.btnReadM.Text = "Read M100";
            // 
            // txtMResult
            // 
            this.txtMResult.Location = new System.Drawing.Point(235, 83);
            this.txtMResult.Name = "txtMResult";
            this.txtMResult.ReadOnly = true;
            this.txtMResult.Size = new System.Drawing.Size(150, 21);
            this.txtMResult.TabIndex = 8;
            // 
            // lblDSep
            // 
            this.lblDSep.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold);
            this.lblDSep.Location = new System.Drawing.Point(15, 120);
            this.lblDSep.Name = "lblDSep";
            this.lblDSep.Size = new System.Drawing.Size(100, 15);
            this.lblDSep.TabIndex = 9;
            this.lblDSep.Text = "D区(int16)";
            // 
            // lblDInput
            // 
            this.lblDInput.Location = new System.Drawing.Point(15, 153);
            this.lblDInput.Name = "lblDInput";
            this.lblDInput.Size = new System.Drawing.Size(25, 15);
            this.lblDInput.TabIndex = 10;
            this.lblDInput.Text = "值:";
            // 
            // txtDInput
            // 
            this.txtDInput.Location = new System.Drawing.Point(40, 150);
            this.txtDInput.Name = "txtDInput";
            this.txtDInput.Size = new System.Drawing.Size(60, 21);
            this.txtDInput.TabIndex = 11;
            this.txtDInput.Text = "1024";
            // 
            // btnWriteD
            // 
            this.btnWriteD.Location = new System.Drawing.Point(110, 148);
            this.btnWriteD.Name = "btnWriteD";
            this.btnWriteD.Size = new System.Drawing.Size(90, 28);
            this.btnWriteD.TabIndex = 12;
            this.btnWriteD.Text = "Write D100";
            // 
            // btnReadD
            // 
            this.btnReadD.Location = new System.Drawing.Point(210, 148);
            this.btnReadD.Name = "btnReadD";
            this.btnReadD.Size = new System.Drawing.Size(90, 28);
            this.btnReadD.TabIndex = 13;
            this.btnReadD.Text = "Read D100";
            // 
            // txtDResult
            // 
            this.txtDResult.Location = new System.Drawing.Point(310, 150);
            this.txtDResult.Name = "txtDResult";
            this.txtDResult.ReadOnly = true;
            this.txtDResult.Size = new System.Drawing.Size(150, 21);
            this.txtDResult.TabIndex = 14;
            // 
            // lblDFSep
            // 
            this.lblDFSep.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold);
            this.lblDFSep.Location = new System.Drawing.Point(15, 190);
            this.lblDFSep.Name = "lblDFSep";
            this.lblDFSep.Size = new System.Drawing.Size(100, 15);
            this.lblDFSep.TabIndex = 15;
            this.lblDFSep.Text = "D区(float)";
            // 
            // lblDFInput
            // 
            this.lblDFInput.Location = new System.Drawing.Point(15, 223);
            this.lblDFInput.Name = "lblDFInput";
            this.lblDFInput.Size = new System.Drawing.Size(25, 15);
            this.lblDFInput.TabIndex = 16;
            this.lblDFInput.Text = "值:";
            // 
            // txtDFInput
            // 
            this.txtDFInput.Location = new System.Drawing.Point(40, 220);
            this.txtDFInput.Name = "txtDFInput";
            this.txtDFInput.Size = new System.Drawing.Size(60, 21);
            this.txtDFInput.TabIndex = 17;
            this.txtDFInput.Text = "3.14";
            // 
            // btnWriteDF
            // 
            this.btnWriteDF.Location = new System.Drawing.Point(110, 218);
            this.btnWriteDF.Name = "btnWriteDF";
            this.btnWriteDF.Size = new System.Drawing.Size(90, 28);
            this.btnWriteDF.TabIndex = 18;
            this.btnWriteDF.Text = "Write D200";
            // 
            // btnReadDF
            // 
            this.btnReadDF.Location = new System.Drawing.Point(210, 218);
            this.btnReadDF.Name = "btnReadDF";
            this.btnReadDF.Size = new System.Drawing.Size(90, 28);
            this.btnReadDF.TabIndex = 19;
            this.btnReadDF.Text = "Read D200";
            // 
            // txtDFResult
            // 
            this.txtDFResult.Location = new System.Drawing.Point(310, 220);
            this.txtDFResult.Name = "txtDFResult";
            this.txtDFResult.ReadOnly = true;
            this.txtDFResult.Size = new System.Drawing.Size(150, 21);
            this.txtDFResult.TabIndex = 20;
            // 
            // lblStatus
            // 
            this.lblStatus.ForeColor = System.Drawing.Color.Gray;
            this.lblStatus.Location = new System.Drawing.Point(15, 265);
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(200, 15);
            this.lblStatus.TabIndex = 21;
            this.lblStatus.Text = "状态: 未连接";
            // 
            // Form1
            // 
            this.ClientSize = new System.Drawing.Size(485, 295);
            this.Controls.Add(this.lblIp);
            this.Controls.Add(this.txtIp);
            this.Controls.Add(this.lblPort);
            this.Controls.Add(this.txtPort);
            this.Controls.Add(this.btnConnect);
            this.Controls.Add(this.lblMSep);
            this.Controls.Add(this.btnToggleM);
            this.Controls.Add(this.btnReadM);
            this.Controls.Add(this.txtMResult);
            this.Controls.Add(this.lblDSep);
            this.Controls.Add(this.lblDInput);
            this.Controls.Add(this.txtDInput);
            this.Controls.Add(this.btnWriteD);
            this.Controls.Add(this.btnReadD);
            this.Controls.Add(this.txtDResult);
            this.Controls.Add(this.lblDFSep);
            this.Controls.Add(this.lblDFInput);
            this.Controls.Add(this.txtDFInput);
            this.Controls.Add(this.btnWriteDF);
            this.Controls.Add(this.btnReadDF);
            this.Controls.Add(this.txtDFResult);
            this.Controls.Add(this.lblStatus);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Inovance PLC Test";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }
    }
}
