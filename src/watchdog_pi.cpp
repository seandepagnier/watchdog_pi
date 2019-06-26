/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  watchdog Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2017 by Sean D'Epagnier                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 */

#include <wx/wx.h>
#include <wx/stdpaths.h>

#include "json/json.h"

#include "wddc.h"

#include "watchdog_pi.h"
#include "WatchdogDialog.h"
#include "ConfigurationDialog.h"
#include "WatchdogPropertiesDialog.h"
#include "icons.h"
#include "AIS_Target_Info.h"

Json::Value g_ReceivedPathGUIDJSONMsg;
wxString    g_ReceivedPathGUIDMessage;
Json::Value g_ReceivedBoundaryTimeJSONMsg;
wxString    g_ReceivedBoundaryTimeMessage;
Json::Value g_ReceivedBoundaryDistanceJSONMsg;
wxString    g_ReceivedBoundaryDistanceMessage;
Json::Value g_ReceivedBoundaryAnchorJSONMsg;
wxString    g_ReceivedBoundaryAnchorMessage;
Json::Value g_ReceivedBoundaryGUIDJSONMsg;
wxString    g_ReceivedBoundaryGUIDMessage;
Json::Value g_ReceivedGuardZoneJSONMsg;
wxString    g_ReceivedGuardZoneMessage;
Json::Value g_ReceivedGuardZoneGUIDJSONMsg;
wxString    g_ReceivedGuardZoneGUIDMessage;
Json::Value g_ReceivedODVersionJSONMsg;
wxString    g_ReceivedODVersionMessage;
Json::Value g_ReceivedODAPIJSONMsg;
wxString    g_ReceivedODAPIMessage;
Json::Value g_ReceivedAISJSONMsg;
wxString    g_ReceivedAISMessage;

wxString    g_BoundaryName;
wxString    g_BoundaryDescription;
wxString    g_BoundaryGUID;
wxString    g_GuardZoneName;
wxString    g_GuardZoneDescription;
wxString    g_GuardZoneGUID;

AIS_Target_Info g_AISTarget;


double heading_resolve(double degrees, double offset)
{
    while(degrees < offset-180)
        degrees += 360;
    while(degrees >= offset+180)
        degrees -= 360;
    return degrees;
}

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new watchdog_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

//-----------------------------------------------------------------------------
//
//    Watchdog PlugIn Implementation
//
//-----------------------------------------------------------------------------
watchdog_pi *g_watchdog_pi = NULL;

watchdog_pi::watchdog_pi(void *ppimgr)
    : opencpn_plugin_113(ppimgr)
{
    // Create the PlugIn icons
    initialize_images();
    m_lastfix.Lat = NAN;
    m_lasttimerfix.Lat = NAN;
    m_lasttimerfix.FixTime = m_lastfix.FixTime = 0;
    m_sog = m_cog = m_hdm = 0;
    m_declination = NAN;
    
    g_ReceivedPathGUIDMessage = wxEmptyString;
    g_ReceivedBoundaryTimeMessage = wxEmptyString;
    g_ReceivedBoundaryDistanceMessage = wxEmptyString;
    g_ReceivedBoundaryAnchorMessage = wxEmptyString;
    g_ReceivedBoundaryGUIDMessage = wxEmptyString;
    g_ReceivedGuardZoneMessage = wxEmptyString;
    g_ReceivedGuardZoneGUIDMessage = wxEmptyString;
    g_ReceivedODVersionMessage = wxEmptyString;
    g_GuardZoneName = wxEmptyString;
    g_GuardZoneDescription = wxEmptyString;
    g_GuardZoneGUID = wxEmptyString;
    
    g_AISTarget.m_dLat = 0.;
    g_AISTarget.m_dLon = 0.;
    g_AISTarget.m_dSOG = 0.;
    g_AISTarget.m_dCOG = 0.;
    g_AISTarget.m_dHDG = 0.;
    g_AISTarget.m_iMMSI = 0;
    g_AISTarget.m_sShipName = wxEmptyString;

    g_watchdog_pi = this;
}

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

