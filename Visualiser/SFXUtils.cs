using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Numerics;
using System.IO;

namespace Visualiser
{
    class SFXUtils
    {

        public static List<Complex> GenerateWave(int length, double[] freqs)
        {
            List<Complex> wave = new List<Complex>(length);
            
            for (int i = 0; i < length; ++i)
            {
                double point = 0.0;

                for (int f = 0; f < freqs.Length; ++f)
                {
                    point += Math.Sin(2 * Math.PI * freqs[f] * i / length);
                }

                wave.Add(new Complex(point, 0.0));
            }

            var stream = File.AppendText(@"D:\Test\CS Wave.txt");
            foreach (Complex c in wave)
                stream.WriteLine(c.ToString());
            stream.Close();

            return wave;
        }

        public static void PadArray(ref List<Complex> arr)
        {
            if ((arr.Count & arr.Count - 1) == 0)
                return;

            uint size = (uint)arr.Count;
            size--;
            size |= size >> 1;
            size |= size >> 2;
            size |= size >> 4;
            size |= size >> 8;
            size |= size >> 16;
            size++;

            for (int i = arr.Count; i < size; ++i)
            {
                arr.Add(new Complex(0.0, 0.0));
            }
        }

        public static List<Complex> FFT(List<Complex> wave)
        {
            List<Complex> fftWave = new List<Complex>(wave);

            FFT(ref fftWave);

            var stream = File.AppendText(@"D:\Test\CS FFT.txt");
            foreach (Complex c in fftWave)
                stream.WriteLine(c.ToString());
            stream.Close();

            return fftWave;
        }
        public static void FFT(ref List<Complex> wave)
        {
            // Store the size of the wave as N so equations become much clearer to read
            int N = wave.Count;

            // If the wave is not a valid size then return
            if (N <= 1)
                return;

            // Slice the array into even/odd values
            List<Complex> even = new List<Complex>();
            List<Complex> odd  = new List<Complex>();

            for (int i = 0; i < wave.Count; ++i)
            {
                if (i % 2 == 0)
                    even.Add(wave[i]);
                else
                    odd.Add(wave[i]);
            }

            // Using a recursive approach here because it reads nicer than the original iterative algorithm.
            // The original algorithm is described in a method "four1" in the book Numerical Recipes in C++ (pg. 513)
            FFT(ref even);
            FFT(ref odd);

            // Loop through half of the array (values are reversed after N/2)
            // Using k as the iterator because that is the character used in the mathematical formula
            for (int k = 0; k < N / 2; ++k)
            {
                // This section is the actual FFT, formula shown: https://wikimedia.org/api/rest_v1/media/math/render/svg/687516d6bb74ec0b695242afdf22b88d8d3d89cc
                // Calculating 't' by using std::polar to convert
                // The following calculations describe the process (where k is time)
                // e^(pi i) = -1 ==> Euler's formula describes the distance walked around the circumference of a circle of radius 1 (unit circle)
                // e^( 2 pi i k) ==> Describes 1 full, anti-clockwise, rotation around a circle
                // e^(-2 pi i k) ==> In Fourier transforms, the convention is to use clockwise rotation so the exponent is negated to flip it
                // This can be described as a polar coordinate with a distance (rho) of 1 and the rotation (theta) of (-2 pi k / N)
                // the final step is to multiply by the value at (k + 1), which in this case is in the odd array at position k
                Complex t = Complex.FromPolarCoordinates(1.0, -2.0 * Math.PI * k / N) * odd[k];
                wave[k]         = even[k] + t;
                wave[k + N / 2] = even[k] - t;
            }
        }

        public static List<Complex> IFFT(List<Complex> wave)
        {
            List<Complex> ifftWave = new List<Complex>(wave);

            IFFT(ref ifftWave);

            return ifftWave;
        }
        public static void IFFT(ref List<Complex> wave)
        {
            // Swap real and imaginary parts
            for (int i = 0; i < wave.Count; ++i)
            {
                Complex pt = new Complex(wave[i].Imaginary, wave[i].Real);
                wave[i] = pt;
            }

            // Compute FFT
            FFT(ref wave);

            // Swap back real and imaginary parts and divide by N
            for (int i = 0; i < wave.Count; ++i)
            {
                Complex pt = new Complex(wave[i].Imaginary / wave.Count, wave[i].Real / wave.Count);
                wave[i] = pt;
            }
        }
    }
}
