#ifndef INC_SFXUTILS_H
#define INC_SFXUTILS_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <complex>
#include <valarray>

#include "../Utils/Sound.h"

namespace AudioMaster
{
	typedef std::complex<double> Complex;
	typedef std::valarray<Complex> ComplexArray;

	class SFXUtils
	{
	private:
		static void FFT(ComplexArray& x);
		static void IFFT(ComplexArray& x);
		static void RemoveFrequencies(ComplexArray& x, double lower, double higher);

	public:
		static ComplexArray ToComplexArray(Sound* x);
		static void ToSound(ComplexArray& x, Sound* s);
	};
}

#endif