///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "WatchdogUI.h"
#include "ocpn_plugin.h"

///////////////////////////////////////////////////////////////////////////

WatchdogDialogBase::WatchdogDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    wxFont *pFont = GetOCPNScaledFont_PlugIn(_("Dialog"));
    wxFont dFont = *pFont;
    SetFont(dFont);

    this->SetSizeHints( wxSize( -1,-1 ), wxSize( -1,-1 ) );

	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer8->AddGrowableCol( 0 );
	fgSizer8->AddGrowableRow( 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );

	m_lStatus = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ),
                                wxLC_NO_HEADER | wxLC_REPORT | wxLC_HRULES );
    m_lStatus->SetFont(dFont);     // Default
    m_lStatus->SetMinSize( wxSize( 300,50 ) );

	fgSizer8->Add( m_lStatus, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( fgSizer8 );
	this->Layout();
	fgSizer8->Fit( this );
	m_Menu = new wxMenu();
	wxMenuItem* m_New;
    wxFont *sFont = GetOCPNScaledFont_PlugIn(_("Menu"));

    m_New = new wxMenuItem( m_Menu, wxID_ANY, wxString( _("New") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __OCPN__ANDROID__
    m_New->SetFont(*sFont);
#endif
    m_Menu->Append( m_New );

	m_Edit = new wxMenuItem( m_Menu, wxID_ANY, wxString( _("Edit") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __OCPN__ANDROID__
    m_Edit->SetFont(*sFont);
#endif
	m_Menu->Append( m_Edit );

	m_Reset = new wxMenuItem( m_Menu, wxID_ANY, wxString( _("Reset") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __OCPN__ANDROID__
    m_Reset->SetFont(*sFont);
#endif
	m_Menu->Append( m_Reset );

	m_Delete = new wxMenuItem( m_Menu, wxID_ANY, wxString( _("Delete") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __OCPN__ANDROID__
    m_Delete->SetFont(*sFont);
#endif
	m_Menu->Append( m_Delete );

	m_Menu->AppendSeparator();

	wxMenuItem* m_ResetAll;
	m_ResetAll = new wxMenuItem( m_Menu, wxID_ANY, wxString( _("Reset All") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __OCPN__ANDROID__
    m_ResetAll->SetFont(*sFont);
#endif
	m_Menu->Append( m_ResetAll );

	wxMenuItem* m_DeleteAll;
	m_DeleteAll = new wxMenuItem( m_Menu, wxID_ANY, wxString( _("DeleteAll") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __OCPN__ANDROID__
    m_DeleteAll->SetFont(*sFont);
#endif
	m_Menu->Append( m_DeleteAll );

	wxMenuItem* m_Configuration;
	m_Configuration = new wxMenuItem( m_Menu, wxID_ANY, wxString( _("Configuration") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __OCPN__ANDROID__
    m_Configuration->SetFont(*sFont);
#endif
	m_Menu->Append( m_Configuration );

	this->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( WatchdogDialogBase::WatchdogDialogBaseOnContextMenu ), NULL, this );


	this->Centre( wxBOTH );

	// Connect Events
	m_lStatus->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( WatchdogDialogBase::OnDoubleClick ), NULL, this );
	m_lStatus->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( WatchdogDialogBase::OnLeftDown ), NULL, this );
	m_lStatus->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( WatchdogDialogBase::OnRightDown ), NULL, this );
	m_Menu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WatchdogDialogBase::OnNew ), this, m_New->GetId());
	m_Menu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WatchdogDialogBase::OnEdit ), this, m_Edit->GetId());
	m_Menu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WatchdogDialogBase::OnReset ), this, m_Reset->GetId());
	m_Menu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WatchdogDialogBase::OnDelete ), this, m_Delete->GetId());
	m_Menu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WatchdogDialogBase::OnResetAll ), this, m_ResetAll->GetId());
	m_Menu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WatchdogDialogBase::OnDeleteAll ), this, m_DeleteAll->GetId());
	m_Menu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( WatchdogDialogBase::OnConfiguration ), this, m_Configuration->GetId());
}

WatchdogDialogBase::~WatchdogDialogBase()
{
	// Disconnect Events
	m_lStatus->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( WatchdogDialogBase::OnDoubleClick ), NULL, this );
	m_lStatus->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( WatchdogDialogBase::OnLeftDown ), NULL, this );
	m_lStatus->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( WatchdogDialogBase::OnRightDown ), NULL, this );

	delete m_Menu;
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
	bSizerOK = new wxBoxSizer( wxHORIZONTAL );

	m_button7 = new wxButton( this, wxID_ANY, _("About Author"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOK->Add( m_button7, 0, wxALL, 5 );


	m_fgSizerProperties->Add( bSizerOK, 1, wxEXPAND, 5 );

	m_buttonOK = new wxButton( this, wxID_ANY, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fgSizerProperties->Add( m_buttonOK, 0, wxALL, 5 );


	this->SetSizer( m_fgSizerProperties );
	this->Layout();
	m_fgSizerProperties->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogPropertiesDialogBase::OnAboutAuthor ), NULL, this );
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogPropertiesDialogBase::OnWatchdogPropertiesOKClick ), NULL, this );
}

