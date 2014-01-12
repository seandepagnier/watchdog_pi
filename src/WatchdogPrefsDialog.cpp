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
#include "WatchdogPrefsDialog.h"

WatchdogPrefsDialog::WatchdogPrefsDialog( watchdog_pi &_watchdog_pi, wxWindow* parent)
    : WatchdogPrefsDialogBase( parent ), m_watchdog_pi(_watchdog_pi)
{
    ReadAlarmActions();
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

    if(read) {
        m_rbEnabled->SetValue(alarm->m_bEnabled);
        m_rbDisabled->SetValue(!alarm->m_bEnabled);
        m_cbGraphicsEnabled->SetValue(alarm->m_bgfxEnabled);
        m_cbSound->SetValue(alarm->m_bSound);
        m_fpSound->SetPath(alarm->m_sSound);
        m_cbCommand->SetValue(alarm->m_bCommand);
        m_tCommand->SetValue(alarm->m_sCommand);
        m_cbMessageBox->SetValue(alarm->m_bMessageBox);
        m_cbAutoReset->SetValue(alarm->m_bAutoReset);
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

    case UNDERSPEED:
        alarm->ConfigItem(read, _T ( "Knots" ), m_tUnderSpeed);
        break;

    case OVERSPEED:
        alarm->ConfigItem(read, _T ( "Knots" ), m_tOverSpeed);
        break;
    }
}

Alarm &WatchdogPrefsDialog::CurrentAlarm()
{
    return *Alarms[m_lbAlarm->GetSelection()];
}

void WatchdogPrefsDialog::OnInformation( wxCommandEvent& event )
{
    wxMessageDialog mdlg(this, _("\
Most alarms should be self-explanatory\n\
Normally a sound is played, however you can execute any command you like, eg:  \
\"aplay /usr/local/share/opencpn/sounds/2bells.wav\""),
                         _("Watchdog Information"), wxOK | wxICON_INFORMATION);
    mdlg.ShowModal();
}

void WatchdogPrefsDialog::OnAboutAuthor( wxCommandEvent& event )
{
/*
      wxLaunchDefaultBrowser(_T("https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=sean%40depagnier%2ecom&lc=US&item_name=watchdog&no_note=0&currency_code=USD&bn=PP%2dDonationsBF%3abtn_donateCC_LG%2egif%3aNonHostedGuest"));
*/
}
