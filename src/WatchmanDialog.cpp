/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  watchman Plugin
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

#include "watchman_pi.h"
#include "WatchmanDialog.h"

WatchmanDialog::WatchmanDialog( watchman_pi &_watchman_pi, wxWindow* parent)
    : WatchmanDialogBase( parent ), m_watchman_pi(_watchman_pi)
{
    m_Timer.Connect(wxEVT_TIMER, wxTimerEventHandler
                    ( WatchmanDialog::OnTimer ), NULL, this);
    m_Timer.Start(1000);
}

WatchmanDialog::~WatchmanDialog()
{
}

void WatchmanDialog::UpdateAlarms()
{
    UpdateAlarm(m_stTextLandFallTime, m_stLandFallTime, m_watchman_pi.m_bLandFallTime);
    UpdateAlarm(m_stTextLandFallDistance, m_stLandFallDistance, m_watchman_pi.m_bLandFallDistance);
    UpdateAlarm(m_stTextActivity, m_stActivity, m_watchman_pi.m_bDeadman);
    UpdateAlarm(m_stTextAnchor, m_stAnchorDistance, m_watchman_pi.m_bAnchor);
    UpdateAlarm(m_stTextGPS, m_stGPS, m_watchman_pi.m_bGPSAlarm);
    UpdateAlarm(m_stTextAIS, m_stAIS, m_watchman_pi.m_bAISAlarm);
    UpdateAlarm(m_stTextUnderSpeed, m_stUnderSpeed, m_watchman_pi.m_bUnderSpeedAlarm);
    UpdateAlarm(m_stTextOverSpeed, m_stOverSpeed, m_watchman_pi.m_bOverSpeedAlarm);
    UpdateAlarm(m_stTextCourseError, m_stCourseError, m_watchman_pi.m_bOffCourseAlarm);

    Fit();
    Refresh();
}

void WatchmanDialog::OnPreferences( wxCommandEvent& event )
{
    m_watchman_pi.ShowPreferencesDialog(this);
}

void WatchmanDialog::OnResetLastAlarm( wxCommandEvent& event )
{
    m_watchman_pi.ResetLastAlarm();
}

void WatchmanDialog::UpdateLandFallTime()
{
    wxString s, fmt(_T("%d "));
    wxTimeSpan span = m_watchman_pi.m_LandFallTime;
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
                    
        m_stLandFallTime->SetForegroundColour(m_watchman_pi.Color(watchman_pi::LANDFALLTIME));
        m_stLandFallTime->SetLabel(s);
    }                    

    m_stLandFallTime->SetForegroundColour(m_watchman_pi.Color(watchman_pi::LANDFALLDISTANCE));
    m_stLandFallDistance->SetLabel(wxString::Format(_T("%.2f nm"), m_watchman_pi.m_dLandFallDistance));
}

void WatchmanDialog::UpdateAnchorDistance(double distance)
{
    m_stAnchorDistance->SetForegroundColour(m_watchman_pi.Color(watchman_pi::ANCHOR));

    if(isnan(distance))
        m_stAnchorDistance->SetLabel(_T("N/A"));
    else {
        wxString fmt(_T("%.0f "));
        m_stAnchorDistance->SetLabel(
            wxString::Format(fmt + _("meter(s)"), distance));
    }
}

void WatchmanDialog::UpdateAlarm(wxControl *ctrl1,  wxControl *ctrl2, bool show)
{
    if(show) {
        ctrl1->Show();
        ctrl2->Show();
    } else {
        ctrl1->Hide();
        ctrl2->Hide();
    }
}

void WatchmanDialog::OnTimer( wxTimerEvent & )
{
    m_stActivity->SetForegroundColour(m_watchman_pi.Color(watchman_pi::DEADMAN));

    wxTimeSpan span = wxDateTime::Now() - m_watchman_pi.m_DeadmanUpdateTime;
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

void WatchmanDialog::UpdateGPSTime(double seconds)
{
    m_stGPS->SetForegroundColour(m_watchman_pi.Color(watchman_pi::GPS));
    if(isnan(seconds))
        m_stGPS->SetLabel(_T("N/A"));
    else {
        wxString fmt(_T("%.0f "));
        m_stGPS->SetLabel(
            wxString::Format(fmt + _("second(s)"), seconds));
    }
}

void WatchmanDialog::UpdateAISTime(double seconds)
{
    m_stAIS->SetForegroundColour(m_watchman_pi.Color(watchman_pi::AIS));
    if(isnan(seconds))
        m_stAIS->SetLabel(_T("N/A"));
    else {
        wxString fmt(_T("%.0f "));
        m_stAIS->SetLabel(
            wxString::Format(fmt + _("second(s)"), seconds));
    }
}

void WatchmanDialog::UpdateUnderSpeed(double speed)
{
    m_stUnderSpeed->SetForegroundColour(m_watchman_pi.Color(watchman_pi::UNDERSPEED));
    if(isnan(speed))
        m_stUnderSpeed->SetLabel(_T("N/A"));
    else {
        wxString fmt(_T("%.1f "));
        m_stUnderSpeed->SetLabel(wxString::Format(fmt, speed));
    }
}

void WatchmanDialog::UpdateOverSpeed(double speed)
{
    m_stOverSpeed->SetForegroundColour(m_watchman_pi.Color(watchman_pi::OVERSPEED));
    if(isnan(speed))
        m_stOverSpeed->SetLabel(_T("N/A"));
    else {
        wxString fmt(_T("%.1f "));
        m_stOverSpeed->SetLabel(wxString::Format(fmt, speed));
    }
}

void WatchmanDialog::UpdateCourseError(double courseerror)
{
    m_stCourseError->SetForegroundColour(m_watchman_pi.Color(watchman_pi::OFFCOURSE));
    if(isnan(courseerror))
        m_stCourseError->SetLabel(_T("N/A"));
    else {
        wxString fmt(_T("%.0f "));
        m_stCourseError->SetLabel(
            wxString::Format(fmt + _("degrees(s)"), courseerror));
    }
}
