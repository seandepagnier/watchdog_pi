/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  watch dog Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2015 by Sean D'Epagnier                                 *
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

#include <map>

#include <wx/wx.h>
#include "wddc.h"

#include <wx/process.h>

#include "watchdog_pi.h"
#include "WatchdogDialog.h"
#include "wx/jsonreader.h"
#include "wx/jsonwriter.h"

extern wxJSONValue g_ReceivedBoundaryTimeJSONMsg;
extern wxString    g_ReceivedBoundaryTimeMessage;
extern wxJSONValue g_ReceivedBoundaryAnchorJSONMsg;
extern wxString    g_ReceivedBoundaryAnchorMessage;


///////// The Alarm classes /////////
class LandFallAlarm : public Alarm
{
public:
    LandFallAlarm() : Alarm(5 /* seconds */), m_bFiredTime(false) {}

    wxString Name() { return _("LandFall"); }
    bool Test() {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();

        if(isnan(lastfix.Lat))
            return false;

        double lat1 = lastfix.Lat, lon1 = lastfix.Lon, lat2, lon2;
        double dist = 0, dist1 = 1000;
        int count = 0;
                
        wxFileConfig *pConf = GetConfigObject();
        m_crossinglat1 = m_crossinglon1 = NAN;
        m_LandFallTime = wxTimeSpan();

        if(pConf->Read ( _T ( "TimeAlarm" ), 1L)) {
            double LandFallTimeMinutes;
            pConf->Read ( _T ( "Minutes" ), &LandFallTimeMinutes, 20 );

            m_bFiredTime = false;
            while(count < 10) {
                PositionBearingDistanceMercator_Plugin
                    (lastfix.Lat, lastfix.Lon, lastfix.Cog, dist + dist1, &lat2, &lon2);
                if(PlugIn_GSHHS_CrossesLand(lat1, lon1, lat2, lon2)) {
                    if(dist1 < 1) {
                        m_LandFallTime = wxTimeSpan::Seconds(3600.0 * dist / lastfix.Sog);
                        m_crossinglat1 = lat1, m_crossinglon1 = lon1;
                        m_crossinglat2 = lat2, m_crossinglon2 = lon2;
                        if(m_LandFallTime.GetMinutes() <= LandFallTimeMinutes)
                            m_bFiredTime = true;
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
  
        if(pConf->Read ( _T ( "DistanceAlarm" ), 0L)) {
            for(double t = 0; t<360; t+=9) {
                double dlat, dlon;
                double LandFallDistance;
                pConf->Read ( _T ( "Distance" ), &LandFallDistance, 3 );
                PositionBearingDistanceMercator_Plugin(lastfix.Lat, lastfix.Lon, t,
                                                       LandFallDistance, &dlat, &dlon);
            
                if(PlugIn_GSHHS_CrossesLand(lastfix.Lat, lastfix.Lon, dlat, dlon)) {
                    m_crossinglat1 = dlat, m_crossinglon1 = dlon;
                    m_crossinglat2 = dlat, m_crossinglon2 = dlon;
                    return true;
                }
            }
        }

        return m_bFiredTime;
    }

    wxString GetStatus() {
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;

        wxString s, fmt(_T(" %d "));
        wxFileConfig *pConf = GetConfigObject();

        if(pConf->Read ( _T ( "TimeAlarm" ), 1L)) {
            if(m_LandFallTime.IsNull())
                s = _("LandFall not Detected");
            else {
                int days = m_LandFallTime.GetDays();
                if(days > 1)
                    s = wxString::Format(fmt + _("Days"), days);
                else {
                    if(days)
                        s = wxString::Format(fmt + _("Day"), days);
                    
                    int hours = m_LandFallTime.GetHours();
                    if(hours > 1)
                        s += wxString::Format(fmt + _("Hours"), hours);
                    else {
                        if(hours)
                            s += wxString::Format(fmt + _("Hour"), hours);

                        int minutes = m_LandFallTime.GetMinutes() - 60*hours;
                        if(minutes > 1)
                            s += wxString::Format(fmt + _("Minutes"), minutes);
                        else {
                            if(minutes)
                                s += wxString::Format(fmt + _("Minute"), minutes);
                            
                            int seconds = m_LandFallTime.GetSeconds().ToLong() - 60*minutes;
                            if(seconds > 1)
                                s += wxString::Format(fmt + _("Seconds"), seconds);
                            else
                                s += wxString::Format(fmt + _("Second"), seconds);
                        }
                    }
                }
            } 
            if(m_bFired && m_bFiredTime)
                dlg.m_stLandFallTime->SetForegroundColour(*wxRED);
            else
                dlg.m_stLandFallTime->SetForegroundColour(*wxBLACK);

            dlg.m_stLandFallTime->SetLabel(s);
        }
        
        if(pConf->Read ( _T ( "DistanceAlarm" ), 0L)) {
            double LandFallDistance;
            pConf->Read ( _T ( "Distance" ), &LandFallDistance, 3 );
            s = wxString::Format(_T(" ") + wxString(_("Distance")) +
                                 (m_bFired ? _T(" <") : _T(" >")) +
                                 _T(" %.2f nm"), LandFallDistance);
            
            if(m_bFired && !m_bFiredTime)
                dlg.m_stLandFallDistance->SetForegroundColour(*wxRED);
            else
                dlg.m_stLandFallDistance->SetForegroundColour(*wxBLACK);

            dlg.m_stLandFallDistance->SetLabel(s);
        }

        return _T("");
    }

    void Repopulate() {
        wxFileConfig *pConf = GetConfigObject();

        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;
        wxFlexGridSizer &sizer = *dlg.m_fgAlarms;

        if(pConf->Read ( _T ( "TimeAlarm" ), 1L) && m_bEnabled) {
            sizer.Add(dlg.m_stTextLandFallTime, 0, wxALL, 5);
            sizer.Add(dlg.m_stLandFallTime, 0, wxALL, 5);

            dlg.m_stTextLandFallTime->Show();
            dlg.m_stLandFallTime->Show();
        } else {
            dlg.m_stTextLandFallTime->Hide();
            dlg.m_stLandFallTime->Hide();
        }

        if(pConf->Read ( _T ( "DistanceAlarm" ), 0L) && m_bEnabled) {
            sizer.Add(dlg.m_stTextLandFallDistance, 0, wxALL, 5);
            sizer.Add(dlg.m_stLandFallDistance, 0, wxALL, 5);

            dlg.m_stTextLandFallDistance->Show();
            dlg.m_stLandFallDistance->Show();
        } else {
            dlg.m_stTextLandFallDistance->Hide();
            dlg.m_stLandFallDistance->Hide();
        }
    }

    void Render(wdDC &dc, PlugIn_ViewPort &vp) {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();
        if(isnan(m_crossinglat1))
            return;

        wxPoint r1, r2, r3, r4;
        GetCanvasPixLL(&vp, &r1, lastfix.Lat, lastfix.Lon);
        GetCanvasPixLL(&vp, &r2, m_crossinglat1, m_crossinglon1);
        GetCanvasPixLL(&vp, &r3, m_crossinglat2, m_crossinglon2);
        r4.x = (r2.x+r3.x)/2, r4.y = (r2.y+r3.y)/2;
        
        dc.SetPen(wxPen(wxColour(255, 255, 0), 2));
        dc.DrawLine( r1.x, r1.y, r4.x, r4.y );
        
        if(m_bFired)
            dc.SetPen(wxPen(*wxRED, 3));
        else
            dc.SetPen(wxPen(*wxGREEN, 3));

        dc.DrawCircle( r4.x, r4.y, hypot(r2.x-r3.x, r2.y-r3.y) / 2 );
    }

private:
    double m_crossinglat1, m_crossinglon1;
    double m_crossinglat2, m_crossinglon2;

    wxTimeSpan m_LandFallTime;

    bool m_bFiredTime;
} g_LandfallAlarm;

class NMEADataAlarm : public Alarm
{
public:
    NMEADataAlarm() { start = wxDateTime::Now(); }

    wxString Name() { return _("NMEA Data"); }
    void GetStatusControls(wxControl *&Text, wxControl *&status) {
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;
        Text = dlg.m_stTextNMEAData;
        status = dlg.m_stNMEAData;
    }

    bool Test() {
        wxFileConfig *pConf = GetConfigObject();
        return ElapsedSeconds() > pConf->Read ( _T ( "Seconds" ), 0L );
    }

    wxString GetStatus() {
        int seconds = ElapsedSeconds();
        wxString s;
        if(isnan(seconds))
            s = _T("N/A");
        else {
            wxString fmt(_T("%d "));
            s = wxString::Format(fmt + _("second(s)"), seconds);
        }

        return s;
    }

    void NMEAString(const wxString &string) {
        wxString name = string.BeforeFirst(',');
        NMEAStringTimes[name] = wxDateTime::Now();
    }

private:
    wxDateTime start;
    std::map<wxString, wxDateTime> NMEAStringTimes;

    int ElapsedSeconds() {
        wxString sentences;
        wxFileConfig *pConf = GetConfigObject();
        pConf->Read(_T ( "Sentences" ), &sentences, _T(""));

        wxDateTime now = wxDateTime::Now(), time = now;
        /* take oldest message time */
        for(;;) {
            wxString cur = sentences.BeforeFirst('\n');
            cur = cur.BeforeFirst(' '); /* remove trailing spaces */

            if(cur.size()) {
                wxDateTime dt = NMEAStringTimes[cur];
                if(!dt.IsValid())
                    dt = start;
                if(dt < time)
                    time = dt;
            }

            if((int)sentences.find('\n') == wxNOT_FOUND)
                break;

            sentences = sentences.AfterFirst('\n');
        }

        return (now - time).GetSeconds().ToLong();
    }
} g_NMEADataAlarm;

class DeadmanAlarm : public Alarm
{
public:
    DeadmanAlarm() {}

    wxString Name() { return _("Deadman"); }
    void GetStatusControls(wxControl *&Text, wxControl *&status) {
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;
        Text = dlg.m_stTextDeadman;
        status = dlg.m_stDeadman;
    }

    bool Test() {
        wxFileConfig *pConf = GetConfigObject();

        wxTimeSpan DeadmanSpan = wxTimeSpan::Minutes(pConf->Read ( _T ( "Minutes" ), 20L ));
        return wxDateTime::Now() - g_watchdog_pi->m_cursor_time > DeadmanSpan;
    }

    wxString GetStatus() {
        wxTimeSpan span = wxDateTime::Now() - g_watchdog_pi->m_cursor_time;
        int days = span.GetDays();
        span -= wxTimeSpan::Days(days);
        int hours = span.GetHours();
        span -= wxTimeSpan::Hours(hours);
        int minutes = span.GetMinutes();
        span -= wxTimeSpan::Minutes(minutes);
        int seconds = span.GetSeconds().ToLong();
        wxString d, fmt(_T("%d "));
        if(days)
            d = wxString::Format(fmt + _T("days "), days);
        return d + wxString::Format(_T("%02d:%02d:%02d"),
                                    hours, minutes, seconds);
    }
} g_DeadmanAlarm;

class SecondDeadmanAlarm : public DeadmanAlarm
{
public:
    SecondDeadmanAlarm() {}

    wxString Name() { return _("Second Deadman"); }
    void GetStatusControls(wxControl *&Text, wxControl *&status) {
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;
        Text = dlg.m_stTextSecondDeadman;
        status = dlg.m_stSecondDeadman;
    }
} g_SecondDeadmanAlarm;

class AnchorAlarm : public Alarm
{
public:
    AnchorAlarm() { minoldfix.FixTime = 0; }

    wxString Name() { return _("Anchor"); }
    void GetStatusControls(wxControl *&Text, wxControl *&status) {
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;
        Text = dlg.m_stTextAnchor;
        status = dlg.m_stAnchorDistance;
    }

    bool Test() {
        wxFileConfig *pConf = GetConfigObject();

        long AnchorRadius = pConf->Read ( _T ( "Radius" ), 50L );

        if( pConf->Read ( _T ( "AutoSync" ), 0L ) ) {
            PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();
            if(lastfix.FixTime - minoldfix.FixTime >= 60) {
                if(minoldfix.FixTime) {
                    double dist;
                    DistanceBearingMercator_Plugin(lastfix.Lat, lastfix.Lon,
                                                   minoldfix.Lat, minoldfix.Lon,
                                       0, &dist);
                    dist *= 1853.248; /* in meters */

                    if(dist < AnchorRadius) {
                        pConf->Write(_T("Latitude"), lastfix.Lat);
                        pConf->Write(_T("Longitude"), lastfix.Lon);
                        g_watchdog_pi->UpdatePreferences();
                    }
                }
                minoldfix = lastfix;
            }
        }

        double anchordist = Distance();

        return anchordist > AnchorRadius;
    }

    wxString GetStatus() {
        double anchordist = Distance();
        wxString s;
        if(isnan(anchordist))
            s = _T("N/A");
        else {
            wxString fmt(_T("%.0f "));
            s = wxString::Format(fmt + _("meter(s)"), anchordist);
        }

        return s;
    }

    void Render(wdDC &dc, PlugIn_ViewPort &vp) {
        wxFileConfig *pConf = GetConfigObject();
        
        wxPoint r1, r2;
        double AnchorLatitude, AnchorLongitude;
        pConf->Read ( _T ( "Latitude" ), &AnchorLatitude, NAN );
        pConf->Read ( _T ( "Longitude" ), &AnchorLongitude, NAN );
        
        GetCanvasPixLL(&vp, &r1, AnchorLatitude, AnchorLongitude);
        GetCanvasPixLL(&vp, &r2, AnchorLatitude +
                       pConf->Read ( _T ( "Radius" ), 50 )/1853.0/60.0,
                       AnchorLongitude);
        
        if(m_bEnabled) {
            if(m_bFired)
                dc.SetPen(wxPen(*wxRED, 2));
            else
                dc.SetPen(wxPen(*wxGREEN, 2));
        } else
            dc.SetPen(wxPen(wxColour(128, 192, 0, 128), 2, wxLONG_DASH));
         
        dc.DrawCircle( r1.x, r1.y, hypot(r1.x-r2.x, r1.y-r2.y) );
    }

private:
    double Distance() {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();

        wxFileConfig *pConf = GetConfigObject();
        double AnchorLatitude, AnchorLongitude;
        pConf->Read ( _T ( "Latitude" ), &AnchorLatitude, NAN );
        pConf->Read ( _T ( "Longitude" ), &AnchorLongitude, NAN );

        double anchordist;
        DistanceBearingMercator_Plugin(lastfix.Lat, lastfix.Lon,
                                       AnchorLatitude, AnchorLongitude,
                                       0, &anchordist);
        anchordist *= 1853.248; /* in meters */
        return anchordist;
    }

    PlugIn_Position_Fix_Ex minoldfix;

} g_AnchorAlarm;

class BoundaryAlarm : public Alarm
{
public:
    BoundaryAlarm() : Alarm(5 /* seconds */), m_bFiredTime(false) {}
    
    wxString Name() { return _("Boundary"); }
    bool Test() {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();
        
        if(isnan(lastfix.Lat))
            return false;
        
        double lat1 = lastfix.Lat, lon1 = lastfix.Lon, lat2, lon2;
        double dist = 0, dist1 = 1000;
        int count = 0;
        
        wxFileConfig *pConf = GetConfigObject();
        m_crossinglat1 = m_crossinglon1 = NAN;
        m_BoundaryTime = wxTimeSpan();
        
        if(pConf->Read ( _T ( "TimeAlarm" ), 1L)) {
            double BoundaryTimeMinutes;
            pConf->Read ( _T ( "Minutes" ), &BoundaryTimeMinutes, 20 );
            
            m_bFiredTime = false;
            while(count < 10) {
                PositionBearingDistanceMercator_Plugin
                (lastfix.Lat, lastfix.Lon, lastfix.Cog, dist + dist1, &lat2, &lon2);
                // Do JSON message to OD Plugin to check if boundary m_crossinglat
                wxJSONValue jMsg;
                wxJSONWriter writer;
                wxString    MsgString;
                jMsg[wxS("Source")] = wxS("WATCHDOG_PI");
                jMsg[wxS("Type")] = wxS("FindPointInAnyBoundary");
                jMsg[wxS("MsgId")] = wxS("time");
                jMsg[wxS("lat")] = lat2;
                jMsg[wxS("lon")] = lon2;
                writer.Write( jMsg, MsgString );
                SendPluginMessage( wxS("BOUNDARY_CHECK"), MsgString );
                if(g_ReceivedBoundaryTimeMessage != wxEmptyString && g_ReceivedBoundaryTimeJSONMsg[wxS("MsgId")].AsString() == wxS("time") && g_ReceivedBoundaryTimeJSONMsg[wxS("Found")].AsBool() == true ) {
                    // This is our message
                    if(dist1 < 1) {
                        m_BoundaryTime = wxTimeSpan::Seconds(3600.0 * dist / lastfix.Sog);
                        m_crossinglat1 = lat1, m_crossinglon1 = lon1;
                        m_crossinglat2 = lat2, m_crossinglon2 = lon2;
                        if(m_BoundaryTime.GetMinutes() <= BoundaryTimeMinutes)
                            m_bFiredTime = true;
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
                g_ReceivedBoundaryTimeMessage = wxEmptyString;
                g_ReceivedBoundaryTimeJSONMsg.Clear();
            }
        }
        
        if(pConf->Read ( _T ( "DistanceAlarm" ), 0L)) {
            for(double t = 0; t<360; t+=9) {
                double dlat, dlon;
                double BoundaryDistance;
                pConf->Read ( _T ( "Distance" ), &BoundaryDistance, 3 );
                PositionBearingDistanceMercator_Plugin(lastfix.Lat, lastfix.Lon, t,
                                                       BoundaryDistance, &dlat, &dlon);
                
                wxJSONValue jMsg;
                jMsg[wxS("Type")] = wxS("FindPointInAnyBoundary");
                jMsg[wxS("id")] = wxS("time");
                jMsg[wxS("lat")] = lat2;
                jMsg[wxS("lon")] = lon2;
                SendPluginMessage( wxS("OD_BOUNDARY_CHECK"), jMsg.AsString() );
                if(PlugIn_GSHHS_CrossesLand(lastfix.Lat, lastfix.Lon, dlat, dlon)) {
                    m_crossinglat1 = dlat, m_crossinglon1 = dlon;
                    m_crossinglat2 = dlat, m_crossinglon2 = dlon;
                    return true;
                }
            }
        }
        
        return m_bFiredTime;
    }
    
    wxString GetStatus() {
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;
        
        wxString s, fmt(_T(" %d "));
        wxFileConfig *pConf = GetConfigObject();
        
        if(pConf->Read ( _T ( "TimeAlarm" ), 1L)) {
            if(m_BoundaryTime.IsNull())
                s = _("Boundary not Detected");
            else {
                int days = m_BoundaryTime.GetDays();
                if(days > 1)
                    s = wxString::Format(fmt + _("Days"), days);
                else {
                    if(days)
                        s = wxString::Format(fmt + _("Day"), days);
                    
                    int hours = m_BoundaryTime.GetHours();
                    if(hours > 1)
                        s += wxString::Format(fmt + _("Hours"), hours);
                    else {
                        if(hours)
                            s += wxString::Format(fmt + _("Hour"), hours);
                        
                        int minutes = m_BoundaryTime.GetMinutes() - 60*hours;
                        if(minutes > 1)
                            s += wxString::Format(fmt + _("Minutes"), minutes);
                        else {
                            if(minutes)
                                s += wxString::Format(fmt + _("Minute"), minutes);
                            
                            int seconds = m_BoundaryTime.GetSeconds().ToLong() - 60*minutes;
                            if(seconds > 1)
                                s += wxString::Format(fmt + _("Seconds"), seconds);
                            else
                                s += wxString::Format(fmt + _("Second"), seconds);
                        }
                    }
                }
            } 
            if(m_bFired && m_bFiredTime)
                dlg.m_stBoundaryTime->SetForegroundColour(*wxRED);
            else
                dlg.m_stBoundaryTime->SetForegroundColour(*wxBLACK);
            
            dlg.m_stLandFallTime->SetLabel(s);
        }
        
        if(pConf->Read ( _T ( "DistanceAlarm" ), 0L)) {
            double BoundaryDistance;
            pConf->Read ( _T ( "Distance" ), &BoundaryDistance, 3 );
            s = wxString::Format(_T(" ") + wxString(_("Distance")) +
            (m_bFired ? _T(" <") : _T(" >")) +
            _T(" %.2f nm"), BoundaryDistance);
            
            if(m_bFired && !m_bFiredTime)
                dlg.m_stBoundaryDistance->SetForegroundColour(*wxRED);
            else
                dlg.m_stBoundaryDistance->SetForegroundColour(*wxBLACK);
            
            dlg.m_stBoundaryDistance->SetLabel(s);
        }
        
        return _T("");
    }
    
    void Repopulate() {
        wxFileConfig *pConf = GetConfigObject();
        
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;
        wxFlexGridSizer &sizer = *dlg.m_fgAlarms;
        
        if(pConf->Read ( _T ( "TimeAlarm" ), 1L) && m_bEnabled) {
            sizer.Add(dlg.m_stTextBoundaryTime, 0, wxALL, 5);
            sizer.Add(dlg.m_stBoundaryTime, 0, wxALL, 5);
            
            dlg.m_stTextBoundaryTime->Show();
            dlg.m_stBoundaryTime->Show();
        } else {
            dlg.m_stTextBoundaryTime->Hide();
            dlg.m_stBoundaryTime->Hide();
        }
        
        if(pConf->Read ( _T ( "DistanceAlarm" ), 0L) && m_bEnabled) {
            sizer.Add(dlg.m_stTextBoundaryDistance, 0, wxALL, 5);
            sizer.Add(dlg.m_stBoundaryDistance, 0, wxALL, 5);
            
            dlg.m_stTextBoundaryDistance->Show();
            dlg.m_stBoundaryDistance->Show();
        } else {
            dlg.m_stTextBoundaryDistance->Hide();
            dlg.m_stBoundaryDistance->Hide();
        }
    }
    
    void Render(wdDC &dc, PlugIn_ViewPort &vp) {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();
        if(isnan(m_crossinglat1))
            return;
        
        wxPoint r1, r2, r3, r4;
        GetCanvasPixLL(&vp, &r1, lastfix.Lat, lastfix.Lon);
        GetCanvasPixLL(&vp, &r2, m_crossinglat1, m_crossinglon1);
        GetCanvasPixLL(&vp, &r3, m_crossinglat2, m_crossinglon2);
        r4.x = (r2.x+r3.x)/2, r4.y = (r2.y+r3.y)/2;
        
        dc.SetPen(wxPen(wxColour(255, 255, 0), 2));
        dc.DrawLine( r1.x, r1.y, r4.x, r4.y );
        
        if(m_bFired)
            dc.SetPen(wxPen(*wxRED, 3));
        else
            dc.SetPen(wxPen(*wxGREEN, 3));
        
        dc.DrawCircle( r4.x, r4.y, hypot(r2.x-r3.x, r2.y-r3.y) / 2 );
    }
    
private:
    double m_crossinglat1, m_crossinglon1;
    double m_crossinglat2, m_crossinglon2;
    
    wxTimeSpan m_BoundaryTime;
    
    bool m_bFiredTime;
} g_BoundaryAlarm;

class CourseAlarm : public Alarm
{
public:
    CourseAlarm() : m_Mode(BOTH) {}

    wxString Name() { return _("Off Course") +
            (m_Mode == PORT ? wxString(_T(" ")) + _("Port") : _T("")); }

    wxString ConfigName() { return _("Off Course"); }

    void SetPort(bool port) {
        m_Mode = port ? PORT : BOTH;
    }

    void GetStatusControls(wxControl *&Text, wxControl *&status) {
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;
        Text = dlg.m_stTextCourseError;
        status = dlg.m_stCourseError;
    }

    bool Test() {
        wxFileConfig *pConf = GetConfigObject();
        double Tolerance;
        pConf->Read ( _T ( "Tolerance" ), &Tolerance, 20L );

        return CourseError() > Tolerance;
    }

    wxString GetStatus() {
        double courseerror = CourseError();
        wxString s;
        if(isnan(courseerror))
            s = _T("N/A");
        else {
            wxString fmt(_T("%.0f "));
            s = wxString::Format(fmt + _("degrees(s)"), courseerror);
        }

        if(m_Mode == PORT)
            s += _T(" Port");

        return s;
    }

    void Render(wdDC &dc, PlugIn_ViewPort &vp) {
        wxFileConfig *pConf = GetConfigObject();

        double Tolerance;
        pConf->Read ( _T ( "Tolerance" ), &Tolerance, 20L );
        double Course;
        pConf->Read ( _T ( "Course" ), &Course, 20L );

        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();

        double lat1 = lastfix.Lat, lon1 = lastfix.Lon, lat2, lon2, lat3, lon3;
        double dist = lastfix.Sog;

        if(isnan(dist))
            return;

        PositionBearingDistanceMercator_Plugin(lat1, lon1, Course+Tolerance,
                                               dist, &lat2, &lon2);
        PositionBearingDistanceMercator_Plugin(lat1, lon1, Course-Tolerance,
                                               dist, &lat3, &lon3);
        wxPoint r1, r2, r3;
        GetCanvasPixLL(&vp, &r1, lat1, lon1);
        GetCanvasPixLL(&vp, &r2, lat2, lon2);
        GetCanvasPixLL(&vp, &r3, lat3, lon3);

        if(m_bFired)
            dc.SetPen(wxPen(*wxRED, 2));
        else
            dc.SetPen(wxPen(*wxGREEN, 2));

        dc.DrawLine( r1.x, r1.y, r2.x, r2.y );
        dc.DrawLine( r1.x, r1.y, r3.x, r3.y );
    }

private:
    double CourseError() {
        wxFileConfig *pConf = GetConfigObject();

        double Course;
        pConf->Read ( _T ( "Course" ), &Course, 0L );

        double error = heading_resolve(g_watchdog_pi->m_cog - Course);

        if(m_Mode == PORT)
            return -error;

        return fabs(error);
    }
    
    enum Mode { PORT, BOTH } m_Mode;
} g_CourseAlarm;

class CourseStarboardAlarm : public Alarm
{
public:
    CourseStarboardAlarm() : Alarm() {}

    wxString Name() { return _("Off Course") + wxString(_T(" ")) + _("Starboard"); }
    void GetStatusControls(wxControl *&Text, wxControl *&status) {
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;
        Text = dlg.m_stTextStarboardCourseError;
        status = dlg.m_stStarboardCourseError;
    }

    bool Test() {
        wxFileConfig *pConf = g_CourseAlarm.GetConfigObject();
        double Tolerance;
        pConf->Read ( _T ( "Tolerance" ), &Tolerance, 20L );

        return CourseError() > Tolerance;
    }

    wxString GetStatus() {
        double courseerror = CourseError();
        wxString s;
        if(isnan(courseerror))
            s = _T("N/A");
        else {
            wxString fmt(_T("%.0f "));
            s = wxString::Format(fmt + _("degrees(s) Starboard"), courseerror);
        }

        return s;
    }

private:
    double CourseError() {
        wxFileConfig *pConf = g_CourseAlarm.GetConfigObject();

        double Course;
        pConf->Read ( _T ( "Course" ), &Course, 0L );

        return heading_resolve(g_watchdog_pi->m_cog - Course);
    }
    
} g_CourseStarboardAlarm;

class SpeedAlarm : public Alarm
{
public:
    SpeedAlarm() {}

    double Knots() {
        wxFileConfig *pConf = GetConfigObject();

        double knots;
        pConf->Read ( _T ( "Knots" ), &knots, 0L );
        return knots;
    }

    wxString GetStatus() {
        wxString s;
        if(isnan(g_watchdog_pi->m_sog))
            s = _T("N/A");
        else {
            wxString fmt(_T("%.1f"));
            double knots = Knots();
            s = wxString::Format(fmt + (g_watchdog_pi->m_sog < knots ?
                                        _T(" < ") : _T(" > "))
                                 + fmt, g_watchdog_pi->m_sog, knots);
        }

        return s;
    }

    void Render(wdDC &dc, PlugIn_ViewPort &vp) {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();

        double knots = Knots();

        double lat1 = lastfix.Lat, lon1 = lastfix.Lon, lat2, lon2;
        PositionBearingDistanceMercator_Plugin(lat1, lon1, 0, knots, &lat2, &lon2);

        wxPoint r1, r2;
        GetCanvasPixLL(&vp, &r1, lat1, lon1);
        GetCanvasPixLL(&vp, &r2, lat2, lon2);

        if(m_bFired)
            dc.SetPen(wxPen(*wxRED, 2));
        else
            dc.SetPen(wxPen(*wxBLUE, 2));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawCircle( r1.x, r1.y, hypot(r1.x-r2.x, r1.y-r2.y) );
    }
};

class UnderSpeedAlarm : public SpeedAlarm
{
public:
    UnderSpeedAlarm() {}

    wxString Name() { return _("Under Speed"); }
    void GetStatusControls(wxControl *&Text, wxControl *&status) {
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;
        Text = dlg.m_stTextUnderSpeed;
        status = dlg.m_stUnderSpeed;
    }

    bool Test() {
        return g_watchdog_pi->m_sog < Knots();
    }
} g_UnderSpeedAlarm;

class OverSpeedAlarm : public SpeedAlarm
{
public:
    OverSpeedAlarm() {}

    wxString Name() { return _("Over Speed"); }
    void GetStatusControls(wxControl *&Text, wxControl *&status) {
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;
        Text = dlg.m_stTextOverSpeed;
        status = dlg.m_stOverSpeed;
    }

    bool Test() {
        return g_watchdog_pi->m_sog > Knots();
    }
} g_OverSpeedAlarm;

////////// Alarm Base Class /////////////////

Alarm *Alarms[] = {&g_LandfallAlarm, &g_NMEADataAlarm, &g_DeadmanAlarm, &g_SecondDeadmanAlarm,
                   &g_AnchorAlarm, &g_BoundaryAlarm, &g_CourseAlarm, &g_CourseStarboardAlarm,
                   &g_UnderSpeedAlarm, &g_OverSpeedAlarm, 0, 0, 0};

void Alarm::RenderAll(wdDC &dc, PlugIn_ViewPort &vp)
{
    for(Alarm **alarm = Alarms; *alarm; alarm++)
        if((*alarm)->m_bgfxEnabled)
            (*alarm)->Render(dc, vp);
}

void Alarm::ConfigAll(bool load)
{
    for(Alarm **alarm = Alarms; *alarm; alarm++)
        if(load)
            (*alarm)->LoadConfig();
        else
            (*alarm)->SaveConfig();
}

void Alarm::ResetAll()
{
    for(Alarm **alarm = Alarms; *alarm; alarm++)
        (*alarm)->m_bFired = false;
}

void Alarm::UpdateStatusAll()
{
    for(Alarm **alarm = Alarms; *alarm; alarm++)
        (*alarm)->UpdateStatus();
}

void Alarm::RepopulateAll()
{
    for(Alarm **alarm = Alarms; *alarm; alarm++)
        (*alarm)->Repopulate();
}

void Alarm::NMEAString(const wxString &string)
{
    g_NMEADataAlarm.NMEAString(string);
}

void Alarm::ConfigSecondDeadman(bool read, wxCheckBox *control)
{
    g_DeadmanAlarm.ConfigItem(read, _T ( "SecondDeadman" ), control);

    if(control->GetValue())
       g_SecondDeadmanAlarm.m_bEnabled = g_DeadmanAlarm.m_bEnabled;
    else
        g_SecondDeadmanAlarm.m_bEnabled = false;
}

void Alarm::ConfigCoursePort(bool read, wxCheckBox *control)
{
    g_CourseAlarm.ConfigItem(read, _T ( "SeparatePortAndStarboard" ), control);
    g_CourseAlarm.SetPort(control->GetValue());

    if(control->GetValue())
       g_CourseStarboardAlarm.m_bEnabled = g_CourseAlarm.m_bEnabled;
    else
        g_CourseStarboardAlarm.m_bEnabled = false;
}

Alarm::Alarm(int interval)
    : m_interval(interval)
{
}

void Alarm::Run()
{
    if(m_bSound)
        PlugInPlaySound(m_sSound);

    if(m_bCommand)
        if(!wxProcess::Open(m_sCommand)) {
            wxMessageDialog mdlg(GetOCPNCanvasWindow(),
                                 Name() + _T(" ") +
                                 _("Failed to execute command: ") + m_sCommand,
                                 _("Watchdog"), wxOK | wxICON_ERROR);
            mdlg.ShowModal();
            m_bCommand = false;
        }

    if(m_bMessageBox) {
        wxMessageDialog mdlg(GetOCPNCanvasWindow(), Name() + _T(" ") + _("ALARM!"),
                             _("Watchman"), wxOK | wxICON_WARNING);
        mdlg.ShowModal();
    }
}

void Alarm::SaveConfig()
{
    wxFileConfig *pConf = GetConfigObject();

    pConf->Write ( _T ( "Enabled" ), m_bEnabled);
    pConf->Write ( _T ( "gfxEnabled" ), m_bgfxEnabled);
    pConf->Write ( _T ( "SoundEnabled" ), m_bSound);
    pConf->Write ( _T ( "SoundFilepath" ), m_sSound);
    pConf->Write ( _T ( "CommandEnabled" ), m_bCommand);
    pConf->Write ( _T ( "CommandFilepath" ), m_sCommand);
    pConf->Write ( _T ( "MessageBox" ), m_bMessageBox);
    pConf->Write ( _T ( "Repeat" ), m_bRepeat);
    pConf->Write ( _T ( "RepeatSeconds" ), m_iRepeatSeconds);
    pConf->Write ( _T ( "AutoReset" ), m_bAutoReset);
}

void Alarm::LoadConfig()
{  
    wxFileConfig *pConf = GetConfigObject();

    pConf->Read ( _T ( "Enabled" ), &m_bEnabled, 0 );
    pConf->Read ( _T ( "gfxEnabled" ), &m_bgfxEnabled, 0 );
    pConf->Read ( _T ( "SoundEnabled" ), &m_bSound, 1 );
    pConf->Read ( _T ( "SoundFilepath" ), &m_sSound, _T("") );
    pConf->Read ( _T ( "CommandEnabled" ), &m_bCommand, 0 );
    pConf->Read ( _T ( "CommandFilepath" ), &m_sCommand, _T("") );
    pConf->Read ( _T ( "MessageBox" ), &m_bMessageBox, 0);
    pConf->Read ( _T ( "Repeat" ), &m_bRepeat, 0);
    pConf->Read ( _T ( "RepeatSeconds" ), &m_iRepeatSeconds, 60);
    pConf->Read ( _T ( "AutoReset" ), &m_bAutoReset, 1);

    m_Timer.Connect(wxEVT_TIMER, wxTimerEventHandler( Alarm::OnTimer ), NULL, this);
    m_Timer.Start(m_interval * 1000, wxTIMER_CONTINUOUS);
}

void Alarm::ConfigItem(bool read, wxString name, wxControl *control)
{
    wxFileConfig *pConf = GetConfigObject();

    wxCheckBox *cb = dynamic_cast<wxCheckBox*>(control);
    if(cb) {
        if(read)
            cb->SetValue(pConf->Read(name, (long)cb->GetValue()));
        else
            pConf->Write(name, cb->GetValue());
        return;
    }

    wxSpinCtrl *s = dynamic_cast<wxSpinCtrl*>(control);
    if(s) {
        if(read)
            s->SetValue(pConf->Read(name, (long)s->GetValue()));
        else
            pConf->Write(name, s->GetValue());
        return;
    }

    wxSlider *sp = dynamic_cast<wxSlider*>(control);
    if(sp) {
        if(read)
            sp->SetValue(pConf->Read(name, (long)sp->GetValue()));
        else
            pConf->Write(name, sp->GetValue());
        return;
    }

    wxTextCtrl *tc = dynamic_cast<wxTextCtrl*>(control);
    if(tc) {
        if(read) {
            wxString str;
            pConf->Read(name, &str, tc->GetValue());
            tc->SetValue(str);
        } else
            pConf->Write(name, tc->GetValue());
        return;
    }

    wxLogMessage(_T("Unrecognized control in Alarm::ConfigItem"));
}

void Alarm::OnTimer( wxTimerEvent & )
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/Settings/Watchdog" ) );
    int enabled = pConf->Read ( _T ( "Enabled" ), 0L );

    if(enabled == 2 && !g_watchdog_pi->m_pWatchdogDialog)
        enabled = 0;

    if(enabled == 3 && !g_watchdog_pi->m_pWatchdogDialog->IsShown())
       enabled = 0;

    if(enabled && m_bEnabled) {
        if(Test()) {        
            wxDateTime now = wxDateTime::Now();
            if(m_bFired) {
                if((now - m_LastAlarmTime).GetSeconds() > m_iRepeatSeconds && m_bRepeat) {
                    Run();
                    m_LastAlarmTime = now;
                }
            } else {
                m_bFired = true;
                Run();
                m_LastAlarmTime = now;
            }
        } else if(m_bAutoReset)
            m_bFired = false;
    }

    if(g_watchdog_pi->m_pWatchdogDialog && g_watchdog_pi->m_pWatchdogDialog->IsShown())
        UpdateStatus();
}

void Alarm::UpdateStatus()
{
    wxControl *text, *status;
    GetStatusControls(text, status);

    if(!status || !m_bEnabled)
        return;

    wxString s = GetStatus();
    if(m_bFired)
        status->SetForegroundColour(*wxRED);
    else
        status->SetForegroundColour(*wxBLACK);
            
    status->SetLabel(s);
}

void Alarm::Repopulate()
{
    wxControl *text, *status;
    GetStatusControls(text, status);

    if(!status || !text)
        return;

    wxFlexGridSizer &sizer = *g_watchdog_pi->m_pWatchdogDialog->m_fgAlarms;

    text->Show(m_bEnabled);
    status->Show(m_bEnabled);

    if(m_bEnabled) {
        sizer.Add(text, 0, wxALL, 5);
        sizer.Add(status, 0, wxALL, 5);
    }
}

wxFileConfig *Alarm::GetConfigObject()
{
    wxFileConfig *pConf = GetOCPNConfigObject();

    if(!pConf)
        return NULL;
        
    pConf->SetPath ( _T( "/Settings/Watchdog/Alarms/" ) + ConfigName() );
    return pConf;
}
