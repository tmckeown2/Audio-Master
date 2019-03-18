// Only load the DLLs at runtime if Windows is being used as DLLs are different for different OS (linux uses shared object *.so files instead, etc.)
// This is from Charphacy's answer at:
// https://stackoverflow.com/questions/2989810/which-cross-platform-preprocessor-defines-win32-or-win32-or-win32
#if defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <filesystem>
#include "SFX.h"
#endif

#include "ProjectFrame.h"

// Outputs memory leaks after the program has closed
#ifdef __WXMSW__
#include <wx/msw/msvcrt.h> // redefines the new() operator 
#endif

namespace AudioMaster
{
	wxBEGIN_EVENT_TABLE(ProjectFrame, wxFrame)

		EVT_CLOSE	(	ProjectFrame::OnClose  )
		EVT_SIZE	(	ProjectFrame::OnResize )
		EVT_MOVE_END(	ProjectFrame::OnMove   )
		EVT_TIMER   (1, ProjectFrame::OnTimer  )

		EVT_MENU(EFileMenu::Quit, ProjectFrame::OnQuit)

	wxEND_EVENT_TABLE()

    ProjectFrame::ProjectFrame(wxString title, wxPoint pos, wxSize size, bool maximised)
		: wxFrame(NULL, wxID_ANY, title, pos, size)
    {
		Logger::GetInstance()->Log("Initialising Project Frame");

		// Set position and size
		this->pos = pos;
		this->size = size;

		// Set maximised state
		this->Maximize(maximised);

		// Initialise the AUI manager
		Logger::GetInstance()->Log("Initialising Advanced UI manager");
		this->auiManager.SetManagedWindow(this);

		// Initialise the displays - This MUST be before the menu init because for some reason it causes an access violation if you don't
		Logger::GetInstance()->Log("Initialising displays");
		this->InitDisplays();

		// Initialise the menus
		Logger::GetInstance()->Log("Initialising menu bar");
		this->InitMenus();

		// Initialise the toolbars
		Logger::GetInstance()->Log("Initialising toolbars");
		this->InitToolbars();

		// Initialise the plugins
		Logger::GetInstance()->Log("Initialising toolbars");
		this->InitPlugins();

		// Initialise the timers
		this->refreshTimer.SetOwner(this, 1);
		this->refreshTimer.Start(1000);
    }

