/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  watchdog Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2014 by Sean D'Epagnier                                 *
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
#include "WatchdogPrefsDialog.h"

WatchdogPrefsDialog::WatchdogPrefsDialog( watchdog_pi &_watchdog_pi, wxWindow* parent)
    : WatchdogPrefsDialogBase( parent ), m_watchdog_pi(_watchdog_pi), m_breading(false)
{
    ReadAlarmActions();
    Alarm::ConfigCoursePort(true, m_cbSeparatePortAndStarboard);
    m_lbAlarm->RemovePage(COURSESTARBOARD);
    ConfigurePortAlarms();
}

WatchdogPrefsDialog::~WatchdogPrefsDialog()
{
}

void WatchdogPrefsDialog::OnAlarmChanged( wxListbookEvent& event )
{
    ReadAlarmActions();
}

void WatchdogPrefsDialog::OnAlarmUpdate()
{
    WriteAlarmActions();

    m_watchdog_pi.m_pWatchdogDialog->UpdateAlarms();
}

void WatchdogPrefsDialog::OnCheckSeparatePortAndStarboard( wxCommandEvent& event )
{
    Alarm::ConfigCoursePort(false, m_cbSeparatePortAndStarboard);
    ConfigurePortAlarms();
    m_lbAlarm->SetSelection(COURSE);
}

void WatchdogPrefsDialog::OnSyncToBoat( wxCommandEvent& event )
{
    m_tAnchorLatitude->SetValue(wxString::Format(_T("%f"), m_watchdog_pi.LastFix().Lat));
    m_tAnchorLongitude->SetValue(wxString::Format(_T("%f"), m_watchdog_pi.LastFix().Lon));

    OnAlarmUpdate();
}

void WatchdogPrefsDialog::OnCurrentCourse( wxCommandEvent& event )
{
    m_sCourse->SetValue(m_watchdog_pi.m_cog);

    OnAlarmUpdate();
}

void WatchdogPrefsDialog::OnTestAlarm( wxCommandEvent& event )
{
    CurrentAlarm().Run();
}

void WatchdogPrefsDialog::AlarmActions(bool read)
{
    int selection = m_lbAlarm->GetSelection();

    Alarm *alarm = Alarms[selection];
    if(!alarm)
        return;

    if(m_breading)
        return;

    if(read) {
        m_breading = true;
        m_rbEnabled->SetValue(alarm->m_bEnabled);
        m_rbDisabled->SetValue(!alarm->m_bEnabled);
        m_cbGraphicsEnabled->SetValue(alarm->m_bgfxEnabled);
        m_cbSound->SetValue(alarm->m_bSound);
        m_fpSound->SetPath(alarm->m_sSound);
        
        m_cbCommand->SetValue(alarm->m_bCommand);
        m_tCommand->SetValue(alarm->m_sCommand);
        m_cbMessageBox->SetValue(alarm->m_bMessageBox);
        m_cbAutoReset->SetValue(alarm->m_bAutoReset);
        m_cbRepeat->SetValue(alarm->m_bRepeat);
        m_sRepeatSeconds->SetValue(alarm->m_iRepeatSeconds);
    } else {
        alarm->m_bEnabled = m_rbEnabled->GetValue();
        alarm->m_bgfxEnabled = m_cbGraphicsEnabled->GetValue();
        alarm->m_bSound = m_cbSound->GetValue();
        alarm->m_sSound = m_fpSound->GetPath();
        alarm->m_bCommand = m_cbCommand->GetValue();
        alarm->m_sCommand = m_tCommand->GetValue();
        alarm->m_bMessageBox = m_cbMessageBox->GetValue();
        alarm->m_bAutoReset = m_cbAutoReset->GetValue();
        alarm->m_bRepeat = m_cbRepeat->GetValue();
        alarm->m_iRepeatSeconds = m_sRepeatSeconds->GetValue();
    }

    m_cbGraphicsEnabled->Enable();

    switch(selection) {
    case LANDFALL:
        alarm->ConfigItem(read, _T ( "TimeAlarm" ), m_cbLandFallTime);
        alarm->ConfigItem(read, _T ( "Minutes" ), m_sLandFallTimeMinutes);
        alarm->ConfigItem(read, _T ( "DistanceAlarm" ), m_cbLandFallDistance);
        alarm->ConfigItem(read, _T ( "Distance" ), m_tcLandFallDistance);
        break;

    case NMEADATA:
        alarm->ConfigItem(read, _T ( "Sentences" ), m_tNMEASentences);
        alarm->ConfigItem(read,  _T ( "Seconds" ), m_sNMEASeconds);;
        m_cbGraphicsEnabled->Disable();
        break;

    case DEADMAN:
        alarm->ConfigItem(read, _T ( "Minutes" ), m_sDeadmanMinutes);
        m_cbGraphicsEnabled->Disable();
        break;

    case ANCHOR:
        alarm->ConfigItem(read, _T ( "Latitude" ), m_tAnchorLatitude);
        alarm->ConfigItem(read, _T ( "Longitude" ), m_tAnchorLongitude);
        alarm->ConfigItem(read, _T ( "Radius" ), m_sAnchorRadius);
        break;

    case COURSE:
        alarm->ConfigItem(read, _T ( "Tolerance" ), m_sCourseTolerance);
        alarm->ConfigItem(read, _T ( "Course" ), m_sCourse);
        break;

    case COURSESTARBOARD:
        m_cbGraphicsEnabled->Disable();
        break;

    case UNDERSPEED:
        alarm->ConfigItem(read, _T ( "Knots" ), m_tUnderSpeed);
        break;

    case OVERSPEED:
        alarm->ConfigItem(read, _T ( "Knots" ), m_tOverSpeed);
        break;
    }

    if(read)
        m_breading = false;
}

Alarm &WatchdogPrefsDialog::CurrentAlarm()
{
    return *Alarms[m_lbAlarm->GetSelection()];
}

void WatchdogPrefsDialog::ConfigurePortAlarms()
{
    wxString offcourse = _("Off Course"), courseerror = _("Course Error"), port = _("Port");
    m_lbAlarm->RemovePage(COURSE);
    if(m_cbSeparatePortAndStarboard->GetValue()) {
        m_lbAlarm->InsertPage(COURSE, m_pCourse, offcourse + _T(" ") + port);
        m_lbAlarm->InsertPage(COURSESTARBOARD, m_pCourseStarboard, _("Off Course Starboard"));
        m_watchdog_pi.m_pWatchdogDialog->m_stTextCourseError
            ->SetLabel(courseerror + _T(" ") + port);
    } else {
        m_lbAlarm->InsertPage(COURSE, m_pCourse, offcourse);
        m_lbAlarm->RemovePage(COURSESTARBOARD);
        m_watchdog_pi.m_pWatchdogDialog->m_stTextCourseError->SetLabel(courseerror);
    }
}

void WatchdogPrefsDialog::OnInformation( wxCommandEvent& event )
{
    wxMessageDialog mdlg(this, _("\
Most alarms should be self-explanatory\n\
Normally a sound is played, however you can execute any command you like, eg:  \
\"aplay /usr/local/share/opencpn/sounds/2bells.wav\"\n\
Messagebox may be useful to interrupt other applications (like a movie player)\n\
Automatically Reset will reset the alarm once it is no longer triggered, and it may \
be triggered again later."),
                         _("Watchdog Information"), wxOK | wxICON_INFORMATION);
    mdlg.ShowModal();
}

void WatchdogPrefsDialog::OnAboutAuthor( wxCommandEvent& event )
{
    wxLaunchDefaultBrowser(_T(ABOUT_AUTHOR_URL));
}
