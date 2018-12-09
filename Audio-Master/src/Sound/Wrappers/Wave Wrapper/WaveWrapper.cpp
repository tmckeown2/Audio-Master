#include "WaveWrapper.h"

namespace AudioMaster
{

	void WaveWrapper::GetWAVChunkInfo(std::ifstream &file, std::string &name, unsigned int &size)
	{
		char chunk[4];
		file.read((char*)&chunk, 4);
		file.read((char*)&size, 4);

		name = std::string(chunk, 4);
	}

	void WaveWrapper::AddStringToFileData(std::vector<unsigned char> &fileData, std::string str)
	{
		for (int i = 0; i < str.length(); ++i)
		{
			fileData.push_back((unsigned char)str[i]);
		}
	}
	void WaveWrapper::AddInt32ToFileData (std::vector<unsigned char> &fileData, int   i, Endianness endianness)
	{
		int bytes[4];

		if (endianness == Endianness::BigEndian)
		{
			bytes[0] = (i >> 24) & 0xff;
			bytes[1] = (i >> 16) & 0xff;
			bytes[2] = (i >> 8) & 0xff;
			bytes[3] = i & 0xff;
		}
		else if (endianness == Endianness::LittleEndian)
		{
			bytes[3] = (i >> 24) & 0xff;
			bytes[2] = (i >> 16) & 0xff;
			bytes[1] = (i >> 8) & 0xff;
			bytes[0] = i & 0xff;
		}

		for (int i = 0; i < 4; ++i)
		{
			fileData.push_back(bytes[i]);
		}
	}
	void WaveWrapper::AddInt16ToFileData (std::vector<unsigned char> &fileData, short i, Endianness endianness)
	{
		int bytes[2];

		if (endianness == Endianness::BigEndian)
		{
			bytes[0] = (i >> 8) & 0xff;
			bytes[1] = i & 0xff;
		}
		else if (endianness == Endianness::LittleEndian)
		{
			bytes[1] = (i >> 8) & 0xff;
			bytes[0] = i & 0xff;
		}

		for (int i = 0; i < 2; ++i)
		{
			fileData.push_back(bytes[i]);
		}
	}

	Sound& WaveWrapper::Import(const char* filename)
	{
		// Create a sound struct to store data in and return
		Sound sound = Sound();

		// Open the file
		std::ifstream file(filename, std::ios::in | std::ios::binary);

		if (!file || file.fail())
		{
			// Error here
			return sound;
		}

		std::string  chunkName;
		unsigned int chunkSize;

		// Loop through the file
		while (!file.eof())
		{
			// Get the next chunk information
			GetWAVChunkInfo(file, chunkName, chunkSize);

			// RIFF Chunk is the start of the wave file header
			// Indicates that the file is valid
			if (chunkName == "RIFF")
			{
				file.seekg(4, std::ios_base::cur);
			}
			// fmt  Chunk is the start of the format chunk
			// Contains the metadata such as format, number of channels, etc.
			else if (chunkName == "fmt ")
			{
				FMTCHUNK fmt;

				// Read the file's fmt chunk into the FMTCHUNK struct
				file.read((char*)&fmt, sizeof(FMTCHUNK));

				sound.format = fmt.format;
				sound.channels = fmt.channels;
				sound.sampleRate = fmt.sampleRate;
				sound.bitsPerSample = fmt.bitsPerSample;
			}
			// data Chunk is the start of the data part of the file
			// Contains the actual byte data of the wave file (2 channels = interleaved data 8-bit=[LR LR LR] 16-bit=[LLRR LLRR LLRR])
			else if (chunkName == "data")
			{
				sound.size = chunkSize;
				sound.data = new unsigned char[chunkSize];

				// Read the byte data into the sound struct
				file.read((char*)sound.data, chunkSize);

				// Avoids any possible duplication of the data chunk
				// This should work fine as "data" should be the final chunk
				break;
			}
			// We can safely ignore any potential other chunks as they are unnecessary
			else
			{
				file.seekg(chunkSize, std::ios_base::cur);
			}
		}

		// Set the sound's length
		sound.length = (double)sound.size / (sound.channels * sound.sampleRate * (sound.bitsPerSample / 8.0)) * 1000.0;

		// Close the file stream now that we are done with it
		file.close();

		return sound;
	}
	void   WaveWrapper::Export(const char* filename, Sound& data)
	{
		// Open the file
		std::ofstream file(filename, std::ios::out | std::ios::binary);
		file.imbue(std::locale::classic());

		if (!file || file.fail())
		{
			// Error here
			return;
		}

		std::vector<unsigned char> fileData;
		int dataChunkSize = data.size;

		// Create the fmt  Chunk
		FMTCHUNK fmt;
		fmt.format = data.format;
		fmt.channels = data.channels;
		fmt.sampleRate = data.sampleRate;
		fmt.bitsPerSample = data.bitsPerSample;
		fmt.blockAlign = data.channels * (data.bitsPerSample / 8.0);
		fmt.bytesPerSecond = data.sampleRate * fmt.blockAlign;

		// Write the RIFF Chunk
		AddStringToFileData(fileData, "RIFF");
		AddInt32ToFileData (fileData, 4 + sizeof(FMTCHUNK) + 8 + 8 + dataChunkSize);
		AddStringToFileData(fileData, "WAVE");

		// Write the fmt  Chunk
		AddStringToFileData(fileData, "fmt ");
		AddInt32ToFileData (fileData, sizeof(FMTCHUNK));
		AddInt16ToFileData (fileData, fmt.format);
		AddInt16ToFileData (fileData, fmt.channels);
		AddInt32ToFileData (fileData, fmt.sampleRate);
		AddInt32ToFileData (fileData, fmt.bytesPerSecond);
		AddInt16ToFileData (fileData, fmt.blockAlign);
		AddInt16ToFileData (fileData, fmt.bitsPerSample);

		// Write the data Chunk
		AddStringToFileData(fileData, "data");
		AddInt32ToFileData (fileData, dataChunkSize);

		for (int i = 0; i < data.size; i += data.channels)
		{
			for (int channel = 0; channel < data.channels; ++channel)
			{
				if (data.bitsPerSample == 8)
				{
					unsigned char byte = data.data[i + channel];
					fileData.push_back(byte);
				}
				else if (data.bitsPerSample == 16)
				{
					unsigned char bytes[2];
					bytes[0] = data.data[i + channel];
					bytes[1] = data.data[(i + channel) + 1];

					fileData.push_back(bytes[0]);
					fileData.push_back(bytes[1]);

					i++;
				}
			}
		}

		// Write data to file
		for (int i = 0; i < fileData.size(); ++i)
		{
			char value = (char)fileData[i];
			file.write(&value, sizeof(char));
		}

		file.close();
	}

}