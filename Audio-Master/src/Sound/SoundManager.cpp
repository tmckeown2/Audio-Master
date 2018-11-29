#include "SoundManager.h"

// TODO: PRIORITY!!! Recording MUST be done on a separate thread to avoid the crashing

namespace AudioMaster
{
	SoundManager* SoundManager::instance = nullptr;
	Logger*		  SoundManager::logger   = nullptr;

	ALenum SoundManager::errorCode = 0;

	const char* SoundManager::inputDeviceStr  = "";
	ALCdevice*	SoundManager::input			  = nullptr;
	ALCuint		SoundManager::inputFrequency  = 0;
	ALCenum		SoundManager::inputFormat	  = 0;
	ALCsizei	SoundManager::inputBufferSize = 0;
	ALbyte*		SoundManager::inputBuffer	  = nullptr;

	bool SoundManager::recording = false;

	std::thread SoundManager::recordingThread = std::thread();

    SoundManager::SoundManager()
    {
		// Get the logger
		this->logger = Logger::GetInstance();

		this->errorCode = AL_NO_ERROR;

		this->input = nullptr;
		this->inputFormat = AL_FORMAT_MONO16;
		this->inputFrequency = 48000; // DVD standard
		this->inputBufferSize = this->inputFrequency; // 1 second buffer size
		this->inputBuffer = new ALbyte[this->inputBufferSize];

		this->SetInput(NULL);
		this->SetOutput(NULL);

		this->recording = false;
    }
    SoundManager::~SoundManager()
    {
		//delete this->input;
		//delete this->output;

		alcCaptureStop(this->input);
		alcCaptureCloseDevice(this->input);
		//alcCloseDevice(this->output);

		delete[] this->inputBuffer;
		//delete[] this->source;

		this->input = nullptr;
		//this->output = nullptr;
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

	bool SoundManager::SetInput(const char* input)
	{
		if (recording)
		{
			this->logger->Log("Cannot set input, already recording");
			return false;
		}

		this->inputDeviceStr = input;

		return true;
	}
	bool SoundManager::SetOutput(const char* output)
	{
		//this->output = alcOpenDevice(output);
		//this->errorCode = alcGetError(this->output);

		if (this->errorCode != ALC_NO_ERROR)
		{
			// TODO: Log error - return false
			return false;
		}

		// Create a context
		//this->context = alcCreateContext(this->output, NULL);
		//alcMakeContextCurrent(this->context);

		//this->errorCode = alcGetError(this->output);

		if (this->errorCode != ALC_NO_ERROR)
		{
			// TODO: Log error - return false
			return false;
		}

		return true;
	}

	void SoundManager::SetInputFrequency(unsigned int frequency)
	{
		this->inputFrequency = frequency;
	}
	void SoundManager::SetBufferSize(int size)
	{
		this->inputBufferSize = size;
	}

	void SoundManager::Import(std::string file)
	{

	}
	void SoundManager::Export(std::string file)
	{

	}

	void SoundManager::Play()
	{
		
	}
	void SoundManager::Pause()
	{

	}
	void SoundManager::Record()
	{
		SoundManager::input = alcCaptureOpenDevice(SoundManager::inputDeviceStr, SoundManager::inputFrequency, SoundManager::inputFormat, SoundManager::inputBufferSize);
		SoundManager::errorCode = alcGetError(SoundManager::input);

		if (SoundManager::errorCode != ALC_NO_ERROR)
		{
			SoundManager::logger->Log("Unable to open input device");
			return;
		}

		SoundManager::recording = true;
		SoundManager::logger->Log("Started recording");

		// Start the recording thread
		SoundManager::recordingThread = std::thread([&](SoundManager* sm) { sm->Capture(); }, this);
		SoundManager::recordingThread.detach();
	}
	void SoundManager::Stop()
	{
		this->logger->Log("Stopping...");

		if (recording)
		{
			recording = false;
			
			alcCaptureStop(this->input);
			alcCaptureCloseDevice(this->input);

			this->logger->Log("Stopped recording");
		}
	}

	void SoundManager::Capture()
	{
		ALint sample;

		SoundManager::logger->Log("Started recording thread");
		while (SoundManager::recording)
		{
			alcGetIntegerv(SoundManager::input, ALC_CAPTURE_SAMPLES, (ALCsizei)sizeof(ALint), &sample);
			alcCaptureSamples(SoundManager::input, (ALCvoid*)SoundManager::inputBuffer, sample);

			// Output to a file

		}

		SoundManager::logger->Log("Stopped recording thread");
	}
}