#ifndef INC_PROJECT_FRAME_H
#define INC_PROJECT_FRAME_H

#include <wx/frame.h>
#include <wx/event.h>
#include <wx/aui/framemanager.h>

#include "Menus/MenuBar.h"

#include "Toolbars/ControlToolbar.h"
#include "Toolbars/IOToolbar.h"

// Include the core files used in every file
#include "../../AudioMaster.h"

namespace AudioMaster
{

	class ProjectFrame final : public wxFrame
	{
	private:
		Logger* logger;
		wxAuiManager auiManager;

		wxPoint pos;
		wxSize size;

		// Menus
		MenuBar* menuBar;

		// Toolbars
		ControlToolbar* controlBar;
		IOToolbar* ioBar;

	public:
		ProjectFrame(wxString title, wxPoint pos, wxSize size, bool maximised);
		~ProjectFrame();

		void OnClose(wxCloseEvent& e);
		void OnResize(wxSizeEvent& e);
		void OnMove(wxMoveEvent& e);

	private:
		void InitMenus();
		void InitToolbars();

		wxDECLARE_EVENT_TABLE();
	};
	
}

#endif