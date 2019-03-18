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
		EVT_LEFT_DCLICK (WaveformDisplay::OnDClick)
		EVT_KEY_DOWN	(WaveformDisplay::OnKeyDown)
		EVT_KEY_UP		(WaveformDisplay::OnKeyUp)

	wxEND_EVENT_TABLE()

    WaveformDisplay::WaveformDisplay(wxFrame* parent, wxSize size)
		: wxScrolled<wxPanel>(parent, wxID_ANY, wxDefaultPosition, size, wxHSCROLL)
    {
		Logger::GetInstance()->Log("Creating Waveform Display");

		this->cachedWaveData = nullptr;

		// Set the scroll rate and original virtual size
		this->SetScrollRate(10, 0);
		this->SetVirtualSize(wxSize(0,0));
		UpdateVirtualSize(wxSize(0,0));

		// Initialise the mouse scroll properties
		this->scrollMultiplier = -1.0f; // Invert the direction of the scrolling for horizontal scrolling to be more intuitive
		this->scrolled = 0;
		this->minScrollMultiplier = -0.75f;

		// Set the alternative scrolling to false
		this->ctrlScroll = false;

		// Set the waveform zoom and multiplier to the default
		this->zoom = 100.0f;
		this->zoomMultiplier = -2.0f;
		this->minZoom = 0.1f;
		this->maxZoom = 250.0f;

		// Set to double buffered to avoid flickering when drawing
		this->SetDoubleBuffered(true);

		// Set the background colour to opaque white
		this->SetBackgroundColour(wxColour(0xff, 0xff, 0xff, 0xff));
    }

    WaveformDisplay::~WaveformDisplay()
    {
		Logger::GetInstance()->Log("Destroying Waveform Display");

		if (this->cachedWaveData != nullptr)
			delete this->cachedWaveData;
    }

	void WaveformDisplay::UpdateVirtualSize(wxSize newSize)
	{
		Logger::GetInstance()->Log("Updating Waveform Display Size: { x:" + std::to_string(newSize.x) + ", y: " + std::to_string(newSize.y) + " }");

		// Get the current virtual size
		wxSize s = this->GetVirtualSize();

		// Update the scroll multiplier
		double m = static_cast<double>(newSize.x) / static_cast<double>(s.x);
		this->scrollMultiplier *= m;
		if (abs(this->scrollMultiplier) <= abs(this->minScrollMultiplier))
			this->scrollMultiplier = this->minScrollMultiplier;

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
		if (this->ctrlScroll)
		{
			this->zoom += (e.GetWheelRotation() / e.GetWheelDelta()) * this->zoomMultiplier;
			if (this->zoom <= this->minZoom)
			{
				this->zoom = this->minZoom;
			}
			else if (this->zoom >= this->maxZoom)
			{
				this->zoom = this->maxZoom;
			}
		}
		else
		{
			this->scrolled = this->GetScrollPos(wxHORIZONTAL) + (e.GetWheelRotation() / e.GetWheelDelta()) * this->scrollMultiplier;

			this->DoScroll(this->scrolled, 0);
		}

		this->Refresh();
	}

	void WaveformDisplay::OnResize(wxSizeEvent& e)
	{
		Logger::GetInstance()->Log("Waveform Display Resized");

		wxSize size = e.GetSize();
		this->displayHeight = size.GetHeight();
	}

	void WaveformDisplay::OnDClick(wxMouseEvent& WXUNUSED(e))
	{
		Logger::GetInstance()->Log("Waveform Display Refreshed");

		this->Refresh();
		this->ForceRepaint();
	}

	void WaveformDisplay::OnKeyDown(wxKeyEvent& e)
	{
		this->ctrlScroll = e.ControlDown();
	}

	void WaveformDisplay::OnKeyUp(wxKeyEvent& e)
	{
		this->ctrlScroll = e.ControlDown();
	}

	void WaveformDisplay::Render(wxDC& dc)
	{
		Sound* s = SoundManager::GetInstance()->GetSound();

		if (s->size <= 0 || s->data == nullptr)
		{
			Logger::GetInstance()->Log("\tNo Sound To Display");
			return;
		}

		Logger::GetInstance()->Log("Rendering Waveform");

		// Cache the wave data to prevent re-logging the same data multiple times
		bool logData = true;
		if (this->cachedWaveData == s)
			logData = false;
		else
			this->cachedWaveData = s;

		// Log the waveform being drawn using space-separated values
		if (logData)
		{
			for (int i = 0; i < s->size; ++i)
			{
				Logger::GetInstance()->Print(std::to_string((int)s->data[i]) + " ");
			}
		}

		// Create the waveform pen and set to be used
		wxPen wavePen = wxPen(wxColour(0x00, 0x00, 0x00, 0xff), 1, wxPenStyle::wxPENSTYLE_SOLID); // 5px width opaque black pen
		dc.SetPen(wavePen);

		// If there is no data to draw then skip drawing the wave
		if (s->size > 0)
		{
			// Calculate the waveform point data
			wxPoint* points = new wxPoint[s->size / (s->bitsPerSample / 8)];
			int pointIndex = 0;

			for (int i = 0; i < s->size; i += s->bitsPerSample / 8)
			{
				if (s->bitsPerSample == 8)
				{
					uint8_t sample = s->data[i];

					// Midpoint is 128 therefore 128 ==> 0.0
					int y = sample - 128;
					points[pointIndex] = wxPoint(pointIndex / zoom, y);
					++pointIndex;
				}
				else if (s->bitsPerSample == 16)
				{
					int16_t sample = s->data[i] | s->data[i + 1] << 8;

					// Midpoint is 0 therefore 0 ==> 0.0
					int y = sample;
					points[pointIndex] = wxPoint(pointIndex / zoom, y / 500);
					++pointIndex;
				}
			}

			// Draw the waveform data
			dc.DrawLines(pointIndex, points, 0, this->displayHeight / 2);

			// Update the virtual size if necessary for the scroll bar
			if (points[pointIndex - 1].x > this->m_virtualSize.x)
				this->UpdateVirtualSize(wxSize(points[pointIndex - 1].x, points[pointIndex - 1].y));
			else if (points[pointIndex - 1].x < this->m_virtualSize.x)
				this->UpdateVirtualSize(wxSize(points[pointIndex - 1].x, points[pointIndex - 1].y));

			delete[] points;
		}

		dc.DrawLine(wxPoint(0, this->displayHeight / 2), wxPoint(this->m_virtualSize.x, this->displayHeight / 2));
	}

}