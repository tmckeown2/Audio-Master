#include "Fourier.h"

CArray GetComplexWave(std::vector<double> frequencies, int numPoints)
{
	std::vector<Complex> samples;

	for (int n = 0; n < numPoints; ++n)
	{
		double sample = 0.0;
		for (auto &f : frequencies)
		{
			sample += sin(2 * M_PI * f * n / numPoints);
		}
		samples.push_back(Complex(sample, 0.0));
	}

	return CArray(samples.data(), numPoints);
}

// Find the programmation git repo (for using valarrays to simplify the code)
// Reference the fft pages and the numerical recipes book for influence.
void FFT(CArray& x)
{
	const int N = x.size();
	if (N <= 1) return;

	CArray even = x[std::slice(0, N/2, 2)];
	CArray odd  = x[std::slice(1, N/2, 2)];

	FFT(even);
	FFT(odd);

	for (int k = 0; k < N / 2; ++k)
	{
		Complex t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
		x[k]       = even[k] + t;
		x[k + N/2] = even[k] - t;
	}
}

// https://www.dsprelated.com/showarticle/800.php (Method 3)
void IFFT(CArray& x)
{
	// Swap real and imaginary parts
	for (auto& c : x)
	{
		double tmp = c.real();
		c.real(c.imag());
		c.imag(tmp);
	}

	// Compute FFT
	FFT(x);

	// Swap back real and imaginary parts and divide by N
	for (auto& c : x)
	{
		double tmp = c.real() / x.size();
		c.real(c.imag() / x.size());
		c.imag(tmp);
	}
}

void RemoveFreq(CArray& x, double freqLower, double freqHigher)
{
	// FFT the array
	FFT(x);

	// Flatten the peaks between freqLower and freqHigher
	for (int i = 0; i < x.size(); ++i)
	{
		// Round to 3dp
		double roundX = std::round(std::abs(x[i]) * 1000.0) / 1000.0;

		if (roundX > std::numeric_limits<double>::epsilon())
		{
			if (freqLower <= i && i <= freqHigher)
			{
				x[i] = 0.0;
			}
		}
	}

	// IFFT the array
	IFFT(x);
}

/// <summary>
/// Initial signal is stored in the array data of length 2*nn,
/// where each even element corresponds to the real part and each odd element to the imaginary part of a complex number.
/// </summary>
/// <param name="data">Sampled data (even = real, odd = imaginary)</param>
/// <param name="nn">Number of samples</param>
/// <remarks>Comes from "Numerical Recipes in C++: the art of scientific computing" 2002.; 2nd ed. p.513</remarks>
void four1(double* data, unsigned long nn)
{
	unsigned long n, mmax, m, j, istep, i;
	double wtemp, wr, wpr, wpi, wi, theta;
	double tempr, tempi;

	// reverse-binary reindexing
	n = nn << 1;
	j = 1;
	for (i = 1; i < n; i += 2)
	{
		if (j > i)
		{
			std::swap(data[j-1], data[i-1]);
			std::swap(data[j], data[i]);
		}
		m = nn;
		while (m >= 2 && j > m)
		{
			j -= m;
			m >>= 1;
		}
		j += m;
	}

	// here begins the Danielson-Lanczos section
	mmax = 2;
	while (n > mmax)
	{
		istep = mmax << 1;
		theta = -(2 * M_PI / mmax);
		wtemp = sin(0.5 * theta);
		wpr = -2.0 * wtemp * wtemp;
		wpi = sin(theta);
		wr = 1.0;
		wi = 0.0;
		for (m = 1; m < mmax; m += 2)
		{
			for (i = m; i <= n; i += istep)
			{
				j = i + mmax;
				tempr = wr * data[j - 1] - wi * data[j];
				tempi = wr * data[j] + wi * data[j - 1];

				data[j - 1] = data[i - 1] - tempr;
				data[j] = data[i] - tempi;
				data[i - 1] += tempr;
				data[i] += tempi;
			}
			wtemp = wr;
			wr += wr * wpr - wi * wpi;
			wi += wi * wpr + wtemp * wpi;
		}
		mmax = istep;
	}
}