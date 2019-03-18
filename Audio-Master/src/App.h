#ifndef INC_APP_H
#define INC_APP_H

#include <wx/app.h>

#include "AudioMaster.h"
#include "GUI/Project Frame/ProjectFrame.h"

namespace AudioMaster
{

	class App final : public wxApp
	{
	private:
		// Window variables
		wxPoint windowPos;
		wxSize  windowSize;
		bool    windowMax;

		// Project Frame
		ProjectFrame* projectFrame;

	public:
		bool OnInit();
		int OnExit();

	private:
		void SetDefaultSettings();
		void LoadSettings();
	};
	
	wxDECLARE_APP(App);

}

#endif


//TODO: Memory leaks when 