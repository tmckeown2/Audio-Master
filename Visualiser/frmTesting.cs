using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Numerics;
using System.Diagnostics;

namespace Visualiser
{
    public partial class frmTesting : Form
    {
        Complex[] wave = new Complex[64000];

        public frmTesting()
        {
            InitializeComponent();

            for (int i = 0; i < wave.Length; ++i)
            {
                wave[i] = new Complex(i, -i);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            const int FRAME_SIZE = 32768;
            int remaining = wave.Length;
            int offset = 0;

            List<Complex[]> frames = new List<Complex[]>();

            while (remaining > 0)
            {
                Complex[] frame = new Complex[FRAME_SIZE];
                if (remaining - FRAME_SIZE >= 0)
                {
                    for (int i = 0; i < FRAME_SIZE; ++i)
                    {
                        frame[i] = wave[offset + i];
                    }
                    Debug.WriteLine("[DBG] last in frame: " + frame.Last());
                    remaining -= FRAME_SIZE;
                    offset += FRAME_SIZE;
                }
                else
                {
                    for (int i = 0; i < wave.Length - offset; ++i)
                    {
                        frame[i] = wave[offset + i];
                    }
                    Debug.WriteLine("[DBG] last in frame: " + frame.Last());
                    remaining -= (wave.Length - offset);
                    offset += (wave.Length - offset);
                }

                frames.Add(frame);
            }

            Debug.WriteLine("[DBG] frames.Length * FRAME_SIZE: " + (frames.Count * FRAME_SIZE));
        }
    }
}
