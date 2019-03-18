#ifndef INC_SOUND_H
#define INC_SOUND_H

#include "..\..\Core\Logger.h"

namespace AudioMaster
{
	typedef unsigned long DWORD;

	enum Endianness
	{
		LittleEndian,
		BigEndian
	};

	struct Sound
	{
		unsigned char*	data;	// Data array
		int				size;	// Data array's size
		double			length;	// Sound length

		short format;			// Format type
		short channels;			// Number of channels
		DWORD sampleRate;		// Samples per second
		short bitsPerSample;	// Number of bits per sample

		Sound()
		{
			data = nullptr;
			size = 0;
			length = 0.0;

			// Use a "safe" default format chunk
			format = 1;		// Default is PCM (Linear Quantisation)
			channels = 1;		// Default is Mono
			sampleRate = 44100;	// Default is CD standard
			bitsPerSample = 8;		// Default is 8-bit audio

			std::string output = "Sound : {\n";
			output += "\tSize : " + std::to_string(size) + "\n";
			output += "\tChannels : " + std::to_string(channels) + "\n";
			output += "\tSample Rate : " + std::to_string(sampleRate) + "\n";
			output += "\tBits Per Sample : " + std::to_string(bitsPerSample) + "\n";
			output += "}";
			Logger::GetInstance()->Log("Sound Created");
			Logger::GetInstance()->Log(output);
		}

		~Sound()
		{
			//delete[] this->data;
			this->data = nullptr;
			Logger::GetInstance()->Log("Sound Deleted");
		}

		void Resize(int newSize)
		{
			Logger::GetInstance()->Log("Sound Resized from " + std::to_string(size) + " to " + std::to_string(newSize));
			//delete[] data;
			data = new unsigned char[newSize];
			size = newSize;
		}
	};
}

#endif