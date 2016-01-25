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

#include "WatchdogUI.h"

class watchdog_pi;

class ConfigurationDialog : public ConfigurationDialogBase
{
public:
    ConfigurationDialog( watchdog_pi &_watchdog_pi, wxWindow* parent);

    void OnEnabled( wxCommandEvent& event );

    void OnNewAlarm( wxCommandEvent& event );
    void OnEditAlarm( wxCommandEvent& event );
    void OnDeleteAlarm( wxCommandEvent& event );
    void OnDeleteAllAlarms( wxCommandEvent& event );
    void AlarmSelected( wxListEvent& event ) { UpdateStates(); }
    void OnDoubleClick( wxMouseEvent& event );

private:
    void UpdateStates();
    int CurrentSelection();
    Alarm *CurrentAlarm();

    void UpdateItem(int index);

    void OnInformation( wxCommandEvent& event );
    void OnAboutAuthor( wxCommandEvent& event );

    watchdog_pi &m_watchdog_pi;
    bool m_bOnEditAlarmOK;
//    watchdog_pi m_watchdog_pi;
};
