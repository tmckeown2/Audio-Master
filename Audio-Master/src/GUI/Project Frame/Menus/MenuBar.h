#ifndef INC_MENU_BAR_H
#define INC_MENU_BAR_H

#include <wx/menu.h>
#include <wx/frame.h>

#include "FileMenu.h"

namespace AudioMaster
{

	class MenuBar final : public wxMenuBar
	{
	private:
		wxMenu* fileMenu;

	public:
		MenuBar();
		~MenuBar();
	};
	
}

#endif