int watchdog_pi::Init(void)
{
    AddLocaleCatalog( PLUGIN_CATALOG_NAME );

    Alarm::LoadConfigAll();

#ifdef WATCHDOG_USE_SVG
    m_leftclick_tool_id = InsertPlugInToolSVG(  "Watchdog" , _svg_watchdog, _svg_watchdog,
        _svg_watchdog, wxITEM_CHECK, _( "Watchdog" ),  "" , NULL, WATCHDOG_TOOL_POSITION, 0, this);
#else
    m_leftclick_tool_id  = InsertPlugInTool
        ("", _img_watchdog, _img_watchdog, wxITEM_NORMAL,
         _("Watchdog"), "", NULL, WATCHDOG_TOOL_POSITION, 0, this);
#endif
    
    m_PropertiesDialog = NULL;
    m_Timer.Connect(wxEVT_TIMER, wxTimerEventHandler
                    ( watchdog_pi::OnTimer ), NULL, this);
    m_Timer.Start(3000);
    
    m_WatchdogDialog = new WatchdogDialog(*this, GetOCPNCanvasWindow());
    m_ConfigurationDialog = new ConfigurationDialog(*this, m_WatchdogDialog);
        
    wxIcon icon;
    icon.CopyFromBitmap(*_img_watchdog);
    m_WatchdogDialog->SetIcon(icon);
    m_ConfigurationDialog->SetIcon(icon);

    m_bWatchdogDialogShown = false;
    m_cursor_time = wxDateTime::Now();
    m_ValidFixTime = wxDateTime::Now();

    return (WANTS_OVERLAY_CALLBACK |
            WANTS_OPENGL_OVERLAY_CALLBACK |
            WANTS_TOOLBAR_CALLBACK    |
            WANTS_CURSOR_LATLON       |
            WANTS_NMEA_SENTENCES      |
            WANTS_NMEA_EVENTS         |
            WANTS_AIS_SENTENCES       |
            WANTS_PLUGIN_MESSAGING    |
            WANTS_PREFERENCES         |
            WANTS_CONFIG);
}

bool watchdog_pi::DeInit(void)
{
    Alarm::SaveConfigAll();
    Alarm::DeleteAll();

    //    Record the dialog position
    if (m_WatchdogDialog)
    {
        if(m_ConfigurationDialog) {
            delete m_ConfigurationDialog;
        }
        m_WatchdogDialog->Close();
        delete m_WatchdogDialog;
        m_WatchdogDialog = NULL;
        m_ConfigurationDialog = NULL;
    }
    
    m_Timer.Stop();
    m_Timer.Disconnect(wxEVT_TIMER, wxTimerEventHandler( watchdog_pi::OnTimer ), NULL, this);
    
    RemovePlugInTool(m_leftclick_tool_id);

    return true;
}

int watchdog_pi::GetAPIVersionMajor()
{
    return MY_API_VERSION_MAJOR;
}

int watchdog_pi::GetAPIVersionMinor()
{
    return MY_API_VERSION_MINOR;
}

int watchdog_pi::GetPlugInVersionMajor()
{
    return PLUGIN_VERSION_MAJOR;
}

int watchdog_pi::GetPlugInVersionMinor()
{
    return PLUGIN_VERSION_MINOR;
}

wxBitmap *watchdog_pi::GetPlugInBitmap()
{
    return new wxBitmap(_img_watchdog->ConvertToImage().Copy());
}

wxString watchdog_pi::GetCommonName()
{
    return _("Watchdog");
}

wxString watchdog_pi::GetShortDescription()
{
    return _("Watchdog PlugIn for OpenCPN");
}

wxString watchdog_pi::GetLongDescription()
{
    return _("Watchdog PlugIn for OpenCPN\n\
Alarm user of changing conditions.");
}

int watchdog_pi::GetToolbarToolCount(void)
{
    return 1;
}

void watchdog_pi::ShowPreferencesDialog( wxWindow* parent )
{
    //dlgShow = false;
    if( NULL == m_PropertiesDialog )
        m_PropertiesDialog = new WatchdogPropertiesDialog( parent );
    
    m_PropertiesDialog->ShowModal();
    
    delete m_PropertiesDialog;
    m_PropertiesDialog = NULL;
    
}

void watchdog_pi::SetColorScheme(PI_ColorScheme cs)
{
    if (NULL == m_WatchdogDialog)
        return;

    DimeWindow(m_WatchdogDialog);
}

void watchdog_pi::RearrangeWindow()
{
    if (NULL == m_WatchdogDialog)
        return;

    SetColorScheme(PI_ColorScheme());
    
    m_WatchdogDialog->Fit();
}

