using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace UHeimdall
{
    public partial class FormUnknownDevice : Form
    {
        private FormMain Main;
        private string DeviceId;
        private string InstanceId;
        public FormUnknownDevice(FormMain Caller, string DeviceId, string InstanceId)
        {
            InitializeComponent();

            this.Main = Caller;
            this.DeviceId = DeviceId;
            this.InstanceId = InstanceId;
        }

        private void ButtonClose_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void ButtonYes_Click(object sender, EventArgs e)
        {
            if (!Controller.AddDevice(this.DeviceId, this.InstanceId))
                return;

            var Item = new AllowedDeviceControl(this.Main);
            Item.DeviceId = this.DeviceId;
            Item.InstanceId = this.InstanceId;

            Main.PanelBody.Controls.Add(Item);
        }

        private void ButtonNo_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
