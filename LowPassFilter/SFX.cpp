#include "SFX.h"
#include <fstream>

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

/// <summary>
/// Performs Short-Time Fourier Transform on an array of Complex values.
/// The resulting frequency spectrum overwrites the passed array.
/// </summary>
/// <param name="wave">The array of Complex wave data points</param>
/// <remarks>
/// Splits the wave into several frames of 32768 points and runs the FFT on each frame.
/// This allows you to get several frames of frequency data, which means the frequency data
/// gathered is a more accurate representation of the waveform over time, rather than the
/// average frequency of the whole waveform.
/// </remarks>
void STFT(ComplexArray& wave)
{
	const int FRAME_SIZE = 32768; // (2^15) points
	int remaining = wave.size();
	int offset = 0;

	std::vector<ComplexArray> frames;

	// Split the wave into frames of 32768 points each
	while (remaining > 0)
	{
		ComplexArray frame = ComplexArray(FRAME_SIZE);
		if (remaining - FRAME_SIZE >= 0)
		{
			for (int i = 0; i < FRAME_SIZE; ++i)
			{
				frame[i] = wave[offset + i];
			}
			remaining -= FRAME_SIZE;
			offset += FRAME_SIZE;
		}
		else
		{
			for (int i = 0; i < wave.size() - offset; ++i)
			{
				frame[i] = wave[offset + i];
			}
			remaining -= (wave.size() - offset);
			offset += (wave.size() - offset);
		}
		frames.push_back(frame);
	}

	// Resize the wave array to fit all the frames inside
	wave.resize(frames.size() * FRAME_SIZE);
	offset = 0;

	// Perform the FFT on each frame, no need to pad because the frames are padded individually
	// to 32768 points each, which is 2^15 so will work nicely in FFT
	for (ComplexArray frame : frames)
	{
		// Convert to frequency domain
		FFT(frame);

		// Combine each frames back into a wave
		for (int i = 0; i < frame.size(); ++i)
		{
			wave[offset + i] = frame[i];
		}
		offset += frame.size();
	}

	// Once all the frames have passed through the FFT and been added back into the wave,
	// the wave should have precise frequencies for very short periods of time.
	return;
}

/// <summary>
/// Performs Short-Time Inverse Fourier Transform on an array of Complex values.
/// The resulting time spectrum overwrites the passed array.
/// </summary>
/// <param name="wave">The array of Complex wave data points</param>
/// <remarks>
/// This function is the opposite of the STFT, in that it reverses the frequency spectrum into the
/// wave data that would create that frequency spectrum. When using STFT to get the frequency spectrum
/// you should use the STIFT (Short-Time Inverse Fourier Transform) to get the time spectrum data back
/// </remarks>
void STIFT(ComplexArray& wave)
{
	const int FRAME_SIZE = 32768; // (2^15) points
	int remaining = wave.size();
	int offset = 0;

	std::vector<ComplexArray> frames;

	// Split the wave into frames of 32768 points each
	while (remaining > 0)
	{
		ComplexArray frame = ComplexArray(FRAME_SIZE);
		if (remaining - FRAME_SIZE >= 0)
		{
			for (int i = 0; i < FRAME_SIZE; ++i)
			{
				frame[i] = wave[offset + i];
			}
			remaining -= FRAME_SIZE;
			offset += FRAME_SIZE;
		}
		else
		{
			for (int i = 0; i < wave.size() - offset; ++i)
			{
				frame[i] = wave[offset + i];
			}
			remaining -= (wave.size() - offset);
			offset += (wave.size() - offset);
		}
		frames.push_back(frame);
	}

	// Resize the wave array to fit all the frames inside
	wave.resize(frames.size() * FRAME_SIZE);
	offset = 0;

	// Perform the IFFT on each frame, no need to pad because the frames are padded individually
	// to 32768 points each, which is 2^15 so will work nicely in FFT
	for (ComplexArray frame : frames)
	{
		// Convert to time domain
		IFFT(frame);

		// Combine each frames back into a wave
		for (int i = 0; i < frame.size(); ++i)
		{
			wave[offset + i] = frame[i];
		}
		offset += frame.size();
	}

	// Once all the frames have passed through the IFFT and been added back into the wave,
	// the wave should have the waveform that would create the frequency spectrums passed in.
	return;
}

/////////////////////////////////////////////////////////////////////////////////
// Implementation of the ISfx Interface
// Change the name of the 
class Sfx : public ISfx
{
private:
	const char* name = "Low Pass Filter";
	std::ofstream logfile;

	void Log(std::string message)
	{
		if (!logfile || logfile.fail())
			// Opens the file for output and truncates the file to delete any previous content
			// See: https://stackoverflow.com/questions/17032970/clear-data-inside-text-file-in-c
			logfile.open(name, std::ofstream::out | std::ofstream::trunc);

		logfile << message;
	}
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
	this->Log("Performing SFX: " + std::string(this->name) + "\n");

	// Set the threshold
	const double FREQ_THRESHOLD = 150;
	this->Log("Using threshold: " + std::to_string(FREQ_THRESHOLD) + "\n");

	// Get the frequency spectrum of the wave - the analysable part of the wave
	STFT(arr);

	// Log the values after the STFT
	this->Log("STFT values:\n");
	for (int i = 0; i < arr.size(); ++i)
	{
		this->Log("{ i:" + std::to_string(i) + " real:" + std::to_string(arr[i].real()) + ", imag:" + std::to_string(arr[i].imag()) + " } ");
	}

	// Remove all frequencies above the threshold
	this->Log(std::string(this->name) + " - flattened values:\n");
	for (int i = 0; i < arr.size(); ++i)
	{
		// Round to 3dp so we only flatten the peaks that are not negligible size
		double rounded = std::round(std::abs(arr[i]) * 1000.0) / 1000.0;

		// If the frequency (after rounding) is greater than the smallest possible value stored in a double - meaning a peak that we need to check
		// Then flatten the peak if i, the frequency of the peak we are analysing, is greater than the threshold
		if (rounded > std::numeric_limits<double>::epsilon())
		{
			if (FREQ_THRESHOLD <= i)
			{
				arr[i] /= 4;
				this->Log("{ i:" + std::to_string(i) + " real:" + std::to_string(arr[i].real()) + ", imag:" + std::to_string(arr[i].imag()) + " } ");
			}
		}
	}

	// Get the time spectrum of the wave back - the audible part of the wave
	STIFT(arr);

	this->Log("STIFT values:\n");
	for (int i = 0; i < arr.size(); ++i)
	{
		this->Log("{ i:" + std::to_string(i) + " real:" + std::to_string(arr[i].real()) + ", imag:" + std::to_string(arr[i].imag()) + " } ");
	}
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