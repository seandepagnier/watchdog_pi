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

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "wx/process.h"

#include "../../../include/ocpndc.h"

#include "WatchdogPrefsDialog.h"
#include "watchdog_pi.h"
#include "WatchdogDialog.h"
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

watchdog_pi::watchdog_pi(void *ppimgr)
    : opencpn_plugin_18(ppimgr),
{
    // Create the PlugIn icons
    initialize_images();
    m_lastfix.Lat = NAN;
    m_lasttimerfix.Lat = NAN;
    sog = cog = NAN;
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
    
    //    Get a pointer to the opencpn display canvas, to use as a parent for the POI Manager dialog
    m_parent_window = GetOCPNCanvasWindow();
    m_pconfig = GetOCPNConfigObject(); //    Get opencpn configuration object
    
    LoadConfig(); //    And load the configuration items
    
    m_leftclick_tool_id  = InsertPlugInTool
        (_T(""), _img_watchdog, _img_watchdog, wxITEM_NORMAL,
         _("Watchdog"), _T(""), NULL, WATCHMAN_TOOL_POSITION, 0, this);
    
    wxMenu dummy_menu;
    m_anchor_menu_id = AddCanvasContextMenuItem
        (new wxMenuItem(&dummy_menu, -1, _("Watchdog Anchor Watch")), this );
    
    m_pWatchdogDialog = NULL;
    m_pWatchdogPrefsDialog = NULL;
    m_Timer.Connect(wxEVT_TIMER, wxTimerEventHandler
                    ( watchdog_pi::OnTimer ), NULL, this);
    m_Timer.Start(3000);

    m_DeaddogUpdateTime = now;

    return (WANTS_OVERLAY_CALLBACK |
            WANTS_OPENGL_OVERLAY_CALLBACK |
            WANTS_TOOLBAR_CALLBACK    |
            WANTS_CURSOR_LATLON       |
            WANTS_PREFERENCES         |
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
        }

        if(m_pWatchdogPrefsDialog) {
            m_pWatchdogPrefsDialog->Close();
            delete m_pWatchdogPrefsDialog;
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
Alarm user of possible dangerous situations. \n\
\n\
The Watchdog plugin was written by Sean D'Epagnier\n\
");
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
        m_pWatchdogDialog = new WatchdogDialog(*this, m_parent_window);
        m_pWatchdogDialog->Move(wxPoint(m_watchdog_dialog_x, m_watchdog_dialog_y));
    }

    m_pWatchdogDialog->Show(!m_pWatchdogDialog->IsShown());
    m_pWatchdogDialog->UpdateAlarms();

    wxPoint p = m_pWatchdogDialog->GetPosition();
    m_pWatchdogDialog->Move(0, 0);        // workaround for gtk autocentre dialog behavior
    m_pWatchdogDialog->Move(p);
}

void watchdog_pi::OnContextMenuItemCallback(int id)
{
    if(id == m_anchor_menu_id) {
        wxFileConfig *pConf = m_pconfig;

        pConf->Write ( _T ( "AnchorAlarm" ), true );
        pConf->Write ( _T ( "AnchorLatitude" ), m_cursor_lat);
        pConf->Write ( _T ( "AnchorLongitude" ), m_cursor_lon);
    }
}

wxColour watchdog_pi::Color(enum Alarm alarm_mask)
{
    if(m_iAlarm & alarm_mask)
        return *wxRED;
    else if(m_iLastAlarm & alarm_mask)
        return wxColor(200, 200, 0); // darker yellow

    return *wxGREEN;
}

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

    wxFileConfig *pConf = m_pconfig;

    if(pConf->Read ( _T ( "LandFallTimeAlarm" ), 0L )) {
    }

    if(pConf->Read ( _T ( "AnchorAlarm" ), 0L)) {
        wxPoint r1, r2;
        double AnchorLatitude, AnchorLongitude;
        pConf->Read ( _T ( "AnchorLatitude" ), &AnchorLatitude, NAN );
        pConf->Read ( _T ( "AnchorLongitude" ), &AnchorLatitude, NAN );

        GetCanvasPixLL(&vp, &r1, AnchorLatitude, AnchorLongitude);
        GetCanvasPixLL(&vp, &r2, AnchorLatitude +
                       pConf->Read ( _T ( "AnchorRadius" ), 50 )/1853.0/60.0,
                       AnchorLongitude);

        dc.SetPen(wxPen(Color(ANCHOR), 2));
        dc.DrawCircle( r1.x, r1.y, r2.y - r1.y );
    }
}

