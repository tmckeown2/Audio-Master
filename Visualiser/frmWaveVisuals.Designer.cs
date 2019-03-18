namespace Visualiser
{
    partial class frmWaveVisuals
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
            this.btnGenerate = new System.Windows.Forms.Button();
            this.panelButtons = new System.Windows.Forms.Panel();
            this.picWaveRaw = new System.Windows.Forms.PictureBox();
            this.picFreq = new System.Windows.Forms.PictureBox();
            this.picWaveEffect = new System.Windows.Forms.PictureBox();
            this.btnGetFFT = new System.Windows.Forms.Button();
            this.panelButtons.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picWaveRaw)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.picFreq)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.picWaveEffect)).BeginInit();
            this.SuspendLayout();
            // 
            // btnGenerate
            // 
            this.btnGenerate.Location = new System.Drawing.Point(3, 0);
            this.btnGenerate.Name = "btnGenerate";
            this.btnGenerate.Size = new System.Drawing.Size(111, 42);
            this.btnGenerate.TabIndex = 0;
            this.btnGenerate.Text = "Generate Wave";
            this.btnGenerate.UseVisualStyleBackColor = true;
            this.btnGenerate.Click += new System.EventHandler(this.btnGenerate_Click);
            // 
            // panelButtons
            // 
            this.panelButtons.Controls.Add(this.btnGetFFT);
            this.panelButtons.Controls.Add(this.btnGenerate);
            this.panelButtons.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panelButtons.Location = new System.Drawing.Point(0, 450);
            this.panelButtons.Name = "panelButtons";
            this.panelButtons.Size = new System.Drawing.Size(800, 57);
            this.panelButtons.TabIndex = 1;
            // 
            // picWaveRaw
            // 
            this.picWaveRaw.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.picWaveRaw.Dock = System.Windows.Forms.DockStyle.Top;
            this.picWaveRaw.Location = new System.Drawing.Point(0, 0);
            this.picWaveRaw.Name = "picWaveRaw";
            this.picWaveRaw.Size = new System.Drawing.Size(800, 150);
            this.picWaveRaw.TabIndex = 2;
            this.picWaveRaw.TabStop = false;
            // 
            // picFreq
            // 
            this.picFreq.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.picFreq.Dock = System.Windows.Forms.DockStyle.Top;
            this.picFreq.Location = new System.Drawing.Point(0, 150);
            this.picFreq.Name = "picFreq";
            this.picFreq.Size = new System.Drawing.Size(800, 150);
            this.picFreq.TabIndex = 3;
            this.picFreq.TabStop = false;
            // 
            // picWaveEffect
            // 
            this.picWaveEffect.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.picWaveEffect.Dock = System.Windows.Forms.DockStyle.Top;
            this.picWaveEffect.Location = new System.Drawing.Point(0, 300);
            this.picWaveEffect.Name = "picWaveEffect";
            this.picWaveEffect.Size = new System.Drawing.Size(800, 150);
            this.picWaveEffect.TabIndex = 4;
            this.picWaveEffect.TabStop = false;
            // 
            // btnGetFFT
            // 
            this.btnGetFFT.Location = new System.Drawing.Point(120, 0);
            this.btnGetFFT.Name = "btnGetFFT";
            this.btnGetFFT.Size = new System.Drawing.Size(111, 42);
            this.btnGetFFT.TabIndex = 1;
            this.btnGetFFT.Text = "FFT Wave";
            this.btnGetFFT.UseVisualStyleBackColor = true;
            this.btnGetFFT.Click += new System.EventHandler(this.btnGetFFT_Click);
            // 
            // frmWaveVisuals
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 507);
            this.Controls.Add(this.picWaveEffect);
            this.Controls.Add(this.picFreq);
            this.Controls.Add(this.picWaveRaw);
            this.Controls.Add(this.panelButtons);
            this.Name = "frmWaveVisuals";
            this.Text = "Form1";
            this.panelButtons.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.picWaveRaw)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.picFreq)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.picWaveEffect)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnGenerate;
        private System.Windows.Forms.Panel panelButtons;
        private System.Windows.Forms.PictureBox picWaveRaw;
        private System.Windows.Forms.PictureBox picFreq;
        private System.Windows.Forms.PictureBox picWaveEffect;
        private System.Windows.Forms.Button btnGetFFT;
    }
}

