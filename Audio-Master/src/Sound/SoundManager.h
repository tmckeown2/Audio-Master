#ifndef INC_SOUNDMANAGER_H
#define INC_SOUNDMANAGER_H

#include <string>
#include <vector>
#include <thread>

#include "al.h"
#include "alc.h"
#include "efx.h"
#include "efx-creative.h"
#include "EFX-Util.h"
#include "xram.h"

#include "../Core/Logger.h"

namespace AudioMaster
{

	class SoundManager
	{
	private:
		static SoundManager* instance;
		static Logger* logger;
		
		static ALenum errorCode;

		static const char* inputDeviceStr;
		static ALCdevice* input;
		static ALCuint inputFrequency;
		static ALCenum inputFormat;
		static ALCsizei inputBufferSize;
		static ALbyte* inputBuffer;

		static bool recording;

		static std::thread recordingThread;
	public:
		SoundManager();
		~SoundManager();

		// Specify no copy constructor or assignment operator to stop cloning the SoundManager singleton
		SoundManager(const SoundManager&) = delete;
		SoundManager& operator=(const SoundManager&) = delete;

		static SoundManager* GetInstance();
		static void ResetInstance();

		std::vector<std::string> GetInputDevices();
		std::vector<std::string> GetOutputDevices();

		bool SetInput(const char* inputDevice);
		bool SetOutput(const char* outputDevice);

		void SetInputFrequency(unsigned int frequency);
		void SetBufferSize(int size);

		void Import(std::string file);
		void Export(std::string file);

		void Play();
		void Pause();
		void Record();
		void Stop();

	private:
		static void Capture();
	};
	
}

#endif