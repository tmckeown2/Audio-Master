#include "Settings.h"
#include <Windows.h>

namespace am
{
	bool SaveSetting(const char* appKey, const char* fieldKey, const char* dataValue)
	{
		return WritePrivateProfileString(appKey, fieldKey, dataValue, SETTINGS_FILE_PATH);
	}

	const char* LoadSetting(const char* appKey, const char* fieldKey, const char* defaultValue)
	{
		TCHAR value[1000];
		GetPrivateProfileString(appKey, fieldKey, defaultValue, value, sizeof(value) / sizeof(value[0]), SETTINGS_FILE_PATH);

		return value;
	}
}