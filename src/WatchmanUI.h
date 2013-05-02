///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __WATCHMANUI_H__
#define __WATCHMANUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/spinctrl.h>
#include <wx/filepicker.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class WatchmanDialogBase
///////////////////////////////////////////////////////////////////////////////
class WatchmanDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText24;
		wxStaticText* m_stLandFallTime;
		wxStaticText* m_staticText4;
		wxStaticText* m_stActivity;
		wxButton* m_bPreferences;
		wxButton* m_bClose;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnPreferences( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		WatchmanDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Watch Man"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxTAB_TRAVERSAL ); 
		~WatchmanDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class WatchmanPrefsDialog
///////////////////////////////////////////////////////////////////////////////
class WatchmanPrefsDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText6;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
	
	public:
		wxCheckBox* m_cbLandFall;
		wxTextCtrl* m_tcLandFallDistance;
		wxCheckBox* m_cbDeadman;
		wxSpinCtrl* m_sDeadmanMinutes;
		wxCheckBox* m_cbSound;
		wxFilePickerCtrl* m_fpSound;
		wxCheckBox* m_cbCommand;
		wxTextCtrl* m_tCommand;
		wxCheckBox* m_cbMessageBox;
		
		WatchmanPrefsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Watchman Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE ); 
		~WatchmanPrefsDialog();
	
};

#endif //__WATCHMANUI_H__
