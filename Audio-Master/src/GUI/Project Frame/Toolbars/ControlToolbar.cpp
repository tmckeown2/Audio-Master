#include "ControlToolbar.h"

// Outputs memory leaks after the program has closed
#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>      // redefines the new() operator 
#endif

namespace AudioMaster
{
	wxBEGIN_EVENT_TABLE(ControlToolbar, wxAuiToolBar)

		EVT_TOOL(CONTROL_TOOL_PLAY,   ControlToolbar::Play)
		EVT_TOOL(CONTROL_TOOL_PAUSE,  ControlToolbar::Pause)
		EVT_TOOL(CONTROL_TOOL_RECORD, ControlToolbar::Record)
		EVT_TOOL(CONTROL_TOOL_STOP,   ControlToolbar::Stop)

	wxEND_EVENT_TABLE()

    ControlToolbar::ControlToolbar(wxFrame* parent)
		: wxAuiToolBar(parent, wxID_ANY)
    {
		// Get the logger
		this->logger = Logger::GetInstance();

		this->logger->Log("Initialising Control Toolbar");

		// Get the sound manager
		this->soundManager = SoundManager::GetInstance();

		// Initialise the tools and their images
		this->InitImages();
		this->InitTools();

		// Call Realize to setup the display size and positions of the new tools
		this->Realize();
    }

    ControlToolbar::~ControlToolbar()
    {
        
    }

	void ControlToolbar::InitImages()
	{
		// Get resources
		// TODO: Update to get resources using a macro or something
		std::string icons[CONTROL_TOOL_MAX] = 
		{
			"Play.png",
			"Pause.png",
			"Record.png",
			"Stop.png"
		};

		// Load the images, if it cannot load then log an error and skip that file.
		// Do not want to cause a crash because an image did not load correctly, it is not a fatal issue.
		for (int i = 0; i < CONTROL_TOOL_MAX; ++i)
		{
			this->toolImg[i] = wxBitmap(icons[i], wxBITMAP_TYPE_PNG);
		}
	}

	void ControlToolbar::InitTools()
	{
		// Add all the tools
		this->AddTool(CONTROL_TOOL_PLAY,   _("Play"),   this->toolImg[CONTROL_TOOL_PLAY  ], _("Starts Playback"		    ));
		this->AddTool(CONTROL_TOOL_PAUSE,  _("Pause"),  this->toolImg[CONTROL_TOOL_PAUSE ], _("Pauses Playback"		    ));
		this->AddTool(CONTROL_TOOL_RECORD, _("Record"), this->toolImg[CONTROL_TOOL_RECORD], _("Starts Recording"		));
		this->AddTool(CONTROL_TOOL_STOP,   _("Stop"),   this->toolImg[CONTROL_TOOL_STOP  ], _("Stops Playback/Recording"));
	}

	void ControlToolbar::Play(wxCommandEvent& WXUNUSED(e))
	{
		this->soundManager->Play();
	}
	void ControlToolbar::Pause(wxCommandEvent& WXUNUSED(e))
	{
		this->soundManager->Pause();
	}
	void ControlToolbar::Record(wxCommandEvent& WXUNUSED(e))
	{
		this->soundManager->Record();
	}
	void ControlToolbar::Stop(wxCommandEvent& WXUNUSED(e))
	{
		this->soundManager->Stop();
	}
}