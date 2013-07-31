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
#include "WatchmanPrefsDialog.h"
#include "AboutDialog.h"

WatchmanPrefsDialog::WatchmanPrefsDialog( watchman_pi &_watchman_pi, wxWindow* parent)
    : WatchmanPrefsDialogBase( parent ), m_watchman_pi(_watchman_pi)
{
}

void WatchmanPrefsDialog::OnSyncToBoat( wxCommandEvent& event )
{
    m_tAnchorLatitude->SetValue(wxString::Format(_T("%f"), m_watchman_pi.m_lastfix.Lat));
    m_tAnchorLongitude->SetValue(wxString::Format(_T("%f"), m_watchman_pi.m_lastfix.Lon));
}

void WatchmanPrefsDialog::OnCurrentCourse( wxCommandEvent& event )
{
    m_sCourseDegrees->SetValue(m_watchman_pi.cog/*m_lastfix.Cog*/);
}

void WatchmanPrefsDialog::OnTestAlarm( wxCommandEvent& event )
{
    m_watchman_pi.RunAlarm(m_cbSound->GetValue() ? m_fpSound->GetPath() : _T(""),
                           m_cbCommand->GetValue() ? m_tCommand->GetValue() : _T(""),
                           m_cbMessageBox->GetValue());
}

void WatchmanPrefsDialog::OnInformation( wxCommandEvent& event )
{
    wxMessageDialog mdlg(this, _("Each alarm is enabled/disabled with a check box.\n\n\
Most alarms should be self-explanatory, however the AIS alarm may be only useful \
in areas where AIS traffic is known to regularly occur, or from a reciever on a ship \
which also has an active transmitter.  Otherwise an alarm will occur if there are no \
ships\n\n\
Alarm action:\n\n\
Normally a sound is played, however you can execute any command you like.  \
On linux for example, builtin sound playing can block, so instead consider a command of:\n\
\"aplay /usr/local/share/opencpn/sounds/2bells.wav\""),
                         _("Watchman Information"), wxOK | wxICON_INFORMATION);
    mdlg.ShowModal();
}

void WatchmanPrefsDialog::OnAbout( wxCommandEvent& event )
{
    AboutDialog dlg(this);
    dlg.ShowModal();
}
