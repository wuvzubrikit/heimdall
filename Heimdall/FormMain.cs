using System;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace UHeimdall
{
    public partial class FormMain : Form
    {
        public FormMain()
        {

            InitializeComponent();

            //var Item1 = new AllowedDeviceControl(this);
            //Item1.DeviceId = "Disk&Ven_JetFlash&Prod_Transcend_16GB&Rev_1100";
            //Item1.InstanceId = "75U295UT5WSAHEVZ&0";

            //var Item2 = new AllowedDeviceControl(this);
            //Item2.DeviceId = "Disk&Ven__USB&Prod__SanDisk_3.2Gen1&Rev_1.00";
            //Item2.InstanceId = "058F091111B&0";

            //PanelBody.Controls.Add(Item1);
            //PanelBody.Controls.Add(Item2);
        }
        private void FormMain_Load(object sender, EventArgs e)
        {
            EventWaitHandle WaitHandle = new EventWaitHandle(false, EventResetMode.ManualReset);
            Controller.hEvent = WaitHandle.SafeWaitHandle;

            if (Controller.StartPlugNotification(Controller.hEvent))
            {
                var NotificationThread = new Thread(() =>
                {
                    StringBuilder PluggedDeviceId = new StringBuilder(64);
                    StringBuilder PluggedInstanceId = new StringBuilder(64);

                    while (true)
                    {
                        WaitHandle.WaitOne();
                        if (Controller.QueryDevice(PluggedDeviceId, PluggedInstanceId))
                        {
                            var Unknown = new FormUnknownDevice(this, PluggedDeviceId.ToString(), PluggedInstanceId.ToString());
                            Unknown.Show();
                        }
                        WaitHandle.Reset();
                    }
                }
                );

                NotificationThread.Start();
            }
        }
        private void ButtonHeaderMinimize_Click(object sender, EventArgs e)
        {
            
        }
        private void ButtonHeaderClose_Click(object sender, EventArgs e)
        {
            Controller.StopPlugNotification();
            Application.Exit();
        }

        private void ButtonFooterAdd_Click(object sender, EventArgs e)
        {
            var Add = new FormAddDevice(this);
            Add.ShowDialog();
        }
    }
}
