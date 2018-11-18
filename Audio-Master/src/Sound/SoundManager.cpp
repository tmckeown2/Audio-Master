#include "SoundManager.h"

namespace AudioMaster
{
	SoundManager* SoundManager::instance = nullptr;

    SoundManager::SoundManager()
    {
		this->input = nullptr;
		this->output = nullptr;
    }
    SoundManager::~SoundManager()
    {
		this->input = nullptr;
		this->output = nullptr;
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
}