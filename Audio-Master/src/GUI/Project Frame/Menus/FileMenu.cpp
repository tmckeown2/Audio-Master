#include "FileMenu.h"

// Outputs memory leaks after the program has closed
#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>      // redefines the new() operator 
#endif

namespace AudioMaster
{
    FileMenu::FileMenu()
    {
		this->Append(EFileMenu::New,	"New",		"Creates a new project");
		this->Append(EFileMenu::Open,	"Open",		"Opens an existing project");
		this->Append(EFileMenu::Save,	"Save",		"Saves the current project");
		this->Append(EFileMenu::SaveAs, "Save As",	"Saves the current project under a new name");

		this->AppendSeparator();
		this->Append(EFileMenu::Quit,	"Quit",		"Quits the application");
    }

    FileMenu::~FileMenu()
    {
        
    }
}