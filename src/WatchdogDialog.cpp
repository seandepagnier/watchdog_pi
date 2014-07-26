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
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/Settings/Watchdog" ) );
    m_cbDisableAllAlarms->SetValue(pConf->Read ( _T ( "DisableAllAlarms" ), false ));
}

WatchdogDialog::~WatchdogDialog()
{
}

void WatchdogDialog::UpdateAlarms()
{
    if(IsShown())
        Alarm::UpdateStatusAll();
    else
        m_fgAlarms->Show(false);

    Fit();
    Refresh();
}

void WatchdogDialog::OnDisableAllAlarms( wxCommandEvent& event )
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/Settings/Watchdog" ) );
    pConf->Write ( _T ( "DisableAllAlarms" ), m_cbDisableAllAlarms->GetValue());
}

void WatchdogDialog::OnPreferences( wxCommandEvent& event )
{
    m_watchdog_pi.ShowPreferencesDialog( this );
}

void WatchdogDialog::OnReset( wxCommandEvent& event )
{
    Alarm::ResetAll();
    UpdateAlarms();
}