WatchdogPropertiesDialogBase::~WatchdogPropertiesDialogBase()
{
	// Disconnect Events
	m_button7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogPropertiesDialogBase::OnAboutAuthor ), NULL, this );
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WatchdogPropertiesDialogBase::OnWatchdogPropertiesOKClick ), NULL, this );

}

ConfigurationDialogBase::ConfigurationDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
    wxFont *pFont = GetOCPNScaledFont_PlugIn(_("Dialog"));
    wxFont dFont = *pFont;
    SetFont(dFont);

    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

#ifndef __OCPN__ANDROID__
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

	m_rbAlways = new wxRadioButton( sbSizer41->GetStaticBox(), wxID_ANY, _("Enable All Alarms"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer241->Add( m_rbAlways, 0, wxALL, 5 );

	m_rbOnce = new wxRadioButton( sbSizer41->GetStaticBox(), wxID_ANY, _("Enabled after first time Watchdog Dialog is visible"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer241->Add( m_rbOnce, 0, wxALL, 5 );

	m_rbVisible = new wxRadioButton( sbSizer41->GetStaticBox(), wxID_ANY, _("Enabled only if Watchdog Dialog is visible"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer241->Add( m_rbVisible, 0, wxALL, 5 );

	m_rbNever = new wxRadioButton( sbSizer41->GetStaticBox(), wxID_ANY, _("Disable All Alarms"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer241->Add( m_rbNever, 0, wxALL, 5 );


	sbSizer41->Add( fgSizer241, 1, wxEXPAND, 5 );


	fgSizer4->Add( sbSizer41, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer11->AddGrowableCol( 1 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText61 = new wxStaticText( this, wxID_ANY, _("Font"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText61->Wrap( -1 );
	fgSizer11->Add( m_staticText61, 0, wxALL, 5 );

	m_font = new wxFontPickerCtrl( this, wxID_ANY, wxNullFont, wxDefaultPosition, wxDefaultSize, wxFNTP_DEFAULT_STYLE );
	m_font->SetMaxPointSize( 100 );
	fgSizer11->Add( m_font, 0, wxALL, 5 );

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

#else
    wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* sbSizer41;
	sbSizer41 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("General Setup") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer241;
	fgSizer241 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer241->SetFlexibleDirection( wxBOTH );
	fgSizer241->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_rbAlways = new wxRadioButton( sbSizer41->GetStaticBox(), wxID_ANY, _("Enable All Alarms"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer241->Add( m_rbAlways, 0, wxALL, 5 );

	m_rbOnce = new wxRadioButton( sbSizer41->GetStaticBox(), wxID_ANY, _("Enabled after first time Watchdog Dialog is visible"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer241->Add( m_rbOnce, 0, wxALL, 5 );

	m_rbVisible = new wxRadioButton( sbSizer41->GetStaticBox(), wxID_ANY, _("Enabled only if Watchdog Dialog is visible"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer241->Add( m_rbVisible, 0, wxALL, 5 );

	m_rbNever = new wxRadioButton( sbSizer41->GetStaticBox(), wxID_ANY, _("Disable All Alarms"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer241->Add( m_rbNever, 0, wxALL, 5 );


	sbSizer41->Add( fgSizer241, 1, wxEXPAND, 5 );


	fgSizer4->Add( sbSizer41, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer11->AddGrowableCol( 1 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText61 = new wxStaticText( this, wxID_ANY, _("Font"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText61->Wrap( -1 );
	fgSizer11->Add( m_staticText61, 0, wxALL, 5 );
    m_staticText61->Hide();

	m_font = new wxFontPickerCtrl( this, wxID_ANY, wxNullFont, wxDefaultPosition, wxDefaultSize, wxFNTP_DEFAULT_STYLE );
	m_font->SetMaxPointSize( 100 );
	fgSizer11->Add( m_font, 0, wxALL, 5 );
    m_font->SetSize(1,1);

	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1->Realize();

	fgSizer11->Add( m_sdbSizer1, 0, wxALIGN_RIGHT|wxBOTTOM|wxEXPAND|wxTOP, 5 );
	fgSizer4->Add( fgSizer11, 1, wxEXPAND, 5 );
	this->SetSizer( fgSizer4 );
	fgSizer4->Fit( this );

    wxRect tbRect = GetMasterToolbarRect();
    wxPoint pNew;
    pNew.x = tbRect.x + tbRect.width + 4;
    pNew.y = tbRect.x + tbRect.width + 4;  // Reasonable spot
    Move( pNew);

    int widthAvail =
        GetCanvasByIndex(0)->GetClientSize().x - pNew.x;
    wxSize sizef;
    sizef.x = widthAvail;
    sizef.y = 10 * GetCharHeight();   //Allow a few lines.
    SetSize(sizef);

#endif

    // Connect Events
	m_rbAlways->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ConfigurationDialogBase::OnEnabled ), NULL, this );
	m_rbOnce->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ConfigurationDialogBase::OnEnabled ), NULL, this );
	m_rbVisible->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ConfigurationDialogBase::OnEnabled ), NULL, this );
	m_rbNever->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ConfigurationDialogBase::OnEnabled ), NULL, this );
	m_font->Connect( wxEVT_COMMAND_FONTPICKER_CHANGED, wxFontPickerEventHandler( ConfigurationDialogBase::OnFont ), NULL, this );

}

ConfigurationDialogBase::~ConfigurationDialogBase()
{
	// Disconnect Events
	m_rbAlways->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ConfigurationDialogBase::OnEnabled ), NULL, this );
	m_rbOnce->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ConfigurationDialogBase::OnEnabled ), NULL, this );
	m_rbVisible->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ConfigurationDialogBase::OnEnabled ), NULL, this );
	m_rbNever->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ConfigurationDialogBase::OnEnabled ), NULL, this );
	m_font->Disconnect( wxEVT_COMMAND_FONTPICKER_CHANGED, wxFontPickerEventHandler( ConfigurationDialogBase::OnFont ), NULL, this );

}

NewAlarmDialogBase::NewAlarmDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
    wxFont *dFont = GetOCPNScaledFont_PlugIn(_("Dialog"));
    SetFont(*dFont);
    //this->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	wxFlexGridSizer* fgSizer40;
	fgSizer40 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer40->AddGrowableCol( 0 );
	fgSizer40->AddGrowableRow( 0 );
	fgSizer40->SetFlexibleDirection( wxBOTH );
	fgSizer40->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );

	m_lAlarmType = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_NO_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL );
    m_lAlarmType-SetFont(*dFont);

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

#ifdef __OCPN__ANDROID__
    wxRect tbRect = GetMasterToolbarRect();
    wxPoint pNew;
    pNew.x = tbRect.x + tbRect.width + 4;
    pNew.y = tbRect.x + tbRect.width + 4;  // Reasonable spot
    Move( pNew);

    int widthAvail =
        GetCanvasByIndex(0)->GetClientSize().x - pNew.x;
    wxSize sizef;
    sizef.x = widthAvail;
    sizef.y = 10 * GetCharHeight();   //Allow a few lines.
    SetSize(sizef);
#else
    fgSizer40->Fit( this );
	this->Centre( wxBOTH );
#endif

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

#ifndef __OCPN__ANDROID__
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

    m_cbSound = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("Sound"), wxDefaultPosition, wxDefaultSize, 0 );
    fgSizer6->Add( m_cbSound, 0, wxALL, 5 );

    m_fpSound = new wxFilePickerCtrl( sbSizer4->GetStaticBox(), wxID_ANY, wxT("/initrd.img"), _("Select a file"), _("Wav Files (*.wav)|*.WAV;*.wav|All Files (*.*)|*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
    fgSizer6->Add( m_fpSound, 0, wxALL|wxEXPAND, 5 );

    m_cbCommand = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("Command"), wxDefaultPosition, wxDefaultSize, 0 );
    fgSizer6->Add( m_cbCommand, 0, wxALL, 5 );

    m_tCommand = new wxTextCtrl( sbSizer4->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
#ifdef __WXGTK__
    if ( !m_tCommand->HasFlag( wxTE_MULTILINE ) )
    {
        m_tCommand->SetMaxLength( 255 );
    }
#else
    m_tCommand->SetMaxLength( 255 );
#endif
    fgSizer6->Add( m_tCommand, 0, wxALL|wxEXPAND, 5 );

    m_cbMessageBox = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("Message Box"), wxDefaultPosition, wxDefaultSize, 0 );
    fgSizer6->Add( m_cbMessageBox, 0, wxALL, 5 );

    m_cbNoData = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("Alarm if no Data"), wxDefaultPosition, wxDefaultSize, 0 );
    fgSizer6->Add( m_cbNoData, 0, wxALL, 5 );

    m_cbRepeat = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("Repeat Alarm after seconds"), wxDefaultPosition, wxDefaultSize, 0 );
    fgSizer6->Add( m_cbRepeat, 0, wxALL, 5 );

    m_sRepeatSeconds = new wxSpinCtrl( sbSizer4->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 60 );
    fgSizer6->Add( m_sRepeatSeconds, 0, wxALL, 5 );

    m_staticText56 = new wxStaticText( sbSizer4->GetStaticBox(), wxID_ANY, _("Condition triggered for"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText56->Wrap( -1 );
    fgSizer6->Add( m_staticText56, 0, wxALL, 5 );

    wxFlexGridSizer* fgSizer37;
    fgSizer37 = new wxFlexGridSizer( 0, 2, 0, 0 );
    fgSizer37->SetFlexibleDirection( wxBOTH );
    fgSizer37->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    m_sDelay = new wxSpinCtrl( sbSizer4->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 3600, 0 );
    fgSizer37->Add( m_sDelay, 0, wxALL, 5 );

    m_staticText57 = new wxStaticText( sbSizer4->GetStaticBox(), wxID_ANY, _("Seconds"), wxDefaultPosition, wxDefaultSize, 0 );
    m_staticText57->Wrap( -1 );
    fgSizer37->Add( m_staticText57, 0, wxALL, 5 );


    fgSizer6->Add( fgSizer37, 1, wxEXPAND, 5 );

    m_cbAutoReset = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("Automatically Reset"), wxDefaultPosition, wxDefaultSize, 0 );
    fgSizer6->Add( m_cbAutoReset, 0, wxALL, 5 );

    m_cbgfxEnabled = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("Graphics Enabled"), wxDefaultPosition, wxDefaultSize, 0 );
    fgSizer6->Add( m_cbgfxEnabled, 0, wxALL, 5 );

    m_bTest = new wxButton( sbSizer4->GetStaticBox(), wxID_ANY, _("Test"), wxDefaultPosition, wxDefaultSize, 0 );
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

#else
    m_scrollWin = new wxScrolledWindow(
            this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxVSCROLL | wxHSCROLL);
    m_scrollWin->SetScrollRate(1, 1);

    wxBoxSizer *BoxSizerMainPanel = new wxBoxSizer(wxVERTICAL);
    SetSizer(BoxSizerMainPanel);

    BoxSizerMainPanel->Add(m_scrollWin, 1,  wxEXPAND | wxALL, 0);

    m_fgSizer = new wxFlexGridSizer( 0, 1, 0, 0 );
	m_fgSizer->AddGrowableRow( 0 );
	m_fgSizer->SetFlexibleDirection( wxBOTH );
	m_fgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    m_scrollWin->SetSizer(m_fgSizer);

    wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( m_scrollWin, wxID_ANY, _("Alarm Action") ), wxVERTICAL );
    m_fgSizer->Add( sbSizer4, 0, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer6->AddGrowableCol( 1 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_cbSound = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("Sound"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbSound, 0, wxALL, 5 );

	m_fpSound = new wxFilePickerCtrl( sbSizer4->GetStaticBox(), wxID_ANY, wxT("/initrd.img"), _("Select a file"), _("Wav Files (*.wav)|*.WAV;*.wav|All Files (*.*)|*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	fgSizer6->Add( m_fpSound, 0, wxALL|wxEXPAND, 5 );

	m_cbCommand = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("Command"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbCommand, 0, wxALL, 5 );

	m_tCommand = new wxTextCtrl( sbSizer4->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_tCommand->HasFlag( wxTE_MULTILINE ) )
	{
	m_tCommand->SetMaxLength( 255 );
	}
	#else
	m_tCommand->SetMaxLength( 255 );
	#endif
	fgSizer6->Add( m_tCommand, 0, wxALL|wxEXPAND, 5 );

	m_cbMessageBox = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("Message Box"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbMessageBox, 0, wxALL, 5 );

	m_cbNoData = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("Alarm if no Data"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbNoData, 0, wxALL, 5 );

	m_cbRepeat = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("Repeat Alarm after seconds"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbRepeat, 0, wxALL, 5 );

	m_sRepeatSeconds = new wxSpinCtrl( sbSizer4->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 60 );
	fgSizer6->Add( m_sRepeatSeconds, 0, wxALL, 5 );

	m_staticText56 = new wxStaticText( sbSizer4->GetStaticBox(), wxID_ANY, _("Condition triggered for"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText56->Wrap( -1 );
	fgSizer6->Add( m_staticText56, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer37;
	fgSizer37 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer37->SetFlexibleDirection( wxBOTH );
	fgSizer37->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_sDelay = new wxSpinCtrl( sbSizer4->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 3600, 0 );
	fgSizer37->Add( m_sDelay, 0, wxALL, 5 );

	m_staticText57 = new wxStaticText( sbSizer4->GetStaticBox(), wxID_ANY, _("Seconds"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57->Wrap( -1 );
	fgSizer37->Add( m_staticText57, 0, wxALL, 5 );

	fgSizer6->Add( fgSizer37, 1, wxEXPAND, 5 );

	m_cbAutoReset = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("Automatically Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbAutoReset, 0, wxALL, 5 );

	m_cbgfxEnabled = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("Graphics Enabled"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbgfxEnabled, 0, wxALL, 5 );

	m_bTest = new wxButton( sbSizer4->GetStaticBox(), wxID_ANY, _("Test"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_bTest, 0, wxALL, 5 );
	sbSizer4->Add( fgSizer6, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer15;
	fgSizer15 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer15->AddGrowableCol( 0 );
	fgSizer15->SetFlexibleDirection( wxBOTH );
	fgSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    m_fgSizer->Add( fgSizer15, 0, wxEXPAND, 5 );

	m_button6 = new wxButton( m_scrollWin, wxID_ANY, _("Information"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer15->Add( m_button6, 0, wxALL, 5 );

    wxStaticLine* staticline4;
    staticline4 =
            new wxStaticLine(m_scrollWin, wxID_ANY, wxDefaultPosition,
                             wxDefaultSize, wxLI_HORIZONTAL);
    m_fgSizer->Add(staticline4, 0, wxEXPAND | wxALL, 50);

    wxBoxSizer *bSizer = new wxBoxSizer(wxHORIZONTAL);
    m_fgSizer->Add( bSizer, 1 );

	m_sdbSizer4OK = new wxButton( m_scrollWin, wxID_OK );
	bSizer->Add( m_sdbSizer4OK );
    bSizer->AddSpacer( 5 * GetCharWidth());
	m_sdbSizer4Cancel = new wxButton( m_scrollWin, wxID_CANCEL );
	bSizer->Add( m_sdbSizer4Cancel );

    wxStaticLine* staticline5;
    staticline5 =
            new wxStaticLine(m_scrollWin, wxID_ANY, wxDefaultPosition,
                             wxDefaultSize, wxLI_HORIZONTAL);
    m_fgSizer->Add(staticline5, 0, wxEXPAND | wxALL, 50);

    this->Layout();
    m_fgSizer->Fit( this );
	this->Centre( wxBOTH );

#endif
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

AnchorPanel::AnchorPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
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

CoursePanel::CoursePanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
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

	wxFlexGridSizer* fgSizer38;
	fgSizer38 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer38->SetFlexibleDirection( wxBOTH );
	fgSizer38->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_rbGPSCourse = new wxRadioButton( this, wxID_ANY, _("GPS Course"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer38->Add( m_rbGPSCourse, 0, wxALL, 5 );

	m_rbHeading = new wxRadioButton( this, wxID_ANY, _("Heading Sensor"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer38->Add( m_rbHeading, 0, wxALL, 5 );


	fgSizer22->Add( fgSizer38, 1, wxEXPAND, 5 );


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

SpeedPanel::SpeedPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Speed Alarm") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText51 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	fgSizer14->Add( m_staticText51, 0, wxALL, 5 );

	wxString m_cModeChoices[] = { _("Under Speed"), _("Over Speed") };
	int m_cModeNChoices = sizeof( m_cModeChoices ) / sizeof( wxString );
	m_cMode = new wxChoice( sbSizer7->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cModeNChoices, m_cModeChoices, 0 );
	m_cMode->SetSelection( 0 );
	fgSizer14->Add( m_cMode, 0, wxALL, 5 );


	fgSizer14->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText44 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("Speed over ground"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	fgSizer14->Add( m_staticText44, 0, wxALL, 5 );

	m_tSpeed = new wxTextCtrl( sbSizer7->GetStaticBox(), wxID_ANY, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_tSpeed->HasFlag( wxTE_MULTILINE ) )
	{
	m_tSpeed->SetMaxLength( 10 );
	}
	#else
	m_tSpeed->SetMaxLength( 10 );
	#endif
	fgSizer14->Add( m_tSpeed, 0, wxALL, 5 );

	m_staticText24 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	fgSizer14->Add( m_staticText24, 0, wxALL, 5 );


	sbSizer7->Add( fgSizer14, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer31;
	fgSizer31 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer31->AddGrowableCol( 1 );
	fgSizer31->SetFlexibleDirection( wxBOTH );
	fgSizer31->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText28 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("Seconds to Average\nSOG over"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	fgSizer31->Add( m_staticText28, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_sliderSOGAverageNumber = new wxSlider( sbSizer7->GetStaticBox(), wxID_ANY, 10, 1, 120, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer31->Add( m_sliderSOGAverageNumber, 0, wxALL|wxEXPAND, 5 );


	sbSizer7->Add( fgSizer31, 1, wxEXPAND, 5 );

	m_staticText391 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("Graphical overlay displays circle with radius of this speed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText391->Wrap( -1 );
	sbSizer7->Add( m_staticText391, 0, wxALL, 5 );


	this->SetSizer( sbSizer7 );
	this->Layout();
	sbSizer7->Fit( this );
}

SpeedPanel::~SpeedPanel()
{
}

WindPanelBase::WindPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Wind Alarm") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText51 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	fgSizer14->Add( m_staticText51, 0, wxALL, 5 );

	wxString m_cModeChoices[] = { _("Under Speed"), _("Over Speed"), _("Direction") };
	int m_cModeNChoices = sizeof( m_cModeChoices ) / sizeof( wxString );
	m_cMode = new wxChoice( sbSizer7->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cModeNChoices, m_cModeChoices, 0 );
	m_cMode->SetSelection( 0 );
	fgSizer14->Add( m_cMode, 0, wxALL, 5 );


	fgSizer14->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer14->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText55 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText55->Wrap( -1 );
	fgSizer14->Add( m_staticText55, 0, wxALL, 5 );

	wxString m_cTypeChoices[] = { _("Apparent"), _("True Relative"), _("True Absolute") };
	int m_cTypeNChoices = sizeof( m_cTypeChoices ) / sizeof( wxString );
	m_cType = new wxChoice( sbSizer7->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cTypeNChoices, m_cTypeChoices, 0 );
	m_cType->SetSelection( 0 );
	fgSizer14->Add( m_cType, 0, wxALL, 5 );

	m_bAboutWind = new wxButton( sbSizer7->GetStaticBox(), wxID_ANY, _("?"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_bAboutWind, 0, wxALL, 5 );


	fgSizer14->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText44 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("Value"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	fgSizer14->Add( m_staticText44, 0, wxALL, 5 );

	m_sValue = new wxSpinCtrl( sbSizer7->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 360, 5 );
	fgSizer14->Add( m_sValue, 0, wxALL, 5 );

	m_staticText24 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("knots/degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	fgSizer14->Add( m_staticText24, 0, wxALL, 5 );

	m_bSync = new wxButton( sbSizer7->GetStaticBox(), wxID_ANY, _("Sync"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_bSync, 0, wxALL, 5 );

	m_staticText58 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("Range"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText58->Wrap( -1 );
	fgSizer14->Add( m_staticText58, 0, wxALL, 5 );

	m_sRange = new wxSpinCtrl( sbSizer7->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 180, 15 );
	fgSizer14->Add( m_sRange, 0, wxALL, 5 );

	m_staticText59 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText59->Wrap( -1 );
	fgSizer14->Add( m_staticText59, 0, wxALL, 5 );


	sbSizer7->Add( fgSizer14, 1, wxEXPAND, 5 );


	this->SetSizer( sbSizer7 );
	this->Layout();
	sbSizer7->Fit( this );

	// Connect Events
	m_cType->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( WindPanelBase::OnType ), NULL, this );
	m_bAboutWind->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WindPanelBase::OnAboutWind ), NULL, this );
	m_bSync->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WindPanelBase::OnSync ), NULL, this );
}

WindPanelBase::~WindPanelBase()
{
	// Disconnect Events
	m_cType->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( WindPanelBase::OnType ), NULL, this );
	m_bAboutWind->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WindPanelBase::OnAboutWind ), NULL, this );
	m_bSync->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WindPanelBase::OnSync ), NULL, this );

}

WeatherPanelBase::WeatherPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Weather Alarm") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText50 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("Variable"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText50->Wrap( -1 );
	fgSizer14->Add( m_staticText50, 0, wxALL, 5 );

	wxString m_cVariableChoices[] = { _("Barometer"), _("Air Temperature"), _("Sea Temperature"), _("Relative Humidity"), _("Humidity") };
	int m_cVariableNChoices = sizeof( m_cVariableChoices ) / sizeof( wxString );
	m_cVariable = new wxChoice( sbSizer7->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cVariableNChoices, m_cVariableChoices, 0 );
	m_cVariable->SetSelection( 0 );
	fgSizer14->Add( m_cVariable, 0, wxALL, 5 );


	fgSizer14->Add( 0, 0, 1, wxEXPAND, 5 );

	m_rbValue = new wxRadioButton( sbSizer7->GetStaticBox(), wxID_ANY, _("Value"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_rbValue, 0, wxALL, 5 );

	m_rbRate = new wxRadioButton( sbSizer7->GetStaticBox(), wxID_ANY, _("Rate"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_rbRate, 0, wxALL, 5 );


	fgSizer14->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText51 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	fgSizer14->Add( m_staticText51, 0, wxALL, 5 );

	wxString m_cTypeChoices[] = { _("Increasing"), _("Decreasing") };
	int m_cTypeNChoices = sizeof( m_cTypeChoices ) / sizeof( wxString );
	m_cType = new wxChoice( sbSizer7->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cTypeNChoices, m_cTypeChoices, 0 );
	m_cType->SetSelection( 0 );
	fgSizer14->Add( m_cType, 0, wxALL, 5 );


	fgSizer14->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText511 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("Value"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText511->Wrap( -1 );
	fgSizer14->Add( m_staticText511, 0, wxALL, 5 );

	m_tValue = new wxTextCtrl( sbSizer7->GetStaticBox(), wxID_ANY, _("1010"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_tValue, 0, wxALL, 5 );

	m_stUnits = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("mBar/C/%"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stUnits->Wrap( -1 );
	fgSizer14->Add( m_stUnits, 0, wxALL, 5 );

	m_staticText53 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("In Last"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53->Wrap( -1 );
	fgSizer14->Add( m_staticText53, 0, wxALL, 5 );

	m_sRatePeriod = new wxSpinCtrl( sbSizer7->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 3600, 60 );
	fgSizer14->Add( m_sRatePeriod, 0, wxALL, 5 );

	m_staticText54 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("Seconds"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText54->Wrap( -1 );
	fgSizer14->Add( m_staticText54, 0, wxALL, 5 );


	sbSizer7->Add( fgSizer14, 1, wxEXPAND, 5 );

	m_staticText391 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("No Graphical overlay"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText391->Wrap( -1 );
	sbSizer7->Add( m_staticText391, 0, wxALL, 5 );


	this->SetSizer( sbSizer7 );
	this->Layout();
	sbSizer7->Fit( this );

	// Connect Events
	m_cVariable->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( WeatherPanelBase::OnVariable ), NULL, this );
	m_rbValue->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( WeatherPanelBase::OnVariable ), NULL, this );
	m_rbRate->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( WeatherPanelBase::OnVariable ), NULL, this );
}

WeatherPanelBase::~WeatherPanelBase()
{
	// Disconnect Events
	m_cVariable->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( WeatherPanelBase::OnVariable ), NULL, this );
	m_rbValue->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( WeatherPanelBase::OnVariable ), NULL, this );
	m_rbRate->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( WeatherPanelBase::OnVariable ), NULL, this );

}

DeadmanPanel::DeadmanPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
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

NMEADataPanel::NMEADataPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
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

DepthPanel::DepthPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Depth Alarm") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText51 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	fgSizer14->Add( m_staticText51, 0, wxALL, 5 );

	wxString m_cModeChoices[] = { _("Minimum Depth"), _("Depth Decreasing"), _("Maximum Depth"), _("Depth Increasing") };
	int m_cModeNChoices = sizeof( m_cModeChoices ) / sizeof( wxString );
	m_cMode = new wxChoice( sbSizer7->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cModeNChoices, m_cModeChoices, 0 );
	m_cMode->SetSelection( 0 );
	fgSizer14->Add( m_cMode, 0, wxALL, 5 );


	fgSizer14->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText44 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("Depth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	fgSizer14->Add( m_staticText44, 0, wxALL, 5 );

	m_tDepth = new wxTextCtrl( sbSizer7->GetStaticBox(), wxID_ANY, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_tDepth->HasFlag( wxTE_MULTILINE ) )
	{
	m_tDepth->SetMaxLength( 10 );
	}
	#else
	m_tDepth->SetMaxLength( 10 );
	#endif
	fgSizer14->Add( m_tDepth, 0, wxALL, 5 );

	wxString m_cUnitsChoices[] = { _("Meters"), _("Feet") };
	int m_cUnitsNChoices = sizeof( m_cUnitsChoices ) / sizeof( wxString );
	m_cUnits = new wxChoice( sbSizer7->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cUnitsNChoices, m_cUnitsChoices, 0 );
	m_cUnits->SetSelection( 0 );
	fgSizer14->Add( m_cUnits, 0, wxALL, 5 );


	sbSizer7->Add( fgSizer14, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer45;
	fgSizer45 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer45->AddGrowableCol( 0 );
	fgSizer45->AddGrowableRow( 0 );
	fgSizer45->SetFlexibleDirection( wxBOTH );
	fgSizer45->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText64 = new wxStaticText( sbSizer7->GetStaticBox(), wxID_ANY, _("Alarm will trigger if less than the minimum depth, or greater than maximum depth.\n\nAlarm will trigger if increasing/decreasing at the depth in units per second over the last 10 seconds."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText64->Wrap( -1 );
	fgSizer45->Add( m_staticText64, 0, wxALL|wxEXPAND, 5 );


	sbSizer7->Add( fgSizer45, 1, wxEXPAND, 5 );


	this->SetSizer( sbSizer7 );
	this->Layout();
}

DepthPanel::~DepthPanel()
{
}

LandFallPanel::LandFallPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
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

BoundaryPanel::BoundaryPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
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

pypilotPanel::pypilotPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxFlexGridSizer* fgSizer17;
	fgSizer17 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer17->SetFlexibleDirection( wxBOTH );
	fgSizer17->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* sbSizer10;
	sbSizer10 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Host") ), wxVERTICAL );

	m_tHost = new wxTextCtrl( sbSizer10->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer10->Add( m_tHost, 0, wxALL|wxEXPAND, 5 );


	fgSizer17->Add( sbSizer10, 1, wxEXPAND, 5 );

	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel2 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer57;
	fgSizer57 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer57->SetFlexibleDirection( wxBOTH );
	fgSizer57->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer60;
	fgSizer60 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer60->SetFlexibleDirection( wxBOTH );
	fgSizer60->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_cbNoConnection = new wxCheckBox( m_panel2, wxID_ANY, _("No Connection"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer60->Add( m_cbNoConnection, 0, wxALL, 5 );


	fgSizer57->Add( fgSizer60, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer62;
	fgSizer62 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer62->SetFlexibleDirection( wxBOTH );
	fgSizer62->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_cbOverTemperature = new wxCheckBox( m_panel2, wxID_ANY, _("Over Temperature"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer62->Add( m_cbOverTemperature, 0, wxALL, 5 );

	m_cbOverCurrent = new wxCheckBox( m_panel2, wxID_ANY, _("Over Current"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer62->Add( m_cbOverCurrent, 0, wxALL, 5 );


	fgSizer57->Add( fgSizer62, 1, wxEXPAND, 5 );


	m_panel2->SetSizer( fgSizer57 );
	m_panel2->Layout();
	fgSizer57->Fit( m_panel2 );
	m_notebook1->AddPage( m_panel2, _("Hardware Failures"), false );
	m_panel3 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer59;
	fgSizer59 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer59->SetFlexibleDirection( wxBOTH );
	fgSizer59->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer53;
	fgSizer53 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer53->SetFlexibleDirection( wxBOTH );
	fgSizer53->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_cbNoIMU = new wxCheckBox( m_panel3, wxID_ANY, _("No IMU Sensor"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer53->Add( m_cbNoIMU, 0, wxALL, 5 );

	m_cbNoMotorController = new wxCheckBox( m_panel3, wxID_ANY, _("No Motor Controller"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer53->Add( m_cbNoMotorController, 0, wxALL, 5 );

	m_cbNoRudderFeedback = new wxCheckBox( m_panel3, wxID_ANY, _("No Rudder Feedback"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer53->Add( m_cbNoRudderFeedback, 0, wxALL, 5 );

	m_cbNoMotorTemperature = new wxCheckBox( m_panel3, wxID_ANY, _("No Motor Temperature"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer53->Add( m_cbNoMotorTemperature, 0, wxALL, 5 );


	fgSizer59->Add( fgSizer53, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer58;
	fgSizer58 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer58->SetFlexibleDirection( wxBOTH );
	fgSizer58->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_cbDriverTimeout = new wxCheckBox( m_panel3, wxID_ANY, _("Driver Timeout (No Motor)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer58->Add( m_cbDriverTimeout, 0, wxALL, 5 );

	m_button19 = new wxButton( m_panel3, wxID_ANY, _("?"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer58->Add( m_button19, 0, wxALL, 5 );


	fgSizer59->Add( fgSizer58, 1, wxEXPAND, 5 );


	m_panel3->SetSizer( fgSizer59 );
	m_panel3->Layout();
	fgSizer59->Fit( m_panel3 );
	m_notebook1->AddPage( m_panel3, _("Hardware Errors"), false );
	m_panel1 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer54;
	fgSizer54 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer54->SetFlexibleDirection( wxBOTH );
	fgSizer54->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_cbEndOfTravel = new wxCheckBox( m_panel1, wxID_ANY, _("End Of Travel"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer54->Add( m_cbEndOfTravel, 0, wxALL, 5 );

	m_cbLostMode = new wxCheckBox( m_panel1, wxID_ANY, _("Lost Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer54->Add( m_cbLostMode, 0, wxALL, 5 );

	m_cbServoSaturated = new wxCheckBox( m_panel1, wxID_ANY, _("Servo Saturated"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer54->Add( m_cbServoSaturated, 0, wxALL, 5 );

	m_cbPowerConsumption = new wxCheckBox( m_panel1, wxID_ANY, _("Power Consumption >"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer54->Add( m_cbPowerConsumption, 0, wxALL, 5 );

	m_sPowerConsumption = new wxSpinCtrlDouble( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_sPowerConsumption->SetDigits( 0 );
	fgSizer54->Add( m_sPowerConsumption, 0, wxALL, 5 );

	m_staticText72 = new wxStaticText( m_panel1, wxID_ANY, _("Watts"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText72->Wrap( -1 );
	fgSizer54->Add( m_staticText72, 0, wxALL, 5 );

	m_cbCourseError = new wxCheckBox( m_panel1, wxID_ANY, _("Course Error >"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer54->Add( m_cbCourseError, 0, wxALL, 5 );

	m_sCourseError = new wxSpinCtrlDouble( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_sCourseError->SetDigits( 0 );
	fgSizer54->Add( m_sCourseError, 0, wxALL, 5 );

	m_staticText73 = new wxStaticText( m_panel1, wxID_ANY, _("Degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText73->Wrap( -1 );
	fgSizer54->Add( m_staticText73, 0, wxALL, 5 );


	m_panel1->SetSizer( fgSizer54 );
	m_panel1->Layout();
	fgSizer54->Fit( m_panel1 );
	m_notebook1->AddPage( m_panel1, _("Steering Errors"), true );

	fgSizer17->Add( m_notebook1, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( fgSizer17 );
	this->Layout();
	fgSizer17->Fit( this );

	// Connect Events
	m_button19->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( pypilotPanel::OnAboutHardwareErrors ), NULL, this );
}

pypilotPanel::~pypilotPanel()
{
	// Disconnect Events
	m_button19->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( pypilotPanel::OnAboutHardwareErrors ), NULL, this );

}

RudderPanel::RudderPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxFlexGridSizer* fgSizer22;
	fgSizer22 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer22->SetFlexibleDirection( wxBOTH );
	fgSizer22->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer101;
	fgSizer101 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer101->SetFlexibleDirection( wxBOTH );
	fgSizer101->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText32 = new wxStaticText( this, wxID_ANY, _("Lower Limit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	fgSizer101->Add( m_staticText32, 0, wxALL, 5 );

	m_LowerLimit = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -30, 30, 0 );
	fgSizer101->Add( m_LowerLimit, 0, wxALL, 5 );

	m_staticText18 = new wxStaticText( this, wxID_ANY, _("Degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	fgSizer101->Add( m_staticText18, 0, wxALL, 5 );

	m_staticText21 = new wxStaticText( this, wxID_ANY, _("Upper Limit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	fgSizer101->Add( m_staticText21, 0, wxALL, 5 );

	m_UpperLimit = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -30, 30, 0 );
	fgSizer101->Add( m_UpperLimit, 0, wxALL, 5 );

	m_staticText181 = new wxStaticText( this, wxID_ANY, _("Degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText181->Wrap( -1 );
	fgSizer101->Add( m_staticText181, 0, wxALL, 5 );


	fgSizer22->Add( fgSizer101, 1, wxEXPAND, 5 );


	this->SetSizer( fgSizer22 );
	this->Layout();
	fgSizer22->Fit( this );
}

RudderPanel::~RudderPanel()
{
}
