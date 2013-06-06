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
 *   the Free Software Foundation; either version 2 of the License, or     *
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
    UpdateAlarms();
    m_Timer.Connect(wxEVT_TIMER, wxTimerEventHandler
                    ( WatchmanDialog::OnTimer ), NULL, this);
    m_Timer.Start(1000);
}

WatchmanDialog::~WatchmanDialog()
{
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

void WatchmanDialog::UpdateAlarms()
{
    UpdateAlarm(m_stTextLandfall, m_stLandFallTime, m_watchman_pi.m_bLandFall);
    UpdateAlarm(m_stTextActivity, m_stActivity, m_watchman_pi.m_bDeadman);
    UpdateAlarm(m_stTextAnchor, m_stAnchorDistance, m_watchman_pi.m_bAnchor);
    UpdateAlarm(m_stTextGPS, m_stGPS, m_watchman_pi.m_bGPSAlarm);
    UpdateAlarm(m_stTextAIS, m_stAIS, m_watchman_pi.m_bAISAlarm);
}

void WatchmanDialog::OnPreferences( wxCommandEvent& event )
{
    m_watchman_pi.ShowPreferencesDialog(this);
    UpdateAlarms();
}

void WatchmanDialog::UpdateLandFallTime(PlugIn_Position_Fix_Ex &pfix)
{
    m_stLandFallTime->SetForegroundColour(m_watchman_pi.m_bLandFallAlarmed ?
                                          *wxRED : *wxBLACK);

    double lat1 = pfix.Lat, lon1 = pfix.Lon, lat2, lon2;
    double dist = 0, dist1 = 1000;
    wxTimeSpan span;
    int count = 0;
    while(count < 10) {
        PositionBearingDistanceMercator_Plugin(pfix.Lat, pfix.Lon, pfix.Cog, dist + dist1, &lat2, &lon2);
        if(PlugIn_GSHHS_CrossesLand(lat1, lon1, lat2, lon2)) {
            if(dist1 < 1) {
                span = wxTimeSpan::Seconds(3600.0 * dist / pfix.Sog);
                wxString s, fmt(_T("%d "));
                s += wxString::Format(fmt + _("Days "), span.GetDays());
                span -= wxTimeSpan::Days(span.GetDays());
                s += wxString::Format(fmt + _("Hours "), span.GetHours());
                span -= wxTimeSpan::Hours(span.GetHours());
                s += wxString::Format(fmt + _("Minutes "), span.GetMinutes());
                span -= wxTimeSpan::Minutes(span.GetMinutes());
                s += wxString::Format(fmt + _("Seconds"), span.GetSeconds());
                m_stLandFallTime->SetLabel(s);
                return;
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
    m_stLandFallTime->SetLabel(_("LandFall not Detected"));
}

void WatchmanDialog::UpdateAnchorDistance(double distance)
{
    m_stAnchorDistance->SetForegroundColour(m_watchman_pi.m_bAnchorAlarmed ?
                                           *wxRED : *wxBLACK);

    if(isnan(distance))
        m_stAnchorDistance->SetLabel(_T("N/A"));
    else {
        wxString fmt(_T("%.0f "));
        m_stAnchorDistance->SetLabel(
            wxString::Format(fmt + _("meters from anchor"), distance));
    }
}

void WatchmanDialog::OnTimer( wxTimerEvent & )
{
    m_stActivity->SetForegroundColour(m_watchman_pi.m_bDeadmanAlarmed ?
                                      *wxRED : *wxBLACK);

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
    m_stGPS->SetForegroundColour(m_watchman_pi.m_bGPSAlarmed ?
                                           *wxRED : *wxBLACK);
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
    m_stAIS->SetForegroundColour(m_watchman_pi.m_bAISAlarmed ?
                                           *wxRED : *wxBLACK);
    if(isnan(seconds))
        m_stAIS->SetLabel(_T("N/A"));
    else {
        wxString fmt(_T("%.0f "));
        m_stAIS->SetLabel(
            wxString::Format(fmt + _("second(s)"), seconds));
    }
}

void WatchmanDialog::UpdateCourseError(double courseerror)
{
    m_stCourseError->SetForegroundColour(m_watchman_pi.m_bOffCourseAlarmed ?
                                           *wxRED : *wxBLACK);
    if(isnan(courseerror))
        m_stCourseError->SetLabel(_T("N/A"));
    else {
        wxString fmt(_T("%.0f "));
        m_stCourseError->SetLabel(
            wxString::Format(fmt + _("degrees(s)"), courseerror));
    }
}
