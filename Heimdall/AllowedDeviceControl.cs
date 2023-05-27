using System;
using System.Windows.Forms;

namespace UHeimdall
{
    public partial class AllowedDeviceControl : UserControl
    {
        private FormMain Main;
        public AllowedDeviceControl(FormMain Caller)
        {
            InitializeComponent();

            this.Main = Caller;
        }

        public string DeviceId
        {
            get { return this.TextBoxDeviceId.Text; }
            set { this.TextBoxDeviceId.Text = value; }
        }

        public string InstanceId
        {
            get { return this.TextBoxInstanceId.Text; }
            set { this.TextBoxInstanceId.Text = value; }
        }

        private void LabelDelete_Click(object sender, EventArgs e)
        {
            if (!Controller.RemoveDevice(this.DeviceId, this.InstanceId))
                return;
            
            foreach (AllowedDeviceControl ControlInsatnce in Main.PanelBody.Controls)
            {
                if (ControlInsatnce.DeviceId == this.DeviceId && ControlInsatnce.InstanceId == this.InstanceId)
                {
                    Main.PanelBody.Controls.RemoveAt(Main.PanelBody.Controls.IndexOf(ControlInsatnce));
                    break;
                }
            }
            
            this.Dispose();
        }
    }
}
