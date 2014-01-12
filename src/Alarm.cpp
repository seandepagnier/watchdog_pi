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

#include <wx/process.h>

#include "watchdog_pi.h"
#include "WatchdogDialog.h"

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
        m_crossinglat1 = m_crossinglon1 = NAN;

        if(pConf->Read ( _T ( "TimeAlarm" ), 1L)) {
            double LandFallTimeMinutes;
            pConf->Read ( _T ( "Minutes" ), &LandFallTimeMinutes, 20 );
            
            while(count < 10) {
                PositionBearingDistanceMercator_Plugin
                    (lastfix.Lat, lastfix.Lon, lastfix.Cog, dist + dist1, &lat2, &lon2);
                if(PlugIn_GSHHS_CrossesLand(lat1, lon1, lat2, lon2)) {
                    if(dist1 < 1) {
                        LandFallTime = wxTimeSpan::Seconds(3600.0 * dist / lastfix.Sog);
                        if(LandFallTime.GetMinutes() <= LandFallTimeMinutes) {
                            m_crossinglat1 = lat1, m_crossinglon1 = lon1;
                            m_crossinglat2 = lat2, m_crossinglon2 = lon2;
                            return true;
                        }
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
                    m_crossinglat1 = lastfix.Lat, m_crossinglon1 = lastfix.Lon;
                    m_crossinglat2 = dlat, m_crossinglon2 = dlon;
                    return true;
                }
            }
        }
        return false;
    }

    void UpdateStatus() {
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;

        if(!m_bEnabled) {
            dlg.m_stTextLandFallTime->Hide();
            dlg.m_stLandFallTime->Hide();
            dlg.m_stTextLandFallDistance->Hide();
            dlg.m_stLandFallDistance->Hide();
            return;
        }

        wxString s, fmt(_T("%d "));
        wxFileConfig *pConf = GetConfigObject();

        if(pConf->Read ( _T ( "TimeAlarm" ), 1L)) {
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
            dlg.m_stLandFallTime->SetLabel(s);
            dlg.m_stTextLandFallTime->Show();
            dlg.m_stLandFallTime->Show();
        } else {
            dlg.m_stTextLandFallTime->Hide();
            dlg.m_stLandFallTime->Hide();
        }
        
        if(pConf->Read ( _T ( "DistanceAlarm" ), 0L)) {
            double LandFallDistance;
            pConf->Read ( _T ( "Distance" ), &LandFallDistance, 3 );
            s += wxString::Format(_T(" Distance %.2f nm"), LandFallDistance);
            
            dlg.m_stLandFallDistance->SetLabel(s);
            dlg.m_stTextLandFallDistance->Show();
            dlg.m_stLandFallDistance->Show();
        } else {
            dlg.m_stTextLandFallDistance->Hide();
            dlg.m_stLandFallDistance->Hide();
        }
    }

    void Render(ocpnDC &dc, PlugIn_ViewPort &vp) {
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
        
        dc.SetPen(wxPen(*wxRED, 3));
        dc.DrawCircle( r4.x, r4.y, hypot(r2.x-r3.x, r2.y-r3.y) / 2 );
    }

private:
    double m_crossinglat1, m_crossinglon1;
    double m_crossinglat2, m_crossinglon2;
} g_LandfallAlarm;

class NMEADataAlarm : public Alarm
{
public:
    NMEADataAlarm() : Alarm(_("NMEA Data"), 1) {}

    bool Test() {
        wxFileConfig *pConf = GetConfigObject();
        return ElapsedSeconds() > pConf->Read ( _T ( "Seconds" ), 0L );
    }

