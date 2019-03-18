#include "MenuBar.h"

// Outputs memory leaks after the program has closed
#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>      // redefines the new() operator 
#endif

namespace AudioMaster
{
    MenuBar::MenuBar()
		: wxMenuBar()
    {
		Logger::GetInstance()->Log("Creating Menu Bar");

		// Create the menus
		this->fileMenu = new FileMenu();
		this->effectsMenu = new EffectsMenu();

		// Add all the menus to the bar
		this->Append(this->fileMenu, "File");
		this->Append(this->effectsMenu, "Effects");
    }

    MenuBar::~MenuBar()
    {
		Logger::GetInstance()->Log("Destroying Menu Bar");
    }

	std::map<std::string, int> MenuBar::GetEffects()
	{
		return ((EffectsMenu*)this->effectsMenu)->GetEffects();
	}

	std::vector<fs::path> MenuBar::GetEffectsDLLs()
	{
		return ((EffectsMenu*)this->effectsMenu)->GetDLLs();
	}
}