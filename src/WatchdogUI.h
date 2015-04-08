///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __WATCHDOGUI_H__
#define __WATCHDOGUI_H__

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
#include <wx/radiobut.h>
#include <wx/statbox.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/listbook.h>
#include <wx/listctrl.h>
#include <wx/filepicker.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class WatchdogDialogBase
///////////////////////////////////////////////////////////////////////////////
class WatchdogDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxButton* m_bPreferences;
		wxButton* m_bReset;
		wxButton* m_bClose;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnPreferences( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnReset( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxFlexGridSizer* m_fgAlarms;
		wxStaticText* m_stTextLandFallTime;
		wxStaticText* m_stLandFallTime;
		wxStaticText* m_stTextLandFallDistance;
		wxStaticText* m_stLandFallDistance;
		wxStaticText* m_stTextNMEAData;
		wxStaticText* m_stNMEAData;
		wxStaticText* m_stTextDeadman;
		wxStaticText* m_stDeadman;
		wxStaticText* m_stTextSecondDeadman;
		wxStaticText* m_stSecondDeadman;
		wxStaticText* m_stTextAnchor;
		wxStaticText* m_stAnchorDistance;
		wxStaticText* m_stTextCourseError;
		wxStaticText* m_stCourseError;
		wxStaticText* m_stTextStarboardCourseError;
		wxStaticText* m_stStarboardCourseError;
		wxStaticText* m_stTextUnderSpeed;
		wxStaticText* m_stUnderSpeed;
		wxStaticText* m_stTextOverSpeed;
		wxStaticText* m_stOverSpeed;
		
		WatchdogDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("WatchDog"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxTAB_TRAVERSAL ); 
		~WatchdogDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class WatchdogPrefsDialogBase
///////////////////////////////////////////////////////////////////////////////
class WatchdogPrefsDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxRadioButton* m_rbAlways;
		wxRadioButton* m_rbOnce;
		wxRadioButton* m_rbVisible;
		wxRadioButton* m_rbNever;
		wxListbook* m_lbAlarm;
		wxPanel* m_panel1;
		wxStaticText* m_staticText30;
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText39122;
		wxStaticText* m_staticText45;
		wxPanel* m_panel2;
		wxStaticText* m_staticText34;
		wxTextCtrl* m_tNMEASentences;
		wxStaticText* m_staticText35;
		wxStaticText* m_staticText13;
		wxPanel* m_panel3;
		wxStaticText* m_staticText33;
		wxStaticText* m_staticText6;
		wxCheckBox* m_cbSecondDeadman;
		wxPanel* m_pSecondDeadman;
		wxStaticText* m_staticText331;
		wxStaticText* m_staticText61;
		wxPanel* m_panel4;
		wxStaticText* m_staticText71;
		wxStaticText* m_staticText8;
		wxButton* m_bSyncToBoat;
		wxStaticText* m_staticText9;
		wxStaticText* m_staticText39;
		wxCheckBox* m_cbAutoSync;
		wxStaticText* m_staticText39121;
		wxPanel* m_pCourse;
		wxStaticText* m_staticText32;
		wxStaticText* m_staticText18;
		wxStaticText* m_staticText21;
		wxButton* m_bCurrentCourse;
		wxCheckBox* m_cbSeparatePortAndStarboard;
		wxStaticText* m_staticText3912;
		wxPanel* m_pCourseStarboard;
		wxStaticText* m_staticText50;
		wxPanel* m_panel5;
		wxStaticText* m_staticText44;
		wxStaticText* m_staticText24;
		wxStaticText* m_staticText391;
		wxPanel* m_panel51;
		wxStaticText* m_staticText37;
		wxStaticText* m_staticText251;
		wxStaticText* m_staticText3911;
		wxPanel* m_panel10;
		wxStaticText* m_staticText401;
		wxPanel* m_panel11;
		wxStaticText* m_staticText40;
		wxRadioButton* m_rbEnabled;
		wxRadioButton* m_rbDisabled;
		wxCheckBox* m_cbGraphicsEnabled;
		wxCheckBox* m_cbAutoReset;
		wxButton* m_bTestAlarm;
		wxButton* m_button6;
		wxButton* m_button7;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnEnabled( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAlarmChanged( wxListbookEvent& event ) { event.Skip(); }
		virtual void OnAlarmUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAlarmUpdateSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnCheckSecondDeadman( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSyncToBoat( wxCommandEvent& event ) { event.Skip(); }
		virtual void f( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCurrentCourse( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCheckSeparatePortAndStarboard( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAlarmUpdateRB( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAlarmUpdateSound( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnAlarmUpdateCommand( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAlarmUpdateRepeat( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnTestAlarm( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnInformation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAboutAuthor( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxCheckBox* m_cbLandFallTime;
		wxSpinCtrl* m_sLandFallTimeMinutes;
		wxCheckBox* m_cbLandFallDistance;
		wxTextCtrl* m_tcLandFallDistance;
		wxSpinCtrl* m_sNMEASeconds;
		wxSpinCtrl* m_sDeadmanMinutes;
		wxSpinCtrl* m_sSecondDeadmanMinutes;
		wxTextCtrl* m_tAnchorLatitude;
		wxTextCtrl* m_tAnchorLongitude;
		wxSpinCtrl* m_sAnchorRadius;
		wxSpinCtrl* m_sCourseTolerance;
		wxSpinCtrl* m_sCourse;
		wxTextCtrl* m_tUnderSpeed;
		wxTextCtrl* m_tOverSpeed;
		wxCheckBox* m_cbSound;
		wxFilePickerCtrl* m_fpSound;
		wxCheckBox* m_cbCommand;
		wxTextCtrl* m_tCommand;
		wxCheckBox* m_cbMessageBox;
		wxCheckBox* m_cbRepeat;
		wxSpinCtrl* m_sRepeatSeconds;
		
		WatchdogPrefsDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Watchdog Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~WatchdogPrefsDialogBase();
	
};

#endif //__WATCHDOGUI_H__
