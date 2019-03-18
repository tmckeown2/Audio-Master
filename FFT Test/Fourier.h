#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <complex>
#include <valarray>
#include <vector>

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

CArray GetComplexWave(std::vector<double> frequencies, int numPoints);

void  FFT(CArray& x);
void IFFT(CArray& x);

void RemoveFreq(CArray& x, double freqLower, double freqHigher);