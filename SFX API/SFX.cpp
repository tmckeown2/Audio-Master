#include "SFX.h"

/////////////////////////////////////////////////////////////////////////////////
// Functions to be used by the SFX plugin

/// <summary>
/// Converts a double array into a ComplexArray.
/// By default, it will pad the array to the next power of 2 data points.
/// </summary>
/// <param name="wave">The data array of doubles</param>
/// <param name="numPoints">The size of the data array</param>
/// <param name="pad">True = pad to next power of 2. False = don't modify size</param>
/// <returns>
/// ComplexArray filled with the data passed in. 
/// If pad is true then the size will be a power of 2
/// </returns>
ComplexArray ToComplexArray(double* wave, int numPoints, bool pad = true)
{
	std::vector<Complex> arr;

	for (int i = 0; i < numPoints; ++i)
	{
		arr.push_back(Complex(wave[i], 0.0));
	}

	// If pad is true AND the array is not already a power of 2 (avoids padding an already power of 2 array)
	// See: http://graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
	if (pad && (arr.size() & arr.size() - 1) != 0)
	{
		// Pad to the next power of 2
		// See: http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
		unsigned int size = arr.size();
		size--;
		size |= size >> 1;
		size |= size >> 2;
		size |= size >> 4;
		size |= size >> 8;
		size |= size >> 16;
		size++;

		while (arr.size() < size)
		{
			arr.push_back(Complex(0.0, 0.0));
		}
	}

	return ComplexArray(arr.data(), arr.size());
}

/// <summary>
/// Pad a complex array size to the next power of 2
/// </summary>
/// <param name="arr">Array to pad</param>
void PadArray(ComplexArray& arr)
{
	if ((arr.size() & arr.size() - 1) == 0)
		return;

	ComplexArray temp = arr;

	unsigned int size = arr.size();
	size--;
	size |= size >> 1;
	size |= size >> 2;
	size |= size >> 4;
	size |= size >> 8;
	size |= size >> 16;
	size++;

	arr.resize(size);
	for (int i = 0; i < temp.size(); ++i)
	{
		arr[i] = temp[i];
	}
}

/// <summary>
/// Performs Fast-Fourier Transform on an array of Complex values.
/// The resulting frequency spectrum overrides the passed parameter.
/// </summary>
/// <param name="wave">The array of Complex wave data points</param>
/// <remarks>
/// This method uses a recursive adaptation of the Cooley-Tukey algorithm
/// from Numerical Recipes in C++.
/// See: Press, W. (2007). Numerical recipes in C++. 2nd ed. New Dehli: Cambridge Univ. Press India, pp.512-513.
/// </remarks>
void FFT(ComplexArray& wave)
{
	// Store the size of the wave as N so equations become much clearer to read
	const int N = wave.size();
	// If the wave is not a valid size then return
	if (N < 1)
		return;

	// Use slice to get the even/odd values, returning every other value starting from 0/1 respectively
	ComplexArray even = wave[std::slice(0, N / 2, 2)];
	ComplexArray odd = wave[std::slice(1, N / 2, 2)];

	// Using a recursive approach here because it reads nicer than the original iterative algorithm.
	// The original algorithm is described in a method "four1" in the book Numerical Recipes in C++ (pg. 513)
	FFT(even);
	FFT(odd);

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
		Complex t = std::polar(1.0, -2.0 * M_PI * k / N) * odd[k];
		wave[k] = even[k] + t;
		wave[k + N / 2] = even[k] - t;
	}
}

/// <summary>
/// Performs Inverse Fast-Fourier Transform on an array of Complex values.
/// The resulting wave overrides the passed parameter.
/// </summary>
/// <param name="wave">The array of Complex wave data points</param>
/// <remarks>
/// This method uses the forwards FFT to compute the inverse by switching the
/// real and imaginary inputs before and after performing FFT.
/// See: Inverse FFT Method #3 on https://www.dsprelated.com/showarticle/800.php
/// </remarks>
void IFFT(ComplexArray& wave)
{
	// Swap real and imaginary parts
	for (auto& c : wave)
	{
		double tmp = c.real();
		c.real(c.imag());
		c.imag(tmp);
	}

	// Compute FFT
	FFT(wave);

	// Swap back real and imaginary parts and divide by N
	for (auto& c : wave)
	{
		double tmp = c.real() / wave.size();
		c.real(c.imag() / wave.size());
		c.imag(tmp);
	}
}

/////////////////////////////////////////////////////////////////////////////////
// Implementation of the ISfx Interface
// Change the name of the 
class Sfx : public ISfx
{
private:
	const char* name = "Name";
public:
	void		Release() { delete this; }
	const char* GetName() { return name; }
	void		DoSFX(ComplexArray& arr);
};

/// <summary>
/// This is the function that will apply the SFX to the array.
/// When making a plugin, this function is the one to change.
/// </summary>
/// <param name="arr">Array to modify</param>
void Sfx::DoSFX(ComplexArray& arr)
{
	PadArray(arr);
	arr = arr.apply([](Complex val) { return Complex(val.real() * 0.1, 0.0); });
}

/////////////////////////////////////////////////////////////////////////////////
// Factory function that creates instances of the Sfx object

// Export both decorated and undecorated names.
//	GetSfx		- Undecorated name, which can be easily used with GetProcAddress
//				  Win32 API function.
//	_GetSfx@0	- Common name decoration for __stdcall functions in C language.
//
// For more information on name decoration see here:
// "Format of a C Decorated Name"
// http://msdn.microsoft.com/en-us/library/x7kb4e2f.aspx

#if !defined(_WIN64)
// This pragma is required only for 32-bit builds. In a 64-bit environment,
// C functions are not decorated.
#pragma comment(linker, "/export:GetSfx=_GetSfx@0")
#endif

SFXAPI SFXHANDLE __stdcall GetSfx()
{
	return new Sfx;
}