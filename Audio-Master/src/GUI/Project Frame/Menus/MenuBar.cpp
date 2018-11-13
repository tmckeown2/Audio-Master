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
		// Create the menus
		this->fileMenu = new FileMenu();

		// Add all the menus to the bar
		this->Append(this->fileMenu, "File");
    }

    MenuBar::~MenuBar()
    {
        
    }
}