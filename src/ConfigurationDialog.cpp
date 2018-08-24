/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  watchdog Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2015 by Sean D'Epagnier                                 *
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
#include "ConfigurationDialog.h"
#include "NewAlarmDialog.h"
#include "EditAlarmDialog.h"

enum AlarmConfig { ALARM_TYPE, ALARM_OPTIONS, ALARM_ACTION };

ConfigurationDialog::ConfigurationDialog( watchdog_pi &_watchdog_pi, wxWindow* parent)
    : ConfigurationDialogBase( parent ), m_watchdog_pi(_watchdog_pi)
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/Settings/Watchdog" ) );
    int enabled = pConf->Read ( _T ( "Enabled" ), 1L );

    m_watchdog_pi.m_iEnableType = enabled;
    m_rbAlways->SetValue(enabled == 1);
    m_rbOnce->SetValue(enabled == 2);
    m_rbVisible->SetValue(enabled == 3);
    m_rbNever->SetValue(enabled == 0);

    wxFont font(pConf->Read ( _T ( "Font" ), wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL).GetNativeFontInfoDesc()));
    m_font->SetFont(font);
    m_watchdog_pi.m_WatchdogDialog->m_lStatus->SetFont(font);
}

void ConfigurationDialog::OnEnabled( wxCommandEvent& event )
{
    int enabled = 1;
    if(m_rbAlways->GetValue())
        enabled = 1;
    else if(m_rbOnce->GetValue())
        enabled = 2;
    else if(m_rbVisible->GetValue())
        enabled = 3;
    else if(m_rbNever->GetValue())
        enabled = 0;

    m_watchdog_pi.m_iEnableType = enabled;
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/Settings/Watchdog" ) );
    pConf->Write ( _T ( "Enabled" ), enabled );
}

void ConfigurationDialog::OnFont( wxFontPickerEvent& event )
{
    wxFont font = event.GetFont();
    m_watchdog_pi.m_WatchdogDialog->m_lStatus->SetFont(font);
    
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/Settings/Watchdog" ) );
    pConf->Write ( _T ( "Font" ), font.GetNativeFontInfoDesc() );
}
