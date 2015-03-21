///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "WatchdogUI.h"

///////////////////////////////////////////////////////////////////////////

WatchdogDialogBase::WatchdogDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
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
	m_fgAlarms->AddGrowableCol( 1 );
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
	
	m_stTextNMEAData = new wxStaticText( this, wxID_ANY, _("Last NMEA Updates"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTextNMEAData->Wrap( -1 );
	m_fgAlarms->Add( m_stTextNMEAData, 0, wxALL, 5 );
	
	m_stNMEAData = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_stNMEAData->Wrap( -1 );
	m_fgAlarms->Add( m_stNMEAData, 0, wxALL, 5 );
	
	m_stTextDeadman = new wxStaticText( this, wxID_ANY, _("Deadman activity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTextDeadman->Wrap( -1 );
	m_fgAlarms->Add( m_stTextDeadman, 0, wxALL, 5 );
	
	m_stDeadman = new wxStaticText( this, wxID_ANY, _("No Updates"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stDeadman->Wrap( -1 );
	m_fgAlarms->Add( m_stDeadman, 0, wxALL, 5 );
	
	m_stTextAnchor = new wxStaticText( this, wxID_ANY, _("Distance to Anchor"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTextAnchor->Wrap( -1 );
	m_fgAlarms->Add( m_stTextAnchor, 0, wxALL, 5 );
	
	m_stAnchorDistance = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
	m_stAnchorDistance->Wrap( -1 );
	m_fgAlarms->Add( m_stAnchorDistance, 0, wxALL, 5 );
	
	m_stTextCourseError = new wxStaticText( this, wxID_ANY, _("Course Error"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTextCourseError->Wrap( -1 );
	m_fgAlarms->Add( m_stTextCourseError, 0, wxALL, 5 );
	
	m_stCourseError = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_stCourseError->Wrap( -1 );
	m_fgAlarms->Add( m_stCourseError, 0, wxALL, 5 );
	
	m_stTextStarboardCourseError = new wxStaticText( this, wxID_ANY, _("Course Error Starboard"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTextStarboardCourseError->Wrap( -1 );
	m_fgAlarms->Add( m_stTextStarboardCourseError, 0, wxALL, 5 );
	
	m_stStarboardCourseError = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_stStarboardCourseError->Wrap( -1 );
	m_fgAlarms->Add( m_stStarboardCourseError, 0, wxALL, 5 );
	
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
	
	
	fgSizer8->Add( m_fgAlarms, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer71;
	fgSizer71 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer71->SetFlexibleDirection( wxBOTH );
	fgSizer71->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bPreferences = new wxButton( this, wxID_ANY, _("Preferences"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer71->Add( m_bPreferences, 0, wxALL, 5 );
	
	m_bReset = new wxButton( this, wxID_ANY, _("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer71->Add( m_bReset, 0, wxALL, 5 );
	
	m_bClose = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer71->Add( m_bClose, 0, wxALL, 5 );
	
	
	fgSizer8->Add( fgSizer71, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer8 );
	this->Layout();
	fgSizer8->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_bPreferences->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogDialogBase::OnPreferences ), NULL, this );
	m_bReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogDialogBase::OnReset ), NULL, this );
	m_bClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogDialogBase::OnClose ), NULL, this );
}

WatchdogDialogBase::~WatchdogDialogBase()
{
	// Disconnect Events
	m_bPreferences->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogDialogBase::OnPreferences ), NULL, this );
	m_bReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogDialogBase::OnReset ), NULL, this );
	m_bClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogDialogBase::OnClose ), NULL, this );
	
}

WatchdogPrefsDialogBase::WatchdogPrefsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer41;
	sbSizer41 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("General Setup") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer241;
	fgSizer241 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer241->SetFlexibleDirection( wxBOTH );
	fgSizer241->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_rbAlways = new wxRadioButton( this, wxID_ANY, _("Enable Alarms"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer241->Add( m_rbAlways, 0, wxALL, 5 );
	
	m_rbOnce = new wxRadioButton( this, wxID_ANY, _("Enabled first time Dialog is opened"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer241->Add( m_rbOnce, 0, wxALL, 5 );
	
	m_rbVisible = new wxRadioButton( this, wxID_ANY, _("Enabled only if Dialog is visible"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer241->Add( m_rbVisible, 0, wxALL, 5 );
	
	m_rbNever = new wxRadioButton( this, wxID_ANY, _("Disable Alarms"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer241->Add( m_rbNever, 0, wxALL, 5 );
	
	
	sbSizer41->Add( fgSizer241, 1, wxEXPAND, 5 );
	
	
	fgSizer4->Add( sbSizer41, 1, wxEXPAND, 5 );
	
	m_lbAlarm = new wxListbook( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxLB_DEFAULT );
	m_lbAlarm->SetMinSize( wxSize( -1,200 ) );
	
	m_panel1 = new wxPanel( m_lbAlarm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer17;
	fgSizer17 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer17->SetFlexibleDirection( wxBOTH );
	fgSizer17->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer16;
	fgSizer16 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer16->SetFlexibleDirection( wxBOTH );
	fgSizer16->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbLandFallTime = new wxCheckBox( m_panel1, wxID_ANY, _("GPS course crosses land in <"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer16->Add( m_cbLandFallTime, 0, wxALL, 5 );
	
	m_sLandFallTimeMinutes = new wxSpinCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 20 );
	fgSizer16->Add( m_sLandFallTimeMinutes, 0, wxALL, 5 );
	
	m_staticText30 = new wxStaticText( m_panel1, wxID_ANY, _("minutes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText30->Wrap( -1 );
	fgSizer16->Add( m_staticText30, 0, wxALL, 5 );
	
	
	fgSizer17->Add( fgSizer16, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbLandFallDistance = new wxCheckBox( m_panel1, wxID_ANY, _("GPS fix is <"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_cbLandFallDistance, 0, wxALL, 5 );
	
	m_tcLandFallDistance = new wxTextCtrl( m_panel1, wxID_ANY, _("3"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_tcLandFallDistance, 0, wxALL, 5 );
	
	m_staticText7 = new wxStaticText( m_panel1, wxID_ANY, _("nm from coastline\n(any direction)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer10->Add( m_staticText7, 0, wxALL, 5 );
	
	
	fgSizer17->Add( fgSizer10, 1, wxEXPAND, 5 );
	
	m_staticText39122 = new wxStaticText( m_panel1, wxID_ANY, _("Graphical overlay for time displays line from boat to location of land crossing"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39122->Wrap( 400 );
	fgSizer17->Add( m_staticText39122, 0, wxALL, 5 );
	
	m_staticText45 = new wxStaticText( m_panel1, wxID_ANY, _("Landfall detection uses gshhs (global heirarchical high-resolution shoreline data.)\nConsider installing the highest resolution database for best accuracy."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText45->Wrap( 350 );
	fgSizer17->Add( m_staticText45, 0, wxALL, 5 );
	
	
	m_panel1->SetSizer( fgSizer17 );
	m_panel1->Layout();
	fgSizer17->Fit( m_panel1 );
	m_lbAlarm->AddPage( m_panel1, _("Landfall"), true );
	m_panel2 = new wxPanel( m_lbAlarm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer91;
	fgSizer91 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer91->AddGrowableCol( 0 );
	fgSizer91->SetFlexibleDirection( wxBOTH );
	fgSizer91->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText34 = new wxStaticText( m_panel2, wxID_ANY, _("Require following NMEA sentences:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText34->Wrap( -1 );
	fgSizer91->Add( m_staticText34, 0, wxALL, 5 );
	
	m_tNMEASentences = new wxTextCtrl( m_panel2, wxID_ANY, _("$GPGGA"), wxDefaultPosition, wxSize( -1,60 ), wxTE_MULTILINE );
	fgSizer91->Add( m_tNMEASentences, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer19;
	fgSizer19 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer19->SetFlexibleDirection( wxBOTH );
	fgSizer19->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText35 = new wxStaticText( m_panel2, wxID_ANY, _("at least every"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35->Wrap( -1 );
	fgSizer19->Add( m_staticText35, 0, wxALL, 5 );
	
	m_sNMEASeconds = new wxSpinCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 1, 10000, 10 );
	fgSizer19->Add( m_sNMEASeconds, 0, wxALL, 5 );
	
	m_staticText13 = new wxStaticText( m_panel2, wxID_ANY, _("seconds"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer19->Add( m_staticText13, 0, wxALL, 5 );
	
	
	fgSizer91->Add( fgSizer19, 1, wxEXPAND, 5 );
	
	
	m_panel2->SetSizer( fgSizer91 );
	m_panel2->Layout();
	fgSizer91->Fit( m_panel2 );
	m_lbAlarm->AddPage( m_panel2, _("NMEA Data"), false );
	m_panel3 = new wxPanel( m_lbAlarm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText33 = new wxStaticText( m_panel3, wxID_ANY, _("No User Activity for"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText33->Wrap( -1 );
	fgSizer5->Add( m_staticText33, 0, wxALL, 5 );
	
	m_sDeadmanMinutes = new wxSpinCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000, 20 );
	fgSizer5->Add( m_sDeadmanMinutes, 0, wxALL, 5 );
	
	m_staticText6 = new wxStaticText( m_panel3, wxID_ANY, _("minute(s)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer5->Add( m_staticText6, 0, wxALL, 5 );
	
	
	m_panel3->SetSizer( fgSizer5 );
	m_panel3->Layout();
	fgSizer5->Fit( m_panel3 );
	m_lbAlarm->AddPage( m_panel3, _("Deadman"), false );
	m_panel4 = new wxPanel( m_lbAlarm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer24;
	fgSizer24 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer24->SetFlexibleDirection( wxBOTH );
	fgSizer24->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText71 = new wxStaticText( m_panel4, wxID_ANY, _("Latitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	fgSizer9->Add( m_staticText71, 0, wxALL, 5 );
	
	m_tAnchorLatitude = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( m_tAnchorLatitude, 0, wxALL, 5 );
	
	
	fgSizer9->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText8 = new wxStaticText( m_panel4, wxID_ANY, _("Longitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer9->Add( m_staticText8, 0, wxALL, 5 );
	
	m_tAnchorLongitude = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( m_tAnchorLongitude, 0, wxALL, 5 );
	
	m_bSyncToBoat = new wxButton( m_panel4, wxID_ANY, _("Sync to Boat"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( m_bSyncToBoat, 0, wxALL, 5 );
	
	m_staticText9 = new wxStaticText( m_panel4, wxID_ANY, _("Radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer9->Add( m_staticText9, 0, wxALL, 5 );
	
	m_sAnchorRadius = new wxSpinCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100000, 50 );
	fgSizer9->Add( m_sAnchorRadius, 0, wxALL, 5 );
	
	m_staticText39 = new wxStaticText( m_panel4, wxID_ANY, _("meters"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39->Wrap( -1 );
	fgSizer9->Add( m_staticText39, 0, wxALL, 5 );
	
	
	fgSizer24->Add( fgSizer9, 1, wxEXPAND, 5 );
	
	m_staticText39121 = new wxStaticText( m_panel4, wxID_ANY, _("Graphical overlay displays bounding anchor area"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39121->Wrap( -1 );
	fgSizer24->Add( m_staticText39121, 0, wxALL, 5 );
	
	
	m_panel4->SetSizer( fgSizer24 );
	m_panel4->Layout();
	fgSizer24->Fit( m_panel4 );
	m_lbAlarm->AddPage( m_panel4, _("Anchor Watch"), false );
	m_pCourse = new wxPanel( m_lbAlarm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer22;
	fgSizer22 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer22->SetFlexibleDirection( wxBOTH );
	fgSizer22->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer101;
	fgSizer101 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer101->SetFlexibleDirection( wxBOTH );
	fgSizer101->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText32 = new wxStaticText( m_pCourse, wxID_ANY, _("Off Course By >"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	fgSizer101->Add( m_staticText32, 0, wxALL, 5 );
	
	m_sCourseTolerance = new wxSpinCtrl( m_pCourse, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 180, 20 );
	fgSizer101->Add( m_sCourseTolerance, 0, wxALL, 5 );
	
	m_staticText18 = new wxStaticText( m_pCourse, wxID_ANY, _("Degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	fgSizer101->Add( m_staticText18, 0, wxALL, 5 );
	
	m_staticText21 = new wxStaticText( m_pCourse, wxID_ANY, _("With Course Of"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	fgSizer101->Add( m_staticText21, 0, wxALL, 5 );
	
	m_sCourse = new wxSpinCtrl( m_pCourse, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 359, 0 );
	fgSizer101->Add( m_sCourse, 0, wxALL, 5 );
	
	m_bCurrentCourse = new wxButton( m_pCourse, wxID_ANY, _("Current Course"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer101->Add( m_bCurrentCourse, 0, wxALL, 5 );
	
	
	fgSizer22->Add( fgSizer101, 1, wxEXPAND, 5 );
	
	m_cbSeparatePortAndStarboard = new wxCheckBox( m_pCourse, wxID_ANY, _("Separate Port and Starboard Alarms"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer22->Add( m_cbSeparatePortAndStarboard, 0, wxALL, 5 );
	
	m_staticText3912 = new wxStaticText( m_pCourse, wxID_ANY, _("Graphical overlay displays lines bounding valid courses"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3912->Wrap( -1 );
	fgSizer22->Add( m_staticText3912, 0, wxALL, 5 );
	
	
	m_pCourse->SetSizer( fgSizer22 );
	m_pCourse->Layout();
	fgSizer22->Fit( m_pCourse );
	m_lbAlarm->AddPage( m_pCourse, _("Off Course"), false );
	m_pCourseStarboard = new wxPanel( m_lbAlarm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer221;
	fgSizer221 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer221->SetFlexibleDirection( wxBOTH );
	fgSizer221->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText50 = new wxStaticText( m_pCourseStarboard, wxID_ANY, _("Settings the same as Off Course Port, with different Alarm Actions possible."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText50->Wrap( 300 );
	fgSizer221->Add( m_staticText50, 0, wxALL, 5 );
	
	
	m_pCourseStarboard->SetSizer( fgSizer221 );
	m_pCourseStarboard->Layout();
	fgSizer221->Fit( m_pCourseStarboard );
	m_lbAlarm->AddPage( m_pCourseStarboard, _("Off Course Starboard"), false );
	m_panel5 = new wxPanel( m_lbAlarm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( m_panel5, wxID_ANY, _("Speed Alarm") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText44 = new wxStaticText( m_panel5, wxID_ANY, _("Speed over ground <"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	fgSizer14->Add( m_staticText44, 0, wxALL, 5 );
	
	m_tUnderSpeed = new wxTextCtrl( m_panel5, wxID_ANY, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_tUnderSpeed, 0, wxALL, 5 );
	
	m_staticText24 = new wxStaticText( m_panel5, wxID_ANY, _("knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	fgSizer14->Add( m_staticText24, 0, wxALL, 5 );
	
	
	sbSizer7->Add( fgSizer14, 1, wxEXPAND, 5 );
	
	m_staticText391 = new wxStaticText( m_panel5, wxID_ANY, _("Graphical overlay displays circle with radius of this speed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText391->Wrap( -1 );
	sbSizer7->Add( m_staticText391, 0, wxALL, 5 );
	
	
	m_panel5->SetSizer( sbSizer7 );
	m_panel5->Layout();
	sbSizer7->Fit( m_panel5 );
	m_lbAlarm->AddPage( m_panel5, _("Under Speed"), false );
	m_panel51 = new wxPanel( m_lbAlarm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer71;
	sbSizer71 = new wxStaticBoxSizer( new wxStaticBox( m_panel51, wxID_ANY, _("Speed Alarm") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer141;
	fgSizer141 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer141->SetFlexibleDirection( wxBOTH );
	fgSizer141->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText37 = new wxStaticText( m_panel51, wxID_ANY, _("Speed over ground >"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText37->Wrap( -1 );
	fgSizer141->Add( m_staticText37, 0, wxALL, 5 );
	
	m_tOverSpeed = new wxTextCtrl( m_panel51, wxID_ANY, _("12"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer141->Add( m_tOverSpeed, 0, wxALL, 5 );
	
	m_staticText251 = new wxStaticText( m_panel51, wxID_ANY, _("knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText251->Wrap( -1 );
	fgSizer141->Add( m_staticText251, 0, wxALL, 5 );
	
	
	sbSizer71->Add( fgSizer141, 1, wxEXPAND, 5 );
	
	m_staticText3911 = new wxStaticText( m_panel51, wxID_ANY, _("Graphical overlay displays circle with radius of this speed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3911->Wrap( -1 );
	sbSizer71->Add( m_staticText3911, 0, wxALL, 5 );
	
	
	m_panel51->SetSizer( sbSizer71 );
	m_panel51->Layout();
	sbSizer71->Fit( m_panel51 );
	m_lbAlarm->AddPage( m_panel51, _("Over Speed"), false );
	m_panel10 = new wxPanel( m_lbAlarm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer211;
	fgSizer211 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer211->SetFlexibleDirection( wxBOTH );
	fgSizer211->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText401 = new wxStaticText( m_panel10, wxID_ANY, _("Not implemented"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText401->Wrap( -1 );
	fgSizer211->Add( m_staticText401, 0, wxALL, 5 );
	
	
	m_panel10->SetSizer( fgSizer211 );
	m_panel10->Layout();
	fgSizer211->Fit( m_panel10 );
	m_lbAlarm->AddPage( m_panel10, _("Depth"), false );
	m_panel11 = new wxPanel( m_lbAlarm, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer21;
	fgSizer21 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer21->SetFlexibleDirection( wxBOTH );
	fgSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText40 = new wxStaticText( m_panel11, wxID_ANY, _("Not implemented"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText40->Wrap( -1 );
	fgSizer21->Add( m_staticText40, 0, wxALL, 5 );
	
	
	m_panel11->SetSizer( fgSizer21 );
	m_panel11->Layout();
	fgSizer21->Fit( m_panel11 );
	m_lbAlarm->AddPage( m_panel11, _("Wind"), false );
	#ifndef __WXGTK__ // Small icon style not supported in GTK
	wxListView* m_lbAlarmListView = m_lbAlarm->GetListView();
	long m_lbAlarmFlags = m_lbAlarmListView->GetWindowStyleFlag();
	m_lbAlarmFlags = ( m_lbAlarmFlags & ~wxLC_ICON ) | wxLC_SMALL_ICON;
	m_lbAlarmListView->SetWindowStyleFlag( m_lbAlarmFlags );
	#endif
	
	fgSizer4->Add( m_lbAlarm, 1, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer171;
	fgSizer171 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer171->SetFlexibleDirection( wxBOTH );
	fgSizer171->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_rbEnabled = new wxRadioButton( this, wxID_ANY, _("Enabled"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	fgSizer171->Add( m_rbEnabled, 0, wxALL, 5 );
	
	m_rbDisabled = new wxRadioButton( this, wxID_ANY, _("Disabled"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer171->Add( m_rbDisabled, 0, wxALL, 5 );
	
	m_cbGraphicsEnabled = new wxCheckBox( this, wxID_ANY, _("Graphical Overlay Display"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer171->Add( m_cbGraphicsEnabled, 0, wxALL, 5 );
	
	
	fgSizer4->Add( fgSizer171, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Alarm Action") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer6->AddGrowableCol( 1 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbSound = new wxCheckBox( this, wxID_ANY, _("Sound"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbSound, 0, wxALL, 5 );
	
	m_fpSound = new wxFilePickerCtrl( this, wxID_ANY, wxT("/initrd.img"), _("Select a file"), wxT("Wav Files (*.wav)|*.WAV;*.wav|All Files (*.*)|*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
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
	fgSizer11 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer11->AddGrowableCol( 1 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer15;
	fgSizer15 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer15->SetFlexibleDirection( wxBOTH );
	fgSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_button6 = new wxButton( this, wxID_ANY, _("Information"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer15->Add( m_button6, 0, wxALL, 5 );
	
	m_button7 = new wxButton( this, wxID_ANY, _("About Author"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer15->Add( m_button7, 0, wxALL, 5 );
	
	
	fgSizer11->Add( fgSizer15, 1, wxEXPAND, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1->Realize();
	
	fgSizer11->Add( m_sdbSizer1, 0, wxALIGN_RIGHT|wxBOTTOM|wxEXPAND|wxTOP, 5 );
	
	
	fgSizer4->Add( fgSizer11, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer4 );
	this->Layout();
	fgSizer4->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_rbAlways->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnEnabled ), NULL, this );
	m_rbOnce->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnEnabled ), NULL, this );
	m_rbVisible->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnEnabled ), NULL, this );
	m_rbNever->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnEnabled ), NULL, this );
	m_lbAlarm->Connect( wxEVT_COMMAND_LISTBOOK_PAGE_CHANGED, wxListbookEventHandler( WatchdogPrefsDialogBase::OnAlarmChanged ), NULL, this );
	m_cbLandFallTime->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_sLandFallTimeMinutes->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateSpin ), NULL, this );
	m_cbLandFallDistance->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_tcLandFallDistance->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_tNMEASentences->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_sNMEASeconds->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateSpin ), NULL, this );
	m_sDeadmanMinutes->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateSpin ), NULL, this );
	m_tAnchorLatitude->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_tAnchorLongitude->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_bSyncToBoat->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnSyncToBoat ), NULL, this );
	m_sAnchorRadius->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateSpin ), NULL, this );
	m_sCourseTolerance->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateSpin ), NULL, this );
	m_sCourse->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateSpin ), NULL, this );
	m_bCurrentCourse->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnCurrentCourse ), NULL, this );
	m_cbSeparatePortAndStarboard->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnCheckSeparatePortAndStarboard ), NULL, this );
	m_tUnderSpeed->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_tOverSpeed->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_rbEnabled->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateRB ), NULL, this );
	m_rbDisabled->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateRB ), NULL, this );
	m_cbGraphicsEnabled->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_cbSound->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_fpSound->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateSound ), NULL, this );
	m_cbCommand->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_tCommand->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateCommand ), NULL, this );
	m_cbMessageBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_cbRepeat->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_sRepeatSeconds->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateRepeat ), NULL, this );
	m_cbAutoReset->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_bTestAlarm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnTestAlarm ), NULL, this );
	m_button6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnInformation ), NULL, this );
	m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAboutAuthor ), NULL, this );
}

WatchdogPrefsDialogBase::~WatchdogPrefsDialogBase()
{
	// Disconnect Events
	m_rbAlways->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnEnabled ), NULL, this );
	m_rbOnce->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnEnabled ), NULL, this );
	m_rbVisible->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnEnabled ), NULL, this );
	m_rbNever->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnEnabled ), NULL, this );
	m_lbAlarm->Disconnect( wxEVT_COMMAND_LISTBOOK_PAGE_CHANGED, wxListbookEventHandler( WatchdogPrefsDialogBase::OnAlarmChanged ), NULL, this );
	m_cbLandFallTime->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_sLandFallTimeMinutes->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateSpin ), NULL, this );
	m_cbLandFallDistance->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_tcLandFallDistance->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_tNMEASentences->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_sNMEASeconds->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateSpin ), NULL, this );
	m_sDeadmanMinutes->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateSpin ), NULL, this );
	m_tAnchorLatitude->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_tAnchorLongitude->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_bSyncToBoat->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnSyncToBoat ), NULL, this );
	m_sAnchorRadius->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateSpin ), NULL, this );
	m_sCourseTolerance->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateSpin ), NULL, this );
	m_sCourse->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateSpin ), NULL, this );
	m_bCurrentCourse->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnCurrentCourse ), NULL, this );
	m_cbSeparatePortAndStarboard->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnCheckSeparatePortAndStarboard ), NULL, this );
	m_tUnderSpeed->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_tOverSpeed->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_rbEnabled->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateRB ), NULL, this );
	m_rbDisabled->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateRB ), NULL, this );
	m_cbGraphicsEnabled->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_cbSound->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_fpSound->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateSound ), NULL, this );
	m_cbCommand->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_tCommand->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateCommand ), NULL, this );
	m_cbMessageBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_cbRepeat->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_sRepeatSeconds->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdateRepeat ), NULL, this );
	m_cbAutoReset->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAlarmUpdate ), NULL, this );
	m_bTestAlarm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnTestAlarm ), NULL, this );
	m_button6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnInformation ), NULL, this );
	m_button7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogPrefsDialogBase::OnAboutAuthor ), NULL, this );
	
}
