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

#include "WatchdogPrefsDialog.h"
#include "watchdog_pi.h"

WatchdogPrefsDialog::WatchdogPrefsDialog( watchdog_pi &_watchdog_pi, wxWindow* parent)
    : WatchdogPrefsDialogBase( parent ), m_watchdog_pi(_watchdog_pi)
{
    SetAlarmActions();

    wxFileConfig *pConf = m_watchdog_pi.m_pconfig;

    if(!pConf)
        return;

    pConf->SetPath ( _T ( "/Settings/Watchdog" ) );

    double d;

    m_cbLandFallTime->SetValue(pConf->Read ( _T ( "LandFallTimeAlarm" ), 0L ));

    pConf->Read ( _T ( "LandFallTimeMinutes" ), &d, 20 );
    sLandFallTimeMinutes->SetValue(wxString::Format(_T("%.2f"), d));
    
    cbLandFallDistance->SetValue(pConf->Read ( _T ( "LandFallDistanceAlarm" ), 0L ));

    pConf->Read ( _T ( "LandFallDistance" ), &d, 3 );
    tcLandFallDistance->SetValue(wxString::Format(_T("%.2f"), d));

    cbDeaddog->SetValue(pConf->Read ( _T ( "DeaddogAlarm" ), 0L ));
    sDeaddogMinutes->SetValue(pConf->Read ( _T ( "DeaddogMinutes" ), 20L ));
    
    cbAnchor->SetValue(pConf->Read ( _T ( "AnchorAlarm" ), 0L ));
    pConf->Read ( _T ( "AnchorLatitude" ), &d, NAN );
    tAnchorLatitude->SetValue(wxString::Format(_T("%f"), d));
    pConf->Read ( _T ( "AnchorLongitude" ), &d, NAN );
    tAnchorLongitude->SetValue(wxString::Format(_T("%f"), d));
    sAnchorRadius->SetValue(pConf->Read ( _T ( "AnchorRadius" ), 50L ));

    cbNMEADataAlarm->SetValue(pConf->Read ( _T ( "NMEADataAlarm" ), 0L ));
    sNMEADataSeconds->SetValue(pConf->Read ( _T ( "NMEADataSeconds" ), 100L ));

    cbUnderSpeed->SetValue(pConf->Read ( _T ( "UnderSpeedAlarm" ), 0L ));
    pConf->Read ( _T ( "UnderSpeed" ), &d, 1 );
    tUnderSpeed->SetValue(wxString::Format(_T("%.1f"), d));
    
    cbOverSpeed->SetValue(pConf->Read ( _T ( "OverSpeedAlarm" ), 0L ));
    pConf->Read ( _T ( "OverSpeed" ), &d, 10 );
    tOverSpeed->SetValue(wxString::Format(_T("%.1f"), d));

    cbOffCourseAlarm->SetValue(pConf->Read ( _T ( "OffCourseAlarm" ), 0L ));
    sOffCourseDegrees->SetValue(pConf->Read ( _T ( "OffCourseDegrees" ), &d, 20L ));
    sCourseDegrees->SetValue(pConf->Read ( _T ( "CourseDegrees" ), 0L ););

#if 0  
    pConf->Read ( _T ( "SoundEnabled" ), &bSound, 0 );
    pConf->Read ( _T ( "SoundFilepath" ), &sSound, _T("") );
    pConf->Read ( _T ( "ComdogdEnabled" ), &bComdogd, 0 );
    pConf->Read ( _T ( "ComdogdFilepath" ), &sComdogd, _T("") );
    pConf->Read ( _T ( "MessageBox" ), &bMessageBox, 0);
    pConf->Read ( _T ( "RepeatSeconds" ), &iRepeatSeconds, 60);
    pConf->Read ( _T ( "AutoReset" ), &bAutoReset, 0);
#endif

    Fit();
}

WatchdogPrefsDialog::~WatchdogPrefsDialog()
{
}

