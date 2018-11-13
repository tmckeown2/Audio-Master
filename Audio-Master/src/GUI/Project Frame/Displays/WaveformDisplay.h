#ifndef INC_WAVEFORM_DISPLAY_H
#define INC_WAVEFORM_DISPLAY_H

#include <wx/frame.h>
#include <wx/scrolwin.h>
#include <wx/panel.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/event.h>

namespace AudioMaster
{

	class WaveformDisplay final : public wxScrolled<wxPanel>
	{
	private:
		int scrollMultiplier;
		int scrolled;

	public:
		WaveformDisplay(wxFrame* parent, wxSize size);
		~WaveformDisplay();

		void ForceRepaint();

		void OnPaint(wxPaintEvent& e);
		void OnScroll(wxMouseEvent& e);

	private:
		void Render(wxDC& dc);
		void UpdateVirtualSize(wxSize newSize);

		wxDECLARE_EVENT_TABLE();
	};
	
}

#endif