#include "Settings.h"

//rapidjson
#include "document.h" 
#include "writer.h"
#include "stringbuffer.h"

#include <Windows.h>
#include <fstream>
#include <string>

namespace AudioMaster
{
	bool CreateSettingsFile()
	{
		/*
		{
			"window": 
			{
				"x":-1,
				"y":-1,
				"width":-1,
				"height":-1
			}
		}
		*/
		const char* json = "{\"window\":{\"x\":-1,\"y\":-1,\"width\":-1,\"height\":-1,\"fullscreen\":0}}";
		rapidjson::Document d;
		d.Parse(json);

		rapidjson::Value& s = d["x"];
		s.SetInt(100);

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		d.Accept(writer);

		std::ofstream file(SETTINGS_FILE_PATH);

		if (!file || file.fail())
		{
			return false;
		}

		return true;
		//return CreateFile(SETTINGS_FILE_PATH, GENERIC_ALL, 0, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
	}

	bool SaveSetting(const char* appKey, const char* fieldKey, const char* dataValue)
	{
		return WritePrivateProfileString(appKey, fieldKey, dataValue, SETTINGS_FILE_PATH);
	}
	bool SaveSetting(const char* appKey, const char* fieldKey, int dataValue)
	{
		return WritePrivateProfileString(appKey, fieldKey, std::to_string(dataValue).c_str(), SETTINGS_FILE_PATH);
	}
	bool SaveSetting(const char* appKey, const char* fieldKey, bool dataValue)
	{
		return WritePrivateProfileString(appKey, fieldKey, std::to_string(dataValue).c_str(), SETTINGS_FILE_PATH);
	}
	bool SaveSetting(const char* appKey, const char* fieldKey, float dataValue)
	{
		return WritePrivateProfileString(appKey, fieldKey, std::to_string(dataValue).c_str(), SETTINGS_FILE_PATH);
	}
	bool SaveSetting(const char* appKey, const char* fieldKey, double dataValue)
	{
		return WritePrivateProfileString(appKey, fieldKey, std::to_string(dataValue).c_str(), SETTINGS_FILE_PATH);
	}

	const char* LoadSetting(const char* appKey, const char* fieldKey, const char* defaultValue)
	{
		TCHAR value[1000];
		GetPrivateProfileString(appKey, fieldKey, defaultValue, value, sizeof(value) / sizeof(value[0]), SETTINGS_FILE_PATH);

		// Return the data in value not the address
		return std::string(value).c_str();
	}
	int LoadSetting(const char* appKey, const char* fieldKey, int defaultValue)
	{
		return GetPrivateProfileInt(appKey, fieldKey, defaultValue, SETTINGS_FILE_PATH);
	}
	bool LoadSetting(const char* appKey, const char* fieldKey, bool defaultValue)
	{
		return GetPrivateProfileInt(appKey, fieldKey, defaultValue, SETTINGS_FILE_PATH);
	}
	float LoadSetting(const char* appKey, const char* fieldKey, float defaultValue)
	{
		TCHAR value[1000];
		std::string::size_type sz;
		GetPrivateProfileString(appKey, fieldKey, std::to_string(defaultValue).c_str(), value, sizeof(value) / sizeof(value[0]), SETTINGS_FILE_PATH);

		return std::stof(value, &sz);
	}
	double LoadSetting(const char* appKey, const char* fieldKey, double defaultValue)
	{
		TCHAR value[1000];
		std::string::size_type sz;
		GetPrivateProfileString(appKey, fieldKey, std::to_string(defaultValue).c_str(), value, sizeof(value) / sizeof(value[0]), SETTINGS_FILE_PATH);

		return std::stod(value, &sz);
	}
}