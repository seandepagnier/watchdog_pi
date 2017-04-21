///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jul 15 2016)
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
	fgSizer8 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer8->AddGrowableCol( 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	m_lStatus = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_NO_HEADER|wxLC_REPORT );
	fgSizer8->Add( m_lStatus, 1, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer71;
	fgSizer71 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer71->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer71->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bConfiguration = new wxButton( this, wxID_ANY, _("Configuration"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer71->Add( m_bConfiguration, 0, wxALL, 5 );
	
	m_bReset = new wxButton( this, wxID_ANY, _("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer71->Add( m_bReset, 0, wxALL, 5 );
	
	m_bClose = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer71->Add( m_bClose, 0, wxALL, 5 );
	
	
	fgSizer8->Add( fgSizer71, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer8 );
	this->Layout();
	fgSizer8->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_lStatus->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( WatchdogDialogBase::OnDoubleClick ), NULL, this );
	m_lStatus->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( WatchdogDialogBase::OnLeftDown ), NULL, this );
	m_bConfiguration->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogDialogBase::OnConfiguration ), NULL, this );
	m_bReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogDialogBase::OnReset ), NULL, this );
	m_bClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogDialogBase::OnClose ), NULL, this );
}

WatchdogDialogBase::~WatchdogDialogBase()
{
	// Disconnect Events
	m_lStatus->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( WatchdogDialogBase::OnDoubleClick ), NULL, this );
	m_lStatus->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( WatchdogDialogBase::OnLeftDown ), NULL, this );
	m_bConfiguration->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogDialogBase::OnConfiguration ), NULL, this );
	m_bReset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogDialogBase::OnReset ), NULL, this );
	m_bClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogDialogBase::OnClose ), NULL, this );
	
}

