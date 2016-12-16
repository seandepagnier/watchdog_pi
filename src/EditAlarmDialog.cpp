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
#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "wxJSON/jsonreader.h"
#include "wxJSON/jsonwriter.h"

#include "watchdog_pi.h"
#include "WatchdogUI.h"

#include "EditAlarmDialog.h"
#include "ODAPI.h"

extern wxJSONValue g_ReceivedODVersionJSONMsg;
extern wxString    g_ReceivedODVersionMessage;

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
    m_alarm->m_bFired = false;
    m_alarm->m_bSpecial = false;
    m_alarm->m_bEnabled = true;

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

bool ODVersionNewerThan(int major, int minor, int patch)
{
    if(g_ReceivedODVersionMessage == wxEmptyString) return false;
    if(g_ReceivedODVersionJSONMsg[wxS("Major")].AsInt() > major) return true;
    if(g_ReceivedODVersionJSONMsg[wxS("Major")].AsInt() == major &&
        g_ReceivedODVersionJSONMsg[wxS("Minor")].AsInt() > minor) return true;
    if(g_ReceivedODVersionJSONMsg[wxS("Major")].AsInt() == major &&
        g_ReceivedODVersionJSONMsg[wxS("Minor")].AsInt() == minor &&
        g_ReceivedODVersionJSONMsg[wxS("Patch")].AsInt() >= patch) return true;
    return false;
}

void GetODVersion( void )
{
    wxJSONValue jMsg;
    wxJSONWriter writer;
    wxString    MsgString;
    jMsg[wxS("Source")] = wxS("WATCHDOG_PI");
    jMsg[wxT("Type")] = wxT("Request");
    jMsg[wxT("Msg")] = wxS("Version");
    jMsg[wxT("MsgId")] = wxS("version");
    writer.Write( jMsg, MsgString );
    SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
}

void BoundaryPanel::OnGetBoundaryGUID( wxCommandEvent& event )
{
    extern wxJSONValue g_ReceivedBoundaryGUIDJSONMsg;
    extern wxString    g_ReceivedBoundaryGUIDMessage;
    extern wxString    g_BoundaryName;
    extern wxString    g_BoundaryDescription;
    extern wxString    g_BoundaryGUID;
    
    wxJSONValue jMsg;
    wxJSONWriter writer;
    wxString    MsgString;
    jMsg[wxT("Source")] = wxT("WATCHDOG_PI");
    jMsg[wxT("Type")] = wxT("Request");
    jMsg[wxT("Msg")] = wxS("FindPointInAnyBoundary");
    jMsg[wxT("MsgId")] = wxS("GetGUID");
    jMsg[wxT("lat")] = g_watchdog_pi->LastFix().Lat;
    jMsg[wxT("lon")] = g_watchdog_pi->LastFix().Lon;
    jMsg[wxS("BoundaryType")] = wxT("Any");
    writer.Write( jMsg, MsgString );
    g_ReceivedBoundaryGUIDMessage = wxEmptyString;
    SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
    if(g_ReceivedBoundaryGUIDMessage != wxEmptyString && g_ReceivedBoundaryGUIDJSONMsg[wxT("MsgId")].AsString() == wxS("GetGUID") && g_ReceivedBoundaryGUIDJSONMsg[wxT("Found")].AsBool() == true ) {
        m_tBoundaryGUID->SetValue( g_ReceivedBoundaryGUIDJSONMsg[wxT("GUID")].AsString() );
        g_BoundaryGUID = m_tBoundaryGUID->GetValue();
        g_BoundaryName = g_ReceivedBoundaryGUIDJSONMsg[wxT("Name")].AsString();
        g_BoundaryDescription = g_ReceivedBoundaryGUIDJSONMsg[wxT("Description")].AsString();
    } else {
        m_tBoundaryGUID->Clear();
        g_BoundaryGUID = wxEmptyString;
        g_BoundaryName = wxEmptyString;
        g_BoundaryDescription = wxEmptyString;
    }
}

