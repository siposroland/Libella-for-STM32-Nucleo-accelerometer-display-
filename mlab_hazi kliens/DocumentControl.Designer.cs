namespace Signals
{
    partial class DocumentControl
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.tbSampleText = new System.Windows.Forms.TextBox();
            this.panel2 = new System.Windows.Forms.Panel();
            this.lblFontSelect = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.panelEditors = new System.Windows.Forms.Panel();
            this.sampleTextControl1 = new SampleTextView();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.sampleTextControl1);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.tbSampleText);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(577, 77);
            this.panel1.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(4, 4);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(62, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Sample text";
            // 
            // tbSampleText
            // 
            this.tbSampleText.Location = new System.Drawing.Point(3, 20);
            this.tbSampleText.Name = "tbSampleText";
            this.tbSampleText.Size = new System.Drawing.Size(100, 20);
            this.tbSampleText.TabIndex = 0;
            this.tbSampleText.Text = "abc";
            this.tbSampleText.TextChanged += new System.EventHandler(this.tbSampleText_TextChanged);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.lblFontSelect);
            this.panel2.Controls.Add(this.label2);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel2.Location = new System.Drawing.Point(0, 77);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(577, 31);
            this.panel2.TabIndex = 1;
            // 
            // lblFontSelect
            // 
            this.lblFontSelect.AutoSize = true;
            this.lblFontSelect.Font = new System.Drawing.Font("Courier New", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.lblFontSelect.Location = new System.Drawing.Point(127, 5);
            this.lblFontSelect.Margin = new System.Windows.Forms.Padding(0);
            this.lblFontSelect.Name = "lblFontSelect";
            this.lblFontSelect.Size = new System.Drawing.Size(76, 22);
            this.lblFontSelect.TabIndex = 2;
            this.lblFontSelect.Text = "ABC...";
            this.lblFontSelect.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.lblFontSelect_MouseDoubleClick);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(4, 8);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(120, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Select character to edit:";
            // 
            // panelEditors
            // 
            this.panelEditors.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelEditors.Location = new System.Drawing.Point(0, 108);
            this.panelEditors.Name = "panelEditors";
            this.panelEditors.Size = new System.Drawing.Size(577, 207);
            this.panelEditors.TabIndex = 2;
            // 
            // sampleTextControl1
            // 
            this.sampleTextControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.sampleTextControl1.BackColor = System.Drawing.Color.DarkGray;
            this.sampleTextControl1.Location = new System.Drawing.Point(121, 20);
            this.sampleTextControl1.Name = "sampleTextControl1";
            this.sampleTextControl1.Size = new System.Drawing.Size(437, 51);
            this.sampleTextControl1.TabIndex = 2;
            // 
            // FontDocumentControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panelEditors);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Name = "FontDocumentControl";
            this.Size = new System.Drawing.Size(577, 315);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Panel panelEditors;
        private SampleTextView sampleTextControl1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label lblFontSelect;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tbSampleText;
    }
}
