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
    }

    ProjectFrame::~ProjectFrame()
    {
        this->auiManager.UnInit();
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