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

class WatchdogDialog: public WatchdogDialogBase
{
public:
    WatchdogDialog( watchdog_pi &_watchdog_pi, wxWindow* parent);
    ~WatchdogDialog();

    void UpdateAlarms();
    void UpdateStatus(int index);

    void OnLeftDown( wxMouseEvent& event );
    void OnDoubleClick( wxMouseEvent& event );
    void OnConfiguration( wxCommandEvent& event );
    void OnReset( wxCommandEvent& event );
    void OnClose( wxCommandEvent& event ) { Hide(); }

private:
    watchdog_pi &m_watchdog_pi;
};
