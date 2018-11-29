#ifndef INC_CONTROL_TOOLBAR_H
#define INC_CONTROL_TOOLBAR_H

#include <wx/aui/auibar.h>
#include <wx/frame.h>
#include <wx/bitmap.h>
#include <wx/event.h>

#include "../../../AudioMaster.h"
#include "../../../Sound/SoundManager.h"

namespace AudioMaster
{

	enum EControlEvent
	{
		CONTROL_TOOL_PLAY,
		CONTROL_TOOL_PAUSE,
		CONTROL_TOOL_RECORD,
		CONTROL_TOOL_STOP,
		CONTROL_TOOL_MAX
	};

	class ControlToolbar final : public wxAuiToolBar
	{
	private:
		Logger* logger;
		SoundManager* soundManager;

		wxBitmap toolImg[CONTROL_TOOL_MAX];

	public:
		ControlToolbar(wxFrame* parent);
		~ControlToolbar();

	private:
		void InitImages();
		void InitTools();

		void Play(wxCommandEvent& e);
		void Pause(wxCommandEvent& e);
		void Record(wxCommandEvent& e);
		void Stop(wxCommandEvent& e);

		wxDECLARE_EVENT_TABLE();
	};
	
}

#endif