WatchdogPropertiesDialogBase::WatchdogPropertiesDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_fgSizerProperties = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgSizerProperties->SetFlexibleDirection( wxBOTH );
	m_fgSizerProperties->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextName = new wxStaticText( this, wxID_ANY, _("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextName->Wrap( -1 );
	m_fgSizerProperties->Add( m_staticTextName, 0, wxALL, 5 );
	
	m_staticTextNameVal = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextNameVal->Wrap( -1 );
	m_fgSizerProperties->Add( m_staticTextNameVal, 0, wxALL, 5 );
	
	m_staticTextVersion = new wxStaticText( this, wxID_ANY, _("Version:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextVersion->Wrap( -1 );
	m_fgSizerProperties->Add( m_staticTextVersion, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer34;
	fgSizer34 = new wxFlexGridSizer( 0, 6, 0, 0 );
	fgSizer34->SetFlexibleDirection( wxBOTH );
	fgSizer34->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextMajor = new wxStaticText( this, wxID_ANY, _("Major:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMajor->Wrap( -1 );
	fgSizer34->Add( m_staticTextMajor, 0, wxALL, 5 );
	
	m_staticTextMajorVal = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMajorVal->Wrap( -1 );
	fgSizer34->Add( m_staticTextMajorVal, 0, wxALL, 5 );
	
	m_staticTextMinor = new wxStaticText( this, wxID_ANY, _("Minor:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMinor->Wrap( -1 );
	fgSizer34->Add( m_staticTextMinor, 0, wxALL, 5 );
	
	m_staticTextMinorVal = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMinorVal->Wrap( -1 );
	fgSizer34->Add( m_staticTextMinorVal, 0, wxALL, 5 );
	
	m_staticTextPatch = new wxStaticText( this, wxID_ANY, _("Patch:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPatch->Wrap( -1 );
	fgSizer34->Add( m_staticTextPatch, 0, wxALL, 5 );
	
	m_staticTextPatchVal = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPatchVal->Wrap( -1 );
	fgSizer34->Add( m_staticTextPatchVal, 0, wxALL, 5 );
	
	
	m_fgSizerProperties->Add( fgSizer34, 1, wxEXPAND, 5 );
	
	m_staticTextDate = new wxStaticText( this, wxID_ANY, _("Date:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDate->Wrap( -1 );
	m_fgSizerProperties->Add( m_staticTextDate, 0, wxALL, 5 );
	
	m_staticTextDateVal = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDateVal->Wrap( -1 );
	m_fgSizerProperties->Add( m_staticTextDateVal, 0, wxALL, 5 );
	
	m_staticTextOther = new wxStaticText( this, wxID_ANY, _("Other:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOther->Wrap( -1 );
	m_fgSizerProperties->Add( m_staticTextOther, 0, wxALL, 5 );
	
	m_staticTextOtherVal = new wxStaticText( this, wxID_ANY, _("Please report problems using FlySpray at:\nhttp://willkamp.com/opencpn/flyspray/index.php?project=0&do=index&switch=1\n\nor\n\nat the OpenCPN forum:\nhttp://www.cruisersforum.com/forums/f134\n\nor\n\nRaise an issue in GIT at:\nhttps://github.com/seandepagnier/watchdog_pi/issues\n"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOtherVal->Wrap( -1 );
	m_fgSizerProperties->Add( m_staticTextOtherVal, 0, wxALL, 5 );
	
	wxBoxSizer* bSizerOK;
	bSizerOK = new wxBoxSizer( wxVERTICAL );
	
	m_buttonOK = new wxButton( this, wxID_ANY, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOK->Add( m_buttonOK, 0, wxALL, 5 );
	
	
	m_fgSizerProperties->Add( bSizerOK, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( m_fgSizerProperties );
	this->Layout();
	m_fgSizerProperties->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogPropertiesDialogBase::OnWatchdogPropertiesOKClick ), NULL, this );
}

WatchdogPropertiesDialogBase::~WatchdogPropertiesDialogBase()
{
	// Disconnect Events
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogPropertiesDialogBase::OnWatchdogPropertiesOKClick ), NULL, this );
	
}

ConfigurationDialogBase::ConfigurationDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
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
	
	m_rbAlways = new wxRadioButton( this, wxID_ANY, _("Enable All Alarms"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer241->Add( m_rbAlways, 0, wxALL, 5 );
	
	m_rbOnce = new wxRadioButton( this, wxID_ANY, _("Enabled after first time Watchdog Dialog is visible"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer241->Add( m_rbOnce, 0, wxALL, 5 );
	
	m_rbVisible = new wxRadioButton( this, wxID_ANY, _("Enabled only if Watchdog Dialog is visible"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer241->Add( m_rbVisible, 0, wxALL, 5 );
	
	m_rbNever = new wxRadioButton( this, wxID_ANY, _("Disable All Alarms"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer241->Add( m_rbNever, 0, wxALL, 5 );
	
	
	sbSizer41->Add( fgSizer241, 1, wxEXPAND, 5 );
	
	
	fgSizer4->Add( sbSizer41, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer43;
	fgSizer43 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer43->AddGrowableCol( 0 );
	fgSizer43->AddGrowableRow( 0 );
	fgSizer43->SetFlexibleDirection( wxBOTH );
	fgSizer43->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Alarms") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer29;
	fgSizer29 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer29->AddGrowableCol( 1 );
	fgSizer29->AddGrowableRow( 0 );
	fgSizer29->SetFlexibleDirection( wxBOTH );
	fgSizer29->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer31;
	fgSizer31 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer31->SetFlexibleDirection( wxBOTH );
	fgSizer31->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bNew = new wxButton( this, wxID_ANY, _("New"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer31->Add( m_bNew, 0, wxALL, 5 );
	
	m_bEdit = new wxButton( this, wxID_ANY, _("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer31->Add( m_bEdit, 0, wxALL, 5 );
	
	m_bDelete = new wxButton( this, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer31->Add( m_bDelete, 0, wxALL, 5 );
	
	m_bDeleteAll = new wxButton( this, wxID_ANY, _("Delete All"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer31->Add( m_bDeleteAll, 0, wxALL, 5 );
	
	
	fgSizer29->Add( fgSizer31, 1, wxEXPAND, 5 );
	
	m_lAlarms = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL );
	fgSizer29->Add( m_lAlarms, 1, wxALL|wxEXPAND, 5 );
	
	
	sbSizer7->Add( fgSizer29, 1, wxEXPAND, 5 );
	
	
	fgSizer43->Add( sbSizer7, 1, wxEXPAND, 5 );
	
	
	fgSizer4->Add( fgSizer43, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer11->AddGrowableCol( 1 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_button7 = new wxButton( this, wxID_ANY, _("About Author"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer11->Add( m_button7, 0, wxALL, 5 );
	
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
	m_rbAlways->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ConfigurationDialogBase::OnEnabled ), NULL, this );
	m_rbOnce->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ConfigurationDialogBase::OnEnabled ), NULL, this );
	m_rbVisible->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ConfigurationDialogBase::OnEnabled ), NULL, this );
	m_rbNever->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ConfigurationDialogBase::OnEnabled ), NULL, this );
	m_bNew->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnNewAlarm ), NULL, this );
	m_bEdit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnEditAlarm ), NULL, this );
	m_bDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnDeleteAlarm ), NULL, this );
	m_bDeleteAll->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnDeleteAllAlarms ), NULL, this );
	m_lAlarms->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( ConfigurationDialogBase::OnDoubleClick ), NULL, this );
	m_lAlarms->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( ConfigurationDialogBase::AlarmSelected ), NULL, this );
	m_lAlarms->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( ConfigurationDialogBase::AlarmSelected ), NULL, this );
	m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnAboutAuthor ), NULL, this );
}

ConfigurationDialogBase::~ConfigurationDialogBase()
{
	// Disconnect Events
	m_rbAlways->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ConfigurationDialogBase::OnEnabled ), NULL, this );
	m_rbOnce->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ConfigurationDialogBase::OnEnabled ), NULL, this );
	m_rbVisible->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ConfigurationDialogBase::OnEnabled ), NULL, this );
	m_rbNever->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ConfigurationDialogBase::OnEnabled ), NULL, this );
	m_bNew->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnNewAlarm ), NULL, this );
	m_bEdit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnEditAlarm ), NULL, this );
	m_bDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnDeleteAlarm ), NULL, this );
	m_bDeleteAll->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnDeleteAllAlarms ), NULL, this );
	m_lAlarms->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( ConfigurationDialogBase::OnDoubleClick ), NULL, this );
	m_lAlarms->Disconnect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( ConfigurationDialogBase::AlarmSelected ), NULL, this );
	m_lAlarms->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( ConfigurationDialogBase::AlarmSelected ), NULL, this );
	m_button7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigurationDialogBase::OnAboutAuthor ), NULL, this );
	
}

