/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  watchdog Plugin
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

#include "watchdog_pi.h"
#include "WatchdogDialog.h"

WatchdogDialog::WatchdogDialog( watchdog_pi &_watchdog_pi, wxWindow* parent)
    : WatchdogDialogBase( parent ), m_watchdog_pi(_watchdog_pi)
{
    m_Timer.Connect(wxEVT_TIMER, wxTimerEventHandler
                    ( WatchdogDialog::OnTimer ), NULL, this);
    m_Timer.Start(1000);
}

WatchdogDialog::~WatchdogDialog()
{
}

void WatchdogDialog::UpdateAlarms()
{
    UpdateAlarm(m_stTextLandFallTime, m_stLandFallTime, m_watchdog_pi.m_bLandFallTime);
    UpdateAlarm(m_stTextLandFallDistance, m_stLandFallDistance, m_watchdog_pi.m_bLandFallDistance);
    UpdateAlarm(m_stTextActivity, m_stActivity, m_watchdog_pi.m_bDeaddog);
    UpdateAlarm(m_stTextAnchor, m_stAnchorDistance, m_watchdog_pi.m_bAnchor);
    UpdateAlarm(m_stTextGPS, m_stGPS, m_watchdog_pi.m_bGPSAlarm);
    UpdateAlarm(m_stTextAIS, m_stAIS, m_watchdog_pi.m_bAISAlarm);
    UpdateAlarm(m_stTextUnderSpeed, m_stUnderSpeed, m_watchdog_pi.m_bUnderSpeedAlarm);
    UpdateAlarm(m_stTextOverSpeed, m_stOverSpeed, m_watchdog_pi.m_bOverSpeedAlarm);
    UpdateAlarm(m_stTextCourseError, m_stCourseError, m_watchdog_pi.m_bOffCourseAlarm);

    Fit();
    Refresh();
}

void WatchdogDialog::OnDisableAllAlarms( wxComdogdEvent& event )
{
    wxFileConfig *pConf = m_watchdog_pi.m_pconfig;
    pConf->Write ( _T ( "DisableAllAlarms" ), event.GetValue());
}

void WatchdogDialog::OnPreferences( wxComdogdEvent& event )
{
    m_watchdog_pi.ShowPreferencesDialog(this);
}

void WatchdogDialog::OnResetLastAlarm( wxComdogdEvent& event )
{
    m_watchdog_pi.ResetLastAlarm();
}

void WatchdogDialog::Update(double anchor_distance, double nmea_seconds,
                            double courseerror, double sog)
{
    // landfall
    wxString s, fmt(_T("%d "));
    wxTimeSpan span = m_watchdog_pi.m_LandFallTime;
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
                    
        m_stLandFallTime->SetForegroundColour(m_watchdog_pi.Color(watchdog_pi::LANDFALLTIME));
        m_stLandFallTime->SetLabel(s);
    }                    

    m_stLandFallTime->SetForegroundColour(m_watchdog_pi.Color(watchdog_pi::LANDFALLDISTANCE));
    m_stLandFallDistance->SetLabel(wxString::Format(_T("%.2f nm"), m_watchdog_pi.m_dLandFallDistance));

    // anchor
    m_stAnchorDistance->SetForegroundColour(m_watchdog_pi.Color(watchdog_pi::ANCHOR));

    if(isnan(anchor_distance))
        m_stAnchorDistance->SetLabel(_T("N/A"));
    else {
        wxString fmt(_T("%.0f "));
        m_stAnchorDistance->SetLabel(
            wxString::Format(fmt + _("meter(s)"), anchor_distance));
    }

    // nmea data
    m_stGPS->SetForegroundColour(m_watchdog_pi.Color(watchdog_pi::GPS));
    if(isnan(nmea_seconds))
        m_stGPS->SetLabel(_T("N/A"));
    else {
        wxString fmt(_T("%.0f "));
        m_stGPS->SetLabel(
            wxString::Format(fmt + _("second(s)"), nmea_seconds));
    }

    // course
    m_stCourseError->SetForegroundColour(m_watchdog_pi.Color(watchdog_pi::OFFCOURSE));
    if(isnan(courseerror))
        m_stCourseError->SetLabel(_T("N/A"));
    else {
        wxString fmt(_T("%.0f "));
        m_stCourseError->SetLabel(
            wxString::Format(fmt + _("degrees(s)"), courseerror));
    }

    // speed
    m_stUnderSpeed->SetForegroundColour(m_watchdog_pi.Color(watchdog_pi::UNDERSPEED));
    if(isnan(sog))
        m_stUnderSpeed->SetLabel(_T("N/A"));
    else {
        wxString fmt(_T("%.1f "));
        m_stUnderSpeed->SetLabel(wxString::Format(fmt, sog));
    }

    m_stOverSpeed->SetForegroundColour(m_watchdog_pi.Color(watchdog_pi::OVERSPEED));
    if(isnan(sog))
        m_stOverSpeed->SetLabel(_T("N/A"));
    else {
        wxString fmt(_T("%.1f "));
        m_stOverSpeed->SetLabel(wxString::Format(fmt, sog));
    }
}


void WatchdogDialog::UpdateAlarm(wxControl *ctrl1,  wxControl *ctrl2, bool show)
{
    if(show) {
        ctrl1->Show();
        ctrl2->Show();
    } else {
        ctrl1->Hide();
        ctrl2->Hide();
    }
}

void WatchdogDialog::OnTimer( wxTimerEvent & )
{
    m_stActivity->SetForegroundColour(m_watchdog_pi.Color(watchdog_pi::DEADMAN));

    wxTimeSpan span = wxDateTime::Now() - m_watchdog_pi.m_DeaddogUpdateTime;
    int days = span.GetDays();
    span -= wxTimeSpan::Days(days);
    int hours = span.GetHours();
    span -= wxTimeSpan::Hours(hours);
    int minutes = span.GetMinutes();
    span -= wxTimeSpan::Minutes(minutes);
    int seconds = span.GetSeconds().ToLong();
    wxString d, fmt(_T("%d "));
    if(days)
        d = wxString::Format(fmt + _T("days"), days);
    m_stActivity->SetLabel(wxString::Format(days + _T("%02d:%02d:%02d"),
                                            hours, minutes, seconds));
}

