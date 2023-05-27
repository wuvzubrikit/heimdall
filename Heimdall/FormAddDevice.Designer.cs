
namespace UHeimdall
{
    partial class FormAddDevice
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.ButtonClose = new System.Windows.Forms.Button();
            this.TextBoxDeviceId = new System.Windows.Forms.TextBox();
            this.TextBoxInstanceId = new System.Windows.Forms.TextBox();
            this.ButtonAdd = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // ButtonClose
            // 
            this.ButtonClose.FlatAppearance.BorderSize = 0;
            this.ButtonClose.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonClose.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.ButtonClose.ForeColor = System.Drawing.Color.White;
            this.ButtonClose.Location = new System.Drawing.Point(364, 12);
            this.ButtonClose.Name = "ButtonClose";
            this.ButtonClose.Size = new System.Drawing.Size(25, 25);
            this.ButtonClose.TabIndex = 2;
            this.ButtonClose.Text = "X";
            this.ButtonClose.UseVisualStyleBackColor = true;
            this.ButtonClose.Click += new System.EventHandler(this.ButtonClose_Click);
            // 
            // TextBoxDeviceId
            // 
            this.TextBoxDeviceId.Location = new System.Drawing.Point(27, 88);
            this.TextBoxDeviceId.Name = "TextBoxDeviceId";
            this.TextBoxDeviceId.Size = new System.Drawing.Size(345, 22);
            this.TextBoxDeviceId.TabIndex = 3;
            // 
            // TextBoxInstanceId
            // 
            this.TextBoxInstanceId.Location = new System.Drawing.Point(27, 128);
            this.TextBoxInstanceId.Name = "TextBoxInstanceId";
            this.TextBoxInstanceId.Size = new System.Drawing.Size(345, 22);
            this.TextBoxInstanceId.TabIndex = 4;
            // 
            // ButtonAdd
            // 
            this.ButtonAdd.Location = new System.Drawing.Point(152, 179);
            this.ButtonAdd.Name = "ButtonAdd";
            this.ButtonAdd.Size = new System.Drawing.Size(93, 40);
            this.ButtonAdd.TabIndex = 5;
            this.ButtonAdd.Text = "Add";
            this.ButtonAdd.UseVisualStyleBackColor = true;
            this.ButtonAdd.Click += new System.EventHandler(this.ButtonAdd_Click);
            // 
            // FormAddDevice
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(45)))), ((int)(((byte)(53)))), ((int)(((byte)(75)))));
            this.ClientSize = new System.Drawing.Size(410, 278);
            this.Controls.Add(this.ButtonAdd);
            this.Controls.Add(this.TextBoxInstanceId);
            this.Controls.Add(this.TextBoxDeviceId);
            this.Controls.Add(this.ButtonClose);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "FormAddDevice";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button ButtonClose;
        private System.Windows.Forms.TextBox TextBoxDeviceId;
        private System.Windows.Forms.TextBox TextBoxInstanceId;
        private System.Windows.Forms.Button ButtonAdd;
    }
}