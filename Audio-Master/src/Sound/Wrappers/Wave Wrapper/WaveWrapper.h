#ifndef INC_WAVEWRAPPER_H
#define INC_WAVEWRAPPER_H

#include <string>
#include <fstream>
#include <vector>

#include "..\ISoundWrapper.h"

namespace AudioMaster
{

	struct FMTCHUNK
	{
		short format;			// Type of compression (1 = PCM Linear Quantization)
		short channels;			// Number of channels (1 = Mono, 2 = Stereo)
		DWORD sampleRate;		// Number of samples per second
		DWORD bytesPerSecond;	// Average number of bytes per second
		short blockAlign;		// Number of bytes per block
		short bitsPerSample;	// Number of bits per sample (Low = 8-bit, High = 16-bit)
	};

	class WaveWrapper final : ISoundWrapper
	{
	private:
		void GetWAVChunkInfo(std::ifstream &file, std::string &name, unsigned int &size);

		void AddStringToFileData(std::vector<unsigned char> &fileData, std::string str);
		void AddInt32ToFileData (std::vector<unsigned char> &fileData, int   i, Endianness endianness = Endianness::LittleEndian);
		void AddInt16ToFileData (std::vector<unsigned char> &fileData, short i, Endianness endianness = Endianness::LittleEndian);

	public:
		Sound* Import(const char* filename);
		void   Export(const char* filename, Sound* data);
	};
	
}

#endif