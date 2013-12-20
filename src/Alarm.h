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

class Alarm {
public:
    wxString m_sName;

    bool m_bFired;

    bool m_bEnabled, m_bgfxenabled;
    bool m_bSound, m_bComdogd, m_bMessageBox, m_bAutoReset;
    
    virtual bool TestAlarm();
    void Reset() {
        if(m_bAutoReset)
            m_bFired = false;
    }

    void RunAlarm(enum AlarmName alarm);

private:
    wxDateTime m_LastAlarmTime;
};


class LandFallAlarm : public Alarm
{
public:
    wxDateTime m_LastLandFallCheck;
    LandFallAlarm() {
        m_LastLandFallCheck = wxDateTime::Now();
    }
    virtual bool TestAlarm();
};

class NMEAAlarm : public Alarm
{
public:
    enum AlarmName {LANDFALL, NMEADATA, DEADMAN, ANCHOR, COURSE,
                    UNDERSPEED, OVERSPEED, ALARMCOUNT};
