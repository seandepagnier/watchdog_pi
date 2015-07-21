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

class WatchdogPrefsDialog : public WatchdogPrefsDialogBase
{
public:
    WatchdogPrefsDialog( watchdog_pi &_watchdog_pi, wxWindow* parent);

    void OnEnabled( wxCommandEvent& event );

    void OnAlarmUpdateSound( wxFileDirPickerEvent& event ) { m_cbSound->SetValue(true); OnAlarmUpdate(); }
    void OnAlarmUpdateCommand( wxCommandEvent& event ) { m_cbCommand->SetValue(true); OnAlarmUpdate(); }
    void OnAlarmUpdateRepeat( wxSpinEvent& event ) { m_cbRepeat->SetValue(true); OnAlarmUpdate(); }

    void OnAlarmUpdate( wxCommandEvent& event ) { OnAlarmUpdate(event.IsChecked()); }
    void OnAlarmUpdateSpin( wxSpinEvent& event ) { OnAlarmUpdate(); }
    void OnAlarmUpdateRB( wxCommandEvent& event ) { OnAlarmUpdate(false); }

    void OnAlarmUpdate(bool enable=true);

    void OnAlarmChanged( wxListbookEvent& event );

    void OnCheckSecondDeadman( wxCommandEvent& );
    void OnCheckSeparatePortAndStarboard( wxCommandEvent& event );

    void OnSyncToBoat( wxCommandEvent& event );
    void OnGetBoundaryGUID( wxCommandEvent& event );
    void OnCurrentCourse( wxCommandEvent& event );
    void OnTestAlarm( wxCommandEvent& event );

    void ReadAlarmActions() { AlarmActions(true); }
    void WriteAlarmActions() { AlarmActions(false); }

private:
    void AlarmActions(bool read);

    int CurrentSelection();
    int PanelIndex(int alarm);

    Alarm *CurrentAlarm();
    
    void ConfigureDeadmanAlarms();
    void ConfigurePortAlarms();

    void OnInformation( wxCommandEvent& event );
    void OnAboutAuthor( wxCommandEvent& event );

    watchdog_pi &m_watchdog_pi;

    bool m_breading;
};
