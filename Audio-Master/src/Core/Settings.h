#ifndef INC_SETTINGS_H
#define INC_SETTINGS_H

namespace AudioMaster
{
	constexpr const char* SETTINGS_FILE_PATH = "data\\settings.json";

	bool CreateSettingsFile();

	void SaveSetting(const char* setting, const char* value);
	void SaveSetting(const char* setting, int		  value);
	void SaveSetting(const char* setting, double	  value);
	void SaveSetting(const char* setting, bool		  value);

	const char* LoadSetting(const char* setting, const char* defaultValue);
	int			LoadSetting(const char* setting, int		 defaultValue);
	double		LoadSetting(const char* setting, double		 defaultValue);
	bool		LoadSetting(const char* setting, bool		 defaultValue);
}

#endif