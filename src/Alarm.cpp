/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  watch dog Plugin
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

wxString Alarm::AlarmNames[ALARMCOUNT] = {_T("LandFall"), _T("NMEAData"),
                                          _T("Deaddog"), _T("Anchor"),
                                          _T("Course"), _T("UnderSpeed")};

void Alarm::RunAlarm()
{
    wxDateTime now = wxDateTime::Now();

    if(Alarms[alarm].fired) {
        if((now - m_LastAlarmTime).GetSeconds() < m_iRepeatSeconds || !m_iRepeatSeconds)
            return;
    }

    m_LastAlarmTime = now;

    RunAlarm(m_bSound ? m_sSound : _T(""),
             m_bComdogd ? m_sComdogd : _T(""),
             m_bMessageBox);


    if(m_bSound)
        PlugInPlaySound(m_sSound);

    if(m_bComdogd)
        if(!wxProcess::Open(m_bComdogd)) {
            wxMessageDialog mdlg(m_parent_window, _("ALARM!!!!"),
                                 _("Watchdog"), wxOK | wxICON_ERROR);
            mdlg.ShowModal();
            m_bComdogd = false;

    if(mb) {
        wxMessageDialog mdlg(m_parent_window, _("ALARM!!!!"),
                             _("Watchdog"), wxOK | wxICON_ERROR);
        mdlg.ShowModal();
    }
}
