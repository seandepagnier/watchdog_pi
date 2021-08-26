/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  watchdog Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2018 by Sean D'Epagnier                                 *
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
#include <wx/wx.h>

#include "json/json.h"

#include "EditAlarmDialog.h"
#include "WatchdogUI.h"
#include "watchdog_pi.h"

#include "WindPanel.h"
#include "WeatherPanel.h"
#include "ODAPI.h"

extern Json::Value g_ReceivedODVersionJSONMsg;
extern wxString    g_ReceivedODVersionMessage;

EditAlarmDialog::EditAlarmDialog(wxWindow* parent, Alarm *alarm)
    : EditAlarmDialogBase(parent), m_alarm(alarm)
{
    m_cbSound->SetValue(m_alarm->m_bSound);
    m_fpSound->SetPath(m_alarm->m_sSound);

    m_tCommand->SetValue(m_alarm->m_sCommand);
    m_cbCommand->SetValue(m_alarm->m_bCommand);
    m_cbMessageBox->SetValue(m_alarm->m_bMessageBox);
    m_cbNoData->SetValue(m_alarm->m_bNoData);
    m_cbAutoReset->SetValue(m_alarm->m_bAutoReset);
    m_cbRepeat->SetValue(m_alarm->m_bRepeat);
    m_sRepeatSeconds->SetValue(m_alarm->m_iRepeatSeconds);
    m_sDelay->SetValue(m_alarm->m_iDelay);
    m_cbgfxEnabled->Enable(m_alarm->m_bHasGraphics);
    m_cbgfxEnabled->SetValue(m_alarm->m_bgfxEnabled);

    //  setting does nothing for these alarms
    if(alarm->Type() == _("NMEA Data") || alarm->Type() == _("Deadman"))
        m_cbNoData->Disable();

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
    m_alarm->m_bNoData = m_cbNoData->GetValue();
    m_alarm->m_bAutoReset = m_cbAutoReset->GetValue();
    m_alarm->m_bRepeat = m_cbRepeat->GetValue();
    m_alarm->m_iRepeatSeconds = m_sRepeatSeconds->GetValue();
    m_alarm->m_iDelay = m_sDelay->GetValue();
    m_alarm->m_bgfxEnabled = m_cbgfxEnabled->GetValue();
    m_alarm->m_bFired = false;
    m_alarm->m_bSpecial = false;
    m_alarm->m_bEnabled = true;

    m_alarm->SavePanel(m_fgSizer->GetItem((size_t)0)->GetWindow());
}

void EditAlarmDialog::OnTestAlarm( wxCommandEvent& )
{
    TestAlarm testalarm;
    Alarm *alarm = m_alarm;
    m_alarm = &testalarm;
    Save();
    m_alarm->Run();
    m_alarm = alarm;
}

void EditAlarmDialog::OnInformation( wxCommandEvent& )
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
    if(g_ReceivedODVersionMessage == wxEmptyString) {
        GetODVersion();
        if(g_ReceivedODVersionMessage == wxEmptyString) return false;
    }
    if(g_ReceivedODVersionJSONMsg["Major"].asInt() > major) return true;
    if(g_ReceivedODVersionJSONMsg["Major"].asInt() == major &&
        g_ReceivedODVersionJSONMsg["Minor"].asInt() > minor) return true;
    if(g_ReceivedODVersionJSONMsg["Major"].asInt() == major &&
        g_ReceivedODVersionJSONMsg["Minor"].asInt() == minor &&
        g_ReceivedODVersionJSONMsg["Patch"].asInt() >= patch) return true;
    return false;
}

void GetODVersion( void )
{
    Json::Value jMsg;
    Json::FastWriter writer;
    jMsg["Source"] = "WATCHDOG_PI";
    jMsg["Type"] = "Request";
    jMsg["Msg"] = "Version";
    jMsg["MsgId"] = "version";
    SendPluginMessage( "OCPN_DRAW_PI", writer.write( jMsg ) );
}

