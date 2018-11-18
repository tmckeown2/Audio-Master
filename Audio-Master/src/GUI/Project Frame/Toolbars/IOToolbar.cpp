#include "IOToolbar.h"

// Outputs memory leaks after the program has closed
#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>      // redefines the new() operator 
#endif

namespace AudioMaster
{
    IOToolbar::IOToolbar(wxFrame* parent)
		: wxAuiToolBar(parent, wxID_ANY)
    {
		// Get the logger
		this->logger = Logger::GetInstance();

		this->logger->Log("Initialising IO Toolbar");

		// Get the sound manager
		this->soundManager = SoundManager::GetInstance();

		// Populate the combo boxes (input and output)
		PopulateInputs();
		PopulateOutputs();

		// Initialise the combo boxes (input and output)
		this->inputDevices = new wxComboBox(this, wxID_ANY, wxEmptyString);
		this->inputDevices->Set(this->inputArray);
		this->inputDevices->SetSelection(0);
		
		this->outputDevices = new wxComboBox(this, wxID_ANY, wxEmptyString);
		this->outputDevices->Set(this->outputArray);
		this->outputDevices->SetSelection(0);

		// Initialise the tools and their images
		InitImages();
		InitTools();

		// Call Realize to setup the display size and positions of the new tools
		this->Realize();
    }

    IOToolbar::~IOToolbar()
    {
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
		
		this->AddSpacer(10);

		this->AddControl(this->outputDevices, _("Output Devices"));
	}

	void IOToolbar::PopulateInputs()
	{
		std::vector<std::string> inputs = this->soundManager->GetInputDevices();

		for (auto it = inputs.begin(); it != inputs.end(); ++it)
		{
			this->inputArray.Add(*it);
		}
	}
	
	void IOToolbar::PopulateOutputs()
	{
		std::vector<std::string> outputs = this->soundManager->GetOutputDevices();

		for (auto it = outputs.begin(); it != outputs.end(); ++it)
		{
			this->outputArray.Add(*it);
		}
	}
}