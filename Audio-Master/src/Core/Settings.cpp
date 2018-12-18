#include "Settings.h"
#include <string>

#include "JsonBox.h"

namespace AudioMaster
{
	bool CreateSettingsFile()
	{
		JsonBox::Object windowObj;

		JsonBox::Value v(windowObj);
		v.writeToFile(SETTINGS_FILE_PATH);

		return true;
	}

	void SaveSetting(const char* setting, JsonBox::Value value)
	{
		// Load the current settings
		JsonBox::Value file;
		file.loadFromFile(SETTINGS_FILE_PATH);

		// Save the setting (updates the existing setting or creates a new one if it doesn't exist)
		file[setting] = value;

		// Write the new settings file
		file.writeToFile(SETTINGS_FILE_PATH);
	}

	void SaveSetting(const char* setting, const char* value)
	{
		SaveSetting(setting, JsonBox::Value(value));
	}
	void SaveSetting(const char* setting, int value)
	{
		SaveSetting(setting, JsonBox::Value(value));
	}
	void SaveSetting(const char* setting, double value)
	{
		SaveSetting(setting, JsonBox::Value(value));
	}
	void SaveSetting(const char* setting, bool value)
	{
		SaveSetting(setting, JsonBox::Value(value));
	}

	const char* LoadSetting(const char* setting, const char* defaultValue)
	{
		// Load the current settings
		JsonBox::Value file;
		file.loadFromFile(SETTINGS_FILE_PATH);

		// Try to get the string setting, if it is not there then return the default
		return file[setting].tryGetString(defaultValue).c_str();
	}
	int LoadSetting(const char* setting, int defaultValue)
	{
		// Load the current settings
		JsonBox::Value file;
		file.loadFromFile(SETTINGS_FILE_PATH);

		// Try to get the int setting, if it is not there then return the default
		return file[setting].tryGetInteger(defaultValue);
	}
	double LoadSetting(const char* setting, double defaultValue)
	{
		// Load the current settings
		JsonBox::Value file;
		file.loadFromFile(SETTINGS_FILE_PATH);

		// Try to get the double setting, if it is not there then return the default
		return file[setting].tryGetDouble(defaultValue);
	}
	bool LoadSetting(const char* setting, bool defaultValue)
	{
		// Load the current settings
		JsonBox::Value file;
		file.loadFromFile(SETTINGS_FILE_PATH);

		// Try to get the bool setting, if it is not there then return the default
		return file[setting].tryGetBoolean(defaultValue);
	}
}