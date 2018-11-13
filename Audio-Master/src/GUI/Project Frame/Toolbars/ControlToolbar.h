#ifndef INC_CONTROL_TOOLBAR_H
#define INC_CONTROL_TOOLBAR_H

#include <wx/aui/auibar.h>
#include <wx/frame.h>
#include <wx/bitmap.h>

#include "../../../AudioMaster.h"

namespace AudioMaster
{

	class ControlToolbar final : public wxAuiToolBar
	{
	private:
		Logger* logger;

		wxBitmap toolImg[];

	public:
		ControlToolbar(wxFrame* parent);
		~ControlToolbar();

	private:
		void InitImages();
		void InitTools();
	};
	
}

#endif