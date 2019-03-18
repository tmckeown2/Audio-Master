#include "Player.h"

namespace AudioMaster
{
    ALCdevice*  Player::device		 = nullptr;
	ALCcontext* Player::context		 = nullptr;
	const char* Player::outputDevice = "";

	Player::Player()
	{
		this->sound = new Sound();

		if (Player::device == nullptr)
		{
			Player::device = alcOpenDevice(0);
		}
		if (Player::context == nullptr)
		{
			Player::context = alcCreateContext(Player::device, 0);
			alcMakeContextCurrent(Player::context);
		}

		alListener3f(AL_POSITION,	 0.0f, 0.0f, 0.0f);
		alListener3f(AL_VELOCITY,	 0.0f, 0.0f, 0.0f);
		alListener3f(AL_ORIENTATION, 0.0f, 0.0f, 0.0f);
	}

	Player::~Player()
	{
		alDeleteBuffers	 (1, &this->buffer);
		alDeleteSources	 (1, &this->source);
		alcDestroyContext(Player::context);
		alcCloseDevice	 (Player::device);

		//delete this->sound;
	}

	void Player::SetOutputDevice(const char* device)
	{
		Player::outputDevice = device;

		alcCloseDevice(Player::device);
		alcDestroyContext(Player::context);

		Player::device = alcOpenDevice(Player::outputDevice);
		Player::context = alcCreateContext(Player::device, 0);
		alcMakeContextCurrent(Player::context);
	}

	void Player::SetSound(Sound* s)
	{
		this->sound = s;

		// Generate the source
		alGenSources(1, &this->source);

		alSourcef (this->source, AL_PITCH,	  1.0f);
		alSourcef (this->source, AL_GAIN,	  1.0f);
		alSource3f(this->source, AL_POSITION, 0.0f, 0.0f, 0.0f);
		alSource3f(this->source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
		alSourcei (this->source, AL_LOOPING,  AL_FALSE);

		// Generate the buffer
		alGenBuffers(1, &this->buffer);

		// Get the sound format
		ALenum format = AL_FORMAT_MONO8;

		if		(this->sound->channels == 1 && this->sound->bitsPerSample ==  8)
			format = AL_FORMAT_MONO8;
		else if (this->sound->channels == 1 && this->sound->bitsPerSample == 16)
			format = AL_FORMAT_MONO16;
		else if (this->sound->channels == 2 && this->sound->bitsPerSample ==  8)
			format = AL_FORMAT_STEREO8;
		else if (this->sound->channels == 2 && this->sound->bitsPerSample == 16)
			format = AL_FORMAT_STEREO16;

		// Buffer the sound data
		alBufferData(this->buffer, format,    this->sound->data, this->sound->size, this->sound->sampleRate);
		alSourcei   (this->source, AL_BUFFER, this->buffer);
	}

	void Player::Start()
	{
		alSourcePlay(this->source);
	}

	void Player::Start(int position)
	{
		alSourcei(this->source, AL_SAMPLE_OFFSET, position);
		alSourcePlay(this->source);
	}

	void Player::Start(float time)
	{
		alSourcei(this->source, AL_SAMPLE_OFFSET, (int)(time * this->sound->sampleRate));
		alSourcePlay(this->source);
	}

	void Player::Pause()
	{
		alSourcePause(this->source);
	}

	void Player::Stop()
	{
		alSourceStop(this->source);
	}
}