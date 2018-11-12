#include "DataManager.h"
#include <sys\stat.h>
#include <Windows.h>
#include <cstdio>
#include <string>
#include <regex>
#include <sstream>

namespace AudioMaster
{

	bool CreateDir(const char* path)
	{
		return (CreateDirectory(path, NULL) || GetLastError() == ERROR_ALREADY_EXISTS);
	}

	bool Exists(const char* path)
	{
		struct stat info;
		return (stat(path, &info) == 0);
	}

	const char* GetModuleDir()
	{
		char buffer[FILENAME_MAX];
		int bytes = GetModuleFileName(NULL, buffer, FILENAME_MAX);

		std::istringstream ss(buffer);
		std::string dir = "";
		std::string token;

		while (getline(ss, token, '\\'))
		{
			std::regex reg(".*\\.exe");
			std::smatch match;

			if (!regex_match(token, match, reg))
			{
				dir += token + "\\";
			}
		}

		return dir.c_str();
	}

}