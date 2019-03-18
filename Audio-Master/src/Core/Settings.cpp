#include "Settings.h"
#include <string>
#include <fstream>
#include <iomanip>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace AudioMaster
{
	json GetSettingsFile();
	void SaveSettingsFile(json file);

	bool CreateSettingsFile()
	{
		SaveSettingsFile(json());

		return true;
	}

	json GetSettingsFile()
	{
		std::ifstream infile(SETTINGS_FILE_PATH);
		json file;
		infile >> file;
		infile.close();

		return file;
	}

	void SaveSettingsFile(json file)
	{
		std::ofstream outfile(SETTINGS_FILE_PATH);
		outfile << std::setw(4) << file << std::endl;
	}

	void SaveSetting(const char* setting, const char* value)
	{
		// Load the current settings
		json file = GetSettingsFile();

		// Save the setting (updates the existing setting or creates a new one if it doesn't exist)
		file[setting] = value;

		// Write the new settings file
		SaveSettingsFile(file);
	}
	void SaveSetting(const char* setting, int value)
	{
		// Load the current settings
		json file = GetSettingsFile();

		// Save the setting (updates the existing setting or creates a new one if it doesn't exist)
		file[setting] = value;

		// Write the new settings file
		SaveSettingsFile(file);
	}
	void SaveSetting(const char* setting, double value)
	{
		// Load the current settings
		json file = GetSettingsFile();

		// Save the setting (updates the existing setting or creates a new one if it doesn't exist)
		file[setting] = value;

		// Write the new settings file
		SaveSettingsFile(file);
	}
	void SaveSetting(const char* setting, bool value)
	{
		// Load the current settings
		json file = GetSettingsFile();

		// Save the setting (updates the existing setting or creates a new one if it doesn't exist)
		file[setting] = value;

		// Write the new settings file
		SaveSettingsFile(file);
	}

	const char* LoadSetting(const char* setting, const char* defaultValue)
	{
		// Load the current settings
		json file = GetSettingsFile();

		// If the setting is not found then return the default value
		if (file.find(setting) == file.end())
		{
			return defaultValue;
		}

		// Otherwise, return the setting value
		return file[setting].get<std::string>().c_str();
	}
	int LoadSetting(const char* setting, int defaultValue)
	{
		// Load the current settings
		json file = GetSettingsFile();

		// If the setting is not found then return the default value
		if (file.find(setting) == file.end())
		{
			return defaultValue;
		}

		// Otherwise, return the setting value
		return file[setting].get<int>();
	}
	double LoadSetting(const char* setting, double defaultValue)
	{
		// Load the current settings
		json file = GetSettingsFile();

		// If the setting is not found then return the default value
		if (file.find(setting) == file.end())
		{
			return defaultValue;
		}

		// Otherwise, return the setting value
		return file[setting].get<double>();
	}
	bool LoadSetting(const char* setting, bool defaultValue)
	{
		// Load the current settings
		json file = GetSettingsFile();

		// If the setting is not found then return the default value
		if (file.find(setting) == file.end())
		{
			return defaultValue;
		}

		// Otherwise, return the setting value
		return file[setting].get<bool>();
	}
}