NewAlarmDialogBase::NewAlarmDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer40;
	fgSizer40 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer40->AddGrowableCol( 0 );
	fgSizer40->AddGrowableRow( 0 );
	fgSizer40->SetFlexibleDirection( wxBOTH );
	fgSizer40->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lAlarmType = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_NO_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL );
	fgSizer40->Add( m_lAlarmType, 0, wxALL|wxEXPAND, 5 );
	
	m_sdbSizer3 = new wxStdDialogButtonSizer();
	m_sdbSizer3OK = new wxButton( this, wxID_OK );
	m_sdbSizer3->AddButton( m_sdbSizer3OK );
	m_sdbSizer3Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer3->AddButton( m_sdbSizer3Cancel );
	m_sdbSizer3->Realize();
	
	fgSizer40->Add( m_sdbSizer3, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer40 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_lAlarmType->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( NewAlarmDialogBase::OnDoubleClick ), NULL, this );
}

NewAlarmDialogBase::~NewAlarmDialogBase()
{
	// Disconnect Events
	m_lAlarmType->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( NewAlarmDialogBase::OnDoubleClick ), NULL, this );
	
}

EditAlarmDialogBase::EditAlarmDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_fgSizer = new wxFlexGridSizer( 0, 1, 0, 0 );
	m_fgSizer->AddGrowableRow( 0 );
	m_fgSizer->SetFlexibleDirection( wxBOTH );
	m_fgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
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
	
	m_cbAutoReset = new wxCheckBox( this, wxID_ANY, _("Automatically Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbAutoReset, 0, wxALL, 5 );
	
	m_cbgfxEnabled = new wxCheckBox( this, wxID_ANY, _("Graphics Enabled"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbgfxEnabled, 0, wxALL, 5 );
	
	m_bTest = new wxButton( this, wxID_ANY, _("Test"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_bTest, 0, wxALL, 5 );
	
	
	sbSizer4->Add( fgSizer6, 1, wxEXPAND, 5 );
	
	
	m_fgSizer->Add( sbSizer4, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer15;
	fgSizer15 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer15->AddGrowableCol( 0 );
	fgSizer15->SetFlexibleDirection( wxBOTH );
	fgSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_button6 = new wxButton( this, wxID_ANY, _("Information"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer15->Add( m_button6, 0, wxALL, 5 );
	
	m_sdbSizer4 = new wxStdDialogButtonSizer();
	m_sdbSizer4OK = new wxButton( this, wxID_OK );
	m_sdbSizer4->AddButton( m_sdbSizer4OK );
	m_sdbSizer4Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer4->AddButton( m_sdbSizer4Cancel );
	m_sdbSizer4->Realize();
	
	fgSizer15->Add( m_sdbSizer4, 1, wxEXPAND, 5 );
	
	
	m_fgSizer->Add( fgSizer15, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( m_fgSizer );
	this->Layout();
	m_fgSizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_bTest->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EditAlarmDialogBase::OnTestAlarm ), NULL, this );
	m_button6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EditAlarmDialogBase::OnInformation ), NULL, this );
}

EditAlarmDialogBase::~EditAlarmDialogBase()
{
	// Disconnect Events
	m_bTest->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EditAlarmDialogBase::OnTestAlarm ), NULL, this );
	m_button6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EditAlarmDialogBase::OnInformation ), NULL, this );
	
}