void watchdog_pi::OnTimer( wxTimerEvent & )
{
    wxFileConfig *pConf = m_pconfig;

    wxDateTime now = wxDateTime::Now();

    for(int alarm = 0; alarm < ALARMCOUNT; alarm++)
        Alarms[alarm].Reset();

    /* kind of slow, don't perform as often */
    if((now - m_LastLandFallCheck).GetSeconds() > 10) {
        m_LastLandFallCheck = now;

        if(pConf->Read ( _T ( "LandFallTimeAlarm" ), 0L )) {
            double lat1 = m_lastfix.Lat, lon1 = m_lastfix.Lon, lat2, lon2;
            double dist = 0, dist1 = 1000;
            int count = 0;
            wxTimeSpan LandFallTime;

            double LandFallTimeMinutes;
            pConf->Read ( _T ( "LandFallTimeMinutes" ), &LandFallTimeMinutes, 20 );

            while(count < 10) {
                PositionBearingDistanceMercator_Plugin
                    (m_lastfix.Lat, m_lastfix.Lon, m_lastfix.Cog, dist + dist1, &lat2, &lon2);
                if(PlugIn_GSHHS_CrossesLand(lat1, lon1, lat2, lon2)) {
                if(dist1 < 1) {
                    LandFallTime = wxTimeSpan::Seconds(3600.0 * dist / m_lastfix.Sog);
                    if(LandFallTime.GetMinutes() <= LandFallTimeMinutes)
                        Alarm(LANDFALL);
                    break;
                }
                count = 0;
                dist1 /= 2;
                } else {
                    dist += dist1;
                    lat1 = lat2;
                    lon1 = lon2;
                    count++;
                }
            }
        }

        if(pConf->Read ( _T ( "LandFallDistanceAlarm" ), 0L)) {
            for(double t = 0; t<360; t+=18) {
                double dlat, dlon;
                double LandFallDistance;
                pConf->Read ( _T ( "LandFallDistance" ), &LandFallDistance, 3 );
                PositionBearingDistanceMercator_Plugin(m_lastfix.Lat, m_lastfix.Lon, t,
                                                       LandFallDistance, &dlat, &dlon);
            
                if(PlugIn_GSHHS_CrossesLand(m_lastfix.Lat, m_lastfix.Lon, dlat, dlon)) {
                    Alarm |= LANDFALLDISTANCE;
                    break;
                }
            }
        }
    }

    /* anchor */
    double AnchorLatitude, AnchorLongitude;
    pConf->Read ( _T ( "AnchorLatitude" ), &AnchorLatitude, NAN );
    pConf->Read ( _T ( "AnchorLongitude" ), &AnchorLatitude, NAN );

    double anchordist;
    DistanceBearingMercator_Plugin(m_lastfix.Lat, m_lastfix.Lon,
                                   AnchorLatitude, AnchorLongitude,
                                   0, &anchordist);
    anchordist *= 1853.248; /* in meters */
    long AnchorRadius = pConf->Read ( _T ( "AnchorRadius" ), 50L );
    if(pConf->Read ( _T ( "AnchorAlarm" ), 0L ) && anchordist > AnchorRadius)
        Alarm(ANCHOR);

    double gpsseconds =
        //m_LastPositionFix.IsValid() ?
        //(now - m_LastPositionFix).GetSeconds().ToLong() :
        NAN;

    if(pConf->Read ( _T ( "NMEADataAlarm" ), 0L ) &&
       gpsseconds > pConf->Read ( _T ( "NMEADataSeconds" ), 0L ))
        Alarm(NMEADATA);

    wxTimeSpan DeaddogSpan = wxTimeSpan::Minutes(pConf->Read ( _T ( "DeaddogMinutes" ), 20L ));
    if(pConf->Read ( _T ( "DeaddogAlarm" ), 0L ) &&
       (now - m_DeaddogUpdateTime) > DeaddogSpan)
        Alarm(DEADMAN);

    /* calculate course and speed over ground from gps */
    if(m_LastPositionFix.IsValid()) {
        if(!m_LastTimerFix.IsValid())
            goto setfix;

        if((m_LastPositionFix - m_LastTimerFix).GetSeconds() > 10) {
            if(!isnan(m_lastfix.Lat) && !isnan(m_lasttimerfix.Lat)) {
                DistanceBearingMercator_Plugin(m_lastfix.Lat, m_lastfix.Lon,
                                               m_lasttimerfix.Lat, m_lasttimerfix.Lon, &cog, &sog);
                /* this way helps avoid surge speed from gps from surfing waves etc... */
                sog *= 3600.0 / (m_lastfix.FixTime - m_lasttimerfix.FixTime);
            } else
                sog = cog = NAN;

        setfix:
            m_lasttimerfix = m_lastfix;
            m_LastTimerFix = m_LastPositionFix;
        }
    }

    double OffCourseDegrees = pConf->Read ( _T ( "OffCourseDegrees" ), &d, 20L );
    double courseerror = fabs(heading_resolve(cog - OffCourseDegrees));
    if(pConf->Read ( _T ( "OffCourseAlarm" ), 0L ) &&
       courseerror > OffCourseDegrees)
        Alarm(OFFCOURSE);

    double UnderSpeed;
    pConf->Read ( _T ( "UnderSpeed" ), &UnderSpeed, 1 );
    if(pConf->Read ( _T ( "UnderSpeedAlarm" ), 0L ) && sog < UnderSpeed)
        Alarm(UNDERSPEED);

    double OverSpeed;
    pConf->Read ( _T ( "OverSpeed" ), &OverSpeed, 1 );
    if(pConf->Read ( _T ( "OverSpeedAlarm" ), 0L ) && sog < OverSpeed)
        Alarm(OVERSPEED);

    if(m_pWatchdogDialog)
        m_pWatchdogDialog->Update(anchordist, nmea_seconds, courseerror, sog);
}