void BoundaryPanel::OnGetBoundaryGUID( wxCommandEvent& )
{
    extern Json::Value g_ReceivedBoundaryGUIDJSONMsg;
    extern wxString    g_ReceivedBoundaryGUIDMessage;
    extern wxString    g_BoundaryName;
    extern wxString    g_BoundaryDescription;
    extern wxString    g_BoundaryGUID;
    
    Json::Value jMsg;
    Json::FastWriter writer;
    jMsg["Source"] = "WATCHDOG_PI";
    jMsg["Type"] = "Request";
    jMsg["Msg"] = "FindPointInAnyBoundary";
    jMsg["MsgId"] = "GetGUID";
    jMsg["lat"] = g_watchdog_pi->LastFix().Lat;
    jMsg["lon"] = g_watchdog_pi->LastFix().Lon;
    jMsg["BoundaryType"] = "Any";
    g_ReceivedBoundaryGUIDMessage = wxEmptyString;
    SendPluginMessage( "OCPN_DRAW_PI", writer.write( jMsg ));
    if(g_ReceivedBoundaryGUIDMessage != wxEmptyString && g_ReceivedBoundaryGUIDJSONMsg["MsgId"].asString() == "GetGUID" && g_ReceivedBoundaryGUIDJSONMsg["Found"].asBool() == true ) {
        m_tBoundaryGUID->SetValue( g_ReceivedBoundaryGUIDJSONMsg["GUID"].asString() );
        g_BoundaryGUID = m_tBoundaryGUID->GetValue();
        g_BoundaryName = g_ReceivedBoundaryGUIDJSONMsg["Name"].asString();
        g_BoundaryDescription = g_ReceivedBoundaryGUIDJSONMsg["Description"].asString();
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
    extern Json::Value g_ReceivedPathGUIDJSONMsg;
    extern wxString    g_ReceivedPathGUIDMessage;
    extern Json::Value g_ReceivedODAPIJSONMsg;
    extern wxString    g_ReceivedODAPIMessage;
    
    Json::Value     jMsg;
    Json::FastWriter    writer;
    
    bool            l_bCheckDone = false;
    
    if(m_tBoundaryGUID->GetValue().Len() == 0) {
        event.Skip();
        return;
    }
    
    if(ODVersionNewerThan( 1, 1, 15)) {
        jMsg["Source"] = "WATCHDOG_PI";
        jMsg["Type"] = "Request";
        jMsg["Msg"] = "GetAPIAddresses";
        jMsg["MsgId"] = "GetAPIAddresses";
        SendPluginMessage( "OCPN_DRAW_PI", writer.write( jMsg ) );
        if(g_ReceivedODAPIMessage != wxEmptyString &&  g_ReceivedODAPIJSONMsg["MsgId"].asString() == "GetAPIAddresses") {
            wxString sptr = g_ReceivedODAPIJSONMsg["OD_FindPathByGUID"].asString();
            if(sptr != "null") {
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
                    wxString l_s = " " + wxString(_("Error!")) + "\n" 
                    + _("GUID") + ": " + m_tBoundaryGUID->GetValue() + _(" does not exist");
                    wxMessageDialog mdlg(GetOCPNCanvasWindow(), l_s, _("Watchdog"), wxOK | wxICON_WARNING);
                    mdlg.ShowModal();
                    m_tBoundaryGUID->SetFocus();
                }
            }
        }
    } 
    
    if(!l_bCheckDone) {
        jMsg["Source"] = "WATCHDOG_PI";
        jMsg["Type"] = "Request";
        jMsg["Msg"] = "FindPathByGUID";
        jMsg["MsgId"] = "inclusion";
        jMsg["GUID"] = (std::string)m_tBoundaryGUID->GetValue();
        g_ReceivedPathGUIDMessage = wxEmptyString;
        SendPluginMessage( "OCPN_DRAW_PI", writer.write( jMsg ));
        if(g_ReceivedPathGUIDMessage != wxEmptyString && 
            g_ReceivedPathGUIDJSONMsg["MsgId"].asString() == "inclusion" && 
            g_ReceivedPathGUIDJSONMsg["Found"].asBool() == true ) {
            g_BoundaryName = g_ReceivedPathGUIDJSONMsg["Name"].asString();
        g_BoundaryDescription = g_ReceivedPathGUIDJSONMsg["Description"].asString();
        g_BoundaryGUID = g_ReceivedPathGUIDJSONMsg["GUID"].asString();
            } else {
                wxString l_s = " " + wxString(_("Error!")) + "\n" 
                + _("GUID") + ": " + m_tBoundaryGUID->GetValue() + _(" does not exist");
                wxMessageDialog mdlg(GetOCPNCanvasWindow(), l_s, _("Watchdog"), wxOK | wxICON_WARNING);
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
    extern Json::Value g_ReceivedPathGUIDJSONMsg;
    extern wxString    g_ReceivedPathGUIDMessage;
    extern Json::Value g_ReceivedODAPIJSONMsg;
    extern wxString    g_ReceivedODAPIMessage;
    
    bool                l_bCheckDone = false;
    
    Json::Value jMsg;
    Json::FastWriter writer;

    if(m_tGuardZoneGUID->GetValue().Len() == 0) {
        event.Skip();
        return;
    }
    
    if(ODVersionNewerThan( 1, 1, 15)) {
        jMsg["Source"] = "WATCHDOG_PI";
        jMsg["Type"] = "Request";
        jMsg["Msg"] = "GetAPIAddresses";
        jMsg["MsgId"] = "GetAPIAddresses";
        jMsg["GUID"] = (std::string)m_tBoundaryGUID->GetValue();
        SendPluginMessage( "OCPN_DRAW_PI", writer.write( jMsg ));
        if(g_ReceivedODAPIMessage != wxEmptyString &&  g_ReceivedODAPIJSONMsg["MsgId"].asString() == "GetAPIAddresses") {
            wxString sptr = g_ReceivedODAPIJSONMsg["OD_FindPathByGUID"].asString();
            if(sptr != "null") {
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
                    wxString l_s = " " + wxString(_("Error!")) + "\n" 
                    + _("GUID") + ": " + m_tGuardZoneGUID->GetValue() + _(" does not exist");
                    wxMessageDialog mdlg(GetOCPNCanvasWindow(), l_s, _("Watchdog"), wxOK | wxICON_WARNING);
                    mdlg.ShowModal();
                    m_tGuardZoneGUID->SetFocus();
                }
            }
        }
    } 
    if(!l_bCheckDone) {
        jMsg["Source"] = "WATCHDOG_PI";
        jMsg["Type"] = "Request";
        jMsg["Msg"] = "FindPathByGUID";
        jMsg["MsgId"] = "guard";
        jMsg["GUID"] = (std::string)m_tGuardZoneGUID->GetValue();
        g_ReceivedPathGUIDMessage = wxEmptyString;
        SendPluginMessage( "OCPN_DRAW_PI", writer.write( jMsg ) );
        if(g_ReceivedPathGUIDMessage != wxEmptyString && 
            g_ReceivedPathGUIDJSONMsg["MsgId"].asString() == "guard" && 
            g_ReceivedPathGUIDJSONMsg["Found"].asBool() == true ) {
            g_GuardZoneName = g_ReceivedPathGUIDJSONMsg["Name"].asString();
            g_GuardZoneDescription = g_ReceivedPathGUIDJSONMsg["Description"].asString();
            g_GuardZoneGUID = g_ReceivedPathGUIDJSONMsg["GUID"].asString();
        } else {
            wxString l_s = " " + wxString(_("Error!")) + "\n" 
                    + _("GUID") + ": " + m_tGuardZoneGUID->GetValue() + _(" does not exist");
            wxMessageDialog mdlg(GetOCPNCanvasWindow(), l_s, _("Watchdog"), wxOK | wxICON_WARNING);
            mdlg.ShowModal();
            m_tGuardZoneGUID->SetFocus();
        }
    }
    event.Skip();
}

void BoundaryPanel::OnRadioButton(wxCommandEvent& event )
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

void AnchorPanel::OnSyncToBoat( wxCommandEvent& )
{
    m_tLatitude->SetValue(wxString::Format("%f", g_watchdog_pi->LastFix().Lat));
    m_tLongitude->SetValue(wxString::Format("%f", g_watchdog_pi->LastFix().Lon));
}

void CoursePanel::OnCurrentCourse( wxCommandEvent& )
{
    m_sCourse->SetValue(g_watchdog_pi->m_cog);
}

void WindPanel::OnType( wxCommandEvent& )
{
    if(!m_sRange->IsEnabled())
        m_sValue->SetValue(m_direction);
    m_sRange->Enable(m_cType->GetSelection() == 2);
}

void WindPanel::OnAboutWind( wxCommandEvent& )
{
    wxString l_s = _("Wind Alarms can be from:\n\
1) Apparent - measured from moving boat (requires only wind sensors)\n\
2) True Relative - wind would feel like if boat stopped (requires wind sensors + gps)\n\
3) True Absolute - wind would feel if boat stopped and faced north (requires wind sensors + gps + compass)");
    wxMessageDialog mdlg(GetOCPNCanvasWindow(), l_s, _("Watchdog"), wxOK | wxICON_WARNING);
    mdlg.ShowModal();
}

void WindPanel::OnSync( wxCommandEvent& )
{
    m_sValue->SetValue(m_direction);
}


void WeatherPanel::SetupControls()
{
    switch(m_cVariable->GetSelection()) {
    case BAROMETER: // Barometer
        m_stUnits->SetLabel(_("mBar"));
        break;
    case AIR_TEMPERATURE: case SEA_TEMPERATURE:
        m_stUnits->SetLabel(_("Deg C"));
        break;
    case RELATIVE_HUMIDITY:
        m_stUnits->SetLabel(_("Deg C"));
        break;
    }

    int selection = m_cType->GetSelection();
    m_cType->Clear();
    if(m_rbRate->GetValue()) {
        m_cType->Append(_("Increasing"));
        m_cType->Append(_("Decreasing"));
    } else {
        m_cType->Append(_("Above"));
        m_cType->Append(_("Below"));
    }
    m_cType->SetSelection(selection);
    m_sRatePeriod->Enable(m_rbRate->GetValue());
}

void WeatherPanel::OnVariable( wxCommandEvent& )
{
    SetupControls();
}

void pypilotPanel::OnAboutHardwareErrors( wxCommandEvent& )
{
    wxString l_s = _("Hardware errors usually indicate bad wiring or failed components.\n\
No IMU Sensor - inertial sensors cannot be read\n\
No Motor Controller - motor controller lost communication\n\
No Rudder Feedback - optional Rudder feedback sensor not working\n\
No Motor Temperature - optional motor temperature sensor not working\n\
Driver Timeout - Motor not drawing power: motor not connected to controller\n");

    wxMessageDialog mdlg(GetOCPNCanvasWindow(), l_s, _("Watchdog"), wxOK | wxICON_INFORMATION);
    mdlg.ShowModal();
}
