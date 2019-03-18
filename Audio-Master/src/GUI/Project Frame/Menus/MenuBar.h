#ifndef INC_MENU_BAR_H
#define INC_MENU_BAR_H

#include <wx/menu.h>
#include <wx/frame.h>

#include "FileMenu.h"
#include "EffectsMenu.h"

#include "..\..\..\Core\Logger.h"

namespace AudioMaster
{

	class MenuBar final : public wxMenuBar
	{
	private:
		wxMenu* fileMenu;
		wxMenu* effectsMenu;

	public:
		MenuBar();
		~MenuBar();

		std::map<std::string, int> GetEffects();
		std::vector<fs::path> GetEffectsDLLs();
	};
	
}

#endif