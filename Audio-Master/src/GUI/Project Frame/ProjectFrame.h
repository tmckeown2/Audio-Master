#ifndef INC_PROJECT_FRAME_H
#define INC_PROJECT_FRAME_H

#include <wx/frame.h>
#include <wx/event.h>
#include <wx/aui/framemanager.h>

#include "../../Sound/SoundManager.h"
#include "../../Sound/SFX/SFXUtils.h"

#include "Menus/MenuBar.h"

#include "Toolbars/ControlToolbar.h"
#include "Toolbars/IOToolbar.h"

#include "Displays/WaveformDisplay.h"

// Include the core files used in every file
#include "../../AudioMaster.h"

namespace AudioMaster
{

	class ProjectFrame final : public wxFrame
	{
	private:
		wxAuiManager auiManager;

		wxPoint pos;
		wxSize size;

		// Menus
		MenuBar* menuBar;

		// Toolbars
		ControlToolbar* controlBar;
		IOToolbar* ioBar;

		// Displays
		WaveformDisplay* waveformDisplay;
		time_t lastUpdated;

		// Timers
		wxTimer refreshTimer;

	public:
		ProjectFrame(wxString title, wxPoint pos, wxSize size, bool maximised);
		~ProjectFrame();

		void OnClose(wxCloseEvent& e);
		void OnResize(wxSizeEvent& e);
		void OnMove(wxMoveEvent& e);
		void OnTimer(wxTimerEvent& e);

		void OnQuit(wxCommandEvent& e);

		void OnEffect(wxCommandEvent& e);

	private:
		void InitMenus();
		void InitToolbars();
		void InitDisplays();
		void InitPlugins();

		void UpdateDisplays();

		wxDECLARE_EVENT_TABLE();
	};
	
}

#endif