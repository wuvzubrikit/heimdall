
namespace UHeimdall
{
    partial class AllowedDeviceControl
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.TextBoxDeviceId = new System.Windows.Forms.TextBox();
            this.TextBoxInstanceId = new System.Windows.Forms.TextBox();
            this.LabelDelete = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // TextBoxDeviceId
            // 
            this.TextBoxDeviceId.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(85)))), ((int)(((byte)(95)))), ((int)(((byte)(120)))));
            this.TextBoxDeviceId.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.TextBoxDeviceId.Font = new System.Drawing.Font("Footlight MT Light", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TextBoxDeviceId.ForeColor = System.Drawing.Color.White;
            this.TextBoxDeviceId.Location = new System.Drawing.Point(32, 27);
            this.TextBoxDeviceId.Multiline = true;
            this.TextBoxDeviceId.Name = "TextBoxDeviceId";
            this.TextBoxDeviceId.ReadOnly = true;
            this.TextBoxDeviceId.Size = new System.Drawing.Size(420, 45);
            this.TextBoxDeviceId.TabIndex = 2;
            this.TextBoxDeviceId.Text = "Device Id";
            // 
            // TextBoxInstanceId
            // 
            this.TextBoxInstanceId.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(85)))), ((int)(((byte)(95)))), ((int)(((byte)(120)))));
            this.TextBoxInstanceId.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.TextBoxInstanceId.Font = new System.Drawing.Font("Footlight MT Light", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TextBoxInstanceId.ForeColor = System.Drawing.Color.White;
            this.TextBoxInstanceId.Location = new System.Drawing.Point(32, 78);
            this.TextBoxInstanceId.Multiline = true;
            this.TextBoxInstanceId.Name = "TextBoxInstanceId";
            this.TextBoxInstanceId.ReadOnly = true;
            this.TextBoxInstanceId.Size = new System.Drawing.Size(420, 45);
            this.TextBoxInstanceId.TabIndex = 2;
            this.TextBoxInstanceId.Text = "Instance Id";
            // 
            // LabelDelete
            // 
            this.LabelDelete.AutoSize = true;
            this.LabelDelete.Cursor = System.Windows.Forms.Cursors.Hand;
            this.LabelDelete.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F);
            this.LabelDelete.ForeColor = System.Drawing.Color.White;
            this.LabelDelete.Location = new System.Drawing.Point(523, 59);
            this.LabelDelete.Name = "LabelDelete";
            this.LabelDelete.Size = new System.Drawing.Size(30, 29);
            this.LabelDelete.TabIndex = 3;
            this.LabelDelete.Text = "X";
            this.LabelDelete.Click += new System.EventHandler(this.LabelDelete_Click);
            // 
            // AllowedDeviceControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(85)))), ((int)(((byte)(95)))), ((int)(((byte)(120)))));
            this.Controls.Add(this.LabelDelete);
            this.Controls.Add(this.TextBoxInstanceId);
            this.Controls.Add(this.TextBoxDeviceId);
            this.Name = "AllowedDeviceControl";
            this.Size = new System.Drawing.Size(595, 150);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.TextBox TextBoxDeviceId;
        private System.Windows.Forms.TextBox TextBoxInstanceId;
        private System.Windows.Forms.Label LabelDelete;
    }
}
