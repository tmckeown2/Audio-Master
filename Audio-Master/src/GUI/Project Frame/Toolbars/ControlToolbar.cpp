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
		EVT_TOOL(CONTROL_TOOL_IMPORT, ControlToolbar::Import)	

	wxEND_EVENT_TABLE()

    ControlToolbar::ControlToolbar(wxFrame* parent)
		: wxAuiToolBar(parent, wxID_ANY)
    {
		Logger::GetInstance()->Log("Initialising Control Toolbar");
		
		// Initialise the tools and their images
		this->InitImages();
		this->InitTools(); 

		// Call Realize to setup the display size and positions of the new tools
		this->Realize();
    }

    ControlToolbar::~ControlToolbar()
    {
		Logger::GetInstance()->Log("Destroying Control Toolbar");
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
			"Stop.png",
			"Import.png"
		};

		// Load the images, if it cannot load then log an error and skip that file.
		// Do not want to cause a crash because an image did not load correctly, it is not a fatal issue.
		wxString resPath = wxStandardPaths::Get().GetResourcesDir() + "\\res\\";
		for (int i = 0; i < CONTROL_TOOL_MAX; ++i)
		{
			wxImage temp = wxImage(resPath + icons[i]);
			temp = temp.Rescale(30, 30, wxImageResizeQuality::wxIMAGE_QUALITY_HIGH);
			this->toolImg[i] = wxBitmap(temp);

			Logger::GetInstance()->Log("Loaded image: " + (resPath + icons[i]).ToStdString());
		}
	}

	void ControlToolbar::InitTools()
	{
		// Add all the tools
		this->AddTool(CONTROL_TOOL_PLAY,   _("Play"),   this->toolImg[CONTROL_TOOL_PLAY  ], _("Starts Playback"		    ));
		this->AddTool(CONTROL_TOOL_PAUSE,  _("Pause"),  this->toolImg[CONTROL_TOOL_PAUSE ], _("Pauses Playback"		    ));
		this->AddTool(CONTROL_TOOL_RECORD, _("Record"), this->toolImg[CONTROL_TOOL_RECORD], _("Starts Recording"		));
		this->AddTool(CONTROL_TOOL_STOP,   _("Stop"),   this->toolImg[CONTROL_TOOL_STOP  ], _("Stops Playback/Recording"));
		this->AddTool(CONTROL_TOOL_IMPORT, _("Import"), this->toolImg[CONTROL_TOOL_IMPORT], _("Imports some wave data"  ));
	}

	void ControlToolbar::Play(wxCommandEvent& WXUNUSED(e))
	{
		Logger::GetInstance()->Log("Playback clicked");
		SoundManager::GetInstance()->Play();
	}
	void ControlToolbar::Pause(wxCommandEvent& WXUNUSED(e))
	{
		Logger::GetInstance()->Log("Pause clicked");
		SoundManager::GetInstance()->Pause();
	}
	void ControlToolbar::Record(wxCommandEvent& WXUNUSED(e))
	{
		Logger::GetInstance()->Log("Record clicked");
		SoundManager::GetInstance()->Record();
	}
	void ControlToolbar::Stop(wxCommandEvent& WXUNUSED(e))
	{
		Logger::GetInstance()->Log("Stop clicked");
		SoundManager::GetInstance()->Stop();
	}
	void ControlToolbar::Import(wxCommandEvent& WXUNUSED(e))
	{
		Logger::GetInstance()->Log("Import clicked");
		SoundManager::GetInstance()->Import("test.wav");
	}
}