void watchdog_pi::ResetDeaddog()
{
    m_DeaddogUpdateTime = wxDateTime::Now();
}

bool watchdog_pi::LoadConfig(void)
{
    wxFileConfig *pConf = m_pconfig;

    if(!pConf)
        return false;

    pConf->SetPath ( _T( "/Settings/Watchdog" ) );
    
    m_watchdog_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 20L );
    m_watchdog_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 20L );
    
    return true;
}

bool watchdog_pi::SaveConfig(void)
{
    wxFileConfig *pConf = m_pconfig;

    if(!pConf)
        return false;

    pConf->SetPath ( _T ( "/Settings/Watchdog" ) );

    pConf->Write ( _T ( "DialogPosX" ),   m_watchdog_dialog_x );
    pConf->Write ( _T ( "DialogPosY" ),   m_watchdog_dialog_y );
    
    return true;
}

void watchdog_pi::RunAlarm(enum AlarmName alarm)
{
    if(pConf->Read ( _T ( "DisableAllAlarms" ), 0L))
        return;

    Alarms[alarm].RunAlarm();
}


void watchdog_pi::SetCursorLatLon(double lat, double lon)
{
    ResetDeaddog();

    m_cursor_lat = lat;
    m_cursor_lon = lon;
}

void watchdog_pi::SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix)
{
    if(pfix.FixTime && pfix.nSats)
        m_LastPositionFix = wxDateTime::Now();

    m_lastfix = pfix;
}

void watchdog_pi::ShowPreferencesDialog( wxWindow* parent )
{
    if(!m_pWatchdogPrefsDialog)
        m_pWatchdogPrefsDialog = new WatchdogPrefsDialog(*this, m_parent_window);

    m_pWatchdogPrefsDialog->Show();
}
