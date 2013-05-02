/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  weather fax Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2013 by Sean D'Epagnier                                 *
 *   sean at depagnier dot com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
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

#include <vector>

#include "WatchmanUI.h"

class watchman_pi;

class WatchmanDialog: public WatchmanDialogBase
{
public:
    WatchmanDialog( watchman_pi &_watchman_pi, wxWindow* parent);

    ~WatchmanDialog();

    virtual void OnPreferences( wxCommandEvent& event ) { m_watchman_pi.ShowPreferencesDialog(this); }
    virtual void OnClose( wxCommandEvent& event ) { Hide(); }
    void UpdateLandFallTime(PlugIn_Position_Fix_Ex &pfix);

protected:

    watchman_pi &m_watchman_pi;

private:
    void OnTimer( wxTimerEvent & );
    wxTimer m_Timer;
};
