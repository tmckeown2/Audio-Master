#ifndef INC_PROJECTFRAME_H
#define INC_PROJECTFRAME_H

#include <wx/frame.h>
#include <wx/event.h>
#include <wx/aui/framemanager.h>

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

	public:
		ProjectFrame(wxString title, wxPoint pos, wxSize size, bool maximised);
		~ProjectFrame();

		void OnClose(wxCloseEvent& e);
		void OnResize(wxSizeEvent& e);
		void OnMove(wxMoveEvent& e);

	private:
		
		wxDECLARE_EVENT_TABLE();
	};
	
}

#endif