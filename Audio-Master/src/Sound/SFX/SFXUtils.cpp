#include "SFXUtils.h"
#include <vector>

namespace AudioMaster
{
	//// From SFML FFT git repo + some wiki stuff
	//void SFXUtils::FFT(ComplexArray& x)
	//{
	//	const int N = x.size();
	//	if (N <= 1)
	//		return;

	//	ComplexArray even = x[std::slice(0, N / 2, 2)]; // Get every even (  2n  ) value
	//	ComplexArray  odd = x[std::slice(1, N / 2, 2)]; // Get every  odd (2n + 1) value

	//	// Uses recursion for ease of programming
	//	// The original cooley-tukey method, found in numerical recipes for C++, uses iteration
	//	FFT(even);
	//	FFT(odd);

	//	for (int k = 0; k < N / 2; ++k)
	//	{
	//		// Get the complex object from polar components (rho & theta)
	//		// The value is in the complex plane because it is multiplied by "i", the sqrt(-1).
	//		// Let t = e^( ((-2*pi*i)/N )*k ) * O[k] <-- This is the exponent part of the FFT formula
	//		Complex t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];

	//		// This section is why the results mirror at N/2 (Ek +/- t)
	//		x[k] = even[k] + t;
	//		x[k + N / 2] = even[k] - t;
	//	}
	//}
	//// https://www.dsprelated.com/showarticle/800.php (Method 3)
	//void SFXUtils::IFFT(ComplexArray& x)
	//{
	//	// This formula uses the Forwards FFT to resolve the inverse by swapping the inputs (real->imaginary and vice-versa)
	//	// Swap real and imaginary parts
	//	for (auto& c : x)
	//	{
	//		double tmp = c.real();
	//		c.real(c.imag());
	//		c.imag(tmp);
	//	}

	//	// Compute FFT
	//	FFT(x);

	//	// Swap back real and imaginary parts and divide by N
	//	for (auto& c : x)
	//	{
	//		double tmp = c.real() / x.size();
	//		c.real(c.imag() / x.size());
	//		c.imag(tmp);
	//	}
	//}
	//void SFXUtils::RemoveFrequencies(ComplexArray& x, double lower, double higher)
	//{
	//	// FFT the array
	//	FFT(x);

	//	// Flatten the peaks between freqLower and freqHigher
	//	for (int i = 0; i < x.size(); ++i)
	//	{
	//		// Round to 3dp
	//		double roundX = std::round(std::abs(x[i]) * 1000.0) / 1000.0;

	//		if (roundX > std::numeric_limits<double>::epsilon())
	//		{
	//			if (lower <= i && i <= higher)
	//			{
	//				x[i] = 0.0;
	//			}
	//		}
	//	}

	//	// IFFT the array
	//	IFFT(x);
	//}

	ComplexArray SFXUtils::ToComplexArray(Sound* x)
	{
		std::vector<Complex> samples;

		int min = 0;
		int max = 0;

		if (x->bitsPerSample == 8)
		{
			max = std::numeric_limits<uint8_t>::max();
			min = std::numeric_limits<uint8_t>::min();
		}
		else if (x->bitsPerSample == 16)
		{
			max = std::numeric_limits<int16_t>::max();
			min = std::numeric_limits<int16_t>::min();
		}

		int midpoint = (max + min + 1) / 2;

		for (int i = 0; i < x->size; i += x->bitsPerSample / 8)
		{
			double sample = 0.0;

			sample = (x->bitsPerSample == 8) ? ((double)((uint8_t)(x->data[i]))) : ((double)((int16_t)(x->data[i] | x->data[i + 1] << 8)));
			sample -= midpoint;

			samples.push_back(Complex(sample, 0.0));
		}

		return ComplexArray(samples.data(), x->size / (x->bitsPerSample / 8));
	}
	void SFXUtils::ToSound(ComplexArray& x, Sound* s)
	{
		if (s->size != x.size())
		{
			s->Resize(x.size());
		}

		int min = 0;
		int max = 0;

		if (s->bitsPerSample == 8)
		{
			max = std::numeric_limits<uint8_t>::max();
			min = std::numeric_limits<uint8_t>::min();
		}
		else if (s->bitsPerSample == 16)
		{
			max = std::numeric_limits<int16_t>::max();
			min = std::numeric_limits<int16_t>::min();
		}

		int midpoint = (max + min + 1) / 2;

		for (int i = 0; i < x.size(); ++i)
		{
			s->data[i] = (s->bitsPerSample == 8) ? (uint8_t)(x[i].real()) : (int16_t)(x[i].real());
			s->data[i] += midpoint;
		}
	}
}