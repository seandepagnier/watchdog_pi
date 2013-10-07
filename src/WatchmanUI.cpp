///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "WatchmanUI.h"

///////////////////////////////////////////////////////////////////////////

WatchmanDialogBase::WatchmanDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxSize( -1,-1 ) );
	
	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer8->AddGrowableCol( 0 );
	fgSizer8->AddGrowableRow( 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_fgAlarms = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgAlarms->AddGrowableCol( 0 );
	m_fgAlarms->AddGrowableRow( 0 );
	m_fgAlarms->SetFlexibleDirection( wxBOTH );
	m_fgAlarms->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_stTextLandFallTime = new wxStaticText( this, wxID_ANY, _("LandFall in"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTextLandFallTime->Wrap( -1 );
	m_fgAlarms->Add( m_stTextLandFallTime, 0, wxALL, 5 );
	
	m_stLandFallTime = new wxStaticText( this, wxID_ANY, _("No NMEA Messages"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stLandFallTime->Wrap( -1 );
	m_fgAlarms->Add( m_stLandFallTime, 0, wxALL, 5 );
	
	m_stTextLandFallDistance = new wxStaticText( this, wxID_ANY, _("Land Distance >"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTextLandFallDistance->Wrap( -1 );
	m_fgAlarms->Add( m_stTextLandFallDistance, 0, wxALL, 5 );
	
	m_stLandFallDistance = new wxStaticText( this, wxID_ANY, _("No NMEA Messages"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stLandFallDistance->Wrap( -1 );
	m_fgAlarms->Add( m_stLandFallDistance, 0, wxALL, 5 );
	
	m_stTextActivity = new wxStaticText( this, wxID_ANY, _("Last activity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTextActivity->Wrap( -1 );
	m_fgAlarms->Add( m_stTextActivity, 0, wxALL, 5 );
	
	m_stActivity = new wxStaticText( this, wxID_ANY, _("No Updates"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stActivity->Wrap( -1 );
	m_fgAlarms->Add( m_stActivity, 0, wxALL, 5 );
	
	m_stTextAnchor = new wxStaticText( this, wxID_ANY, _("Distance to Anchor"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTextAnchor->Wrap( -1 );
	m_fgAlarms->Add( m_stTextAnchor, 0, wxALL, 5 );
	
	m_stAnchorDistance = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
	m_stAnchorDistance->Wrap( -1 );
	m_fgAlarms->Add( m_stAnchorDistance, 0, wxALL, 5 );
	
	m_stTextGPS = new wxStaticText( this, wxID_ANY, _("Last GPS Update"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTextGPS->Wrap( -1 );
	m_fgAlarms->Add( m_stTextGPS, 0, wxALL, 5 );
	
	m_stGPS = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_stGPS->Wrap( -1 );
	m_fgAlarms->Add( m_stGPS, 0, wxALL, 5 );
	
	m_stTextAIS = new wxStaticText( this, wxID_ANY, _("Last AIS Sentence"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTextAIS->Wrap( -1 );
	m_fgAlarms->Add( m_stTextAIS, 0, wxALL, 5 );
	
	m_stAIS = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_stAIS->Wrap( -1 );
	m_fgAlarms->Add( m_stAIS, 0, wxALL, 5 );
	
	m_stTextUnderSpeed = new wxStaticText( this, wxID_ANY, _("Under Speed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTextUnderSpeed->Wrap( -1 );
	m_fgAlarms->Add( m_stTextUnderSpeed, 0, wxALL, 5 );
	
	m_stUnderSpeed = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_stUnderSpeed->Wrap( -1 );
	m_fgAlarms->Add( m_stUnderSpeed, 0, wxALL, 5 );
	
	m_stTextOverSpeed = new wxStaticText( this, wxID_ANY, _("Over Speed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTextOverSpeed->Wrap( -1 );
	m_fgAlarms->Add( m_stTextOverSpeed, 0, wxALL, 5 );
	
	m_stOverSpeed = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_stOverSpeed->Wrap( -1 );
	m_fgAlarms->Add( m_stOverSpeed, 0, wxALL, 5 );
	
	m_stTextCourseError = new wxStaticText( this, wxID_ANY, _("Course Error"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTextCourseError->Wrap( -1 );
	m_fgAlarms->Add( m_stTextCourseError, 0, wxALL, 5 );
	
	m_stCourseError = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_stCourseError->Wrap( -1 );
	m_fgAlarms->Add( m_stCourseError, 0, wxALL, 5 );
	
	
	fgSizer8->Add( m_fgAlarms, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer71;
	fgSizer71 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer71->SetFlexibleDirection( wxBOTH );
	fgSizer71->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bPreferences = new wxButton( this, wxID_ANY, _("Preferences"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer71->Add( m_bPreferences, 0, wxALL, 5 );
	
	m_bResetLastAlarm = new wxButton( this, wxID_ANY, _("Reset Last Alarm"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer71->Add( m_bResetLastAlarm, 0, wxALL, 5 );
	
	m_bClose = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer71->Add( m_bClose, 0, wxALL, 5 );
	
	
	fgSizer8->Add( fgSizer71, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer8 );
	this->Layout();
	fgSizer8->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_bPreferences->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanDialogBase::OnPreferences ), NULL, this );
	m_bResetLastAlarm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanDialogBase::OnResetLastAlarm ), NULL, this );
	m_bClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanDialogBase::OnClose ), NULL, this );
}

WatchmanDialogBase::~WatchmanDialogBase()
{
	// Disconnect Events
	m_bPreferences->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanDialogBase::OnPreferences ), NULL, this );
	m_bResetLastAlarm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanDialogBase::OnResetLastAlarm ), NULL, this );
	m_bClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanDialogBase::OnClose ), NULL, this );
	
}

WatchmanPrefsDialogBase::WatchmanPrefsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Landfall Alarm") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer16;
	fgSizer16 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer16->SetFlexibleDirection( wxBOTH );
	fgSizer16->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbLandFallTime = new wxCheckBox( this, wxID_ANY, _("GPS fix crosses land in <"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer16->Add( m_cbLandFallTime, 0, wxALL, 5 );
	
	m_sLandFallTimeMinutes = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer16->Add( m_sLandFallTimeMinutes, 0, wxALL, 5 );
	
	m_staticText30 = new wxStaticText( this, wxID_ANY, _("minutes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText30->Wrap( -1 );
	fgSizer16->Add( m_staticText30, 0, wxALL, 5 );
	
	
	sbSizer6->Add( fgSizer16, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbLandFallDistance = new wxCheckBox( this, wxID_ANY, _("GPS fix is <"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_cbLandFallDistance, 0, wxALL, 5 );
	
	m_tcLandFallDistance = new wxTextCtrl( this, wxID_ANY, _("3"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_tcLandFallDistance, 0, wxALL, 5 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, _("nm from coastline"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer10->Add( m_staticText7, 0, wxALL, 5 );
	
	
	sbSizer6->Add( fgSizer10, 1, wxEXPAND, 5 );
	
	
	fgSizer4->Add( sbSizer6, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Deadman Alarm") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbDeadman = new wxCheckBox( this, wxID_ANY, _("No User Activity for"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_cbDeadman, 0, wxALL, 5 );
	
	m_sDeadmanMinutes = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000, 20 );
	fgSizer5->Add( m_sDeadmanMinutes, 0, wxALL, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, _("minute(s)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer5->Add( m_staticText6, 0, wxALL, 5 );
	
	
	sbSizer2->Add( fgSizer5, 1, wxEXPAND, 5 );
	
	
	fgSizer4->Add( sbSizer2, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("NMEA Data Alarm") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer91;
	fgSizer91 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer91->SetFlexibleDirection( wxBOTH );
	fgSizer91->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbGPSAlarm = new wxCheckBox( this, wxID_ANY, _("No GPS for >"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer91->Add( m_cbGPSAlarm, 0, wxALL, 5 );
	
	m_sGPSSeconds = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 1, 10000, 10 );
	fgSizer91->Add( m_sGPSSeconds, 0, wxALL, 5 );
	
	m_staticText13 = new wxStaticText( this, wxID_ANY, _("seconds"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer91->Add( m_staticText13, 0, wxALL, 5 );
	
	m_cbAISAlarm = new wxCheckBox( this, wxID_ANY, _("No AIS for >"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer91->Add( m_cbAISAlarm, 0, wxALL, 5 );
	
	m_sAISSeconds = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 1, 10000, 100 );
	fgSizer91->Add( m_sAISSeconds, 0, wxALL, 5 );
	
	m_staticText12 = new wxStaticText( this, wxID_ANY, _("seconds"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer91->Add( m_staticText12, 0, wxALL, 5 );
	
	
	sbSizer5->Add( fgSizer91, 1, wxEXPAND, 5 );
	
	
	fgSizer4->Add( sbSizer5, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Anchor Watch") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbAnchor = new wxCheckBox( this, wxID_ANY, _("Enabled"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( m_cbAnchor, 0, wxALL, 5 );
	
	m_bSyncToBoat = new wxButton( this, wxID_ANY, _("Sync to Boat"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( m_bSyncToBoat, 0, wxALL, 5 );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, _("Radius (m)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer9->Add( m_staticText9, 0, wxALL, 5 );
	
	m_sAnchorRadius = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000, 50 );
	fgSizer9->Add( m_sAnchorRadius, 0, wxALL, 5 );
	
	m_staticText71 = new wxStaticText( this, wxID_ANY, _("Latitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	fgSizer9->Add( m_staticText71, 0, wxALL, 5 );
	
	m_tAnchorLatitude = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( m_tAnchorLatitude, 0, wxALL, 5 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, _("Longitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer9->Add( m_staticText8, 0, wxALL, 5 );
	
	m_tAnchorLongitude = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( m_tAnchorLongitude, 0, wxALL, 5 );
	
	
	sbSizer3->Add( fgSizer9, 1, wxEXPAND, 5 );
	
	
	fgSizer4->Add( sbSizer3, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Speed Alarm") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbUnderSpeed = new wxCheckBox( this, wxID_ANY, _("Speed over ground <"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_cbUnderSpeed, 0, wxALL, 5 );
	
	m_tUnderSpeed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_tUnderSpeed, 0, wxALL, 5 );
	
	m_staticText24 = new wxStaticText( this, wxID_ANY, _("knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	fgSizer14->Add( m_staticText24, 0, wxALL, 5 );
	
	m_cbOverSpeed = new wxCheckBox( this, wxID_ANY, _("Speed over ground >"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_cbOverSpeed, 0, wxALL, 5 );
	
	m_tOverSpeed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_tOverSpeed, 0, wxALL, 5 );
	
	m_staticText25 = new wxStaticText( this, wxID_ANY, _("knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	fgSizer14->Add( m_staticText25, 0, wxALL, 5 );
	
	
	sbSizer7->Add( fgSizer14, 1, wxEXPAND, 5 );
	
	
	fgSizer4->Add( sbSizer7, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer61;
	sbSizer61 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Off Course Alarm") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer101;
	fgSizer101 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer101->SetFlexibleDirection( wxBOTH );
	fgSizer101->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbOffCourseAlarm = new wxCheckBox( this, wxID_ANY, _("Off Course By >"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer101->Add( m_cbOffCourseAlarm, 0, wxALL, 5 );
	
	m_sOffCourseDegrees = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 180, 20 );
	fgSizer101->Add( m_sOffCourseDegrees, 0, wxALL, 5 );
	
	m_staticText18 = new wxStaticText( this, wxID_ANY, _("Degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	fgSizer101->Add( m_staticText18, 0, wxALL, 5 );
	
	m_staticText21 = new wxStaticText( this, wxID_ANY, _("With Course Of"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	fgSizer101->Add( m_staticText21, 0, wxALL, 5 );
	
	m_sCourseDegrees = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 359, 0 );
	fgSizer101->Add( m_sCourseDegrees, 0, wxALL, 5 );
	
	m_bCurrentCourse = new wxButton( this, wxID_ANY, _("Current Course"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer101->Add( m_bCurrentCourse, 0, wxALL, 5 );
	
	
	sbSizer61->Add( fgSizer101, 1, wxEXPAND, 5 );
	
	
	fgSizer4->Add( sbSizer61, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Alarm Action") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer6->AddGrowableCol( 1 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbSound = new wxCheckBox( this, wxID_ANY, _("Sound"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbSound, 0, wxALL, 5 );
	
	m_fpSound = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a file"), wxT("Wav Files (*.wav)|*.WAV;*.wav|All Files (*.*)|*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	fgSizer6->Add( m_fpSound, 0, wxALL|wxEXPAND, 5 );
	
	m_cbCommand = new wxCheckBox( this, wxID_ANY, _("Command"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbCommand, 0, wxALL, 5 );
	
	m_tCommand = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_tCommand, 0, wxALL|wxEXPAND, 5 );
	
	m_cbMessageBox = new wxCheckBox( this, wxID_ANY, _("Message Box"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbMessageBox, 0, wxALL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_cbRepeat = new wxCheckBox( this, wxID_ANY, _("Repeat Alarm after seconds"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbRepeat, 0, wxALL, 5 );
	
	m_sRepeatSeconds = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 60 );
	fgSizer6->Add( m_sRepeatSeconds, 0, wxALL, 5 );
	
	m_cbAutoReset = new wxCheckBox( this, wxID_ANY, _("Automatically  Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbAutoReset, 0, wxALL, 5 );
	
	m_bTestAlarm = new wxButton( this, wxID_ANY, _("Test Alarm"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_bTestAlarm, 0, wxALL, 5 );
	
	
	sbSizer4->Add( fgSizer6, 1, wxEXPAND, 5 );
	
	
	fgSizer4->Add( sbSizer4, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer15;
	fgSizer15 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer15->SetFlexibleDirection( wxBOTH );
	fgSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_button6 = new wxButton( this, wxID_ANY, _("Information"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer15->Add( m_button6, 0, wxALL, 5 );
	
	m_button7 = new wxButton( this, wxID_ANY, _("About"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer15->Add( m_button7, 0, wxALL, 5 );
	
	
	fgSizer11->Add( fgSizer15, 1, wxEXPAND, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	
	fgSizer11->Add( m_sdbSizer1, 0, wxBOTTOM|wxEXPAND|wxTOP, 5 );
	
	
	fgSizer4->Add( fgSizer11, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer4 );
	this->Layout();
	fgSizer4->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_bSyncToBoat->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanPrefsDialogBase::OnSyncToBoat ), NULL, this );
	m_bCurrentCourse->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanPrefsDialogBase::OnCurrentCourse ), NULL, this );
	m_bTestAlarm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanPrefsDialogBase::OnTestAlarm ), NULL, this );
	m_button6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanPrefsDialogBase::OnInformation ), NULL, this );
	m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanPrefsDialogBase::OnAbout ), NULL, this );
}

WatchmanPrefsDialogBase::~WatchmanPrefsDialogBase()
{
	// Disconnect Events
	m_bSyncToBoat->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanPrefsDialogBase::OnSyncToBoat ), NULL, this );
	m_bCurrentCourse->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanPrefsDialogBase::OnCurrentCourse ), NULL, this );
	m_bTestAlarm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanPrefsDialogBase::OnTestAlarm ), NULL, this );
	m_button6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanPrefsDialogBase::OnInformation ), NULL, this );
	m_button7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanPrefsDialogBase::OnAbout ), NULL, this );
	
}

AboutDialogBase::AboutDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer90;
	fgSizer90 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer90->SetFlexibleDirection( wxBOTH );
	fgSizer90->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText110 = new wxStaticText( this, wxID_ANY, _("The watchman plugin for opencpn is made to add a range of alarms based on various conditions.\n\nOnly a very small fraction of possible alarms are implemented, patches are welcome.\n\nLicense: GPLv3+\n\nSource Code:\nhttps://github.com/seandepagnier/watchman_pi\n\nAuthor:\nSean D'Epagnier\n\nMany thanks to all of the translators and testers."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText110->Wrap( 400 );
	fgSizer90->Add( m_staticText110, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer91;
	fgSizer91 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer91->SetFlexibleDirection( wxBOTH );
	fgSizer91->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bDonate = new wxButton( this, wxID_ANY, _("Donate"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer91->Add( m_bDonate, 0, wxALL, 5 );
	
	m_bClose = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer91->Add( m_bClose, 0, wxALL, 5 );
	
	
	fgSizer90->Add( fgSizer91, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer90 );
	this->Layout();
	fgSizer90->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_bDonate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialogBase::OnDonate ), NULL, this );
	m_bClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialogBase::OnClose ), NULL, this );
}

AboutDialogBase::~AboutDialogBase()
{
	// Disconnect Events
	m_bDonate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialogBase::OnDonate ), NULL, this );
	m_bClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDialogBase::OnClose ), NULL, this );
	
}