void BoundaryPanel::OnBoundaryGUIDKillFocus( wxFocusEvent& event )
{
    extern wxString    g_BoundaryName;
    extern wxString    g_BoundaryDescription;
    extern wxString    g_BoundaryGUID;
    extern wxJSONValue g_ReceivedPathGUIDJSONMsg;
    extern wxString    g_ReceivedPathGUIDMessage;
    extern wxJSONValue g_ReceivedODAPIJSONMsg;
    extern wxString    g_ReceivedODAPIMessage;
    
    wxJSONValue     jMsg;
    wxJSONWriter    writer;
    wxString        MsgString;
    
    bool            l_bCheckDone = false;
    
    if(m_tBoundaryGUID->GetValue().Len() == 0) {
        event.Skip();
        return;
    }
    
    GetODVersion();
    if(ODVersionNewerThan( 1, 1, 15)) {
        jMsg[wxT("Source")] = wxT("WATCHDOG_PI");
        jMsg[wxT("Type")] = wxT("Request");
        jMsg[wxT("Msg")] = wxS("GetAPIAddresses");
        jMsg[wxT("MsgId")] = wxS("GetAPIAddresses");
        writer.Write( jMsg, MsgString );
        SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
        if(g_ReceivedODAPIMessage != wxEmptyString &&  g_ReceivedODAPIJSONMsg[wxT("MsgId")].AsString() == wxS("GetAPIAddresses")) {
            wxString sptr = g_ReceivedODAPIJSONMsg[_T("OD_FindPathByGUID")].AsString();
            if(sptr != _T("null")) {
                l_bCheckDone = true;
                OD_FindPathByGUID pOD_FindPathByGUID;
                sscanf(sptr.To8BitData().data(), "%p", &pOD_FindPathByGUID);
                wxString l_sName;
                wxString l_sDescription;
                if((*pOD_FindPathByGUID)(m_tBoundaryGUID->GetValue(), &l_sName, &l_sDescription)) {
                    g_BoundaryName = l_sName;
                    g_BoundaryDescription = l_sDescription;
                    g_BoundaryGUID = m_tBoundaryGUID->GetValue();
                } else {
                    wxString l_s = _T(" ") + wxString(_("Error!")) + _T("\n") 
                    + _("GUID") + _T(": ") + m_tBoundaryGUID->GetValue() + _(" does not exist");
                    wxMessageDialog mdlg(GetOCPNCanvasWindow(), l_s, _("Watchman"), wxOK | wxICON_WARNING);
                    mdlg.ShowModal();
                    m_tBoundaryGUID->SetFocus();
                }
            }
        }
    } 
    
    if(!l_bCheckDone) {
        jMsg[wxT("Source")] = wxT("WATCHDOG_PI");
        jMsg[wxT("Type")] = wxT("Request");
        jMsg[wxT("Msg")] = wxS("FindPathByGUID");
        jMsg[wxT("MsgId")] = wxS("inclusion");
        jMsg[wxS("GUID")] = m_tBoundaryGUID->GetValue();
        writer.Write( jMsg, MsgString );
        g_ReceivedPathGUIDMessage = wxEmptyString;
        SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
        if(g_ReceivedPathGUIDMessage != wxEmptyString && 
            g_ReceivedPathGUIDJSONMsg[wxT("MsgId")].AsString() == wxS("inclusion") && 
            g_ReceivedPathGUIDJSONMsg[wxT("Found")].AsBool() == true ) {
            g_BoundaryName = g_ReceivedPathGUIDJSONMsg[wxS("Name")].AsString();
        g_BoundaryDescription = g_ReceivedPathGUIDJSONMsg[wxS("Description")].AsString();
        g_BoundaryGUID = g_ReceivedPathGUIDJSONMsg[wxS("GUID")].AsString();
            } else {
                wxString l_s = _T(" ") + wxString(_("Error!")) + _T("\n") 
                + _("GUID") + _T(": ") + m_tBoundaryGUID->GetValue() + _(" does not exist");
                wxMessageDialog mdlg(GetOCPNCanvasWindow(), l_s, _("Watchman"), wxOK | wxICON_WARNING);
                mdlg.ShowModal();
                m_tBoundaryGUID->SetFocus();
            }
    }
    event.Skip();
}

