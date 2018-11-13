#include "WaveformDisplay.h"

// Outputs memory leaks after the program has closed
#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>      // redefines the new() operator 
#endif

namespace AudioMaster
{

	wxBEGIN_EVENT_TABLE(WaveformDisplay, wxScrolled<wxPanel>)
		
		EVT_PAINT		(WaveformDisplay::OnPaint)
		EVT_MOUSEWHEEL	(WaveformDisplay::OnScroll)

	wxEND_EVENT_TABLE()

    WaveformDisplay::WaveformDisplay(wxFrame* parent, wxSize size)
		: wxScrolled<wxPanel>(parent, wxID_ANY, wxDefaultPosition, size, wxHSCROLL)
    {
		// Set the scroll rate and original virtual size
		this->SetScrollRate(10, 0);
		this->SetVirtualSize(size);

		// Initialise the mouse scroll properties
		this->scrollMultiplier = -1; // Invert the direction of the scrolling for horizontal scrolling to be more intuitive
		this->scrolled = 0;

		// Set to double buffered to avoid flickering when drawing
		this->SetDoubleBuffered(true);

		// Set the background colour to opaque white
		this->SetBackgroundColour(wxColour(0xff, 0xff, 0xff, 0xff));
    }

    WaveformDisplay::~WaveformDisplay()
    {
        
    }

	void WaveformDisplay::UpdateVirtualSize(wxSize newSize)
	{
		// Get the current virtual size
		wxSize s = this->GetVirtualSize();

		// Update the scroll multiplier
		double m = static_cast<double>(newSize.x) / static_cast<double>(s.x);
		this->scrollMultiplier *= wxRound(m);

		// Set the new size
		this->SetVirtualSize(newSize);
	}

	void WaveformDisplay::ForceRepaint()
	{
		// Get the device context - ClientDC must be used from outside of a paint event
		wxClientDC dc(this);

		// Adjust dc for current scroll offset
		PrepareDC(dc);

		// Render the image
		this->Render(dc);
	}

	void WaveformDisplay::OnPaint(wxPaintEvent& e)
	{
		// Get the device context - PaintDC must be used from within a paint event
		wxPaintDC dc(this);

		// Adjust dc for current scroll offset
		PrepareDC(dc);

		// Render the image
		this->Render(dc);
	}

	void WaveformDisplay::OnScroll(wxMouseEvent& e)
	{
		this->scrolled += (e.GetWheelRotation() / e.GetWheelDelta()) * this->scrollMultiplier;

		this->DoScroll(this->scrolled, 0);
	}

	void WaveformDisplay::Render(wxDC& dc)
	{
		// Create the waveform pen and set to be used
		wxPen wavePen = wxPen(wxColour(0x00, 0x00, 0x00, 0xff), 1, wxPenStyle::wxPENSTYLE_SOLID); // 5px width opaque black pen
		dc.SetPen(wavePen);

		// Draw the waveform data - Must get this from the SoundManager
		wxPoint* points = new wxPoint[2000];
		double PI = 3.14159265;
		double f = 1.0;
		double w = 2 * PI * f;

		for (int t = 0; t < 2000; ++t)
		{
			points[t] = wxRealPoint(t, 30.f * sin(w * t * (PI / 180)));
		}

		dc.DrawLines(2000, points, 0, 100);

		if (points[1999].x > this->m_virtualSize.x)
			this->UpdateVirtualSize(wxSize(points[1999].x, points[1999].y));

		delete[] points;
	}

}