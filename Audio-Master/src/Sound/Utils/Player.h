#ifndef INC_PLAYER_H
#define INC_PLAYER_H

#include "al.h"
#include "alc.h"

#include "Sound.h"

namespace AudioMaster
{

	class Player
	{
	private:
		static ALCdevice*  device;
		static ALCcontext* context;
		static const char* outputDevice;

		Sound* sound;
		ALuint source;
		ALuint buffer;

	public:
		Player();
		~Player();

		void SetOutputDevice(const char* output);
		void SetSound(Sound* s);

		void Start();
		void Start(int position);
		void Start(float time);
		void Pause();
		void Stop();
	};
	
}

#endif