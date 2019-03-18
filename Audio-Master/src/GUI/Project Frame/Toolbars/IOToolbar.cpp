#include "IOToolbar.h"

// Outputs memory leaks after the program has closed
#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>      // redefines the new() operator 
#endif

namespace AudioMaster
{
	wxBEGIN_EVENT_TABLE(IOToolbar, wxAuiToolBar)

		EVT_COMBOBOX(EIOComboIDs::IO_COMBO_INPUT, IOToolbar::OnInputChanged)
		EVT_COMBOBOX(EIOComboIDs::IO_COMBO_OUTPUT, IOToolbar::OnOutputChanged)
		EVT_COMBOBOX(EIOComboIDs::IO_COMBO_FREQ, IOToolbar::OnFreqChanged)
		EVT_COMBOBOX(EIOComboIDs::IO_COMBO_FORMAT, IOToolbar::OnFormatChanged)

	wxEND_EVENT_TABLE()

    IOToolbar::IOToolbar(wxFrame* parent)
		: wxAuiToolBar(parent, wxID_ANY)
    {
		Logger::GetInstance()->Log("Initialising IO Toolbar");

		// Populate the combo boxes (input and output)
		PopulateInputs();
		PopulateOutputs();

		// Initialise the combo boxes (input and output)
		this->inputDevices = new wxComboBox(this, EIOComboIDs::IO_COMBO_INPUT, wxEmptyString);
		this->inputDevices->Set(this->inputArray);
		this->inputDevices->SetSelection(0);
		
		this->outputDevices = new wxComboBox(this, EIOComboIDs::IO_COMBO_OUTPUT, wxEmptyString);
		this->outputDevices->Set(this->outputArray);
		this->outputDevices->SetSelection(0);

		wxArrayString inputFrequencies;
		wxArrayString inputFormats;

		inputFrequencies.Add(_("44100"));
		inputFrequencies.Add(_("48000"));

		inputFormats.Add(_("MONO 8-bit"));
		inputFormats.Add(_("MONO 16-bit"));
		inputFormats.Add(_("STEREO 8-bit"));
		inputFormats.Add(_("STEREO 16-bit"));

		this->inputFrequency = new wxComboBox(this, EIOComboIDs::IO_COMBO_FREQ, wxEmptyString);
		this->inputFrequency->Set(inputFrequencies);
		this->inputFrequency->SetSelection(0);

		this->inputFormat = new wxComboBox(this, EIOComboIDs::IO_COMBO_FORMAT, wxEmptyString);
		this->inputFormat->Set(inputFormats);
		this->inputFormat->SetSelection(0);

		// Initialise the tools and their images
		InitImages();
		InitTools();

		// Call Realize to setup the display size and positions of the new tools
		this->Realize();
    }

    IOToolbar::~IOToolbar()
    {
		Logger::GetInstance()->Log("Destroying IO Toolbar");

        delete this->inputDevices;
		delete this->outputDevices;
    }

	void IOToolbar::InitImages()
	{
		// Get resources

		// Load the images, if it cannot load then log an error and skip that file.
		// Do not want to cause a crash because an image did not load correctly, it is not a fatal issue.

	}

	void IOToolbar::InitTools()
	{
		// Add all the tools
		this->AddControl(this->inputDevices,  _("Input  Devices"));
		
		this->AddSpacer(5);

		this->AddControl(this->outputDevices, _("Output Devices"));

		this->AddSpacer(10);
		this->AddControl(this->inputFrequency, _("Input Freq"));
		this->AddSpacer(5);
		this->AddControl(this->inputFormat, _("Input Format"));
	}

	void IOToolbar::PopulateInputs()
	{
		Logger::GetInstance()->Log("Populating Input Device List");

		std::vector<std::string> inputs = SoundManager::GetInstance()->GetInputDevices();

		for (auto it = inputs.begin(); it != inputs.end(); ++it)
		{
			this->inputArray.Add(*it);
		}
	}
	
	void IOToolbar::PopulateOutputs()
	{
		Logger::GetInstance()->Log("Populating Output Device List");

		std::vector<std::string> outputs = SoundManager::GetInstance()->GetOutputDevices();

		for (auto it = outputs.begin(); it != outputs.end(); ++it)
		{
			this->outputArray.Add(*it);
		}
	}

	void IOToolbar::OnInputChanged(wxCommandEvent& WXUNUSED(e))
	{
		Logger::GetInstance()->Log("Input Device Changed");

		std::string val = this->inputDevices->GetValue().ToStdString();

		SoundManager::GetInstance()->SetInput(val.c_str());
	}

	void IOToolbar::OnOutputChanged(wxCommandEvent& WXUNUSED(e))
	{
		Logger::GetInstance()->Log("Output Device Changed");

		std::string val = this->outputDevices->GetValue().ToStdString();

		SoundManager::GetInstance()->SetOutput(val.c_str());
	}

	void IOToolbar::OnFreqChanged(wxCommandEvent& WXUNUSED(e))
	{
		Logger::GetInstance()->Log("Sample Rate Changed");

		std::string val = this->inputFrequency->GetValue().ToStdString();

		SoundManager::GetInstance()->SetInputFrequency(stoi(val));
	}

	void IOToolbar::OnFormatChanged(wxCommandEvent& WXUNUSED(e))
	{
		Logger::GetInstance()->Log("Format Changed");

		std::string val = this->inputFormat->GetValue().ToStdString();

		if (val == "MONO 8-bit")
		{
			SoundManager::GetInstance()->SetInputFormat(AL_FORMAT_MONO8);
		}
		else if (val == "MONO 16-bit")
		{
			SoundManager::GetInstance()->SetInputFormat(AL_FORMAT_MONO16);
		}
		else if (val == "STEREO 8-bit")
		{
			SoundManager::GetInstance()->SetInputFormat(AL_FORMAT_STEREO8);
		}
		else if (val == "STEREO 16-bit")
		{
			SoundManager::GetInstance()->SetInputFormat(AL_FORMAT_STEREO16);
		}
	}
}