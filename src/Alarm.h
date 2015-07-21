/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  watch dog Plugin
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

class wdDC;
class Alarm : public wxEvtHandler {
public:
    static void RenderAll(wdDC &dc, PlugIn_ViewPort &vp);
    static void ConfigAll(bool load);
    static void ResetAll();
    static void UpdateStatusAll();
    static void RepopulateAll();
    static void NMEAString(const wxString &string);
    static void ConfigSecondDeadman(bool read, wxCheckBox *control);
    static void ConfigCoursePort(bool read, wxCheckBox *control);

    Alarm(int interval=1);

    void Run();
    virtual void SaveConfig();
    virtual void LoadConfig();

    void UpdateStatus();

    virtual void Repopulate();

    virtual wxString Name() = 0;
    virtual wxString ConfigName() { return Name(); }
    virtual bool Test() = 0;
    virtual wxString GetStatus() = 0;
    virtual void Render(wdDC &dc, PlugIn_ViewPort &vp) {}

    void OnTimer( wxTimerEvent & );
    virtual wxFileConfig *GetConfigObject();

protected:
    bool m_bEnabled, m_bgfxEnabled;
    bool m_bFired;

private:
    friend class WatchdogPrefsDialog;

    void ConfigItem(bool read, wxString name, wxControl *control);
    virtual void GetStatusControls(wxControl *&Text, wxControl *&status) { Text = status = NULL; }

    int m_interval;

    bool m_bSound, m_bCommand, m_bMessageBox, m_bRepeat, m_bAutoReset;
    wxString m_sSound, m_sCommand;
    int m_iRepeatSeconds;

    wxTimer    m_Timer;
    wxDateTime m_LastAlarmTime;
};

enum AlarmNames {LANDFALL, NMEADATA, DEADMAN, SECOND_DEADMAN, ANCHOR, BOUNDARY,
                 COURSE, COURSE_STARBOARD, UNDERSPEED, OVERSPEED, DEPTH, WIND};
extern Alarm *Alarms[];
