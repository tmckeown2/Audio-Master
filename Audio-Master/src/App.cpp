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
		if (LoadSetting(MAIN_APP_KEY, FRESH_INSTALL_KEY, FRESH_INSTALL_DEFAULT) == FRESH_INSTALL_DEFAULT)
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
		// Create the data directory (won't do anything if the directory exists)
		bool fileCheck = CreateDir(DATA_DIR);

		// Create a new settings file (won't do anything if the file exists)
		fileCheck = CreateSettingsFile();

		bool saved = true;

		// Set fresh install to false
		saved = SaveSetting(MAIN_APP_KEY, FRESH_INSTALL_KEY, false);

		// Set main window values to default
		saved = SaveSetting(MAIN_WINDOW_KEY, WINDOW_X_KEY,		WINDOW_X_DEFAULT	 );
		saved = SaveSetting(MAIN_WINDOW_KEY, WINDOW_Y_KEY,		WINDOW_Y_DEFAULT	 );
		saved = SaveSetting(MAIN_WINDOW_KEY, WINDOW_WIDTH_KEY,  WINDOW_WIDTH_DEFAULT );
		saved = SaveSetting(MAIN_WINDOW_KEY, WINDOW_HEIGHT_KEY, WINDOW_HEIGHT_DEFAULT);
		saved = SaveSetting(MAIN_WINDOW_KEY, WINDOW_MAX_KEY,    WINDOW_MAX_DEFAULT   );

		if (saved)
			this->logger->Log("Saved default settings");
		else
			this->logger->Log("Unable to save default settings");
	}

	void App::LoadSettings()
	{
		this->windowPos.x = LoadSetting(MAIN_WINDOW_KEY, WINDOW_X_KEY, WINDOW_X_DEFAULT);
		this->windowPos.y = LoadSetting(MAIN_WINDOW_KEY, WINDOW_Y_KEY, WINDOW_Y_DEFAULT);

		this->windowSize.SetWidth (LoadSetting(MAIN_WINDOW_KEY, WINDOW_WIDTH_KEY,  WINDOW_WIDTH_DEFAULT ));
		this->windowSize.SetHeight(LoadSetting(MAIN_WINDOW_KEY, WINDOW_HEIGHT_KEY, WINDOW_HEIGHT_DEFAULT));
		
		this->windowMax = LoadSetting(MAIN_WINDOW_KEY, WINDOW_MAX_KEY, WINDOW_MAX_DEFAULT);
	}
}