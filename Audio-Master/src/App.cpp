#include "App.h"
#include <string>

// See Automatic leak detection with Microsoft VisualC++ section of https://wiki.wxwidgets.org/Avoiding_Memory_Leaks
// Outputs memory leaks after the program has closed
#ifdef __WXMSW__
	#include <wx/msw/msvcrt.h> // redefines the new() operator 
#endif

namespace AudioMaster
{

	IMPLEMENT_APP(App);

	bool App::OnInit()
	{
		// Set the app name
		wxTheApp->SetAppName(AudioMaster::PROJECT_NAME);
		wxTheApp->SetAppDisplayName(AudioMaster::PROJECT_DISPLAY_NAME);

		::wxInitAllImageHandlers();

		// Reset the log file before writing anything to it
		Logger::GetInstance()->Log("Application Initialised and Running");

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
		Logger::GetInstance()->Log("Exiting application");
		Logger::ResetInstance();

		return 0;
	}

	void App::SetDefaultSettings()
	{
		Logger::GetInstance()->Log("Setting the default settings");

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
	}

	void App::LoadSettings()
	{
		Logger::GetInstance()->Log("Loading Settings");

		this->windowPos.x = LoadSetting("x", -1);
		this->windowPos.y = LoadSetting("y", -1);

		this->windowSize.SetWidth (LoadSetting("width",  -1));
		this->windowSize.SetHeight(LoadSetting("height", -1));
		
		this->windowMax = LoadSetting("fullscreen", false);
	}
}