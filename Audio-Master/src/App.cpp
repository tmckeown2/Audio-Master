#include "App.h"
#include <string>

// Outputs memory leaks after the program has closed
#ifdef __WXMSW__
	#include <wx/msw/msvcrt.h> // redefines the new() operator 
#endif

namespace AudioMaster
{

	IMPLEMENT_APP(App);

	bool App::OnInit()
	{
		// Get the logger
		this->logger = Logger::GetInstance();

		// Set the app name
		wxTheApp->SetAppName(AudioMaster::PROJECT_NAME);
		wxTheApp->SetAppDisplayName(AudioMaster::PROJECT_DISPLAY_NAME);

		// Load settings
		if (LoadSetting("installed", false) == false)
			this->SetDefaultSettings();
		else
			this->LoadSettings();

		// Create the project frame
		this->projectFrame = new ProjectFrame(wxString(PROJECT_DISPLAY_NAME), this->windowPos, this->windowSize, this->windowMax);
		this->projectFrame->Show(true);

		return true;
	}

	int App::OnExit()
	{
		Logger::ResetInstance();

		return 0;
	}

	void App::SetDefaultSettings()
	{
		// Create the settings file
		CreateSettingsFile();

		// Set main application fresh install to true
		SaveSetting("installed", true);

		// Set main window values to default
		SaveSetting("x", -1);
		SaveSetting("y", -1);
		SaveSetting("width", -1);
		SaveSetting("height", -1);

		SaveSetting("fullscreen", false);

		this->logger->Log("Saved default settings");
	}

	void App::LoadSettings()
	{
		this->windowPos.x = LoadSetting("x", -1);
		this->windowPos.y = LoadSetting("y", -1);

		this->windowSize.SetWidth (LoadSetting("width",  -1));
		this->windowSize.SetHeight(LoadSetting("height", -1));
		
		this->windowMax = LoadSetting("fullscreen", false);
	}
}