#ifndef INC_SOUNDMANAGER_H
#define INC_SOUNDMANAGER_H

#include <string>
#include <vector>
#include <thread>

#include "al.h"
#include "alc.h"

#include "../AudioMaster.h"
#include "Utils/Sound.h"
#include "Utils/Recorder.h"
#include "Utils/Player.h"
#include "Wrappers/Wave Wrapper/WaveWrapper.h"

namespace AudioMaster
{

	class SoundManager
	{
	private:
		static SoundManager* instance;
		
		static ALenum errorCode;

		static bool recording;

		static std::thread recordingThread;

		Sound* sound;
		Recorder* r;
		Player* p;

		time_t lastRecorded;
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
		void SetInputFormat(ALenum format);

		void  SetSound(Sound* s);
		Sound* GetSound();

		void Import(std::string file);
		void Export(std::string file);

		void Play();
		void Pause();
		void Record();
		void Stop();

		time_t LastRecorded();
	};
	
}

#endif