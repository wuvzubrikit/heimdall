
namespace UHeimdall
{
    partial class FormMain
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
            this.PanelHeader = new System.Windows.Forms.Panel();
            this.ButtonHeaderMinimize = new System.Windows.Forms.Button();
            this.ButtonHeaderClose = new System.Windows.Forms.Button();
            this.PanelHeaderInfo = new System.Windows.Forms.Panel();
            this.LabelHeaderName = new System.Windows.Forms.Label();
            this.ButtonFooterAdd = new System.Windows.Forms.Button();
            this.PanelBody = new System.Windows.Forms.FlowLayoutPanel();
            this.PanelHeader.SuspendLayout();
            this.PanelHeaderInfo.SuspendLayout();
            this.SuspendLayout();
            // 
            // PanelHeader
            // 
            this.PanelHeader.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(43)))), ((int)(((byte)(65)))));
            this.PanelHeader.Controls.Add(this.ButtonHeaderMinimize);
            this.PanelHeader.Controls.Add(this.ButtonHeaderClose);
            this.PanelHeader.Controls.Add(this.PanelHeaderInfo);
            this.PanelHeader.Dock = System.Windows.Forms.DockStyle.Top;
            this.PanelHeader.Location = new System.Drawing.Point(0, 0);
            this.PanelHeader.Name = "PanelHeader";
            this.PanelHeader.Size = new System.Drawing.Size(600, 100);
            this.PanelHeader.TabIndex = 0;
            // 
            // ButtonHeaderMinimize
            // 
            this.ButtonHeaderMinimize.FlatAppearance.BorderSize = 0;
            this.ButtonHeaderMinimize.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonHeaderMinimize.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.ButtonHeaderMinimize.ForeColor = System.Drawing.Color.White;
            this.ButtonHeaderMinimize.Location = new System.Drawing.Point(499, 32);
            this.ButtonHeaderMinimize.Name = "ButtonHeaderMinimize";
            this.ButtonHeaderMinimize.Size = new System.Drawing.Size(25, 25);
            this.ButtonHeaderMinimize.TabIndex = 1;
            this.ButtonHeaderMinimize.Text = "—";
            this.ButtonHeaderMinimize.UseVisualStyleBackColor = true;
            this.ButtonHeaderMinimize.Click += new System.EventHandler(this.ButtonHeaderMinimize_Click);
            // 
            // ButtonHeaderClose
            // 
            this.ButtonHeaderClose.FlatAppearance.BorderSize = 0;
            this.ButtonHeaderClose.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ButtonHeaderClose.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.ButtonHeaderClose.ForeColor = System.Drawing.Color.White;
            this.ButtonHeaderClose.Location = new System.Drawing.Point(551, 34);
            this.ButtonHeaderClose.Name = "ButtonHeaderClose";
            this.ButtonHeaderClose.Size = new System.Drawing.Size(25, 25);
            this.ButtonHeaderClose.TabIndex = 1;
            this.ButtonHeaderClose.Text = "X";
            this.ButtonHeaderClose.UseVisualStyleBackColor = true;
            this.ButtonHeaderClose.Click += new System.EventHandler(this.ButtonHeaderClose_Click);
            // 
            // PanelHeaderInfo
            // 
            this.PanelHeaderInfo.Controls.Add(this.LabelHeaderName);
            this.PanelHeaderInfo.Cursor = System.Windows.Forms.Cursors.Help;
            this.PanelHeaderInfo.Dock = System.Windows.Forms.DockStyle.Left;
            this.PanelHeaderInfo.Location = new System.Drawing.Point(0, 0);
            this.PanelHeaderInfo.Name = "PanelHeaderInfo";
            this.PanelHeaderInfo.Size = new System.Drawing.Size(200, 100);
            this.PanelHeaderInfo.TabIndex = 0;
            // 
            // LabelHeaderName
            // 
            this.LabelHeaderName.AutoSize = true;
            this.LabelHeaderName.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(43)))), ((int)(((byte)(65)))));
            this.LabelHeaderName.Font = new System.Drawing.Font("Felix Titling", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LabelHeaderName.ForeColor = System.Drawing.Color.White;
            this.LabelHeaderName.Location = new System.Drawing.Point(12, 34);
            this.LabelHeaderName.Name = "LabelHeaderName";
            this.LabelHeaderName.Size = new System.Drawing.Size(174, 35);
            this.LabelHeaderName.TabIndex = 0;
            this.LabelHeaderName.Text = "HEIMDALL";
            this.LabelHeaderName.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // ButtonFooterAdd
            // 
            this.ButtonFooterAdd.Font = new System.Drawing.Font("Felix Titling", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ButtonFooterAdd.Location = new System.Drawing.Point(437, 682);
            this.ButtonFooterAdd.Name = "ButtonFooterAdd";
            this.ButtonFooterAdd.Size = new System.Drawing.Size(121, 38);
            this.ButtonFooterAdd.TabIndex = 2;
            this.ButtonFooterAdd.Text = "Add device";
            this.ButtonFooterAdd.UseVisualStyleBackColor = true;
            this.ButtonFooterAdd.Click += new System.EventHandler(this.ButtonFooterAdd_Click);
            // 
            // PanelBody
            // 
            this.PanelBody.AutoScroll = true;
            this.PanelBody.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.PanelBody.Location = new System.Drawing.Point(0, 106);
            this.PanelBody.Name = "PanelBody";
            this.PanelBody.Size = new System.Drawing.Size(600, 550);
            this.PanelBody.TabIndex = 3;
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(45)))), ((int)(((byte)(53)))), ((int)(((byte)(75)))));
            this.ClientSize = new System.Drawing.Size(600, 750);
            this.Controls.Add(this.PanelBody);
            this.Controls.Add(this.ButtonFooterAdd);
            this.Controls.Add(this.PanelHeader);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "FormMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.PanelHeader.ResumeLayout(false);
            this.PanelHeaderInfo.ResumeLayout(false);
            this.PanelHeaderInfo.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel PanelHeader;
        private System.Windows.Forms.Panel PanelHeaderInfo;
        private System.Windows.Forms.Label LabelHeaderName;
        private System.Windows.Forms.Button ButtonHeaderMinimize;
        private System.Windows.Forms.Button ButtonHeaderClose;
        private System.Windows.Forms.Button ButtonFooterAdd;
        internal System.Windows.Forms.FlowLayoutPanel PanelBody;
    }
}

