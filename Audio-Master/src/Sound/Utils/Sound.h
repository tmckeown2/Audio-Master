#ifndef INC_SOUND_H
#define INC_SOUND_H

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
		}
	};
}

#endif