LandFallPanel::LandFallPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer17;
	fgSizer17 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer17->SetFlexibleDirection( wxBOTH );
	fgSizer17->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer16;
	fgSizer16 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer16->SetFlexibleDirection( wxBOTH );
	fgSizer16->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_rbTime = new wxRadioButton( this, wxID_ANY, _("GPS course crosses land in <"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer16->Add( m_rbTime, 0, wxALL, 5 );
	
	m_sTimeMinutes = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 20 );
	fgSizer16->Add( m_sTimeMinutes, 0, wxALL, 5 );
	
	m_staticText30 = new wxStaticText( this, wxID_ANY, _("minutes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText30->Wrap( -1 );
	fgSizer16->Add( m_staticText30, 0, wxALL, 5 );
	
	
	fgSizer17->Add( fgSizer16, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_rbDistance = new wxRadioButton( this, wxID_ANY, _("GPS fix is <"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_rbDistance, 0, wxALL, 5 );
	
	m_tDistance = new wxTextCtrl( this, wxID_ANY, _("3"), wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_tDistance->HasFlag( wxTE_MULTILINE ) )
	{
	m_tDistance->SetMaxLength( 10 );
	}
	#else
	m_tDistance->SetMaxLength( 10 );
	#endif
	fgSizer10->Add( m_tDistance, 0, wxALL, 5 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, _("nm from coastline\n(any direction)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer10->Add( m_staticText7, 0, wxALL, 5 );
	
	
	fgSizer17->Add( fgSizer10, 1, wxEXPAND, 5 );
	
	m_staticText39122 = new wxStaticText( this, wxID_ANY, _("Graphical overlay for time displays line from boat to location of land crossing"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39122->Wrap( 400 );
	fgSizer17->Add( m_staticText39122, 0, wxALL, 5 );
	
	m_staticText45 = new wxStaticText( this, wxID_ANY, _("Landfall detection uses gshhs (global heirarchical high-resolution shoreline data.)\nConsider installing the highest resolution database for best accuracy."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText45->Wrap( 350 );
	fgSizer17->Add( m_staticText45, 0, wxALL, 5 );
	
	
	this->SetSizer( fgSizer17 );
	this->Layout();
	fgSizer17->Fit( this );
}

LandFallPanel::~LandFallPanel()
{
}

BoundaryPanel::BoundaryPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer17;
	fgSizer17 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer17->SetFlexibleDirection( wxBOTH );
	fgSizer17->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer16;
	fgSizer16 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer16->SetFlexibleDirection( wxBOTH );
	fgSizer16->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_rbTime = new wxRadioButton( this, GPSCOURSE, _("GPS course crosses boundary in <"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	fgSizer16->Add( m_rbTime, 0, wxALL, 5 );
	
	m_sTimeMinutes = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 20 );
	fgSizer16->Add( m_sTimeMinutes, 0, wxALL, 5 );
	
	m_staticText30 = new wxStaticText( this, wxID_ANY, _("minutes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText30->Wrap( -1 );
	fgSizer16->Add( m_staticText30, 0, wxALL, 5 );
	
	
	fgSizer17->Add( fgSizer16, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_rbDistance = new wxRadioButton( this, GPSFIX, _("GPS fix is <"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_rbDistance, 0, wxALL, 5 );
	
	m_tDistance = new wxTextCtrl( this, wxID_ANY, _("3"), wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_tDistance->HasFlag( wxTE_MULTILINE ) )
	{
	m_tDistance->SetMaxLength( 10 );
	}
	#else
	m_tDistance->SetMaxLength( 10 );
	#endif
	fgSizer10->Add( m_tDistance, 0, wxALL, 5 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, _("nm from boundary\n(any direction)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer10->Add( m_staticText7, 0, wxALL, 5 );
	
	
	fgSizer17->Add( fgSizer10, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer30;
	fgSizer30 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer30->SetFlexibleDirection( wxBOTH );
	fgSizer30->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_rbGuard = new wxRadioButton( this, AISGUARDZONE, _("AIS Guard Zone"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_rbGuard, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText321 = new wxStaticText( this, wxID_ANY, _("Guard Zone GUID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText321->Wrap( -1 );
	fgSizer30->Add( m_staticText321, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_tGuardZoneGUID = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_tGuardZoneGUID->HasFlag( wxTE_MULTILINE ) )
	{
	m_tGuardZoneGUID->SetMaxLength( 40 );
	}
	#else
	m_tGuardZoneGUID->SetMaxLength( 40 );
	#endif
	fgSizer30->Add( m_tGuardZoneGUID, 0, wxALL, 5 );
	
	
	fgSizer17->Add( fgSizer30, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer28;
	fgSizer28 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer28->SetFlexibleDirection( wxBOTH );
	fgSizer28->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_rbAnchor = new wxRadioButton( this, ANCHORALARM, _("Inclusion Alarm"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer28->Add( m_rbAnchor, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText32 = new wxStaticText( this, wxID_ANY, _("Boundary GUID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	fgSizer28->Add( m_staticText32, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_tBoundaryGUID = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_tBoundaryGUID->HasFlag( wxTE_MULTILINE ) )
	{
	m_tBoundaryGUID->SetMaxLength( 40 );
	}
	#else
	m_tBoundaryGUID->SetMaxLength( 40 );
	#endif
	fgSizer28->Add( m_tBoundaryGUID, 0, wxALL, 5 );
	
	m_button12 = new wxButton( this, wxID_ANY, _("Get Boundary GUID"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer28->Add( m_button12, 0, wxALL, 5 );
	
	
	fgSizer17->Add( fgSizer28, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer29;
	fgSizer29 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer29->SetFlexibleDirection( wxBOTH );
	fgSizer29->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxString m_radioBoxBoundaryTypeChoices[] = { _("Any"), _("Exclusion"), _("Inclusion"), _("Neither") };
	int m_radioBoxBoundaryTypeNChoices = sizeof( m_radioBoxBoundaryTypeChoices ) / sizeof( wxString );
	m_radioBoxBoundaryType = new wxRadioBox( this, wxID_ANY, _("Boundary Type"), wxDefaultPosition, wxDefaultSize, m_radioBoxBoundaryTypeNChoices, m_radioBoxBoundaryTypeChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxBoundaryType->SetSelection( 0 );
	fgSizer29->Add( m_radioBoxBoundaryType, 0, wxALL, 5 );
	
	wxString m_radioBoxBoundaryStateChoices[] = { _("Any"), _("Active"), _("Inactive") };
	int m_radioBoxBoundaryStateNChoices = sizeof( m_radioBoxBoundaryStateChoices ) / sizeof( wxString );
	m_radioBoxBoundaryState = new wxRadioBox( this, wxID_ANY, _("Boundary State"), wxDefaultPosition, wxDefaultSize, m_radioBoxBoundaryStateNChoices, m_radioBoxBoundaryStateChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxBoundaryState->SetSelection( 1 );
	fgSizer29->Add( m_radioBoxBoundaryState, 0, wxALL, 5 );
	
	
	fgSizer17->Add( fgSizer29, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer32;
	fgSizer32 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer32->AddGrowableCol( 1 );
	fgSizer32->SetFlexibleDirection( wxBOTH );
	fgSizer32->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextCheckFrequency = new wxStaticText( this, wxID_ANY, _("Check Frequency  (Secs)\n(Note: not Guard Zone)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextCheckFrequency->Wrap( -1 );
	fgSizer32->Add( m_staticTextCheckFrequency, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sliderCheckFrequency = new wxSlider( this, wxID_ANY, 10, 1, 120, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_BOTTOM|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer32->Add( m_sliderCheckFrequency, 1, wxALL|wxEXPAND, 5 );
	
	
	fgSizer17->Add( fgSizer32, 1, wxEXPAND, 5 );
	
	m_staticText39122 = new wxStaticText( this, wxID_ANY, _("Graphical overlay for GPS fix displays line from boat to location of the point in the found boundary"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39122->Wrap( 400 );
	fgSizer17->Add( m_staticText39122, 0, wxALL, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer17->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( fgSizer17 );
	this->Layout();
	fgSizer17->Fit( this );
	
	// Connect Events
	m_rbTime->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( BoundaryPanel::OnRadioButton ), NULL, this );
	m_rbDistance->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( BoundaryPanel::OnRadioButton ), NULL, this );
	m_rbGuard->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( BoundaryPanel::OnRadioButton ), NULL, this );
	m_tGuardZoneGUID->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( BoundaryPanel::OnGuardZoneGUIDKillFocus ), NULL, this );
	m_rbAnchor->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( BoundaryPanel::OnRadioButton ), NULL, this );
	m_tBoundaryGUID->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( BoundaryPanel::OnBoundaryGUIDKillFocus ), NULL, this );
	m_button12->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoundaryPanel::OnGetBoundaryGUID ), NULL, this );
}

BoundaryPanel::~BoundaryPanel()
{
	// Disconnect Events
	m_rbTime->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( BoundaryPanel::OnRadioButton ), NULL, this );
	m_rbDistance->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( BoundaryPanel::OnRadioButton ), NULL, this );
	m_rbGuard->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( BoundaryPanel::OnRadioButton ), NULL, this );
	m_tGuardZoneGUID->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( BoundaryPanel::OnGuardZoneGUIDKillFocus ), NULL, this );
	m_rbAnchor->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( BoundaryPanel::OnRadioButton ), NULL, this );
	m_tBoundaryGUID->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( BoundaryPanel::OnBoundaryGUIDKillFocus ), NULL, this );
	m_button12->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BoundaryPanel::OnGetBoundaryGUID ), NULL, this );
	
}

NMEADataPanel::NMEADataPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer91;
	fgSizer91 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer91->AddGrowableCol( 0 );
	fgSizer91->SetFlexibleDirection( wxBOTH );
	fgSizer91->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText34 = new wxStaticText( this, wxID_ANY, _("Require following NMEA sentences:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText34->Wrap( -1 );
	fgSizer91->Add( m_staticText34, 0, wxALL, 5 );
	
	m_tSentences = new wxTextCtrl( this, wxID_ANY, _("$GPGGA"), wxDefaultPosition, wxSize( -1,60 ), wxTE_MULTILINE );
	#ifdef __WXGTK__
	if ( !m_tSentences->HasFlag( wxTE_MULTILINE ) )
	{
	m_tSentences->SetMaxLength( 255 );
	}
	#else
	m_tSentences->SetMaxLength( 255 );
	#endif
	fgSizer91->Add( m_tSentences, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer19;
	fgSizer19 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer19->SetFlexibleDirection( wxBOTH );
	fgSizer19->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText35 = new wxStaticText( this, wxID_ANY, _("at least every"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35->Wrap( -1 );
	fgSizer19->Add( m_staticText35, 0, wxALL, 5 );
	
	m_sSeconds = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 1, 10000, 10 );
	fgSizer19->Add( m_sSeconds, 0, wxALL, 5 );
	
	m_staticText13 = new wxStaticText( this, wxID_ANY, _("seconds"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer19->Add( m_staticText13, 0, wxALL, 5 );
	
	
	fgSizer91->Add( fgSizer19, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer91 );
	this->Layout();
	fgSizer91->Fit( this );
}

NMEADataPanel::~NMEADataPanel()
{
}

DeadmanPanel::DeadmanPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer25;
	fgSizer25 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer25->SetFlexibleDirection( wxBOTH );
	fgSizer25->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText33 = new wxStaticText( this, wxID_ANY, _("No User Activity for"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText33->Wrap( -1 );
	fgSizer5->Add( m_staticText33, 0, wxALL, 5 );
	
	m_sMinutes = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000, 20 );
	fgSizer5->Add( m_sMinutes, 0, wxALL, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, _("minute(s)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer5->Add( m_staticText6, 0, wxALL, 5 );
	
	
	fgSizer25->Add( fgSizer5, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer25 );
	this->Layout();
	fgSizer25->Fit( this );
}

DeadmanPanel::~DeadmanPanel()
{
}

AnchorPanel::AnchorPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer24;
	fgSizer24 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer24->SetFlexibleDirection( wxBOTH );
	fgSizer24->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText71 = new wxStaticText( this, wxID_ANY, _("Latitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	fgSizer9->Add( m_staticText71, 0, wxALL, 5 );
	
	m_tLatitude = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_tLatitude->HasFlag( wxTE_MULTILINE ) )
	{
	m_tLatitude->SetMaxLength( 20 );
	}
	#else
	m_tLatitude->SetMaxLength( 20 );
	#endif
	fgSizer9->Add( m_tLatitude, 0, wxALL, 5 );
	
	
	fgSizer9->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, _("Longitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer9->Add( m_staticText8, 0, wxALL, 5 );
	
	m_tLongitude = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_tLongitude->HasFlag( wxTE_MULTILINE ) )
	{
	m_tLongitude->SetMaxLength( 20 );
	}
	#else
	m_tLongitude->SetMaxLength( 20 );
	#endif
	fgSizer9->Add( m_tLongitude, 0, wxALL, 5 );
	
	m_bSyncToBoat = new wxButton( this, wxID_ANY, _("Sync to Boat"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( m_bSyncToBoat, 0, wxALL, 5 );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, _("Radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer9->Add( m_staticText9, 0, wxALL, 5 );
	
	m_sRadius = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100000, 40 );
	fgSizer9->Add( m_sRadius, 0, wxALL, 5 );
	
	m_staticText39 = new wxStaticText( this, wxID_ANY, _("meters"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39->Wrap( -1 );
	fgSizer9->Add( m_staticText39, 0, wxALL, 5 );
	
	
	fgSizer24->Add( fgSizer9, 1, wxEXPAND, 5 );
	
	m_cbAutoSync = new wxCheckBox( this, wxID_ANY, _("Automatically syncronize to the boat whenever enabled"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer24->Add( m_cbAutoSync, 0, wxALL, 5 );
	
	m_staticText39121 = new wxStaticText( this, wxID_ANY, _("Graphical overlay displays bounding anchor area"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39121->Wrap( -1 );
	fgSizer24->Add( m_staticText39121, 0, wxALL, 5 );
	
	
	this->SetSizer( fgSizer24 );
	this->Layout();
	fgSizer24->Fit( this );
	
	// Connect Events
	m_bSyncToBoat->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AnchorPanel::OnSyncToBoat ), NULL, this );
}

AnchorPanel::~AnchorPanel()
{
	// Disconnect Events
	m_bSyncToBoat->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AnchorPanel::OnSyncToBoat ), NULL, this );
	
}

CoursePanel::CoursePanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer22;
	fgSizer22 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer22->SetFlexibleDirection( wxBOTH );
	fgSizer22->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer101;
	fgSizer101 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer101->SetFlexibleDirection( wxBOTH );
	fgSizer101->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText50 = new wxStaticText( this, wxID_ANY, _("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText50->Wrap( -1 );
	fgSizer101->Add( m_staticText50, 0, wxALL, 5 );
	
	wxString m_cModeChoices[] = { _("Port"), _("Starboard"), _("Port or Starboard") };
	int m_cModeNChoices = sizeof( m_cModeChoices ) / sizeof( wxString );
	m_cMode = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cModeNChoices, m_cModeChoices, 0 );
	m_cMode->SetSelection( 0 );
	fgSizer101->Add( m_cMode, 0, wxALL, 5 );
	
	
	fgSizer101->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText32 = new wxStaticText( this, wxID_ANY, _("Off Course By >"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	fgSizer101->Add( m_staticText32, 0, wxALL, 5 );
	
	m_sTolerance = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 180, 20 );
	fgSizer101->Add( m_sTolerance, 0, wxALL, 5 );
	
	m_staticText18 = new wxStaticText( this, wxID_ANY, _("Degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	fgSizer101->Add( m_staticText18, 0, wxALL, 5 );
	
	m_staticText21 = new wxStaticText( this, wxID_ANY, _("With Course Of"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	fgSizer101->Add( m_staticText21, 0, wxALL, 5 );
	
	m_sCourse = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 359, 0 );
	fgSizer101->Add( m_sCourse, 0, wxALL, 5 );
	
	m_bCurrentCourse = new wxButton( this, wxID_ANY, _("Current Course"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer101->Add( m_bCurrentCourse, 0, wxALL, 5 );
	
	
	fgSizer22->Add( fgSizer101, 1, wxEXPAND, 5 );
	
	m_staticText3912 = new wxStaticText( this, wxID_ANY, _("Graphical overlay displays lines bounding valid courses"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3912->Wrap( -1 );
	fgSizer22->Add( m_staticText3912, 0, wxALL, 5 );
	
	
	this->SetSizer( fgSizer22 );
	this->Layout();
	fgSizer22->Fit( this );
	
	// Connect Events
	m_bCurrentCourse->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CoursePanel::OnCurrentCourse ), NULL, this );
}

CoursePanel::~CoursePanel()
{
	// Disconnect Events
	m_bCurrentCourse->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CoursePanel::OnCurrentCourse ), NULL, this );
	
}

SpeedPanel::SpeedPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Speed Alarm") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText51 = new wxStaticText( this, wxID_ANY, _("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	fgSizer14->Add( m_staticText51, 0, wxALL, 5 );
	
	wxString m_cModeChoices[] = { _("Under Speed"), _("Over Speed") };
	int m_cModeNChoices = sizeof( m_cModeChoices ) / sizeof( wxString );
	m_cMode = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cModeNChoices, m_cModeChoices, 0 );
	m_cMode->SetSelection( 0 );
	fgSizer14->Add( m_cMode, 0, wxALL, 5 );
	
	
	fgSizer14->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText44 = new wxStaticText( this, wxID_ANY, _("Speed over ground"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	fgSizer14->Add( m_staticText44, 0, wxALL, 5 );
	
	m_tSpeed = new wxTextCtrl( this, wxID_ANY, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_tSpeed, 0, wxALL, 5 );
	
	m_staticText24 = new wxStaticText( this, wxID_ANY, _("knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	fgSizer14->Add( m_staticText24, 0, wxALL, 5 );
	
	
	sbSizer7->Add( fgSizer14, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer31;
	fgSizer31 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer31->AddGrowableCol( 1 );
	fgSizer31->SetFlexibleDirection( wxBOTH );
	fgSizer31->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText28 = new wxStaticText( this, wxID_ANY, _("Seconds to Average\nSOG over"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	fgSizer31->Add( m_staticText28, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sliderSOGAverageNumber = new wxSlider( this, wxID_ANY, 10, 1, 120, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer31->Add( m_sliderSOGAverageNumber, 0, wxALL|wxEXPAND, 5 );
	
	
	sbSizer7->Add( fgSizer31, 1, wxEXPAND, 5 );
	
	m_staticText391 = new wxStaticText( this, wxID_ANY, _("Graphical overlay displays circle with radius of this speed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText391->Wrap( -1 );
	sbSizer7->Add( m_staticText391, 0, wxALL, 5 );
	
	
	this->SetSizer( sbSizer7 );
	this->Layout();
	sbSizer7->Fit( this );
}

SpeedPanel::~SpeedPanel()
{
}

WindPanel::WindPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Wind Alarm") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText51 = new wxStaticText( this, wxID_ANY, _("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	fgSizer14->Add( m_staticText51, 0, wxALL, 5 );
	
	wxString m_cModeChoices[] = { _("Under Speed"), _("Over Speed"), _("Apparent Direction Above"), _("Apparent Direction Below"), _("Apparent Direction Port Above"), _("Apparent Direction Port Below"), _("Apparent Direction Starboard Above"), _("Apparent Direction Starboard Below"), _("Apparent Direction Above"), _("Apparent Direction Above"), wxEmptyString };
	int m_cModeNChoices = sizeof( m_cModeChoices ) / sizeof( wxString );
	m_cMode = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cModeNChoices, m_cModeChoices, 0 );
	m_cMode->SetSelection( 0 );
	fgSizer14->Add( m_cMode, 0, wxALL, 5 );
	
	
	fgSizer14->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText44 = new wxStaticText( this, wxID_ANY, _("Value"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	fgSizer14->Add( m_staticText44, 0, wxALL, 5 );
	
	m_sValue = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 180, 5 );
	fgSizer14->Add( m_sValue, 0, wxALL, 5 );
	
	m_staticText24 = new wxStaticText( this, wxID_ANY, _("knots/degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	fgSizer14->Add( m_staticText24, 0, wxALL, 5 );
	
	
	sbSizer7->Add( fgSizer14, 1, wxEXPAND, 5 );
	
	m_staticText391 = new wxStaticText( this, wxID_ANY, _("No Graphical overlay"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText391->Wrap( -1 );
	sbSizer7->Add( m_staticText391, 0, wxALL, 5 );
	
	
	this->SetSizer( sbSizer7 );
	this->Layout();
	sbSizer7->Fit( this );
}

WindPanel::~WindPanel()
{
}

WeatherPanel::WeatherPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Wind Alarm") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText51 = new wxStaticText( this, wxID_ANY, _("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	fgSizer14->Add( m_staticText51, 0, wxALL, 5 );
	
	wxString m_cModeChoices[] = { _("Above"), _("Below") };
	int m_cModeNChoices = sizeof( m_cModeChoices ) / sizeof( wxString );
	m_cMode = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cModeNChoices, m_cModeChoices, 0 );
	m_cMode->SetSelection( 0 );
	fgSizer14->Add( m_cMode, 0, wxALL, 5 );
	
	
	fgSizer14->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText50 = new wxStaticText( this, wxID_ANY, _("Variable"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText50->Wrap( -1 );
	fgSizer14->Add( m_staticText50, 0, wxALL, 5 );
	
	wxString m_cVariableChoices[] = { _("Pressure"), _("Air Temperature"), _("Sea Temperature"), _("Relative Humidity"), _("Humidity") };
	int m_cVariableNChoices = sizeof( m_cVariableChoices ) / sizeof( wxString );
	m_cVariable = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cVariableNChoices, m_cVariableChoices, 0 );
	m_cVariable->SetSelection( 0 );
	fgSizer14->Add( m_cVariable, 0, wxALL, 5 );
	
	
	fgSizer14->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText511 = new wxStaticText( this, wxID_ANY, _("Value"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText511->Wrap( -1 );
	fgSizer14->Add( m_staticText511, 0, wxALL, 5 );
	
	m_tValue = new wxTextCtrl( this, wxID_ANY, _("1010"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_tValue, 0, wxALL, 5 );
	
	m_staticText24 = new wxStaticText( this, wxID_ANY, _("mBar/C/%"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	fgSizer14->Add( m_staticText24, 0, wxALL, 5 );
	
	
	sbSizer7->Add( fgSizer14, 1, wxEXPAND, 5 );
	
	m_staticText391 = new wxStaticText( this, wxID_ANY, _("No Graphical overlay"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText391->Wrap( -1 );
	sbSizer7->Add( m_staticText391, 0, wxALL, 5 );
	
	
	this->SetSizer( sbSizer7 );
	this->Layout();
	sbSizer7->Fit( this );
}

WeatherPanel::~WeatherPanel()
{
}
