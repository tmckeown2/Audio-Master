#ifndef INC_IO_TOOLBAR_H
#define INC_IO_TOOLBAR_H

#include <string>
#include <vector>

#include <wx/aui/auibar.h>
#include <wx/frame.h>
#include <wx/event.h>
#include <wx/combobox.h>

#include "../../../AudioMaster.h"
#include "../../../Sound/SoundManager.h"

namespace AudioMaster
{

	enum EIOComboIDs
	{
		IO_COMBO_INPUT,
		IO_COMBO_OUTPUT,
		IO_COMBO_FREQ,
		IO_COMBO_FORMAT
	};

	class IOToolbar final : public wxAuiToolBar
	{
	private:
		wxArrayString inputArray;
		wxArrayString outputArray;

		wxComboBox* inputDevices;
		wxComboBox* outputDevices;
		wxComboBox* inputFrequency;
		wxComboBox* inputFormat;

	public:
		IOToolbar(wxFrame* frame);
		~IOToolbar();

	private:
		void PopulateInputs();
		void PopulateOutputs();
		void InitImages();
		void InitTools();

		void OnInputChanged(wxCommandEvent& e);
		void OnOutputChanged(wxCommandEvent& e);
		void OnFreqChanged(wxCommandEvent& e);
		void OnFormatChanged(wxCommandEvent& e);

		wxDECLARE_EVENT_TABLE();
	};
	
}

#endif