void watchdog_pi::OnToolbarToolCallback(int id)
{
    if(!m_WatchdogDialog)
    {
        m_WatchdogDialog = new WatchdogDialog(*this, GetOCPNCanvasWindow());
        m_ConfigurationDialog = new ConfigurationDialog(*this, m_WatchdogDialog);

        wxIcon icon;
        icon.CopyFromBitmap(*_img_watchdog);
        m_WatchdogDialog->SetIcon(icon);
        m_ConfigurationDialog->SetIcon(icon);
    }

    m_WatchdogDialog->Show(!m_WatchdogDialog->IsShown());
    if(m_WatchdogDialog->IsShown()) {
        m_bWatchdogDialogShown = true;
        m_WatchdogDialog->UpdateAlarms();
    }

    wxPoint p = m_WatchdogDialog->GetPosition();
    m_WatchdogDialog->Move(0, 0);        // workaround for gtk autocentre dialog behavior
    m_WatchdogDialog->Move(p);
}

void watchdog_pi::OnContextMenuItemCallback(int id)
{
}

bool watchdog_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
    wdDC odc(dc);
    Render(odc, *vp);
    return true;
}

bool watchdog_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
    wdDC odc;
    glEnable( GL_BLEND );
    Render(odc, *vp);
    glDisable( GL_BLEND );
    return true;
}

void watchdog_pi::Render(wdDC &dc, PlugIn_ViewPort &vp)
{
    if((!m_WatchdogDialog || !m_WatchdogDialog->IsShown()) && (m_iEnableType == ID_ALARM_NEVER || m_iEnableType == ID_ALARM_VISIBLE))
        return;
    if(m_iEnableType == ID_ALARM_VISIBLE && !m_bWatchdogDialogShown)
        return;
    if(m_iEnableType == ID_ALARM_ONCE && !m_bWatchdogDialogShown)
        return;

    Alarm::RenderAll(dc, vp);
}

void watchdog_pi::OnTimer( wxTimerEvent & )
{
    /* calculate course and speed over ground from gps */
    double dt = m_lastfix.FixTime - m_lasttimerfix.FixTime;
    if(!wxIsNaN(m_lastfix.Lat) && !wxIsNaN(m_lasttimerfix.Lat) && dt > 0) {
        /* this way helps avoid surge speed from gps from surfing waves etc... */
        double cog, sog;
        DistanceBearingMercator_Plugin(m_lastfix.Lat, m_lastfix.Lon,
                                       m_lasttimerfix.Lat, m_lasttimerfix.Lon, &cog, &sog);
        sog *= (3600.0 / dt);

        if(wxIsNaN(m_cog))
            m_cog = cog, m_sog = sog;
        else {
            cog = heading_resolve(cog, m_cog);
            m_cog = .25*cog + .75*m_cog;
            m_cog = heading_resolve(m_cog, 180);
            m_sog = .25*sog + .75*m_sog;
        }
        m_hdm = m_lastfix.Hdm;
        m_ValidFixTime = wxDateTime::Now();
    } else {
        wxLongLong dt = (wxDateTime::Now() - m_ValidFixTime).GetSeconds();
        //printf("wddt %ld\n", dt.ToLong());
        if(dt > 60 || (dt > 11 && m_lastfix.FixTime > 0))
            // wait 60 seconds from startup because of slowness to receive first nmea message
            m_sog = m_cog = m_hdm = NAN;
    }
    
    m_lasttimerfix = m_lastfix;
}

void watchdog_pi::SetCursorLatLon(double lat, double lon)
{
    wxPoint pos = wxGetMouseState().GetPosition();
    if(pos == m_cursor_position)
        return;
    m_cursor_position = pos;
    m_cursor_time = wxDateTime::Now();
}

void watchdog_pi::SetNMEASentence(wxString &sentence)
{
    Alarm::NMEAStringAll(sentence);
}

void watchdog_pi::SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix)
{
    m_lastfix = pfix;
}

