using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Numerics;

namespace Visualiser
{
    public partial class frmWaveVisuals : Form
    {
        private List<double> freqs;
        private List<Complex> rawWave;
        private List<Complex> sfxWave;

        public frmWaveVisuals()
        {
            InitializeComponent();

            this.freqs = new List<double>();
            this.rawWave = new List<Complex>();
            this.sfxWave = new List<Complex>();

            this.SetFrequencies();

            this.picWaveRaw.Paint += PicWaveRaw_Paint;
        }

        private void SetFrequencies()
        {
            freqs.Add(1.0);
            freqs.Add(2.0);
            freqs.Add(4.0);
            freqs.Add(8.0);
            freqs.Add(16.0);
            freqs.Add(32.0);
            freqs.Add(64.0);
            freqs.Add(128.0);
        }

        private void btnGenerate_Click(object sender, EventArgs e)
        {
            this.rawWave = SFXUtils.GenerateWave((256 * 2), freqs.ToArray());
            SFXUtils.PadArray(ref this.rawWave);

            this.picWaveRaw.Refresh();
        }

        private void PicWaveRaw_Paint(object sender, PaintEventArgs e)
        {
            float X_OFFSET = 0.0f;
            float X_MULT = 2.0f;
            float Y_OFFSET = this.picWaveRaw.Height / 2.0f;
            float Y_MULT = 10.0f;

            if (this.rawWave.Count == 0)
                return;

            List<PointF> wavePoints = new List<PointF>();

            for (int i = 0; i < this.rawWave.Count; ++i)
            {
                wavePoints.Add(new PointF(X_OFFSET + (i * X_MULT), Y_OFFSET + (float)(this.rawWave[i].Real * Y_MULT)));
            }
            
            e.Graphics.DrawLines(Pens.Black, wavePoints.ToArray());
        }

        private void btnGetFFT_Click(object sender, EventArgs e)
        {
            this.sfxWave = SFXUtils.FFT(this.rawWave);

            string msg = "";
            for (int i = 0; i < this.sfxWave.Count / 2; ++i)
            {
                double fftRound = Math.Round(Complex.Abs(this.sfxWave[i]) * 1000.0) / 1000.0;

                if (fftRound > double.Epsilon)
                {
                    msg += i + " => " + fftRound + Environment.NewLine;
                }
            }

            MessageBox.Show(msg);
        }
    }
}

// TODO: Fix memory leaks in the Effects being called
//          - Find the memory location that is not being released and try to release it after usage.
//          - Write down the memory addresses of all the blocks that are power of 2 length and see which one it is
//          - FIXED - Was not deleting the sound.data in the SoundManager class


// TODO: Fix bug with 16 bit audio after effects. Try an empty effect to see if the same result occurs.
//          - Could be the ComplexArrayToSound or SoundToComplexArray function that is messing this up
// TODO: Test Mono 8-bit (DONE - Works)
// TODO: Test Mono 16-bit (DONE - Broken)
// TODO: Test Stereo 8-bit (Not Done)
// TODO: Test Stereo 16-bit (Not Done)