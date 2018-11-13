#ifndef INC_SETTINGS_H
#define INC_SETTINGS_H

namespace AudioMaster
{
	constexpr const char* SETTINGS_FILE_PATH = "data\\settings.ini";

	bool CreateSettingsFile();

	/// <summary>
	/// Saves a new entry / modifies an existing key-value pair under an app name in an initialisation file (*.ini).
	/// Returns <c>true</c> if successful and <c>false</c> if not successful.
	/// </summary>
	/// <param name="appKey">App to save the value under</param>
	/// <param name="fieldKey">Field to save the value to</param>
	/// <param name="dataValue">Value to save</param>
	/// <returns><c>true</c> if the setting saved successfully. Otherwise <c>false</c> is returned</returns>
	/// <remarks>This uses the Windows <c>WritePrivateProfileString</c> so is not portable yet.</remarks>
	bool SaveSetting(const char* appKey, const char* fieldKey, const char* dataValue);
	bool SaveSetting(const char* appKey, const char* fieldKey, int    dataValue);
	bool SaveSetting(const char* appKey, const char* fieldKey, bool   dataValue);
	bool SaveSetting(const char* appKey, const char* fieldKey, float  dataValue);
	bool SaveSetting(const char* appKey, const char* fieldKey, double dataValue);

	/// <summary>
	/// Loads an entry from an initialisation file (*.ini). 
	/// If the file does not exist then the default value is returned.
	/// </summary>
	/// <param name="appKey">App to load the value from</param>
	/// <param name="fieldKey">Filed to load the value from</param>
	/// <param name="defaultValue">Value to return if the key is not found in the file</param>
	/// <returns>C-String data value from the initialisation file or the default value</returns>
	/// <remarks>This uses the Windows <c>GetPrivateProfileString</c> so is not portable yet</remarks>
	const char* LoadSetting(const char* appKey, const char* fieldKey, const char* defaultValue);
	int    LoadSetting(const char* appKey, const char* fieldKey, int    defaultValue);
	bool   LoadSetting(const char* appKey, const char* fieldKey, bool   defaultValue);
	float  LoadSetting(const char* appKey, const char* fieldKey, float  defaultValue);
	double LoadSetting(const char* appKey, const char* fieldKey, double defaultValue);

#pragma region MAIN APP SETTINGS
	constexpr const char* MAIN_APP_KEY          = "MAIN";

	constexpr const char* FRESH_INSTALL_KEY     = "Fresh";
	constexpr const bool  FRESH_INSTALL_DEFAULT = true;
#pragma endregion

#pragma region MAIN WINDOW SETTINGS
	constexpr const char* MAIN_WINDOW_KEY       = "MAIN WINDOW";

	constexpr const char* WINDOW_X_KEY			= "Window X";
	constexpr const int   WINDOW_X_DEFAULT		= -1;

	constexpr const char* WINDOW_Y_KEY			= "Window Y";
	constexpr const int   WINDOW_Y_DEFAULT		= -1;

	constexpr const char* WINDOW_WIDTH_KEY		= "Window Width";
	constexpr const int   WINDOW_WIDTH_DEFAULT	= -1;

	constexpr const char* WINDOW_HEIGHT_KEY		= "Window Height";
	constexpr const int   WINDOW_HEIGHT_DEFAULT = -1;

	constexpr const char* WINDOW_MAX_KEY		= "Window Max";
	constexpr const bool WINDOW_MAX_DEFAULT		= false;
#pragma endregion
}

#endif