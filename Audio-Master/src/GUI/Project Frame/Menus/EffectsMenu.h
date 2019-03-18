#ifndef INC_EFFECTSMENU_H
#define INC_EFFECTSMENU_H

#include <vector>
#include <filesystem>
#include <map>
#include <wx/menu.h>

#include "..\..\..\Core\Logger.h"

//namespace fs = std::filesystem; // C++17 - https://en.cppreference.com/w/cpp/header/filesystem
namespace fs = std::experimental::filesystem; // C++14 - https://en.cppreference.com/w/cpp/header/experimental/filesystem

namespace AudioMaster
{

	class EffectsMenu final : public wxMenu
	{
	private:
		std::map<std::string, int> effects;
		std::vector<fs::path> dllPaths;

		int GetNextEffectID();

	public:
		EffectsMenu();
		~EffectsMenu();

		std::map<std::string, int> GetEffects();
		std::vector<fs::path> GetDLLs();
	};
	
}

#endif