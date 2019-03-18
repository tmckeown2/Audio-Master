#include "Recorder.h"
#include <thread>

namespace AudioMaster
{
    Sound*				 Recorder::sound	 = nullptr;
	bool				 Recorder::recording = false;
	bool				 Recorder::capturing = false;
	std::vector<ALubyte> Recorder::rawData	 = std::vector<ALubyte>();
	const char*			 Recorder::device	 = NULL;

	Recorder::Recorder()
	{
		this->sound = new Sound();
	}

	Recorder::~Recorder()
	{
		//delete this->sound;
	}

	Sound* Recorder::GetSound() const
	{
		return sound;
	}

	void Recorder::SetInputDevice(const char* device)
	{
		Recorder::device = device;
	}

	void Recorder::SetSampleRate(int rate)
	{
		Recorder::sound->sampleRate = rate;
	}

	void Recorder::SetFormat(ALenum format)
	{
		if		(format == AL_FORMAT_MONO8)
		{
			Recorder::sound->channels	  = 1;
			Recorder::sound->bitsPerSample = 8;
		}
		else if (format == AL_FORMAT_MONO16)
		{
			Recorder::sound->channels	  = 1;
			Recorder::sound->bitsPerSample = 16;
		}
		else if (format == AL_FORMAT_STEREO8)
		{
			Recorder::sound->channels	  = 2;
			Recorder::sound->bitsPerSample = 8;
		}
		else if (format == AL_FORMAT_STEREO16)
		{
			Recorder::sound->channels	  = 2;
			Recorder::sound->bitsPerSample = 16;
		}
	}

	void Recorder::Start()
	{
		Recorder::recording = true;

		// Start the capture thread
		std::thread t([&](Recorder* r) { r->Capture(); }, this);
		t.detach();
	}

	void Recorder::Stop()
	{
		Recorder::recording = false;

		// Wait until the recording thread has captured and stored all the data in the Sound struct
		do { } while (Recorder::capturing);

		Recorder::rawData.clear();
	}

	void Recorder::Capture()
	{
		ALenum format = AL_FORMAT_MONO8;

		if		(Recorder::sound->channels == 1 && Recorder::sound->bitsPerSample ==  8)
			format = AL_FORMAT_MONO8;
		else if (Recorder::sound->channels == 1 && Recorder::sound->bitsPerSample == 16)
			format = AL_FORMAT_MONO16;
		else if (Recorder::sound->channels == 2 && Recorder::sound->bitsPerSample ==  8)
			format = AL_FORMAT_STEREO8;
		else if (Recorder::sound->channels == 2 && Recorder::sound->bitsPerSample == 16)
			format = AL_FORMAT_STEREO16;
		else
		{
			Recorder::sound->channels	  = 1;
			Recorder::sound->bitsPerSample = 8;
		}

		// Open the capture device
		int bufferSize = Recorder::sound->sampleRate / 5;

		ALCdevice* capture = alcCaptureOpenDevice(Recorder::device, Recorder::sound->sampleRate, format, bufferSize);

		// Calculate the block align
		int blockAlign = Recorder::sound->channels * (Recorder::sound->bitsPerSample / 8.0);

		// Create a buffer to hold the captured samples
		ALubyte* buffer = new ALubyte[bufferSize * Recorder::sound->channels];
		ALint	 numSamples;
	
		// Start capture
		Recorder::capturing = true;
		alcCaptureStart(capture);

		while (Recorder::recording)
		{
			alcGetIntegerv(capture, ALC_CAPTURE_SAMPLES, 1, &numSamples);

			if (numSamples >= bufferSize)
			{
				alcCaptureSamples(capture, buffer, numSamples / blockAlign);

				for (int i = 0; i < numSamples; ++i)
					Recorder::rawData.push_back(buffer[i]);
			}
		}

		// Stop capture
		alcCaptureStop(capture);
		alcCaptureCloseDevice(capture);

		// Store the data in the sound struct
		Recorder::sound->data = &Recorder::rawData[0];
		Recorder::sound->size =  Recorder::rawData.size();

		delete buffer; 

		Recorder::capturing = false;
	}
}