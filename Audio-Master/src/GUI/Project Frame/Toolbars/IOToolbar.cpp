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

		// Populate the combo boxes (input and output)
		PopulateInputs();
		PopulateOutputs();

		// Initialise the tools and their images
		InitImages();
		InitTools();

		// Call Realize to setup the display size and positions of the new tools
		this->Realize();
    }

    IOToolbar::~IOToolbar()
    {
        
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

	}

	void IOToolbar::PopulateInputs()
	{

	}
	
	void IOToolbar::PopulateOutputs()
	{

	}
}