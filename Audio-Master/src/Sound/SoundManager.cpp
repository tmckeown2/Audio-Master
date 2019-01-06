#include "SoundManager.h"
#include <fstream>

// TODO: PRIORITY!!! Record to file (temp fix. Should really record to Virtual Memory Pages) 
// TODO: Read in the file for WaveDisplay (implement own WaveHeader reading for cross platform)

namespace AudioMaster
{
	SoundManager* SoundManager::instance = nullptr;
	Logger*		  SoundManager::logger   = nullptr;

	ALenum SoundManager::errorCode = 0;

	bool SoundManager::recording = false;

	std::thread SoundManager::recordingThread = std::thread();

    SoundManager::SoundManager()
    {
		// Get the logger
		this->logger = Logger::GetInstance();

		this->errorCode = AL_NO_ERROR;

		// Set default input/output device strings
		this->SetInput (alGetString(ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER));
		this->SetOutput(alGetString(ALC_DEFAULT_DEVICE_SPECIFIER));

		this->recording = false;

		this->sound = Sound();
    }
    SoundManager::~SoundManager()
    {

    }

	SoundManager* SoundManager::GetInstance()
	{
		if (SoundManager::instance == nullptr)
		{
			SoundManager::instance = new SoundManager();
		}

		return SoundManager::instance;
	}
	void SoundManager::ResetInstance()
	{
		delete SoundManager::instance;
		SoundManager::instance = nullptr;
	}

	std::vector<std::string> SoundManager::GetInputDevices()
	{
		std::vector<std::string> inputDevices;

		// Check if input devices are available
		if (alcIsExtensionPresent(nullptr, "ALC_EXT_CAPTURE") == ALC_TRUE)
		{
			// Check if the device lists can be enumerated
			if (alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT") == ALC_TRUE)
			{
				// Get the list of NULL('\0') separated device names
				const ALCchar* input = alcGetString(nullptr, ALC_CAPTURE_DEVICE_SPECIFIER);

				const ALCchar* next = input;
				
				// Move to the end of the next word
				// If the next character is NULL('\0') then the string is over
				// Otherwise there is a new word
				while (*(next += strlen(input) + 1) != '\0')
				{
					// Push onto the vector
					inputDevices.push_back(input);

					// Get the next string
					input = next;
				}

				inputDevices.push_back(input);
			}
			else
			{
				// TODO: Log error - return default device
				const ALCchar* input = alcGetString(nullptr, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);

				inputDevices.push_back(input);
			}
		}
		else
		{
			// TODO: Log error - return empty list
		}

		return inputDevices;
	}
	std::vector<std::string> SoundManager::GetOutputDevices()
	{
		std::vector<std::string> outputDevices;

		// Check if the device lists can be enumerated
		if (alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT") == ALC_TRUE)
		{
			// Check if the all the output devices can be enumerated
			if (alcIsExtensionPresent(nullptr, "ALC_ENUMERATE_ALL_EXT") == ALC_TRUE)
			{
				const ALCchar* output = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);

				const ALCchar* next = output;

				// Move to the end of the next word
				// If the next character is NULL('\0') then the string is over
				// Otherwise there is a new word
				while (*(next += strlen(output) + 1) != '\0')
				{
					// Push onto the vector
					outputDevices.push_back(output);

					// Get the next string
					output = next;
				}

				outputDevices.push_back(output);
			}
			else
			{
				// TODO: Log error - return default device
				const ALCchar* output = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);

				outputDevices.push_back(output);
			}
		}
		else
		{
			// TODO: Log error - return empty list
		}

		return outputDevices;
	}

	bool SoundManager::SetInput (const char* input)
	{
		if (recording)
		{
			this->logger->Log("Cannot set input, already recording");
			return false;
		}

		return true;
	}
	bool SoundManager::SetOutput(const char* output)
	{

		return true;
	}

	void SoundManager::SetInputFrequency(unsigned int frequency)
	{
		this->sound.sampleRate = frequency;
	}
	void SoundManager::SetBufferSize(int size)
	{
		
	}

	Sound* SoundManager::GetSound()
	{
		this->sound = this->r.GetSound();
		return &this->sound;
	}

	void SoundManager::Import(std::string file)
	{
		// TODO: Add file type validation to import/export functions
		// TODO: Make import/export functions actually do something with sound data

		WaveWrapper f = WaveWrapper();
		Sound s = f.Import(file.c_str());
	}
	void SoundManager::Export(std::string file)
	{
		WaveWrapper f = WaveWrapper();
		f.Export(file.c_str(), this->sound);
	}

	void SoundManager::Play()
	{
		p.Start();
	}
	void SoundManager::Pause()
	{
		p.Pause();
	}
	void SoundManager::Record()
	{
		r.SetFormat(AL_FORMAT_MONO8);
		r.SetSampleRate(44100);

		r.Start();
		this->recording = true;
	}
	void SoundManager::Stop()
	{
		if (this->recording)
		{
			r.Stop();

			this->sound = r.GetSound();
			p.SetSound(this->sound);

			this->Export("test.wav");

			this->recording = false;
		}

		p.Stop();
	}
}