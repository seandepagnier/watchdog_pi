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
	
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer7->AddGrowableCol( 0 );
	fgSizer7->AddGrowableRow( 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText24 = new wxStaticText( this, wxID_ANY, _("Next LandFall in"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	fgSizer7->Add( m_staticText24, 0, wxALL, 5 );
	
	m_stLandFallTime = new wxStaticText( this, wxID_ANY, _("No NMEA Messages"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stLandFallTime->Wrap( -1 );
	fgSizer7->Add( m_stLandFallTime, 0, wxALL, 5 );
	
	m_bPreferences = new wxButton( this, wxID_ANY, _("Preferences"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer7->Add( m_bPreferences, 0, wxALL, 5 );
	
	m_bClose = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer7->Add( m_bClose, 0, wxALL, 5 );
	
	
	fgSizer8->Add( fgSizer7, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer8 );
	this->Layout();
	fgSizer8->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_bPreferences->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanDialogBase::OnPreferences ), NULL, this );
	m_bClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanDialogBase::OnClose ), NULL, this );
}

WatchmanDialogBase::~WatchmanDialogBase()
{
	// Disconnect Events
	m_bPreferences->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanDialogBase::OnPreferences ), NULL, this );
	m_bClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchmanDialogBase::OnClose ), NULL, this );
	
}

WatchmanPrefsDialog::WatchmanPrefsDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Landfall Alarm") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText27 = new wxStaticText( this, wxID_ANY, _("If Distance is < nm"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	fgSizer10->Add( m_staticText27, 0, wxALL, 5 );
	
	m_tcLandFallDistance = new wxTextCtrl( this, wxID_ANY, _("3"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_tcLandFallDistance, 0, wxALL, 5 );
	
	m_cbLandFallSound = new wxCheckBox( this, wxID_ANY, _("Sound"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_cbLandFallSound, 0, wxALL, 5 );
	
	m_fpLandFallSound = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a file"), wxT("Wav Files (*.wav)|*.WAV;*.wav|All Files (*.*)|*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	fgSizer10->Add( m_fpLandFallSound, 0, wxALL, 5 );
	
	m_cbLandFallCommand = new wxCheckBox( this, wxID_ANY, _("Command"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_cbLandFallCommand, 0, wxALL, 5 );
	
	m_tLandFallCommand = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_tLandFallCommand, 0, wxALL, 5 );
	
	
	sbSizer6->Add( fgSizer10, 1, wxEXPAND, 5 );
	
	
	bSizer2->Add( sbSizer6, 1, wxEXPAND, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	
	bSizer2->Add( m_sdbSizer1, 0, wxBOTTOM|wxEXPAND|wxTOP, 5 );
	
	
	this->SetSizer( bSizer2 );
	this->Layout();
	bSizer2->Fit( this );
	
	this->Centre( wxBOTH );
}

WatchmanPrefsDialog::~WatchmanPrefsDialog()
{
}