void BoundaryPanel::OnGuardZoneGUIDKillFocus( wxFocusEvent& event )
{
    extern wxString    g_GuardZoneName;
    extern wxString    g_GuardZoneDescription;
    extern wxString    g_GuardZoneGUID;
    extern wxJSONValue g_ReceivedPathGUIDJSONMsg;
    extern wxString    g_ReceivedPathGUIDMessage;
    extern wxJSONValue g_ReceivedODAPIJSONMsg;
    extern wxString    g_ReceivedODAPIMessage;
    
    bool                l_bCheckDone = false;
    
    wxJSONValue jMsg;
    wxJSONWriter writer;
    wxString    MsgString;

    if(m_tGuardZoneGUID->GetValue().Len() == 0) {
        event.Skip();
        return;
    }
    
    GetODVersion();
    if(ODVersionNewerThan( 1, 1, 15)) {
        jMsg[wxT("Source")] = wxT("WATCHDOG_PI");
        jMsg[wxT("Type")] = wxT("Request");
        jMsg[wxT("Msg")] = wxS("GetAPIAddresses");
        jMsg[wxT("MsgId")] = wxS("GetAPIAddresses");
        jMsg[wxS("GUID")] = m_tBoundaryGUID->GetValue();
        writer.Write( jMsg, MsgString );
        SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
        if(g_ReceivedODAPIMessage != wxEmptyString &&  g_ReceivedODAPIJSONMsg[wxT("MsgId")].AsString() == wxS("GetAPIAddresses")) {
            wxString sptr = g_ReceivedODAPIJSONMsg[_T("OD_FindPathByGUID")].AsString();
            if(sptr != _T("null")) {
                l_bCheckDone = true;
                OD_FindPathByGUID pOD_FindPathByGUID;
                sscanf(sptr.To8BitData().data(), "%p", &pOD_FindPathByGUID);
                wxString l_sName;
                wxString l_sDescription;
                if((*pOD_FindPathByGUID)(m_tGuardZoneGUID->GetValue(), &l_sName, &l_sDescription)) {
                    g_GuardZoneName = l_sName;
                    g_GuardZoneDescription = l_sDescription;
                    g_GuardZoneGUID = m_tBoundaryGUID->GetValue();
                } else {
                    wxString l_s = _T(" ") + wxString(_("Error!")) + _T("\n") 
                    + _("GUID") + _T(": ") + m_tGuardZoneGUID->GetValue() + _(" does not exist");
                    wxMessageDialog mdlg(GetOCPNCanvasWindow(), l_s, _("Watchman"), wxOK | wxICON_WARNING);
                    mdlg.ShowModal();
                    m_tGuardZoneGUID->SetFocus();
                }
            }
        }
    } 
    if(!l_bCheckDone) {
        jMsg[wxT("Source")] = wxT("WATCHDOG_PI");
        jMsg[wxT("Type")] = wxT("Request");
        jMsg[wxT("Msg")] = wxS("FindPathByGUID");
        jMsg[wxT("MsgId")] = wxS("guard");
        jMsg[wxS("GUID")] = m_tGuardZoneGUID->GetValue();
        writer.Write( jMsg, MsgString );
        g_ReceivedPathGUIDMessage = wxEmptyString;
        SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
        if(g_ReceivedPathGUIDMessage != wxEmptyString && 
            g_ReceivedPathGUIDJSONMsg[wxT("MsgId")].AsString() == wxS("guard") && 
            g_ReceivedPathGUIDJSONMsg[wxT("Found")].AsBool() == true ) {
            g_GuardZoneName = g_ReceivedPathGUIDJSONMsg[wxS("Name")].AsString();
            g_GuardZoneDescription = g_ReceivedPathGUIDJSONMsg[wxS("Description")].AsString();
            g_GuardZoneGUID = g_ReceivedPathGUIDJSONMsg[wxS("GUID")].AsString();
        } else {
            wxString l_s = _T(" ") + wxString(_("Error!")) + _T("\n") 
                    + _("GUID") + _T(": ") + m_tGuardZoneGUID->GetValue() + _(" does not exist");
            wxMessageDialog mdlg(GetOCPNCanvasWindow(), l_s, _("Watchman"), wxOK | wxICON_WARNING);
            mdlg.ShowModal();
            m_tGuardZoneGUID->SetFocus();
        }
    }
    event.Skip();
}

void BoundaryPanel::OnRadioButton(wxCommandEvent& event)
{
    switch(event.GetId()) {
        case GPSCOURSE: 
        case GPSFIX: 
        case AISGUARDZONE:
            m_radioBoxBoundaryType->Enable();
            break;
        case ANCHORALARM:
            m_radioBoxBoundaryType->Disable();
            break;
        default:
            m_radioBoxBoundaryType->Enable();
            break;
    }
        
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



