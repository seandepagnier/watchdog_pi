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

#include "wxJSON/jsonreader.h"
#include "wxJSON/jsonwriter.h"

#include "watchdog_pi.h"

#include "EditAlarmDialog.h"

EditAlarmDialog::EditAlarmDialog(wxWindow* parent, Alarm *alarm)
    : EditAlarmDialogBase(parent), m_alarm(alarm)
{
    m_cbSound->SetValue(m_alarm->m_bSound);
    m_fpSound->SetPath(m_alarm->m_sSound);

    m_tCommand->SetValue(m_alarm->m_sCommand);
    m_cbCommand->SetValue(m_alarm->m_bCommand);
    m_cbMessageBox->SetValue(m_alarm->m_bMessageBox);
    m_cbAutoReset->SetValue(m_alarm->m_bAutoReset);
    m_cbRepeat->SetValue(m_alarm->m_bRepeat);
    m_sRepeatSeconds->SetValue(m_alarm->m_iRepeatSeconds);
    m_cbgfxEnabled->Enable(m_alarm->m_bHasGraphics);
    m_cbgfxEnabled->SetValue(m_alarm->m_bgfxEnabled);

    m_fgSizer->Insert( 0, m_alarm->OpenPanel(this), 1, wxEXPAND, 5 );

    m_fgSizer->Fit( this );
}

void EditAlarmDialog::Save()
{
    m_alarm->m_bSound = m_cbSound->GetValue();
    m_alarm->m_sSound = m_fpSound->GetPath();
    m_alarm->m_bCommand = m_cbCommand->GetValue();
    m_alarm->m_sCommand = m_tCommand->GetValue();
    m_alarm->m_bMessageBox = m_cbMessageBox->GetValue();
    m_alarm->m_bAutoReset = m_cbAutoReset->GetValue();
    m_alarm->m_bRepeat = m_cbRepeat->GetValue();
    m_alarm->m_iRepeatSeconds = m_sRepeatSeconds->GetValue();
    m_alarm->m_bgfxEnabled = m_cbgfxEnabled->GetValue();

    m_alarm->SavePanel(m_fgSizer->GetItem((size_t)0)->GetWindow());
}

void EditAlarmDialog::OnTestAlarm( wxCommandEvent& event )
{
    TestAlarm testalarm;
    Alarm *alarm = m_alarm;
    m_alarm = &testalarm;
    Save();
    m_alarm->Run();
    m_alarm = alarm;
}

void EditAlarmDialog::OnInformation( wxCommandEvent& event )
{
    wxMessageDialog mdlg(this, _("\
Most alarms should be self-explanatory\n\
Normally a sound is played, however you can execute any command you like, eg:  \
\"aplay /usr/local/share/opencpn/sounds/2bells.wav\"\n\
Messagebox may be useful to interrupt other applications (like a movie player)\n\
Automatically Reset will reset the alarm once it is no longer triggered, and it may \
be triggered again later."),
                         _("Watchdog Information"), wxOK | wxICON_INFORMATION);
    mdlg.ShowModal();
}

void BoundaryPanel::OnGetBoundaryGUID( wxCommandEvent& event )
{
    extern wxJSONValue g_ReceivedBoundaryGUIDJSONMsg;
    extern wxString    g_ReceivedBoundaryGUIDMessage;

    wxJSONValue jMsg;
    wxJSONWriter writer;
    wxString    MsgString;
    wxJSONValue v;
    v[_T("GUID")] = wxT("GUID");
    jMsg[wxT("Source")] = wxT("WATCHDOG_PI");
    jMsg[wxT("Type")] = wxT("Request");
    jMsg[wxT("Msg")] = wxS("FindPointInAnyBoundary");
    jMsg[wxT("MsgId")] = wxS("GetGUID");
    jMsg[wxT("lat")] = g_watchdog_pi->LastFix().Lat;
    jMsg[wxT("lon")] = g_watchdog_pi->LastFix().Lon;
    writer.Write( jMsg, MsgString );
    g_ReceivedBoundaryGUIDMessage = wxEmptyString;
    SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
    if(g_ReceivedBoundaryGUIDMessage != wxEmptyString && g_ReceivedBoundaryGUIDJSONMsg[wxT("MsgId")].AsString() == wxS("GetGUID") && g_ReceivedBoundaryGUIDJSONMsg[wxT("Found")].AsBool() == true ) {
        m_tBoundaryGUID->SetValue( g_ReceivedBoundaryGUIDJSONMsg[wxT("GUID")].AsString() );
    } else
        m_tBoundaryGUID->Clear();
}

void AnchorPanel::OnSyncToBoat( wxCommandEvent& event )
{
    m_tLatitude->SetValue(wxString::Format(_T("%f"), g_watchdog_pi->LastFix().Lat));
    m_tLongitude->SetValue(wxString::Format(_T("%f"), g_watchdog_pi->LastFix().Lon));
}

void CoursePanel::OnCurrentCourse( wxCommandEvent& event )
{
    m_sCourse->SetValue(g_watchdog_pi->m_cog);
}
