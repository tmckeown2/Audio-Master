#include <iostream>
#include "Fourier.h"

using namespace std;

int main()
{
	vector<double> freq = { 1.0 };
	CArray wave = GetComplexWave(freq, 32);
	CArray fftWave = CArray(wave);

	FFT(fftWave);

	cout << "n\t\t" << "x\t\t" << "X" << endl;
	for (int i = 0; i < wave.size(); ++i)
	{
		// Round the results to 3dp
		double roundWave	= round(wave[i].real()  * 1000.0) / 1000.0;
		double roundFFTWave = round(abs(fftWave[i]) * 1000.0) / 1000.0;

		if (roundFFTWave > numeric_limits<double>::epsilon())
			cout << i << "\t\t" << roundWave << "\t\t" << roundFFTWave << endl;
	}

	CArray ifftWave(fftWave);
	IFFT(ifftWave);
	//CArray ifftWave(wave);
	//RemoveFreq(ifftWave, 18000.0, 25000.0);

	cout << endl << endl << endl;
	cout << "n\t\t" << "X\t\t" << "x" << endl;
	for (int i = 0; i < wave.size(); ++i)
	{
		// Round the results to 3dp
		double roundFFTWave = round(abs(fftWave[i])		* 1000.0) / 1000.0;
		double roundIFFTWave = round(ifftWave[i].real() * 1000.0) / 1000.0;

		if (roundFFTWave > numeric_limits<double>::epsilon())
			cout << i << "\t\t" << roundFFTWave << "\t\t" << roundIFFTWave << endl;
	}

	cin.get();
}