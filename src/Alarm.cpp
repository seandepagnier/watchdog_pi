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

#include <wx/process.h>

#include "watchdog_pi.h"

///////// The Alarm classes /////////
class LandFallAlarm : public Alarm
{
public:
    LandFallAlarm() : Alarm(_("LandFall"), 15 /* seconds */) {}

    bool Test() {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();
        double lat1 = lastfix.Lat, lon1 = lastfix.Lon, lat2, lon2;
        double dist = 0, dist1 = 1000;
        int count = 0;
        wxTimeSpan LandFallTime;
                
        wxFileConfig *pConf = GetConfigObject();

        if(pConf->Read ( _T ( "TimeAlarm" ), 1L)) {
            double LandFallTimeMinutes;
            pConf->Read ( _T ( "Minutes" ), &LandFallTimeMinutes, 20 );
            
            while(count < 10) {
                PositionBearingDistanceMercator_Plugin
                    (lastfix.Lat, lastfix.Lon, lastfix.Cog, dist + dist1, &lat2, &lon2);
                if(PlugIn_GSHHS_CrossesLand(lat1, lon1, lat2, lon2)) {
                    if(dist1 < 1) {
                    LandFallTime = wxTimeSpan::Seconds(3600.0 * dist / lastfix.Sog);
                    if(LandFallTime.GetMinutes() <= LandFallTimeMinutes)
                        return true;
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
            for(double t = 0; t<360; t+=18) {
                double dlat, dlon;
                double LandFallDistance;
                pConf->Read ( _T ( "Distance" ), &LandFallDistance, 3 );
                PositionBearingDistanceMercator_Plugin(lastfix.Lat, lastfix.Lon, t,
                                                       LandFallDistance, &dlat, &dlon);
            
                if(PlugIn_GSHHS_CrossesLand(lastfix.Lat, lastfix.Lon, dlat, dlon))
                    return true;
            }
        }
        return false;
    }

    wxString AlarmStatus() {
        wxString s, fmt(_T("%d "));
        wxFileConfig *pConf = GetConfigObject();

        double LandFallTimeMinutes;
        pConf->Read ( _T ( "TimeMinutes" ), &LandFallTimeMinutes, 20 );

        wxTimeSpan span = wxTimeSpan::Minutes(LandFallTimeMinutes);
        if(span.IsNull())
            s = _("LandFall not Detected");
        else {
            if(span.GetDays())
            s = wxString::Format(fmt + _("Days "), span.GetDays());
            else if(span.GetHours())
                s = wxString::Format(fmt + _("Hours "), span.GetHours());
            else if(span.GetMinutes())
                s = wxString::Format(fmt + _("Minutes "), span.GetMinutes());
            else
                s = wxString::Format(fmt + _("Seconds"), span.GetSeconds());
            
        }                    
        
        double LandFallDistance;
        pConf->Read ( _T ( "Distance" ), &LandFallDistance, 3 );
        s += wxString::Format(_T(" Distance %.2f nm"), LandFallDistance);
        return s;
    }


} g_LandfallAlarm;

class NMEADataAlarm : public Alarm
{
public:
    NMEADataAlarm() : Alarm(_("NMEA Data"), 1) {}

    bool Test() {
        wxFileConfig *pConf = GetConfigObject();
        return ElapsedSeconds() > pConf->Read ( _T ( "Seconds" ), 0L );
    }

    wxString AlarmStatus() {
        int seconds = ElapsedSeconds();
        if(isnan(seconds))
            return _T("N/A");

        wxString fmt(_T("%.0f "));
        return wxString::Format(fmt + _("second(s)"), seconds);
    }

private:
    int ElapsedSeconds() {
        return 0;
        //       return (wxDateTime::Now() - g_watchdog_pi->m_LastFixTime).GetSeconds().ToLong();
    }
} g_NMEADataAlarm;

class DeadmanAlarm : public Alarm
{
public:
    DeadmanAlarm() : Alarm(_("Deadman"), 1) {}
    bool Test() {
        wxFileConfig *pConf = GetConfigObject();

        wxTimeSpan DeadmanSpan = wxTimeSpan::Minutes(pConf->Read ( _T ( "Minutes" ), 20L ));
        return wxDateTime::Now() - g_watchdog_pi->m_cursor_time > DeadmanSpan;
    }

    wxString AlarmStatus() {
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

class AnchorAlarm : public Alarm
{
public:
    AnchorAlarm() : Alarm(_("Anchor"), 1) {}

    bool Test() {
        wxFileConfig *pConf = GetConfigObject();

        double anchordist = Distance();
        long AnchorRadius = pConf->Read ( _T ( "Radius" ), 50L );
        return anchordist > AnchorRadius;
    }

    wxString AlarmStatus() {
        double anchordist = Distance();
        if(isnan(anchordist))
            return _T("N/A");
        else {
            wxString fmt(_T("%.0f "));
            return wxString::Format(fmt + _("meter(s)"), anchordist);
        }
    }

    void Render(PlugIn_ViewPort &vp) {
        wxFileConfig *pConf = GetConfigObject();
        
        wxPoint r1, r2;
        double AnchorLatitude, AnchorLongitude;
        pConf->Read ( _T ( "Latitude" ), &AnchorLatitude, NAN );
        pConf->Read ( _T ( "Longitude" ), &AnchorLongitude, NAN );
        
        GetCanvasPixLL(&vp, &r1, AnchorLatitude, AnchorLongitude);
        GetCanvasPixLL(&vp, &r2, AnchorLatitude +
                       pConf->Read ( _T ( "Radius" ), 50 )/1853.0/60.0,
                       AnchorLongitude);
        
//        dc.SetPen(wxPen(Color(ANCHOR), 2));
//        dc.DrawCircle( r1.x, r1.y, r2.y - r1.y );
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


} g_AnchorAlarm;

class CourseAlarm : public Alarm
{
public:
    CourseAlarm() : Alarm(_("Off Course"), 1) {}
    bool Test() {
        wxFileConfig *pConf = GetConfigObject();
        double Tolerance;
        pConf->Read ( _T ( "Tolerance" ), &Tolerance, 20L );

        return CourseError() > Tolerance;
    }

    wxString AlarmStatus() {
        double courseerror = CourseError();

        if(isnan(courseerror))
            return _T("N/A");

        wxString fmt(_T("%.0f "));
        return wxString::Format(fmt + _("degrees(s)"), courseerror);
    }

private:
    double CourseError() {
        wxFileConfig *pConf = GetConfigObject();

        double Course;
        pConf->Read ( _T ( "Course" ), &Course, 20L );

        return fabs(heading_resolve(g_watchdog_pi->m_cog - Course));
    }
} g_CourseAlarm;

class UnderSpeedAlarm : public Alarm
{
public:
    UnderSpeedAlarm() : Alarm(_("Underspeed"), 1) {}

    bool Test() {
        wxFileConfig *pConf = GetConfigObject();

        double knots;
        pConf->Read ( _T ( "Knots" ), &knots, 0L );
        return g_watchdog_pi->m_sog < knots;
    }

    wxString AlarmStatus() {
        if(isnan(g_watchdog_pi->m_sog))
            return _T("N/A");

        wxString fmt(_T("%.1f "));
        return wxString::Format(fmt, g_watchdog_pi->m_sog);
    }
} g_UnderSpeedAlarm;

class OverSpeedAlarm : public Alarm
{
public:
    OverSpeedAlarm() : Alarm(_("Overspeed"), 1) {}

    bool Test() {
        wxFileConfig *pConf = GetConfigObject();

        double knots;
        pConf->Read ( _T ( "Knots" ), &knots, 9L );
        return g_watchdog_pi->m_sog > knots;
    }

    wxString AlarmStatus() {
        if(isnan(g_watchdog_pi->m_sog))
            return _T("N/A");

        wxString fmt(_T("%.1f "));
        return wxString::Format(fmt, g_watchdog_pi->m_sog);
    }
} g_OverSpeedAlarm;

////////// Alarm Base Class /////////////////

Alarm *Alarms[] = {&g_LandfallAlarm, &g_NMEADataAlarm, &g_DeadmanAlarm,
                   &g_AnchorAlarm, &g_CourseAlarm, &g_UnderSpeedAlarm,
                   &g_OverSpeedAlarm, NULL};

void Alarm::RenderAlarms(PlugIn_ViewPort &vp)
{
    for(Alarm **alarm = Alarms; *alarm; alarm++)
        if((*alarm)->m_bEnabled && (*alarm)->m_bgfxEnabled)
            (*alarm)->Render(vp);
}

Alarm::Alarm(wxString name, int interval) : m_sName(name)
{
    m_Timer.Connect(wxEVT_TIMER, wxTimerEventHandler( Alarm::OnTimer ), NULL, this);
    m_Timer.Start(interval * 1000, wxTIMER_CONTINUOUS);
}

void Alarm::Run()
{
    wxDateTime now = wxDateTime::Now();

    if(m_bFired) {
        if((now - m_LastAlarmTime).GetSeconds() < m_iRepeatSeconds || !m_iRepeatSeconds)
            return;
    } else
        m_bFired = true;

    m_LastAlarmTime = now;

    if(m_bSound)
        PlugInPlaySound(m_sSound);

    if(m_bCommand)
        if(!wxProcess::Open(m_sCommand)) {
            wxMessageDialog mdlg(GetOCPNCanvasWindow(),
                                 _("Failed to execute command: ") + m_sCommand,
                                 _("Watchdog"), wxOK | wxICON_ERROR);
            mdlg.ShowModal();
            m_bCommand = false;
        }

    if(m_bMessageBox) {
        wxMessageDialog mdlg(GetOCPNCanvasWindow(), _("ALARM!!!!"),
                             _("Watchman"), wxOK | wxICON_ERROR);
        mdlg.ShowModal();
    }
}

void Alarm::Reset()
{
    if(m_bAutoReset)
        m_bFired = false;
}

void Alarm::SaveConfig()
{
    wxFileConfig *pConf = GetConfigObject();

    pConf->Write ( _T ( "Enabled" ), m_bEnabled);
    pConf->Write ( _T ( "SoundEnabled" ), m_bSound);
    pConf->Write ( _T ( "SoundFilepath" ), m_sSound);
    pConf->Write ( _T ( "CommandEnabled" ), m_bCommand);
    pConf->Write ( _T ( "CommandFilepath" ), m_sCommand);
    pConf->Write ( _T ( "MessageBox" ), m_bMessageBox);
    pConf->Write ( _T ( "RepeatSeconds" ), m_iRepeatSeconds);
    pConf->Write ( _T ( "AutoReset" ), m_bAutoReset);
}

void Alarm::LoadConfig()
{  
    wxFileConfig *pConf = GetConfigObject();

    pConf->Read ( _T ( "Enabled" ), &m_bEnabled, 0 );
    pConf->Read ( _T ( "SoundEnabled" ), &m_bSound, 0 );
    pConf->Read ( _T ( "SoundFilepath" ), &m_sSound, _T("") );
    pConf->Read ( _T ( "CommandEnabled" ), &m_bCommand, 0 );
    pConf->Read ( _T ( "CommandFilepath" ), &m_sCommand, _T("") );
    pConf->Read ( _T ( "MessageBox" ), &m_bMessageBox, 0);
    pConf->Read ( _T ( "RepeatSeconds" ), &m_iRepeatSeconds, 60);
    pConf->Read ( _T ( "AutoReset" ), &m_bAutoReset, 0);
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

    wxSlider *s = dynamic_cast<wxSlider*>(control);
    if(s) {
        if(read)
            s->SetValue(pConf->Read(name, (long)s->GetValue()));
        else
            pConf->Write(name, s->GetValue());
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
    if(m_bEnabled && Test())
        Run();
}

wxFileConfig *Alarm::GetConfigObject()
{
    wxFileConfig *pConf = GetOCPNConfigObject();

    if(!pConf)
        return NULL;
        
    pConf->SetPath ( _T( "/Settings/Watchdog/Alarms/" ) + m_sName );
    return pConf;
}
