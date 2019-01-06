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
		EVT_SIZE		(WaveformDisplay::OnResize)

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

	void WaveformDisplay::OnResize(wxSizeEvent& e)
	{
		wxSize size = e.GetSize();
		this->displayHeight = size.GetHeight();
	}

	void WaveformDisplay::Render(wxDC& dc)
	{
		// Get the sound from the manager
		SoundManager* mgr = SoundManager::GetInstance();
		Sound* s = mgr->GetSound();

		// Create the waveform pen and set to be used
		wxPen wavePen = wxPen(wxColour(0x00, 0x00, 0x00, 0xff), 1, wxPenStyle::wxPENSTYLE_SOLID); // 5px width opaque black pen
		dc.SetPen(wavePen);

		// Draw the waveform data - Must get this from the SoundManager
		wxPoint* points = new wxPoint[s->size];
		double PI = 3.14159265;
		double f = 1.0;
		double w = 2 * PI * f;

		for (int t = 0; t < s->size; t += (s->bitsPerSample / 8))
		{
			// This works for 8-bit audio
			//points[t] = wxRealPoint(t, (s->data[t] - (255.0 / 2)));

			for (int i = 0; i < (s->bitsPerSample / 8); ++i)
			{
				points[t + i] = wxRealPoint(t, (s->data[t + i] - (pow(2, s->bitsPerSample) / 2.0)));
			}
		}

		dc.DrawLines(s->size, points, 0, this->displayHeight / 2);

		if (points[s->size - 1].x > this->m_virtualSize.x)
			this->UpdateVirtualSize(wxSize(points[s->size - 1].x, points[s->size - 1].y));

		delete[] points;
	}

}