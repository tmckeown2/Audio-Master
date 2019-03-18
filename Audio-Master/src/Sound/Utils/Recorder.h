#ifndef INC_RECORDER_H
#define INC_RECORDER_H

#include "al.h"
#include "alc.h"

#include "Sound.h"

#include <vector>

namespace AudioMaster
{

	class Recorder
	{
	private:
		static Sound*				sound;
		static bool					recording;
		static bool					capturing;
		static std::vector<ALubyte> rawData;
		static const char*			device;

	public:
		Recorder();
		~Recorder();

		Sound* GetSound() const;

		void SetInputDevice(const char* device);
		void SetSampleRate(int rate);
		void SetFormat(ALenum format);

		void Start();
		void Stop();

	private:
		static void Capture();
	};
	
}

#endif