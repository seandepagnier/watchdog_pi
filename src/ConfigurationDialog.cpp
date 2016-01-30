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

    m_lAlarms->InsertColumn(ALARM_TYPE, _("Type"));
    m_lAlarms->InsertColumn(ALARM_OPTIONS, _("Options"));
    m_lAlarms->InsertColumn(ALARM_ACTION, _("Action"));

    for(unsigned int i=0; i<Alarm::s_Alarms.size(); i++) {
        wxListItem item;
        m_lAlarms->InsertItem(i, item);
        UpdateItem(i);
    }
    UpdateStates();
    
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

void ConfigurationDialog::OnNewAlarm( wxCommandEvent& event )
{
    NewAlarmDialog dlg(this);
    if(dlg.ShowModal() == wxID_CANCEL)
        return;

    if(Alarm::NewAlarm((AlarmType)dlg.m_lAlarmType->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) == NULL) return;

    
    wxListItem item;
    long index = m_lAlarms->InsertItem(m_lAlarms->GetItemCount(), item);

    m_lAlarms->SetItemState(m_lAlarms->GetItemCount() - 1,
                            wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    g_watchdog_pi->m_WatchdogDialog->UpdateAlarms();

    UpdateItem(index);
    UpdateStates();

    OnEditAlarm(event);
    if(m_bOnEditAlarmOK) return;
    else OnDeleteAlarm(event);
}

void ConfigurationDialog::OnEditAlarm( wxCommandEvent& event )
{
    EditAlarmDialog dlg(this, CurrentAlarm());
    if(dlg.ShowModal() == wxID_OK) {
        dlg.Save();
        UpdateItem(CurrentSelection());
        m_bOnEditAlarmOK = true;
    } else
        m_bOnEditAlarmOK = false;
}

void ConfigurationDialog::OnDeleteAlarm( wxCommandEvent& event )
{
    std::vector<Alarm*>::iterator it = Alarm::s_Alarms.begin();
    for(int i=0; i<CurrentSelection(); i++)
        it++;
    delete *it;
    Alarm::s_Alarms.erase(it);
    m_lAlarms->DeleteItem(CurrentSelection());

    UpdateStates();

    g_watchdog_pi->m_WatchdogDialog->UpdateAlarms();
}

void ConfigurationDialog::OnDeleteAllAlarms( wxCommandEvent& event )
{
    for(unsigned int i=0; i<Alarm::s_Alarms.size(); i++)
        m_lAlarms->DeleteItem(0);
    Alarm::DeleteAll();
    Alarm::s_Alarms.clear();

    UpdateStates();
    g_watchdog_pi->m_WatchdogDialog->UpdateAlarms();
}

void ConfigurationDialog::OnDoubleClick( wxMouseEvent& event )
{
    if(CurrentSelection() < 0)
        return;

    wxCommandEvent e;
    OnEditAlarm(e);
}

void ConfigurationDialog::UpdateStates()
{
    bool enable = CurrentSelection() >= 0;
    m_bEdit->Enable(enable);
    m_bDelete->Enable(enable);
    if(m_lAlarms->GetItemCount() > 0)
        m_bDeleteAll->Enable( true );
    else
        m_bDeleteAll->Enable( false );
    this->GetSizer()->Fit( this );
    this->Layout();
    
}

int ConfigurationDialog::CurrentSelection()
{
    return m_lAlarms->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
}

Alarm *ConfigurationDialog::CurrentAlarm()
{
    return Alarm::s_Alarms[CurrentSelection()];
}

void ConfigurationDialog::UpdateItem(int index)
{
    Alarm *alarm = Alarm::s_Alarms[index];

    m_lAlarms->SetItem(index, ALARM_TYPE, alarm->Type());
    m_lAlarms->SetColumnWidth(ALARM_TYPE, wxLIST_AUTOSIZE);
    m_lAlarms->SetItem(index, ALARM_OPTIONS, alarm->Options());
    m_lAlarms->SetColumnWidth(ALARM_OPTIONS, wxLIST_AUTOSIZE);
    m_lAlarms->SetItem(index, ALARM_ACTION, alarm->Action());
    m_lAlarms->SetColumnWidth(ALARM_ACTION, wxLIST_AUTOSIZE);
}

void ConfigurationDialog::OnAboutAuthor( wxCommandEvent& event )
{
    wxLaunchDefaultBrowser(_T(ABOUT_AUTHOR_URL));
}