    void UpdateStatus() {
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;

        if(!m_bEnabled) {
            dlg.m_stTextNMEAData->Hide();
            dlg.m_stNMEAData->Hide();
            return;
        }

        int seconds = ElapsedSeconds();
        wxString s;
        if(isnan(seconds))
            s = _T("N/A");
        else {
            wxString fmt(_T("%.0f "));
            s = wxString::Format(fmt + _("second(s)"), seconds);
        }

        dlg.m_stNMEAData->SetLabel(s);
        dlg.m_stTextNMEAData->Show();
        dlg.m_stNMEAData->Show();
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

    void UpdateStatus() {
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;

        if(!m_bEnabled) {
            dlg.m_stTextDeadman->Hide();
            dlg.m_stDeadman->Hide();
            return;
        }

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
        wxString s = d + wxString::Format(_T("%02d:%02d:%02d"),
                                          hours, minutes, seconds);

        dlg.m_stDeadman->SetLabel(s);
        dlg.m_stTextDeadman->Show();
        dlg.m_stDeadman->Show();
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

    void UpdateStatus() {
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;

        if(!m_bEnabled) {
            dlg.m_stTextAnchor->Hide();
            dlg.m_stAnchorDistance->Hide();
            return;
        }

        double anchordist = Distance();
        wxString s;
        if(isnan(anchordist))
            s = _T("N/A");
        else {
            wxString fmt(_T("%.0f "));
            s = wxString::Format(fmt + _("meter(s)"), anchordist);
        }

        dlg.m_stAnchorDistance->SetLabel(s);
        dlg.m_stTextAnchor->Show();
        dlg.m_stAnchorDistance->Show();
    }

    void Render(ocpnDC &dc, PlugIn_ViewPort &vp) {
        wxFileConfig *pConf = GetConfigObject();
        
        wxPoint r1, r2;
        double AnchorLatitude, AnchorLongitude;
        pConf->Read ( _T ( "Latitude" ), &AnchorLatitude, NAN );
        pConf->Read ( _T ( "Longitude" ), &AnchorLongitude, NAN );
        
        GetCanvasPixLL(&vp, &r1, AnchorLatitude, AnchorLongitude);
        GetCanvasPixLL(&vp, &r2, AnchorLatitude +
                       pConf->Read ( _T ( "Radius" ), 50 )/1853.0/60.0,
                       AnchorLongitude);
        
        dc.SetPen(wxPen(*wxRED, 2));
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

    void UpdateStatus() {
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;

        if(!m_bEnabled) {
            dlg.m_stTextCourseError->Hide();
            dlg.m_stCourseError->Hide();
            return;
        }

        double courseerror = CourseError();
        wxString s;
        if(isnan(courseerror))
            s = _T("N/A");
        else {
            wxString fmt(_T("%.0f "));
            s = wxString::Format(fmt + _("degrees(s)"), courseerror);
        }

        dlg.m_stCourseError->SetLabel(s);
        dlg.m_stTextCourseError->Show();
        dlg.m_stCourseError->Show();
    }

    void Render(ocpnDC &dc, PlugIn_ViewPort &vp) {
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

        dc.SetPen(wxPen(*wxGREEN, 2));
        dc.DrawLine( r1.x, r1.y, r2.x, r2.y );
        dc.DrawLine( r1.x, r1.y, r3.x, r3.y );
    }

private:
    double CourseError() {
        wxFileConfig *pConf = GetConfigObject();

        double Course;
        pConf->Read ( _T ( "Course" ), &Course, 20L );

        return fabs(heading_resolve(g_watchdog_pi->m_cog - Course));
    }
} g_CourseAlarm;

class SpeedAlarm : public Alarm
{
public:
    SpeedAlarm(wxString name) : Alarm(name, 1) {}

    double Knots() {
        wxFileConfig *pConf = GetConfigObject();

        double knots;
        pConf->Read ( _T ( "Knots" ), &knots, 0L );
        return knots;
    }

    void UpdateStatus(wxStaticText *text, wxStaticText *value) {
        if(!m_bEnabled) {
            text->Hide();
            value->Hide();
            return;
        }

        wxString s;
        if(isnan(g_watchdog_pi->m_sog))
            s = _T("N/A");
        else {
            wxString fmt(_T("%.1f "));
            s = wxString::Format(fmt, g_watchdog_pi->m_sog);
        }

        value->SetLabel(s);
        text->Show();
        text->Show();
    }

    void Render(ocpnDC &dc, PlugIn_ViewPort &vp) {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();

        double knots = Knots();

        double lat1 = lastfix.Lat, lon1 = lastfix.Lon, lat2, lon2;
        PositionBearingDistanceMercator_Plugin(lat1, lon1, 0, knots, &lat2, &lon2);

        wxPoint r1, r2;
        GetCanvasPixLL(&vp, &r1, lat1, lon1);
        GetCanvasPixLL(&vp, &r2, lat2, lon2);

        dc.SetPen(wxPen(*wxBLUE, 2));
        dc.DrawCircle( r1.x, r1.y, hypot(r1.x-r2.x, r1.y-r2.y) );
    }
};

class UnderSpeedAlarm : public SpeedAlarm
{
public:
    UnderSpeedAlarm() : SpeedAlarm(_("UnderSpeed")) {}

    bool Test() {
        return g_watchdog_pi->m_sog < Knots();
    }

    void UpdateStatus() {
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;
        SpeedAlarm::UpdateStatus(dlg.m_stTextUnderSpeed, dlg.m_stUnderSpeed);
    }
} g_UnderSpeedAlarm;

class OverSpeedAlarm : public SpeedAlarm
{
public:
    OverSpeedAlarm() : SpeedAlarm(_("OverSpeed")) {}

    bool Test() {
        return g_watchdog_pi->m_sog > Knots();
    }

    void UpdateStatus() {
        WatchdogDialog &dlg = *g_watchdog_pi->m_pWatchdogDialog;
        SpeedAlarm::UpdateStatus(dlg.m_stTextOverSpeed, dlg.m_stOverSpeed);
    }
} g_OverSpeedAlarm;

////////// Alarm Base Class /////////////////

Alarm *Alarms[] = {&g_LandfallAlarm, &g_NMEADataAlarm, &g_DeadmanAlarm,
                   &g_AnchorAlarm, &g_CourseAlarm, &g_UnderSpeedAlarm,
                   &g_OverSpeedAlarm, 0, 0, 0};

void Alarm::RenderAlarms(ocpnDC &dc, PlugIn_ViewPort &vp)
{
    for(Alarm **alarm = Alarms; *alarm; alarm++)
        if((*alarm)->m_bEnabled && (*alarm)->m_bgfxEnabled)
            (*alarm)->Render(dc, vp);
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
    pConf->Read ( _T ( "SoundEnabled" ), &m_bSound, 1 );
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
    wxFileConfig *pConf = GetConfigObject();
    if(m_bEnabled && Test() && !pConf->Read ( _T ( "DisableAllAlarms" ), 0L ))
        Run();

    if(g_watchdog_pi->m_pWatchdogDialog && g_watchdog_pi->m_pWatchdogDialog->IsShown())
        UpdateStatus();
}

wxFileConfig *Alarm::GetConfigObject()
{
    wxFileConfig *pConf = GetOCPNConfigObject();

    if(!pConf)
        return NULL;
        
    pConf->SetPath ( _T( "/Settings/Watchdog/Alarms/" ) + m_sName );
    return pConf;
}