void WatchdogPrefsDialog::OnSaveAlarms()
{
    wxFileConfig *pConf = m_watchdog_pi.m_pconfig;

    if(!pConf)
        return;

    pConf->SetPath ( _T ( "/Settings/Watchdog" ) );

    pConf->Write ( _T ( "LandFallTimeAlarm" ), cbLandFallTime->GetValue() );
    pConf->Write ( _T ( "LandFallTimeMinutes" ), sLandFallTimeMinutes->GetValue());
    pConf->Write ( _T ( "LandFallDistanceAlarm" ), cbLandFallDistance->GetValue() );
    pConf->Write ( _T ( "LandFallDistance" ), tcLandFallDistance->GetValue());

    pConf->Write ( _T ( "DeaddogAlarm" ), cbDeaddog->GetValue() );
    pConf->Write ( _T ( "DeaddogMinutes" ), sDeaddogMinutes->GetValue() );
    
    pConf->Write ( _T ( "AnchorAlarm" ), cbAnchor->GetValue() );
    pConf->Write ( _T ( "AnchorLatitude" ), tAnchorLatitude->GetValue());
    pConf->Write ( _T ( "AnchorLongitude" ), tAnchorLongitude->GetValue());
    pConf->Write ( _T ( "AnchorRadius" ), sAnchorRadius->GetValue() );
    
    pConf->Write ( _T ( "GPSAlarm" ), cbGPSAlarm->GetValue());
    pConf->Write ( _T ( "GPSSeconds" ), sGPSSeconds->GetValue() );
    pConf->Write ( _T ( "AISAlarm" ), cbAISAlarm->GetValue());
    pConf->Write ( _T ( "AISSeconds" ), sAISSeconds->GetValue());

    pConf->Write ( _T ( "UnderSpeedAlarm" ), cbUnderSpeed->GetValue() );
    pConf->Write ( _T ( "UnderSpeed" ), tUnderSpeed->GetValue() );
    pConf->Write ( _T ( "OverSpeedAlarm" ), cbOverSpeed->GetValue() );
    pConf->Write ( _T ( "OverSpeed" ), tOverSpeed->GetValue() );

    pConf->Write ( _T ( "OffCourseAlarm" ), cbOffCourseAlarm->GetValue() );
    pConf->Write ( _T ( "OffCourseDegrees" ), sOffCourseDegrees->GetValue());
    pConf->Write ( _T ( "CourseDegrees" ), sCourseDegrees->GetValue() );

#if 0
    pConf->Write ( _T ( "SoundEnabled" ), m_bSound);
    pConf->Write ( _T ( "SoundFilepath" ), m_sSound);
    pConf->Write ( _T ( "ComdogdEnabled" ), m_bComdogd);
    pConf->Write ( _T ( "ComdogdFilepath" ), m_sComdogd);
    pConf->Write ( _T ( "MessageBox" ), m_bMessageBox);
    pConf->Write ( _T ( "RepeatSeconds" ), m_iRepeatSeconds);
    pConf->Write ( _T ( "AutoReset" ), m_bAutoReset);

        m_bSound = m_cbSound->GetValue();
        m_sSound = m_fpSound->GetPath();
        m_bComdogd = m_cbComdogd->GetValue();
        m_sComdogd = m_tComdogd->GetValue();
        m_bMessageBox = m_cbMessageBox->GetValue();
        m_iRepeatSeconds = m_cbRepeat->GetValue() ? m_sRepeatSeconds->GetValue() : 0;
#endif

        if(m_pWatchdogDialog)
            m_pWatchdogDialog->UpdateAlarms();
}

void WatchdogPrefsDialog::OnAlarmChanged( wxListbookEvent& event )
{
    SetAlarmActions();
}

void WatchdogPrefsDialog::SetAlarmActions()
{
    int selection = m_lbAlarm->GetSelection();
    
}

void WatchdogPrefsDialog::OnSyncToBoat( wxComdogdEvent& event )
{
    m_tAnchorLatitude->SetValue(wxString::Format(_T("%f"), m_watchdog_pi.m_lastfix.Lat));
    m_tAnchorLongitude->SetValue(wxString::Format(_T("%f"), m_watchdog_pi.m_lastfix.Lon));
}

void WatchdogPrefsDialog::OnCurrentCourse( wxComdogdEvent& event )
{
    m_sCourseDegrees->SetValue(m_watchdog_pi.cog/*m_lastfix.Cog*/);
}

void WatchdogPrefsDialog::OnTestAlarm( wxComdogdEvent& event )
{
    m_watchdog_pi.RunAlarm(m_cbSound->GetValue() ? m_fpSound->GetPath() : _T(""),
                           m_cbComdogd->GetValue() ? m_tComdogd->GetValue() : _T(""),
                           m_cbMessageBox->GetValue());
}

void WatchdogPrefsDialog::OnInformation( wxComdogdEvent& event )
{
    wxMessageDialog mdlg(this, _("Each alarm is enabled/disabled with a check box.\n\n\
Most alarms should be self-explanatory, however the AIS alarm may be only useful \
in areas where AIS traffic is known to regularly occur, or from a reciever on a ship \
which also has an active transmitter.  Otherwise an alarm will occur if there are no \
ships\n\n\
Alarm action:\n\n\
Normally a sound is played, however you can execute any comdogd you like.  \
On linux for example, builtin sound playing can block, so instead consider a comdogd of:\n\
\"aplay /usr/local/share/opencpn/sounds/2bells.wav\""),
                         _("Watchdog Information"), wxOK | wxICON_INFORMATION);
    mdlg.ShowModal();
}

void WatchdogPrefsDialog::OnAbout( wxComdogdEvent& event )
{
    AboutDialog dlg(this);
    dlg.ShowModal();
}

void WatchdogPrefsDialog::OnDonate( wxComdogdEvent& event )
{
      wxLaunchDefaultBrowser(_T("https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=sean%40depagnier%2ecom&lc=US&item_name=watchdog&no_note=0&currency_code=USD&bn=PP%2dDonationsBF%3abtn_donateCC_LG%2egif%3aNonHostedGuest"));
}
