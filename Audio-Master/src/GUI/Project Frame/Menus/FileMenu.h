#ifndef INC_FILE_MENU_H
#define INC_FILE_MENU_H

#include <wx/menu.h>
#include <wx/frame.h>

#include "..\..\..\Core\Logger.h"

namespace AudioMaster
{

	enum EFileMenu
	{
		New		= wxID_NEW,
		Open	= wxID_OPEN,
		Save	= wxID_SAVE,
		SaveAs	= wxID_SAVEAS,
		Quit	= wxID_EXIT
	};

	class FileMenu final : public wxMenu
	{
	public:
		FileMenu();
		~FileMenu();
	};
	
}

#endif