void watchdog_pi::SetPluginMessage(wxString &message_id, wxString &message_body)
{
    // construct the JSON root object
    Json::Value  root;
    // construct a JSON parser
    Json::Reader reader;
    bool        bFail = false;
    
    if(message_id == "WATCHDOG_PI") {
        // now read the JSON text and store it in the 'root' structure
        // check for errors before retreiving values...
        if ( !reader.parse( (std::string)message_body, root ) ) {
            wxLogMessage(wxString("watchdog_pi: Error parsing JSON message: ") + reader.getFormattedErrorMessages() + " : " + message_body );
            return;
        }
        
        if(!root.isMember( "Source")) {
            // Originator
            wxLogMessage( "No Source found in message" );
            bFail = true;
        }
        
        if(!root.isMember( "Msg")) {
            // Message identifier
            wxLogMessage( "No Msg found in message" );
            bFail = true;
        }
        
        if(!root.isMember( "Type")) {
            // Message type, orig or resp
            wxLogMessage( "No Type found in message" );
            bFail = true;
        }
        
        if(!root.isMember( "MsgId")) {
            // Unique (?) Msg number/identifier
            wxLogMessage( "No MsgNo found in message" );
            bFail = true;
        }
        
        if(!bFail) {
            if(root["Type"].asString() == "Response" && root["Source"].asString() == "OCPN_DRAW_PI") {
                if(root["Msg"].asString() == "FindPathByGUID" ) {
                    if(root["MsgId"].asString() == "guard" || root["MsgId"].asString() == "inclusion" || root["MsgId"].asString() == "general") {                        g_ReceivedPathGUIDJSONMsg = root;
                        g_ReceivedPathGUIDMessage = message_body;
                    }
                } else if(root["Msg"].asString() == "FindPointInAnyBoundary" ) {
                    if(root["MsgId"].asString() == "time") {
                    g_ReceivedBoundaryTimeJSONMsg = root;
                    g_ReceivedBoundaryTimeMessage = message_body;
                    } else if(root["MsgId"].asString() == "distance") {
                        g_ReceivedBoundaryDistanceJSONMsg = root;
                        g_ReceivedBoundaryDistanceMessage = message_body;
                    } else if(root["MsgId"].asString() == "GetGUID") {
                        g_ReceivedBoundaryGUIDJSONMsg = root;
                        g_ReceivedBoundaryGUIDMessage = message_body;
                    }
                } else if(root["Msg"].asString() == "FindClosestBoundaryLineCrossing" ) {
                    if(root["MsgId"].asString() == "distance") {
                        g_ReceivedBoundaryDistanceJSONMsg = root;
                        g_ReceivedBoundaryDistanceMessage = message_body;
                    } else if(root["MsgId"].asString() == "time") {
                        g_ReceivedBoundaryTimeJSONMsg = root;
                        g_ReceivedBoundaryTimeMessage = message_body;
                    }
                } else if(root["Msg"].asString() == "FindPointInBoundary" ) {
                    if(root["MsgId"].asString() == "anchor") {
                        g_ReceivedBoundaryAnchorJSONMsg = root;
                        g_ReceivedBoundaryAnchorMessage = message_body;
                    }
                } else if(root["Msg"].asString() == "FindPointInGuardZone" ) {
                    if(root["MsgId"].asString() == "guard") {
                        g_ReceivedGuardZoneJSONMsg = root;
                        g_ReceivedGuardZoneMessage = message_body;
                    }
                } else if(root["Msg"].asString() == "Version" ) {
                    if(root["MsgId"].asString() == "version") {
                        g_ReceivedODVersionJSONMsg = root;
                        g_ReceivedODVersionMessage = message_body;
                    }
                } else if(root["Msg"].asString() == "GetAPIAddresses" ) {
                    if(root["MsgId"].asString() == "GetAPIAddresses") {
                        g_ReceivedODAPIJSONMsg = root;
                        g_ReceivedODAPIMessage = message_body;
                    }
                }
            }
        }
        return;
    } else if(message_id == "AIS") {
        if(!reader.parse( (std::string)message_body, root )) {
            wxLogMessage(wxString("watchdog_pi: Error parsing AIS JSON message: ") + reader.getFormattedErrorMessages() + " : " + message_body );
            return;
        }
        if(!root.isMember( "Source")) {
            wxLogMessage( "No Source found in message" );
            bFail = true;
        }
        if(!root.isMember( "Msg")) {
            wxLogMessage( "No Msg found in message" );
            bFail = true;
        }
        if(!root.isMember( "Type")) {
            wxLogMessage( "No Type found in message" );
            bFail = true;
        }
        if(!root.isMember( "MsgId")) {
            wxLogMessage( "No MsgNo found in message" );
            bFail = true;
        }
        if(!root.isMember( "lat")) {
            wxLogMessage( "No Latitude found in message" );
            bFail = true;
        }
        if(!root.isMember( "lon")) {
            wxLogMessage( "No Longitude found in message" );
            bFail = true;
        }
        if(!root.isMember( "sog")) {
            wxLogMessage( "No SOG found in message" );
            bFail = true;
        }
        if(!root.isMember( "cog")) {
            wxLogMessage( "No COG found in message" );
            bFail = true;
        }
        if(!root.isMember( "hdg")) {
            wxLogMessage( "No Heading found in message" );
            bFail = true;
        }
        if(!root.isMember( "mmsi")) {
            wxLogMessage( "No MMSI found in message" );
            bFail = true;
        }
        if(!root.isMember( "shipname")) {
            wxLogMessage( "No Ship Name found in message" );
            bFail = true;
        }
        
        if(!bFail) {
            if(root["Type"].asString() == "Information" && root["Source"].asString() == "AIS_Decoder") {
                g_ReceivedAISJSONMsg = root;
                g_ReceivedAISMessage = message_body;
                g_AISTarget.m_dLat = g_ReceivedAISJSONMsg["lat"].asDouble();
                g_AISTarget.m_dLon = g_ReceivedAISJSONMsg["lon"].asDouble();
                g_AISTarget.m_dSOG = g_ReceivedAISJSONMsg["sog"].asDouble();
                g_AISTarget.m_dCOG = g_ReceivedAISJSONMsg["cog"].asDouble();
                g_AISTarget.m_dHDG = g_ReceivedAISJSONMsg["hdg"].asDouble();
                g_AISTarget.m_iMMSI = g_ReceivedAISJSONMsg["mmsi"].asDouble();
                g_AISTarget.m_sShipName = wxString(g_ReceivedAISJSONMsg["shipname"].asString()).Trim();
                if(root.isMember( "callsign" ))
                    g_AISTarget.m_sCallSign = wxString(g_ReceivedAISJSONMsg["callsign"].asString()).Trim();
                else
                    g_AISTarget.m_sCallSign = wxEmptyString;
                if(root.isMember( "active" ))
                    g_AISTarget.m_bActive = g_ReceivedAISJSONMsg["active"].asBool();
                else
                    g_AISTarget.m_bActive = true;
                if(root.isMember( "lost" ))
                    g_AISTarget.m_bLost = g_ReceivedAISJSONMsg["lost"].asBool();
                else
                    g_AISTarget.m_bLost = false;
                if(root.isMember( "ownship" ))
                    g_AISTarget.m_bOwnship = g_ReceivedAISJSONMsg["ownship"].asBool();
                else
                    g_AISTarget.m_bOwnship = false;
            }
            for(unsigned int i=0; i<Alarm::s_Alarms.size(); i++) {
                Alarm *p_Alarm = Alarm::s_Alarms[i];
                if(p_Alarm->Type() == _("Guard Zone")) {
                    p_Alarm->OnAISMessage(i);
                }
            }
        }
    } else if(message_id == "WMM_VARIATION_BOAT") {
        if(reader.parse( (std::string)message_body, root )) {
            m_declination = root["Decl"].asDouble();
            m_declinationTime = wxDateTime::Now();
        }
    }
}