    ProjectFrame::~ProjectFrame()
    {
		Logger::GetInstance()->Log("Destroying Project Frame");

		// Stop any playback or recording if any are happening
		SoundManager::GetInstance()->Stop();
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

	void ProjectFrame::InitPlugins()
	{
		// Loop through the plugins in the "Effects" menu
		for (auto &effect : this->menuBar->GetEffects())
		{
			// Bind the effect event handler to all the effects
			// See Dynamic Event Handling section: https://docs.wxwidgets.org/3.1/overview_events.html
			Bind(wxEVT_COMMAND_MENU_SELECTED, &ProjectFrame::OnEffect, this, effect.second);
		}
	}

	void ProjectFrame::UpdateDisplays()
	{
		Logger::GetInstance()->Log("Refreshing Waveform Display");
		this->waveformDisplay->Refresh();
	}

#pragma region FRAME EVENTS

	void ProjectFrame::OnClose(wxCloseEvent& e)
	{
		Logger::GetInstance()->Log("Closing Project Frame");

		// Save the new main window settings (position, size, and maximised)
		Logger::GetInstance()->Log("Saving Settings");
		SaveSetting("x", this->pos.x);
		SaveSetting("y", this->pos.y);
		SaveSetting("width",  this->size.GetWidth ());
		SaveSetting("height", this->size.GetHeight());

		SaveSetting("fullscreen", this->IsMaximized());

		// Call the base class close function
		e.Skip();
	}

	void ProjectFrame::OnResize(wxSizeEvent& e)
	{
		Logger::GetInstance()->Log("Window Resized");

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
		Logger::GetInstance()->Log("Window Moved");

		// Get the position of the frame when not maximised
		if (!this->IsMaximized())
		{
			this->pos = this->GetPosition();
			std::string output = "Window Position : { x : " + std::to_string(this->pos.x) + ", y : " + std::to_string(this->pos.x) + " }";
			Logger::GetInstance()->Log(output);
		}
	}

	void ProjectFrame::OnTimer(wxTimerEvent& e)
	{
		if (SoundManager::GetInstance()->LastRecorded() > this->lastUpdated)
		{
			this->lastUpdated = time(NULL);
			Logger::GetInstance()->Log("New recording detected");
		}

		this->waveformDisplay->ForceRepaint();
	}

	void ProjectFrame::OnQuit(wxCommandEvent& WXUNUSED(e))
	{
		Logger::GetInstance()->Log("Closing Application Down");
		this->Close();
	}

	void ProjectFrame::OnEffect(wxCommandEvent& e)
	{
		Logger::GetInstance()->Log("Effect Triggered");

		// Only load the DLLs at runtime if Windows is being used as DLLs are different for different OS (linux uses shared object *.so files instead, etc.)
		// This is from Charphacy's answer at:
		// https://stackoverflow.com/questions/2989810/which-cross-platform-preprocessor-defines-win32-or-win32-or-win32
		#if defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)

		typedef SFXHANDLE (__stdcall *GETSFXPROC)(void);
		HINSTANCE hInstLib;
		GETSFXPROC getSfxFunc;

		// Find out which effect needs to be called - use e.GetId();
		std::vector<fs::path> effectPaths = this->menuBar->GetEffectsDLLs();
		if (effectPaths.size() > e.GetId())
			return;

		fs::path effectPath = effectPaths.at(e.GetId() - 1);

		// Open the library
		hInstLib = LoadLibraryW(effectPath.c_str());

		if (hInstLib != NULL)
		{
			Logger::GetInstance()->Log("Effect Library loaded into memory");

			// Try to find the address of the "GetSfx" function, without decoration
			getSfxFunc = (GETSFXPROC)GetProcAddress(hInstLib, "GetSfx");

			// If the undecorated function handle could not be found then try the decorated name
			if (getSfxFunc == NULL)
				getSfxFunc = (GETSFXPROC)GetProcAddress(hInstLib, "_GetSfx@0");

			// If the function was found and loaded correctly then attempt to get the Sfx object
			if (getSfxFunc != NULL)
			{
				Logger::GetInstance()->Log("Got the effect dll");

				SFXHANDLE sfx = getSfxFunc();

				// If the Sfx object is loaded correctly then attempt to apply the effect
				if (sfx != NULL)
				{
					Logger::GetInstance()->Log("Starting to perform the effect: ");

					// Get the sound from the sound manager
					Sound* s = SoundManager::GetInstance()->GetSound();
					Logger::GetInstance()->Log("\tGot the current recording");

					// Convert the sound to a complex array
					ComplexArray ca = SFXUtils::ToComplexArray(s);
					Logger::GetInstance()->Log("\tConverted the sound to a complex array");

					// Call the DoSfx function, from the exported class, on the array
					sfx->DoSFX(ca);
					Logger::GetInstance()->Log("\tEffect has been performed");
					
					// Convert the complex array back to a sound
					SFXUtils::ToSound(ca, s);
					Logger::GetInstance()->Log("\tConverting back to sound");

					// Set the sound in the sound manager - for playback
					SoundManager::GetInstance()->SetSound(s);
					Logger::GetInstance()->Log("\tSetting the playback sound");

					// Release the Sfx object from memory
					sfx->Release();
					Logger::GetInstance()->Log("\tReleased the sfx object from memory");
				}
			}
		}

		// Close the DLL
		if (!FreeLibrary(hInstLib))
		{
			Logger::GetInstance()->Log("Cannot free the SFX dll");
		}
		else
		{
			Logger::GetInstance()->Log("Freed the SFX dll");
		}

		#endif
	}

#pragma endregion

}