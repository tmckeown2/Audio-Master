#include "ProjectFrame.h"

// Outputs memory leaks after the program has closed
#ifdef __WXMSW__
#include <wx/msw/msvcrt.h> // redefines the new() operator 
#endif

namespace AudioMaster
{
	wxBEGIN_EVENT_TABLE(ProjectFrame, wxFrame)

		EVT_CLOSE	(ProjectFrame::OnClose	)
		EVT_SIZE	(ProjectFrame::OnResize	)
		EVT_MOVE_END(ProjectFrame::OnMove	)

	wxEND_EVENT_TABLE()

    ProjectFrame::ProjectFrame(wxString title, wxPoint pos, wxSize size, bool maximised)
		: wxFrame(NULL, wxID_ANY, title, pos, size)
    {
		// Get the logger
		this->logger = Logger::GetInstance();

		this->logger->Log("Initialising Project Frame");

		// Set position and size
		this->pos = pos;
		this->size = size;

		// Set maximised state
		this->Maximize(maximised);

		// Initialise the AUI manager
		this->logger->Log("Initialising Advanced UI manager");
		this->auiManager.SetManagedWindow(this);

		// Initialise the displays - This MUST be before the menu init because for some reason it causes an access violation if you don't
		this->logger->Log("Initialising displays");
		this->InitDisplays();

		// Initialise the menus
		this->logger->Log("Initialising menu bar");
		this->InitMenus();

		// Initialise the toolbars
		this->logger->Log("Initialising toolbars");
		this->InitToolbars();
    }

    ProjectFrame::~ProjectFrame()
    {
		SoundManager::ResetInstance();

        this->auiManager.UnInit();
    }

	void ProjectFrame::InitMenus()
	{
		// Create the menu bar
		this->menuBar = new MenuBar();

		// Set the menu bar
		this->SetMenuBar(this->menuBar);
	}

	void ProjectFrame::InitToolbars()
	{
		// Create the toolbars
		this->controlBar = new ControlToolbar(this);
		this->ioBar = new IOToolbar(this);

		// Setup each toolbar's pane info
		wxAuiPaneInfo controlBarInfo;
		controlBarInfo
			.Name("ControlToolbar")
			.Caption("Control Toolbar")
			.ToolbarPane()
			.Top()
			.LeftDockable(false)
			.RightDockable(false)
			.BottomDockable(false)
			.Floatable(false);

		wxAuiPaneInfo ioBarInfo;
		ioBarInfo
			.Name("IOToolbar")
			.Caption("IO Toolbar")
			.ToolbarPane()
			.Top()
			.LeftDockable(false)
			.RightDockable(false)
			.BottomDockable(false)
			.Floatable(false);

		// Add the toolbars as AUI panes
		this->auiManager.AddPane(this->controlBar, controlBarInfo);
		this->auiManager.AddPane(this->ioBar, ioBarInfo);

		// Tell the manager to commit all the changes that were made
		this->auiManager.Update();
	}

	void ProjectFrame::InitDisplays()
	{
		// Create the displays
		this->waveformDisplay = new WaveformDisplay(this, wxSize(this->GetSize().GetWidth() - 20, this->GetSize().GetHeight() / 4));

		// Setup each display's pane info
		wxAuiPaneInfo waveDisplayInfo;
		waveDisplayInfo
			.Name(_("WaveformDisplay"))
			.Caption(_("Waveform Display"))
			.DefaultPane()
			.CloseButton(false)
			.Top()
			.LeftDockable(false)
			.RightDockable(false)
			.MinSize(wxSize(500, 200));

		// Add the pane
		this->auiManager.AddPane(this->waveformDisplay, waveDisplayInfo);

		// Tell the manager to commit all the changes that were made
		this->auiManager.Update();

		// Refresh the displays
		this->UpdateDisplays();
	}

	void ProjectFrame::UpdateDisplays()
	{
		this->waveformDisplay->Refresh();
	}

#pragma region FRAME EVENTS

	void ProjectFrame::OnClose(wxCloseEvent& e)
	{
		// Save the new main window settings (position, size, and maximised)
		SaveSetting(MAIN_WINDOW_KEY, WINDOW_X_KEY, this->pos.x);
		SaveSetting(MAIN_WINDOW_KEY, WINDOW_Y_KEY, this->pos.y);

		SaveSetting(MAIN_WINDOW_KEY, WINDOW_WIDTH_KEY,  this->size.GetWidth ());
		SaveSetting(MAIN_WINDOW_KEY, WINDOW_HEIGHT_KEY, this->size.GetHeight());

		SaveSetting(MAIN_WINDOW_KEY, WINDOW_MAX_KEY, this->IsMaximized());

		// Call the base class close function
		e.Skip();
	}

	void ProjectFrame::OnResize(wxSizeEvent& e)
	{
		// Get the size of the frame when not maximised
		if (!this->IsMaximized())
		{
			this->size = e.GetSize();
		}

		// Update the displays
		this->UpdateDisplays();
	}

	void ProjectFrame::OnMove(wxMoveEvent& WXUNUSED(e))
	{
		// Get the position of the frame when not maximised
		if (!this->IsMaximized())
		{
			this->pos = this->GetPosition();
		}
	}

#pragma endregion

}