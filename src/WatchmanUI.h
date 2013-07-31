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
#include <wx/sizer.h>
#include <wx/button.h>
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
		wxFlexGridSizer* m_fgAlarms;
		wxStaticText* m_stTextLandFall;
		wxStaticText* m_stLandFallTime;
		wxStaticText* m_stTextActivity;
		wxStaticText* m_stActivity;
		wxStaticText* m_stTextAnchor;
		wxStaticText* m_stTextGPS;
		wxStaticText* m_stTextAIS;
		wxStaticText* m_stTextUnderSpeed;
		wxStaticText* m_stUnderSpeed;
		wxStaticText* m_stTextCourseError;
		wxStaticText* m_stCourseError;
		wxButton* m_bPreferences;
		wxButton* m_bResetLastAlarm;
		wxButton* m_bClose;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnPreferences( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnResetLastAlarm( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxStaticText* m_stAnchorDistance;
		wxStaticText* m_stGPS;
		wxStaticText* m_stAIS;
		wxStaticText* m_stTextOverSpeed;
		wxStaticText* m_stOverSpeed;
		
		WatchmanDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Watch Man"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxTAB_TRAVERSAL ); 
		~WatchmanDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class WatchmanPrefsDialogBase
///////////////////////////////////////////////////////////////////////////////
class WatchmanPrefsDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText23;
		wxStaticText* m_staticText6;
		wxStaticText* m_staticText13;
		wxStaticText* m_staticText12;
		wxButton* m_bSyncToBoat;
		wxStaticText* m_staticText9;
		wxStaticText* m_staticText71;
		wxStaticText* m_staticText8;
		wxStaticText* m_staticText24;
		wxStaticText* m_staticText25;
		wxStaticText* m_staticText18;
		wxStaticText* m_staticText21;
		wxButton* m_bCurrentCourse;
		wxButton* m_bTestAlarm;
		wxButton* m_button6;
		wxButton* m_button7;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnSyncToBoat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCurrentCourse( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTestAlarm( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnInformation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxCheckBox* m_cbLandFall;
		wxTextCtrl* m_tcLandFallDistance;
		wxCheckBox* m_cbDeadman;
		wxSpinCtrl* m_sDeadmanMinutes;
		wxCheckBox* m_cbGPSAlarm;
		wxSpinCtrl* m_sGPSSeconds;
		wxCheckBox* m_cbAISAlarm;
		wxSpinCtrl* m_sAISSeconds;
		wxCheckBox* m_cbAnchor;
		wxSpinCtrl* m_sAnchorRadius;
		wxTextCtrl* m_tAnchorLatitude;
		wxTextCtrl* m_tAnchorLongitude;
		wxCheckBox* m_cbUnderSpeed;
		wxTextCtrl* m_tUnderSpeed;
		wxCheckBox* m_cbOverSpeed;
		wxTextCtrl* m_tOverSpeed;
		wxCheckBox* m_cbOffCourseAlarm;
		wxSpinCtrl* m_sOffCourseDegrees;
		wxSpinCtrl* m_sCourseDegrees;
		wxCheckBox* m_cbSound;
		wxFilePickerCtrl* m_fpSound;
		wxCheckBox* m_cbCommand;
		wxTextCtrl* m_tCommand;
		wxCheckBox* m_cbMessageBox;
		wxCheckBox* m_cbRepeat;
		wxSpinCtrl* m_sRepeatSeconds;
		
		WatchmanPrefsDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Watchman Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE ); 
		~WatchmanPrefsDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class AboutDialogBase
///////////////////////////////////////////////////////////////////////////////
class AboutDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText110;
		wxButton* m_bDonate;
		wxButton* m_bClose;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnDonate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		AboutDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("About Watchman"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~AboutDialogBase();
	
};

#endif //__WATCHMANUI_H__
