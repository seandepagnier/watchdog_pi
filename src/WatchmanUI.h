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
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbox.h>
#include <wx/listbook.h>
#include <wx/listctrl.h>
#include <wx/radiobut.h>
#include <wx/filepicker.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class WatchdogDialogBase
///////////////////////////////////////////////////////////////////////////////
class WatchdogDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxFlexGridSizer* m_fgAlarms;
		wxStaticText* m_stTextLandFallTime;
		wxStaticText* m_stLandFallTime;
		wxStaticText* m_stTextLandFallDistance;
		wxStaticText* m_stLandFallDistance;
		wxStaticText* m_stTextActivity;
		wxStaticText* m_stActivity;
		wxStaticText* m_stTextAnchor;
		wxStaticText* m_stTextGPS;
		wxStaticText* m_stTextAIS;
		wxStaticText* m_stTextUnderSpeed;
		wxStaticText* m_stUnderSpeed;
		wxStaticText* m_stTextCourseError;
		wxStaticText* m_stCourseError;
		wxCheckBox* m_cbDisableAllAlarms;
		wxButton* m_bPreferences;
		wxButton* m_bResetLastAlarm;
		wxButton* m_bClose;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnDisableAllAlarms( wxComdogdEvent& event ) { event.Skip(); }
		virtual void OnPreferences( wxComdogdEvent& event ) { event.Skip(); }
		virtual void OnResetLastAlarm( wxComdogdEvent& event ) { event.Skip(); }
		virtual void OnClose( wxComdogdEvent& event ) { event.Skip(); }
		
	
	public:
		wxStaticText* m_stAnchorDistance;
		wxStaticText* m_stGPS;
		wxStaticText* m_stAIS;
		wxStaticText* m_stTextOverSpeed;
		wxStaticText* m_stOverSpeed;
		
		WatchdogDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Watch Man"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxTAB_TRAVERSAL ); 
		~WatchdogDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class WatchdogPrefsDialogBase
///////////////////////////////////////////////////////////////////////////////
class WatchdogPrefsDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxListbook* m_lbAlarm;
		wxPanel* m_panel1;
		wxStaticText* m_staticText30;
		wxStaticText* m_staticText7;
		wxPanel* m_panel2;
		wxStaticText* m_staticText34;
		wxTextCtrl* m_textCtrl7;
		wxStaticText* m_staticText35;
		wxStaticText* m_staticText13;
		wxPanel* m_panel3;
		wxStaticText* m_staticText33;
		wxStaticText* m_staticText6;
		wxPanel* m_panel4;
		wxButton* m_bSyncToBoat;
		wxStaticText* m_staticText9;
		wxStaticText* m_staticText71;
		wxStaticText* m_staticText8;
		wxPanel* m_panel6;
		wxStaticText* m_staticText32;
		wxStaticText* m_staticText18;
		wxStaticText* m_staticText21;
		wxButton* m_bCurrentCourse;
		wxPanel* m_panel5;
		wxStaticText* m_staticText24;
		wxStaticText* m_staticText25;
		wxRadioButton* m_rbEnabled;
		wxRadioButton* m_rbDisabled;
		wxCheckBox* m_checkBox10;
		wxCheckBox* m_cbAutoReset;
		wxButton* m_bTestAlarm;
		wxButton* m_button6;
		wxButton* m_button7;
		wxButton* m_bDonate;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnAlarmChanged( wxListbookEvent& event ) { event.Skip(); }
		virtual void OnSyncToBoat( wxComdogdEvent& event ) { event.Skip(); }
		virtual void OnCurrentCourse( wxComdogdEvent& event ) { event.Skip(); }
		virtual void OnAlarmCheck( wxComdogdEvent& event ) { event.Skip(); }
		virtual void OnTestAlarm( wxComdogdEvent& event ) { event.Skip(); }
		virtual void OnInformation( wxComdogdEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxComdogdEvent& event ) { event.Skip(); }
		virtual void OnDonate( wxComdogdEvent& event ) { event.Skip(); }
		
	
	public:
		wxCheckBox* m_cbLandFallTime;
		wxSpinCtrl* m_sLandFallTimeMinutes;
		wxCheckBox* m_cbLandFallDistance;
		wxTextCtrl* m_tcLandFallDistance;
		wxSpinCtrl* m_sGPSSeconds;
		wxSpinCtrl* m_sDeaddogMinutes;
		wxSpinCtrl* m_sAnchorRadius;
		wxTextCtrl* m_tAnchorLatitude;
		wxTextCtrl* m_tAnchorLongitude;
		wxSpinCtrl* m_sOffCourseDegrees;
		wxSpinCtrl* m_sCourseDegrees;
		wxCheckBox* m_cbUnderSpeed;
		wxTextCtrl* m_tUnderSpeed;
		wxCheckBox* m_cbOverSpeed;
		wxTextCtrl* m_tOverSpeed;
		wxCheckBox* m_cbSound;
		wxFilePickerCtrl* m_fpSound;
		wxCheckBox* m_cbComdogd;
		wxTextCtrl* m_tComdogd;
		wxCheckBox* m_cbMessageBox;
		wxCheckBox* m_cbRepeat;
		wxSpinCtrl* m_sRepeatSeconds;
		
		WatchdogPrefsDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Watchdog Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE ); 
		~WatchdogPrefsDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class AboutDialog
///////////////////////////////////////////////////////////////////////////////
class AboutDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText110;
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
	
	public:
		
		AboutDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("About Watchdog"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~AboutDialog();
	
};

#endif //__WATCHMANUI_H__
