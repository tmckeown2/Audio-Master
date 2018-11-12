#ifndef INC_SETTINGS_H
#define INC_SETTINGS_H

namespace am
{
	// Namespace wide constants used for Settings and Logging
	constexpr const char* PROJECT_NAME			= "AudioMaster";
	constexpr const char* SETTINGS_FILE_PATH	= "data\\settings.ini";

	// Save settings functions
	static bool SaveSetting(const char* appKey, const char* fieldKey, const char* dataValue);

	// Load settings functions
	static const char* LoadSetting(const char* appKey, const char* fieldKey, const char* defaultValue);

	// Main App Settings
	constexpr const char* MAIN_APP_KEY = "MAIN";

	constexpr const char* FRESH_INSTALL_KEY     = "Fresh";
	constexpr const bool  FRESH_INSTALL_DEFAULT = false;

	// Window Settings
	constexpr const char* MAIN_WINDOW_KEY = "MAIN WINDOW";

	constexpr const char* WINDOW_X_KEY			= "Window X";
	constexpr const int   WINDOW_X_DEFAULT		= -1;

	constexpr const char* WINDOW_Y_KEY			= "Window Y";
	constexpr const int   WINDOW_Y_DEFAULT		= -1;

	constexpr const char* WINDOW_WIDTH_KEY		= "Window Width";
	constexpr const int   WINDOW_WIDTH_DEFAULT	= -1;

	constexpr const char* WINDOW_HEIGHT_KEY		= "Window Height";
	constexpr const int   WINDOW_HEIGHT_DEFAULT = -1;
}

#endif