#ifndef INC_SOUNDMANAGER_H
#define INC_SOUNDMANAGER_H

#include <string>
#include <vector>

#include "al.h"
#include "alc.h"
#include "efx.h"
#include "efx-creative.h"
#include "EFX-Util.h"
#include "xram.h"

namespace AudioMaster
{

	class SoundManager
	{
	private:
		static SoundManager* instance;

		ALCdevice* input;
		ALCdevice* output;

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

		bool SetInput(std::string inputDevice);
		bool SetOutput(std::string outputDevice);

	};
	
}

#endif