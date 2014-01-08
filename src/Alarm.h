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

class Alarm : public wxEvtHandler {
public:
    static void RenderAlarms(PlugIn_ViewPort &vp);

    Alarm(wxString name, int interval=1);

    void Run();
    void Reset();
    virtual void SaveConfig();
    virtual void LoadConfig();

    virtual bool Test() = 0;
    virtual void Render(PlugIn_ViewPort &vp) {}
    virtual wxString AlarmStatus() = 0;

    void OnTimer( wxTimerEvent & );

protected:
    wxFileConfig *GetConfigObject();

    bool m_bEnabled, m_bgfxEnabled;

private:
    friend class WatchdogPrefsDialog;

    void ConfigItem(bool read, wxString name, wxControl *control);

    wxString m_sName;

    bool m_bFired;

    bool m_bSound, m_bCommand, m_bMessageBox, m_bAutoReset;
    wxString m_sSound, m_sCommand;
    int m_iRepeatSeconds;

    wxTimer    m_Timer;
    wxDateTime m_LastAlarmTime;
};

enum AlarmNames {LANDFALL, NMEADATA, DEADMAN, ANCHOR, COURSE, UNDERSPEED, OVERSPEED, DEPTH, WIND};
extern Alarm *Alarms[];