void watchdog_pi::ShowConfigurationDialog( wxWindow* )
{
    m_ConfigurationDialog->Show();
}

wxString watchdog_pi::StandardPath()
{
    wxStandardPathsBase& std_path = wxStandardPathsBase::Get();
    wxString s = wxFileName::GetPathSeparator();

#if defined(__WXMSW__)
    wxString stdPath  = std_path.GetConfigDir();
#elif defined(__WXGTK__) || defined(__WXQT__)
    wxString stdPath  = std_path.GetUserDataDir();
#elif defined(__WXOSX__)
    wxString stdPath  = (std_path.GetUserConfigDir() + s + "opencpn");
#endif

    stdPath += s + "plugins";
    if (!wxDirExists(stdPath))
      wxMkdir(stdPath);

    stdPath += s + "watchdog";

#ifdef __WXOSX__
    // Compatibility with pre-OCPN-4.2; move config dir to
    // ~/Library/Preferences/opencpn if it exists
    wxString oldPath = (std_path.GetUserConfigDir() + s + "plugins" + s + "weatherfax");
    if (wxDirExists(oldPath) && !wxDirExists(stdPath)) {
	wxLogMessage("weatherfax_pi: moving config dir %s to %s", oldPath, stdPath);
	wxRenameFile(oldPath, stdPath);
    }
#endif

    if (!wxDirExists(stdPath))
      wxMkdir(stdPath);

    stdPath += s; // is this necessary?
    return stdPath;
}

double watchdog_pi::Declination()
{
    if(!m_declinationTime.IsValid() || (wxDateTime::Now() - m_declinationTime).GetSeconds() > 1200)
        SendPluginMessage("WMM_VARIATION_BOAT_REQUEST", "");

    return m_declination;
}
