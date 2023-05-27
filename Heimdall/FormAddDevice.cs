using System;
using System.Windows.Forms;

namespace UHeimdall
{
    public partial class FormAddDevice : Form
    {
        private FormMain Main;
        public FormAddDevice(FormMain Caller)
        {
            InitializeComponent();

            this.Main = Caller;
            this.TextBoxDeviceId.Focus();
        }

        private void ButtonClose_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void ButtonAdd_Click(object sender, EventArgs e)
        {
            string DeviceId = this.TextBoxDeviceId.Text;
            string InstanceId  = this.TextBoxInstanceId.Text;

            if (!Controller.AddDevice(DeviceId, InstanceId))
                return;

            var Item = new AllowedDeviceControl(this.Main);
            Item.DeviceId = DeviceId;
            Item.InstanceId = InstanceId;

            Main.PanelBody.Controls.Add(Item);
        }
    }
}
