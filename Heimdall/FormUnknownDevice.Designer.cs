
namespace UHeimdall
{
    partial class FormUnknownDevice
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
            this.ButtonYes = new System.Windows.Forms.Button();
            this.ButtonNo = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // ButtonClose
            // 
            this.ButtonClose.FlatAppearance.BorderSize = 0;
            this.ButtonClose.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonClose.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.ButtonClose.ForeColor = System.Drawing.Color.White;
            this.ButtonClose.Location = new System.Drawing.Point(356, 12);
            this.ButtonClose.Name = "ButtonClose";
            this.ButtonClose.Size = new System.Drawing.Size(25, 25);
            this.ButtonClose.TabIndex = 2;
            this.ButtonClose.Text = "X";
            this.ButtonClose.UseVisualStyleBackColor = true;
            this.ButtonClose.Click += new System.EventHandler(this.ButtonClose_Click);
            // 
            // ButtonYes
            // 
            this.ButtonYes.Location = new System.Drawing.Point(82, 206);
            this.ButtonYes.Name = "ButtonYes";
            this.ButtonYes.Size = new System.Drawing.Size(75, 23);
            this.ButtonYes.TabIndex = 3;
            this.ButtonYes.Text = "Yes";
            this.ButtonYes.UseVisualStyleBackColor = true;
            this.ButtonYes.Click += new System.EventHandler(this.ButtonYes_Click);
            // 
            // ButtonNo
            // 
            this.ButtonNo.Location = new System.Drawing.Point(239, 206);
            this.ButtonNo.Name = "ButtonNo";
            this.ButtonNo.Size = new System.Drawing.Size(75, 23);
            this.ButtonNo.TabIndex = 3;
            this.ButtonNo.Text = "No";
            this.ButtonNo.UseVisualStyleBackColor = true;
            this.ButtonNo.Click += new System.EventHandler(this.ButtonNo_Click);
            // 
            // FormUnknownDevice
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(45)))), ((int)(((byte)(53)))), ((int)(((byte)(75)))));
            this.ClientSize = new System.Drawing.Size(408, 268);
            this.Controls.Add(this.ButtonNo);
            this.Controls.Add(this.ButtonYes);
            this.Controls.Add(this.ButtonClose);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "FormUnknownDevice";
            this.Text = "FormUnknownDevice";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button ButtonClose;
        private System.Windows.Forms.Button ButtonYes;
        private System.Windows.Forms.Button ButtonNo;
    }
}