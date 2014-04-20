/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  watch dog Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2013 by Sean D'Epagnier                                 *
 *   sean at depagnier dot com                                             *
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

#include "ocpndc.h"

#include "watchdog_pi.h"
#include "WatchdogDialog.h"
#include "WatchdogPrefsDialog.h"
#include "icons.h"


double heading_resolve(double degrees)
{
    while(degrees < -180)
        degrees += 360;
    while(degrees >= 180)
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
    : opencpn_plugin_110(ppimgr)
{
    // Create the PlugIn icons
    initialize_images();
    m_lastfix.Lat = NAN;
    m_lasttimerfix.Lat = NAN;
    m_sog = m_cog = NAN;

    g_watchdog_pi = this;
}

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

int watchdog_pi::Init(void)
{
    AddLocaleCatalog( _T("opencpn-watchdog_pi") );

    // Set some default private member parameters
    m_watchdog_dialog_x = 0;
    m_watchdog_dialog_y = 0;

    ::wxDisplaySize(&m_display_width, &m_display_height);
    
    LoadConfig(); //    And load the configuration items
    
    m_leftclick_tool_id  = InsertPlugInTool
        (_T(""), _img_watchdog, _img_watchdog, wxITEM_NORMAL,
         _("Watchdog"), _T(""), NULL, WATCHMAN_TOOL_POSITION, 0, this);
    
    m_pWatchdogDialog = NULL;
    m_pWatchdogPrefsDialog = NULL;
    m_Timer.Connect(wxEVT_TIMER, wxTimerEventHandler
                    ( watchdog_pi::OnTimer ), NULL, this);
    m_Timer.Start(3000);

    return (WANTS_OVERLAY_CALLBACK |
            WANTS_OPENGL_OVERLAY_CALLBACK |
            WANTS_TOOLBAR_CALLBACK    |
            WANTS_CURSOR_LATLON       |
//            WANTS_PREFERENCES         |
            WANTS_NMEA_SENTENCES |
            WANTS_NMEA_EVENTS         |
            WANTS_AIS_SENTENCES       |
            WANTS_CONFIG);
}

bool watchdog_pi::DeInit(void)
{
    //    Record the dialog position
    if (m_pWatchdogDialog)
    {
        wxPoint p = m_pWatchdogDialog->GetPosition();
        SetWatchdogDialogX(p.x);
        SetWatchdogDialogY(p.y);
        
        if(m_pWatchdogDialog) {
            m_pWatchdogDialog->Close();
            delete m_pWatchdogDialog;
            m_pWatchdogDialog = NULL;
            m_pWatchdogPrefsDialog = NULL;
        }
    }
    SaveConfig();

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

void watchdog_pi::SetColorScheme(PI_ColorScheme cs)
{
    if (NULL == m_pWatchdogDialog)
        return;

    DimeWindow(m_pWatchdogDialog);
}

void watchdog_pi::RearrangeWindow()
{
    if (NULL == m_pWatchdogDialog)
        return;

    SetColorScheme(PI_ColorScheme());
    
    m_pWatchdogDialog->Fit();
}

void watchdog_pi::OnToolbarToolCallback(int id)
{
    if(!m_pWatchdogDialog)
    {
        m_pWatchdogDialog = new WatchdogDialog(*this, GetOCPNCanvasWindow());
        m_pWatchdogDialog->Move(wxPoint(m_watchdog_dialog_x, m_watchdog_dialog_y));

        m_pWatchdogPrefsDialog = new WatchdogPrefsDialog(*this, m_pWatchdogDialog);
    }

    m_pWatchdogDialog->Show(!m_pWatchdogDialog->IsShown());
    m_pWatchdogDialog->UpdateAlarms();

    wxPoint p = m_pWatchdogDialog->GetPosition();
    m_pWatchdogDialog->Move(0, 0);        // workaround for gtk autocentre dialog behavior
    m_pWatchdogDialog->Move(p);
}

void watchdog_pi::OnContextMenuItemCallback(int id)
{
}

#if 0
wxColour watchdog_pi::Color(enum Alarm alarm_mask)
{
    if(m_iAlarm & alarm_mask)
        return *wxRED;
    else if(m_iLastAlarm & alarm_mask)
        return wxColor(200, 200, 0); // darker yellow

    return *wxGREEN;
}
#endif

bool watchdog_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
    ocpnDC odc(dc);
    Render(odc, *vp);
    return true;
}

bool watchdog_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
    glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_HINT_BIT ); //Save state
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

    ocpnDC odc;
    Render(odc, *vp);

    glPopAttrib();
    return true;
}

void watchdog_pi::Render(ocpnDC &dc, PlugIn_ViewPort &vp)
{
    if(!m_pWatchdogDialog || !m_pWatchdogDialog->IsShown())
        return;

    Alarm::RenderAll(dc, vp);
}

void watchdog_pi::OnTimer( wxTimerEvent & )
{
    /* calculate course and speed over ground from gps */
    double dt = m_lastfix.FixTime - m_lasttimerfix.FixTime;
    if(!isnan(m_lastfix.Lat) && !isnan(m_lasttimerfix.Lat) && dt > 0) {
        DistanceBearingMercator_Plugin(m_lastfix.Lat, m_lastfix.Lon,
                                       m_lasttimerfix.Lat, m_lasttimerfix.Lon, &m_cog, &m_sog);
        /* this way helps avoid surge speed from gps from surfing waves etc... */
        m_sog *= 3600.0 / dt;
    } else
        m_sog = m_cog = NAN;
    
    m_lasttimerfix = m_lastfix;

    if(m_pWatchdogDialog)
        m_pWatchdogDialog->Fit();
}

bool watchdog_pi::LoadConfig(void)
{
    wxFileConfig *pConf = GetOCPNConfigObject();

    if(!pConf)
        return false;

    pConf->SetPath ( _T( "/Settings/Watchdog" ) );
    
    m_watchdog_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 20L );
    m_watchdog_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 20L );

    Alarm::ConfigAll(true);
    
    return true;
}

bool watchdog_pi::SaveConfig(void)
{
    wxFileConfig *pConf = GetOCPNConfigObject();

    if(!pConf)
        return false;

    pConf->SetPath ( _T ( "/Settings/Watchdog" ) );

    pConf->Write ( _T ( "DialogPosX" ),   m_watchdog_dialog_x );
    pConf->Write ( _T ( "DialogPosY" ),   m_watchdog_dialog_y );

    Alarm::ConfigAll(false);
    
    return true;
}

void watchdog_pi::SetCursorLatLon(double lat, double lon)
{
    m_cursor_lat = lat;
    m_cursor_lon = lon;
    m_cursor_time = wxDateTime::Now();
}

void watchdog_pi::SetNMEASentence(wxString &sentence)
{
    Alarm::NMEAString(sentence);
}

void watchdog_pi::SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix)
{
    if(pfix.FixTime && pfix.nSats)
        m_LastFixTime = wxDateTime::Now();

    m_lastfix = pfix;
}

void watchdog_pi::ShowPreferencesDialog( wxWindow* )
{
    m_pWatchdogPrefsDialog->Show();
}
