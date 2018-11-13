#ifndef INC_IO_TOOLBAR_H
#define INC_IO_TOOLBAR_H

#include <wx/aui/auibar.h>
#include <wx/frame.h>
#include <wx/combobox.h>

#include "../../../AudioMaster.h"

namespace AudioMaster
{

	class IOToolbar final : public wxAuiToolBar
	{
	private:
		Logger* logger;

		wxComboBox* inputDevices;
		wxComboBox* outputDevices;

	public:
		IOToolbar(wxFrame* frame);
		~IOToolbar();

	private:
		void PopulateInputs();
		void PopulateOutputs();
		void InitImages();
		void InitTools();
	};
	
}

#endif