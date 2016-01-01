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

#include <map>

#include <wx/wx.h>
#include "wx28compat.h"
#include "wddc.h"

#include <wx/process.h>

#include "wxJSON/jsonwriter.h"
#include "tinyxml/tinyxml.h"

#include "watchdog_pi.h"
#include "WatchdogDialog.h"
#include "AIS_Target_Info.h"

///////// The Alarm classes /////////
class LandFallAlarm : public Alarm
{
public:
    LandFallAlarm() : Alarm(true, 5 /* seconds */),
                      m_Mode(TIME),
                      m_TimeMinutes(20),
                      m_Distance(3)
        {}

    wxString Type() { return _("LandFall"); }
    wxString Options() {
        switch(m_Mode) {
        case TIME: return _("Time") + wxString::Format(_T(" < %f"), m_TimeMinutes);
        case DISTANCE: return _("Distance") + wxString::Format(_T(" < %f nm"), m_Distance);
        default: return _T("");
        }
    }

    bool Test() {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();

        if(isnan(lastfix.Lat))
            return false;

        double lat1 = lastfix.Lat, lon1 = lastfix.Lon, lat2, lon2;
        double dist = 0, dist1 = 1000;
        int count = 0;
                
        m_crossinglat1 = m_crossinglon1 = NAN;
        m_LandFallTime = wxTimeSpan();

        switch(m_Mode) {
        case TIME:
            while(count < 10 && dist1 > 1e-6) {
                PositionBearingDistanceMercator_Plugin
                    (lastfix.Lat, lastfix.Lon, lastfix.Cog, dist + dist1, &lat2, &lon2);
                if(!wxIsNaN(lat2) && PlugIn_GSHHS_CrossesLand(lat1, lon1, lat2, lon2)) {
                    if(dist1 < 1) {
                        m_LandFallTime = wxTimeSpan::Seconds(3600.0 * dist / lastfix.Sog);
                        m_crossinglat1 = lat1, m_crossinglon1 = lon1;
                        m_crossinglat2 = lat2, m_crossinglon2 = lon2;
                        if(m_LandFallTime.GetMinutes() <= m_TimeMinutes)
                            return true;
                    }
                    count = 0;
                    dist1 /= 2;
                } else {
                    dist += dist1;
                    lat1 = lat2;
                    lon1 = lon2;
                    count++;
                }
            }
            break;
        case DISTANCE:
            for(double t = 0; t<360; t+=9) {
                double dlat, dlon;
                PositionBearingDistanceMercator_Plugin(lastfix.Lat, lastfix.Lon, t,
                                                       m_Distance, &dlat, &dlon);
            
                if(PlugIn_GSHHS_CrossesLand(lastfix.Lat, lastfix.Lon, dlat, dlon)) {
                    m_crossinglat1 = dlat, m_crossinglon1 = dlon;
                    m_crossinglat2 = dlat, m_crossinglon2 = dlon;
                    return true;
                }
            }
            break;
        }

        return false;
    }

    wxString GetStatus() {
        switch(m_Mode) {
        case TIME:
        {
            if(m_LandFallTime.IsNull())
                return _("LandFall Time Invalid");

            wxString s, fmt(_T(" %d "));
            int days = m_LandFallTime.GetDays();
            if(days > 1)
                s = wxString::Format(fmt + _("Days"), days);
            else {
                if(days)
                    s = wxString::Format(fmt + _("Day"), days);
                    
                int hours = m_LandFallTime.GetHours();
                if(hours > 1)
                    s += wxString::Format(fmt + _("Hours"), hours);
                else {
                    if(hours)
                        s += wxString::Format(fmt + _("Hour"), hours);

                    int minutes = m_LandFallTime.GetMinutes() - 60*hours;
                    if(minutes > 1)
                        s += wxString::Format(fmt + _("Minutes"), minutes);
                    else {
                        if(minutes)
                            s += wxString::Format(fmt + _("Minute"), minutes);
                            
                        int seconds = m_LandFallTime.GetSeconds().ToLong() - 60*minutes;
                        if(seconds > 1)
                            s += wxString::Format(fmt + _("Seconds"), seconds);
                        else
                            s += wxString::Format(fmt + _("Second"), seconds);
                    }
                }
            }
            return s;
        }
        case DISTANCE:
        {
            return wxString::Format(_T(" ") + wxString(_("Distance")) +
                                    (m_bFired ? _T(" <") : _T(" >")) +
                                    _T(" %.2f nm"), m_Distance);
        } break;
        }
        return _T("");
    }

    void Render(wdDC &dc, PlugIn_ViewPort &vp) {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();
        if(isnan(m_crossinglat1))
            return;

        wxPoint r1, r2, r3, r4;
        GetCanvasPixLL(&vp, &r1, lastfix.Lat, lastfix.Lon);
        GetCanvasPixLL(&vp, &r2, m_crossinglat1, m_crossinglon1);
        GetCanvasPixLL(&vp, &r3, m_crossinglat2, m_crossinglon2);
        r4.x = (r2.x+r3.x)/2, r4.y = (r2.y+r3.y)/2;
        
        dc.SetPen(wxPen(wxColour(255, 255, 0), 2));
        dc.DrawLine( r1.x, r1.y, r4.x, r4.y );
        
        if(m_bFired)
            dc.SetPen(wxPen(*wxRED, 3));
        else
            dc.SetPen(wxPen(*wxGREEN, 3));

        dc.DrawCircle( r4.x, r4.y, hypot(r2.x-r3.x, r2.y-r3.y) / 2 );
    }

    wxWindow *OpenPanel(wxWindow *parent) {
        LandFallPanel *panel = new LandFallPanel(parent);
        panel->m_rbTime->SetValue(m_Mode == TIME);
        panel->m_rbDistance->SetValue(m_Mode == DISTANCE);
        panel->m_sTimeMinutes->SetValue(m_TimeMinutes);
        panel->m_tDistance->SetValue(wxString::Format(_T("%f"), m_Distance));
        return panel;
    }

    void SavePanel(wxWindow *p) {
        LandFallPanel *panel = (LandFallPanel*)p;
        m_Mode = panel->m_rbDistance->GetValue() ? DISTANCE : TIME;
        m_TimeMinutes = panel->m_sTimeMinutes->GetValue();
        panel->m_tDistance->GetValue().ToDouble(&m_Distance);
    }

    void LoadConfig(TiXmlElement *e) {
        const char *mode = e->Attribute("Mode");
        if(!strcasecmp(mode, "Time")) m_Mode = TIME;
        else if(!strcasecmp(mode, "Distance")) m_Mode = DISTANCE;
        else wxLogMessage(_T("Watchdog: ") + wxString(_("invalid LandFall mode")) + _T(": ")
                         + wxString::FromUTF8(mode));

        e->Attribute("TimeMinutes", &m_TimeMinutes);
        e->Attribute("Distance", &m_Distance);
    }

    void SaveConfig(TiXmlElement *c) {
        c->SetAttribute("Type", "LandFall");
        switch(m_Mode) {
        case TIME: c->SetAttribute("Mode", "Time");
        case DISTANCE: c->SetAttribute("Mode", "Distance");
        }

        c->SetAttribute("TimeMinutes", m_TimeMinutes);
        c->SetDoubleAttribute("Distance", m_Distance);
    }

protected:
    double m_crossinglat1, m_crossinglon1;
    double m_crossinglat2, m_crossinglon2;
    wxTimeSpan m_LandFallTime;

    enum Mode { TIME, DISTANCE } m_Mode;
    double m_TimeMinutes, m_Distance;
};

extern wxJSONValue g_ReceivedBoundaryTimeJSONMsg;
extern wxString    g_ReceivedBoundaryTimeMessage;
extern wxJSONValue g_ReceivedBoundaryDistanceJSONMsg;
extern wxString    g_ReceivedBoundaryDistanceMessage;
extern wxJSONValue g_ReceivedBoundaryAnchorJSONMsg;
extern wxString    g_ReceivedBoundaryAnchorMessage;
extern wxJSONValue g_ReceivedGuardZoneJSONMsg;
extern wxString    g_ReceivedGuardZoneMessage;
extern wxString    g_GuardZoneName;
extern AIS_Target_Info g_AISTarget;

enum 
{   
    ID_BOUNDARY_ANY = 0,
    ID_BOUNDARY_EXCLUSION,
    ID_BOUNDARY_INCLUSION,
    ID_BOUNDARY_NEITHER,
    
    ID_BOUNDARY_TYPE_LAST
};

class BoundaryAlarm : public Alarm
{
public:
    BoundaryAlarm() : Alarm(false, 5 /* seconds */),
                      m_Mode(TIME),
                      m_TimeMinutes(20),
                      m_Distance(3),
                      m_BoundaryType(ID_BOUNDARY_ANY),
                      m_bAnchorOutside(false),
                      m_bGuardZoneFired(false)
        {}

    wxString Type() { 
        switch(m_Mode) {
            case TIME: return _("Boundary Time");
            case DISTANCE: return _("Boundary Distance");
            case ANCHOR: return _("Boundary GUID");
            case GUARD: return _("Guard Zone GUID");
            default: return _("Boundary");
        }
    }
    
    wxString Options() {
        wxString s;
        switch(m_Mode) {
        case TIME: s = _("Time") + wxString::Format(_T(" < %f"), m_TimeMinutes);
        case DISTANCE: s = _("Distance") + wxString::Format(_T(" < %f nm"), m_Distance);
        case ANCHOR: s = _("Boundary GUID") + wxString(_T(" ")) + m_BoundaryGUID;
        case GUARD: s = _("Guard Zone GUID") + wxString(_T(" ")) + m_GuardZoneGUID;
        default: s = _T("");
        }
        return s;
    }

    bool Test() {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();

        if(isnan(lastfix.Lat))
            return false;

        double lat1 = lastfix.Lat, lon1 = lastfix.Lon, lat2, lon2;
        double dist = 0, dist1 = 1000;
        int count = 0;
                
        m_crossinglat1 = m_crossinglon1 = NAN;
        m_BoundaryTime = wxTimeSpan();

        switch(m_Mode) {
            case TIME: {
                while(count < 10) {
                    PositionBearingDistanceMercator_Plugin
                        (lastfix.Lat, lastfix.Lon, lastfix.Cog, lastfix.Sog * ( m_TimeMinutes / 60 ) + dist1, &lat2, &lon2);

                    // Do JSON message to OD Plugin to check if boundary m_crossinglat
                    wxJSONValue jMsg;
                    wxJSONWriter writer;
                    wxString    MsgString;
                    jMsg[wxS("Source")] = wxS("WATCHDOG_PI");
                    jMsg[wxT("Type")] = wxT("Request");
                    jMsg[wxT("Msg")] = wxS("FindPointInAnyBoundary");
                    jMsg[wxT("MsgId")] = wxS("time");
                    jMsg[wxS("lat")] = lat2;
                    jMsg[wxS("lon")] = lon2;
                    writer.Write( jMsg, MsgString );
                    SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
                    if(g_ReceivedBoundaryTimeMessage != wxEmptyString &&
                    g_ReceivedBoundaryTimeJSONMsg[wxS("MsgId")].AsString() == wxS("time") &&
                    g_ReceivedBoundaryTimeJSONMsg[wxS("Found")].AsBool() == true ) {
                        // This is our message
                        int l_BoundaryType = ID_BOUNDARY_ANY;
                        if(g_ReceivedBoundaryTimeJSONMsg[wxS("BoundaryType")].AsString() == wxT("Exclusion")) l_BoundaryType = ID_BOUNDARY_EXCLUSION;
                        else if(g_ReceivedBoundaryTimeJSONMsg[wxS("BoundaryType")].AsString() == wxT("Inclusion")) l_BoundaryType = ID_BOUNDARY_INCLUSION;
                        else if(g_ReceivedBoundaryTimeJSONMsg[wxS("BoundaryType")].AsString() == wxT("Neither")) l_BoundaryType = ID_BOUNDARY_NEITHER;
                        if(m_BoundaryType == ID_BOUNDARY_ANY || m_BoundaryType == l_BoundaryType ) {
                            if(dist1 < 1) {
                                m_BoundaryTime = wxTimeSpan::Seconds(3600.0 * dist / lastfix.Sog);
                                m_crossinglat1 = lat1, m_crossinglon1 = lon1;
                                m_crossinglat2 = lat2, m_crossinglon2 = lon2;
                                if(m_BoundaryTime.GetMinutes() <= m_TimeMinutes) {
                                    m_MsgBoundaryName = g_ReceivedBoundaryTimeJSONMsg[wxS("Name")].AsString();
                                    m_MsgBoundaryDescription = g_ReceivedBoundaryTimeJSONMsg[wxS("Description")].AsString();
                                    m_MsgBoundaryGUID = g_ReceivedBoundaryTimeJSONMsg[wxS("GUID")].AsString();
                                    g_ReceivedBoundaryDistanceMessage = wxEmptyString;
                                    return true;
                                }
                            }
                        }
                        count = 0;
                        dist1 /= 2;
                    } else {
                        dist += dist1;
                        lat1 = lat2;
                        lon1 = lon2;
                        count++;
                    }
                    g_ReceivedBoundaryTimeMessage = wxEmptyString;
                    //g_ReceivedBoundaryTimeJSONMsg.Clear();
                }
                break;
            }
            case DISTANCE: {
                for(double t = 0; t<360; t+=9) {
                    double dlat, dlon;
                    PositionBearingDistanceMercator_Plugin(lastfix.Lat, lastfix.Lon, t,
                                                        m_Distance, &dlat, &dlon);
                    
                    wxJSONValue jMsg;
                    wxJSONWriter writer;
                    wxString    MsgString;
                    jMsg[wxS("Source")] = wxS("WATCHDOG_PI");
                    jMsg[wxT("Type")] = wxT("Request");
                    jMsg[wxT("Msg")] = wxS("FindPointInAnyBoundary");
                    jMsg[wxT("MsgId")] = wxS("distance");
                    jMsg[wxS("lat")] = dlat;
                    jMsg[wxS("lon")] = dlon;
                    writer.Write( jMsg, MsgString );
                    SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
                    if(g_ReceivedBoundaryDistanceMessage != wxEmptyString &&
                    g_ReceivedBoundaryDistanceJSONMsg[wxS("MsgId")].AsString() == wxS("distance") &&
                    g_ReceivedBoundaryDistanceJSONMsg[wxS("Found")].AsBool() == true ) {
                        // This is our message
                        int l_BoundaryType = ID_BOUNDARY_ANY;
                        if(g_ReceivedBoundaryDistanceJSONMsg[wxS("BoundaryType")].AsString() == wxS("Exclusion")) l_BoundaryType = ID_BOUNDARY_EXCLUSION;
                        else if(g_ReceivedBoundaryDistanceJSONMsg[wxS("BoundaryType")].AsString() == wxS("Inclusion")) l_BoundaryType = ID_BOUNDARY_INCLUSION;
                        else if(g_ReceivedBoundaryDistanceJSONMsg[wxS("BoundaryType")].AsString() == wxS("Neither")) l_BoundaryType = ID_BOUNDARY_NEITHER;
                        if(m_BoundaryType == ID_BOUNDARY_ANY || m_BoundaryType == l_BoundaryType ) {
                            m_MsgBoundaryName = g_ReceivedBoundaryDistanceJSONMsg[wxS("Name")].AsString();
                            m_MsgBoundaryDescription = g_ReceivedBoundaryDistanceJSONMsg[wxS("Description")].AsString();
                            m_MsgBoundaryGUID = g_ReceivedBoundaryDistanceJSONMsg[wxS("GUID")].AsString();
                            g_ReceivedBoundaryDistanceMessage = wxEmptyString;
                            return true;
                        }
                    }
                    g_ReceivedBoundaryDistanceMessage = wxEmptyString;
                }
                break;
            }
            case ANCHOR: {
                wxJSONValue jMsg;
                wxJSONWriter writer;
                wxString    MsgString;
                jMsg[wxS("Source")] = wxS("WATCHDOG_PI");
                jMsg[wxT("Type")] = wxT("Request");
                jMsg[wxT("Msg")] = wxS("FindPointInBoundary");
                jMsg[wxT("MsgId")] = wxS("anchor");
                jMsg[wxS("GUID")] = m_BoundaryGUID;
                jMsg[wxS("lat")] = lastfix.Lat;
                jMsg[wxS("lon")] = lastfix.Lon;
                
                writer.Write( jMsg, MsgString );
                SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
                if(g_ReceivedBoundaryAnchorMessage != wxEmptyString &&
                    g_ReceivedBoundaryAnchorJSONMsg[wxS("MsgId")].AsString() == wxS("anchor") &&
                    g_ReceivedBoundaryAnchorJSONMsg[wxS("Found")].AsBool() == false ) {
                    // This is our message
                    g_ReceivedBoundaryDistanceMessage = wxEmptyString;
                    m_MsgBoundaryName = wxEmptyString;
                    m_MsgBoundaryDescription = wxEmptyString;
                    m_MsgBoundaryGUID = wxEmptyString;
                    m_bAnchorOutside = true;
                    return true;
                }
                m_bAnchorOutside = false;
                g_ReceivedBoundaryDistanceMessage = wxEmptyString;
                break;
            }
            case GUARD: {
                //if(g_ReceivedGuardZoneMessage == wxEmptyString) return false;
                wxJSONValue jMsg;
                wxJSONWriter writer;
                wxString    MsgString;
                jMsg[wxS("Source")] = wxS("WATCHDOG_PI");
                jMsg[wxT("Type")] = wxT("Request");
                jMsg[wxT("Msg")] = wxS("FindPointInGuardZone");
                jMsg[wxT("MsgId")] = wxS("guard");
                jMsg[wxS("GUID")] = m_GuardZoneGUID;
                jMsg[wxS("lat")] = g_AISTarget.m_dLat;
                jMsg[wxS("lon")] = g_AISTarget.m_dLon;
                
                writer.Write( jMsg, MsgString );
                SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
                if(g_ReceivedGuardZoneMessage != wxEmptyString &&
                    g_ReceivedGuardZoneJSONMsg[wxS("MsgId")].AsString() == wxS("guard") &&
                    g_ReceivedGuardZoneJSONMsg[wxS("Found")].AsBool() == true ) {
                    // This is our message
                    g_ReceivedGuardZoneMessage = wxEmptyString;
                    m_GuardZoneName = g_ReceivedGuardZoneJSONMsg[wxS("Name")].AsString();
                    m_GuardZoneDescription = g_ReceivedGuardZoneJSONMsg[wxS("Description")].AsString();
                    m_GuardZoneGUID = g_ReceivedGuardZoneJSONMsg[wxS("GUID")].AsString();
                    m_MsgBoundaryName = m_GuardZoneName;
                    m_MsgBoundaryDescription = m_GuardZoneDescription;
                    m_MsgBoundaryGUID = m_GuardZoneGUID;
                    return true;
                }
                g_ReceivedGuardZoneMessage = wxEmptyString;
                break;
            }
        }
       
        return false;
    }

    wxString GetStatus() {
        switch(m_Mode) {
            case TIME:
            {
                if(m_BoundaryTime.IsNull())
                    return _("Boundary Time Invalid");

                wxString s, fmt(_T(" %d "));
                int days = m_BoundaryTime.GetDays();
                if(days > 1)
                    s = wxString::Format(fmt + _("Days"), days);
                else {
                    if(days)
                        s = wxString::Format(fmt + _("Day"), days);
                        
                    int hours = m_BoundaryTime.GetHours();
                    if(hours > 1)
                        s += wxString::Format(fmt + _("Hours"), hours);
                    else {
                        if(hours)
                            s += wxString::Format(fmt + _("Hour"), hours);

                        int minutes = m_BoundaryTime.GetMinutes() - 60*hours;
                        if(minutes > 1)
                            s += wxString::Format(fmt + _("Minutes"), minutes);
                        else {
                            if(minutes)
                                s += wxString::Format(fmt + _("Minute"), minutes);
                                
                            int seconds = m_BoundaryTime.GetSeconds().ToLong() - 60*minutes;
                            if(seconds > 1)
                                s += wxString::Format(fmt + _("Seconds"), seconds);
                            else
                                s += wxString::Format(fmt + _("Second"), seconds);
                        }
                    }
                }
                return s;
            }
            case DISTANCE:
            {
                return wxString::Format(_T(" ") + wxString(_("Distance")) +
                                        (m_bFired ? _T(" <") : _T(" >")) +
                                        _T(" %.2f nm"), m_Distance);
                break;
            } 
            case ANCHOR:
            {
                return _T(" ") + wxString(_("Anchor")) + _T(" ") +
                    (m_bAnchorOutside ? _("Outside") : _("Inside")) +
                    wxString(_T(" boundary ")) + m_BoundaryGUID;
                break;
            }
            case GUARD:
            {
                return _T(" ") + _("Guard Zone") + _T(": ") + m_GuardZoneName + _T(": ") +
                (m_bGuardZoneFired ? _("AIS Target in zone") : _("NO AIS tagets found in zone"));
                break;
            }
        }
        return _T("");
    }

    wxWindow *OpenPanel(wxWindow *parent) {
        BoundaryPanel *panel = new BoundaryPanel(parent);
        panel->m_rbTime->SetValue(m_Mode == TIME);
        panel->m_rbDistance->SetValue(m_Mode == DISTANCE);
        panel->m_rbAnchor->SetValue(m_Mode == ANCHOR);
        panel->m_rbGuard->SetValue(m_Mode == GUARD);
        panel->m_sTimeMinutes->SetValue(m_TimeMinutes);
        panel->m_tDistance->SetValue(wxString::Format(_T("%f"), m_Distance));
        panel->m_radioBoxBoundaryType->SetSelection(m_BoundaryType);
        panel->m_tBoundaryGUID->SetValue(m_BoundaryGUID);
        panel->m_tGuardZoneGUID->SetValue(m_GuardZoneGUID);
        return panel;
    }

    void SavePanel(wxWindow *p) {
        BoundaryPanel *panel = (BoundaryPanel*)p;
        if(panel->m_rbTime->GetValue()) m_Mode = TIME;
        else if(panel->m_rbDistance->GetValue()) m_Mode = DISTANCE;
        else if(panel->m_rbAnchor->GetValue()) m_Mode = ANCHOR;
        else if(panel->m_rbGuard->GetValue()) m_Mode = GUARD;
        else m_Mode = TIME;
        m_TimeMinutes = panel->m_sTimeMinutes->GetValue();
        panel->m_tDistance->GetValue().ToDouble(&m_Distance);
        m_BoundaryType = panel->m_radioBoxBoundaryType->GetSelection();
        m_BoundaryGUID = panel->m_tBoundaryGUID->GetValue();
        m_GuardZoneGUID = panel->m_tGuardZoneGUID->GetValue();
        m_GuardZoneName = g_GuardZoneName;
    }

    void LoadConfig(TiXmlElement *e) {
        const char *mode = e->Attribute("Mode");
        if(!strcasecmp(mode, "Time")) m_Mode = TIME;
        else if(!strcasecmp(mode, "Distance")) m_Mode = DISTANCE;
        else if(!strcasecmp(mode, "Anchor")) m_Mode = ANCHOR;
        else if(!strcasecmp(mode, "Guard")) m_Mode = GUARD;
        else wxLogMessage(_T("Watchdog: ") + wxString(_("invalid Boundary mode")) + _T(": ")
                         + wxString::FromUTF8(mode));

        e->Attribute("TimeMinutes", &m_TimeMinutes);
        e->Attribute("Distance", &m_Distance);
        e->Attribute("BoundaryType", &m_BoundaryType);
        m_BoundaryGUID = wxString::FromUTF8(e->Attribute("BoundaryGUID"));
        m_GuardZoneGUID = wxString::FromUTF8(e->Attribute("GuardZoneGUID"));
        m_GuardZoneName = wxString::FromUTF8(e->Attribute("GuardZoneName"));
    }

    void SaveConfig(TiXmlElement *c) {
        c->SetAttribute("Type", "Boundary");
        c->SetAttribute(("BoundaryType"), m_BoundaryType);
        switch(m_Mode) {
        case TIME: 
            c->SetAttribute("Mode", "Time");
            break;
        case DISTANCE: 
            c->SetAttribute("Mode", "Distance");
            break;
        case ANCHOR: 
            c->SetAttribute("Mode", "Anchor");
            break;
        case GUARD: 
            c->SetAttribute("Mode", "Guard");
            break;
        }

        c->SetAttribute("TimeMinutes", m_TimeMinutes);
        c->SetDoubleAttribute("Distance", m_Distance);
        c->SetAttribute("BoundaryGUID", m_BoundaryGUID.mb_str());
        c->SetAttribute("GuardZoneGUID", m_GuardZoneGUID.mb_str());
        c->SetAttribute("GuardZoneName", m_GuardZoneName.mb_str());
        //        alarm->ConfigItem(read, _T ( "LatLonorBoundary" ), m_rbUse );
    }

    void Run()
    {
        if(m_bSound)
            PlugInPlaySound(m_sSound);
        
        if(m_bCommand)
            if(!wxProcess::Open(m_sCommand)) {
                wxMessageDialog mdlg(GetOCPNCanvasWindow(),
                                     Type() + _T(" ") +
                                     _("Failed to execute command: ") + m_sCommand,
                                     _("Watchdog"), wxOK | wxICON_ERROR);
                mdlg.ShowModal();
                m_bCommand = false;
            }
            
            if(m_bMessageBox) {
                wxMessageDialog mdlg(GetOCPNCanvasWindow(), Type() + _T(" ") + _("ALARM!") + _T("\n") 
                                    + _("Name") + _T(": ") + m_MsgBoundaryName + _T("\n")
                                    + _("Description") + _T(": ") + m_MsgBoundaryDescription + _T("\n")
                                    + _("GUID") + _T(": ") + m_MsgBoundaryGUID, 
                                     _("Watchman"), wxOK | wxICON_WARNING);
                mdlg.ShowModal();
            }
    }
    
    void OnAISMessage (int iAlarmIndex) 
    {
        bool l_bFoundMMSI = false;
        AISMMSITIME l_AISMsgInfo;
        
        DEBUGST("Testing zone: ");
        DEBUGEND(m_GuardZoneName);
        if(m_bEnabled) {
            std::list<AISMMSITIME>::iterator it = AISMsgInfoList.begin();
            while(it != AISMsgInfoList.end()) {
                if((wxDateTime::Now() - it->MsgTime).GetSeconds() > m_iRepeatSeconds) {
                    AISMsgInfoList.erase(it);
                    it = AISMsgInfoList.begin();
                    continue;
                } else if(it->MMSI == g_AISTarget.m_iMMSI) {
                    l_bFoundMMSI = true;
                    break;
                } else if(it == AISMsgInfoList.end()) break;
                else {
                    it++;
                }
            }
            
            if(Test()) {  
                DEBUGST("Found target in zone: ");
                DEBUGEND(m_GuardZoneName);
                wxDateTime now = wxDateTime::Now();
                if(l_bFoundMMSI) {
                    if((now - it->MsgTime).GetSeconds() > m_iRepeatSeconds && m_bRepeat) {
                        AISMsgInfoList.erase(it);
                        l_AISMsgInfo.MMSI = g_AISTarget.m_iMMSI;
                        l_AISMsgInfo.MsgTime = now;
                        AISMsgInfoList.push_back(l_AISMsgInfo);
                        Run();
                    }
                } else {
                    l_AISMsgInfo.MMSI = g_AISTarget.m_iMMSI;
                    l_AISMsgInfo.MsgTime = wxDateTime::Now();
                    AISMsgInfoList.push_back(l_AISMsgInfo);
                    Run();
                }
            }
            
            if(AISMsgInfoList.empty()) {
                if(m_bAutoReset) {
                    m_bGuardZoneFired = false;
                    m_bFired = false;
                }
            }
            else {
                m_bGuardZoneFired = true;
                m_bFired = true;
            }
            if(g_watchdog_pi->m_WatchdogDialog && g_watchdog_pi->m_WatchdogDialog->IsShown())
                g_watchdog_pi->m_WatchdogDialog->UpdateStatus(iAlarmIndex);
            
        }
    }

    void OnTimer( wxTimerEvent & )
    {
        if(g_watchdog_pi->m_WatchdogDialog && g_watchdog_pi->m_WatchdogDialog->IsShown())
            for(unsigned int i=0; i<Alarm::s_Alarms.size(); i++)
                if(Alarm::s_Alarms[i] == this)
                    g_watchdog_pi->m_WatchdogDialog->UpdateStatus(i);
        return;
    }
    
private:
    double m_crossinglat1, m_crossinglon1;
    double m_crossinglat2, m_crossinglon2;
    wxTimeSpan m_BoundaryTime;

    enum Mode { TIME, DISTANCE, ANCHOR, GUARD } m_Mode;
    double m_TimeMinutes, m_Distance;
    int m_BoundaryType;
    bool m_bAnchorOutside;
    wxString m_BoundaryGUID;
    wxString m_BoundaryName;
    wxString m_MsgBoundaryName;
    wxString m_MsgBoundaryDescription;
    wxString m_MsgBoundaryGUID;
    wxString m_GuardZoneName;
    wxString m_GuardZoneDescription;
    wxString m_GuardZoneGUID;
    bool    m_bGuardZoneFired;
    
    struct AISMMSITIME {
        int MMSI;
        wxDateTime MsgTime;
    };
    
    std::list<AISMMSITIME> AISMsgInfoList;
};

class NMEADataAlarm : public Alarm
{
public:
    NMEADataAlarm() :
        start(wxDateTime::Now()),
        m_sentences(_T("$GPGGA")),
        m_seconds(10)
        {}

    wxString Type() { return _("NMEA Data"); }
    wxString Options() {
        return m_sentences;
    }

    bool Test() {
        return ElapsedSeconds() > m_seconds;
    }

    wxString GetStatus() {
        int seconds = ElapsedSeconds();
        wxString s;
        if(isnan(seconds))
            s = _T("N/A");
        else {
            wxString fmt(_T("%d "));
            s = wxString::Format(fmt + _("second(s)"), seconds);
        }

        return s;
    }

    wxWindow *OpenPanel(wxWindow *parent) {
        NMEADataPanel *panel = new NMEADataPanel(parent);
        panel->m_tSentences->SetValue(m_sentences);
        panel->m_sSeconds->SetValue(m_seconds);
        return panel;
    }

    void SavePanel(wxWindow *p) {
        NMEADataPanel *panel = (NMEADataPanel*)p;
        m_sentences = panel->m_tSentences->GetValue();
        m_seconds = panel->m_sSeconds->GetValue();
    }

    void LoadConfig(TiXmlElement *e) {
        m_sentences = wxString::FromUTF8(e->Attribute("Sentences"));
        e->Attribute("Seconds", &m_seconds);
    }

    void SaveConfig(TiXmlElement *c) {
        c->SetAttribute("Type", "NMEAData");
        c->SetAttribute("Sentences", m_sentences.mb_str());
        c->SetAttribute("Seconds", m_seconds);
    }

private:
    void NMEAString(const wxString &string) {
        wxString name = string.BeforeFirst(',');
        NMEAStringTimes[name] = wxDateTime::Now();
    }

    int ElapsedSeconds() {
        wxDateTime now = wxDateTime::Now(), time = now;
        wxString sentences = m_sentences;
        /* take oldest message time */
        for(;;) {
            wxString cur = sentences.BeforeFirst('\n');
            cur = cur.BeforeFirst(' '); /* remove trailing spaces */

            if(cur.size()) {
                wxDateTime dt = NMEAStringTimes[cur];
                if(!dt.IsValid())
                    dt = start;
                if(dt < time)
                    time = dt;
            }

            if((int)sentences.find('\n') == wxNOT_FOUND)
                break;

            sentences = sentences.AfterFirst('\n');
        }

        return (now - time).GetSeconds().ToLong();
    }

    wxDateTime start;
    std::map<wxString, wxDateTime> NMEAStringTimes;

    wxString m_sentences;
    double m_seconds;

};

class DeadmanAlarm : public Alarm
{
public:
    DeadmanAlarm() : m_Minutes(20) {}

    wxString Type() { return _("Deadman"); }
    wxString Options() {
        return wxString::Format(_T("%f minutes"), m_Minutes);
    }

    bool Test() {
        wxTimeSpan DeadmanSpan = wxTimeSpan::Minutes(m_Minutes);
        return wxDateTime::Now() - g_watchdog_pi->m_cursor_time > DeadmanSpan;
    }

    wxString GetStatus() {
        wxTimeSpan span = wxDateTime::Now() - g_watchdog_pi->m_cursor_time;
        int days = span.GetDays();
        span -= wxTimeSpan::Days(days);
        int hours = span.GetHours();
        span -= wxTimeSpan::Hours(hours);
        int minutes = span.GetMinutes();
        span -= wxTimeSpan::Minutes(minutes);
        int seconds = span.GetSeconds().ToLong();
        wxString d, fmt(_T("%d "));
        if(days)
            d = wxString::Format(fmt + _T("days "), days);
        return d + wxString::Format(_T("%02d:%02d:%02d"),
                                    hours, minutes, seconds);
    }

    wxWindow *OpenPanel(wxWindow *parent) {
        DeadmanPanel *panel = new DeadmanPanel(parent);
        panel->m_sMinutes->SetValue(m_Minutes);
        return panel;
    }

    void SavePanel(wxWindow *p) {
        DeadmanPanel *panel = (DeadmanPanel*)p;
        m_Minutes = panel->m_sMinutes->GetValue();
    }

    void LoadConfig(TiXmlElement *e) {
        e->Attribute("Minutes", &m_Minutes);
    }

    void SaveConfig(TiXmlElement *c) {
        c->SetAttribute("Type", "Deadman");
        c->SetAttribute("Minutes", m_Minutes);
    }

private:
    double m_Minutes;
};

class AnchorAlarm : public Alarm
{
public:
    AnchorAlarm() : Alarm(true),
                    m_Radius(50)
        {
            minoldfix.FixTime = 0;
            m_Latitude = g_watchdog_pi->LastFix().Lat;
            m_Longitude = g_watchdog_pi->LastFix().Lon;
            m_bWasEnabled = false;
        }

    wxString Type() { return _("Anchor"); }
    wxString Options() {
        return _("radius") + wxString::Format(_T(" %f "), m_Radius) + _("meters")
            + (m_bAutoSync ? _T(" ") + wxString(_("bAutoSync")) : _T(""));
    }

    bool Test() {
        return Distance() > m_Radius;
    }

    wxString GetStatus() {
        if(!m_bWasEnabled && m_bEnabled && m_bAutoSync) {
            PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();
            m_Latitude = lastfix.Lat;
            m_Longitude = lastfix.Lon;
            RequestRefresh(GetOCPNCanvasWindow());
        }

        m_bWasEnabled = m_bEnabled;

        double anchordist = Distance();
        wxString s;
        if(isnan(anchordist))
            s = _T("N/A");
        else {
            wxString fmt(_T("%.0f "));
            s = wxString::Format(fmt + _("meter(s)"), anchordist);
        }

        return s;
    }

    void Render(wdDC &dc, PlugIn_ViewPort &vp) {
        wxPoint r1, r2;
        
        GetCanvasPixLL(&vp, &r1, m_Latitude, m_Longitude);
        GetCanvasPixLL(&vp, &r2, m_Latitude +
                       m_Radius/1853.0/60.0,
                       m_Longitude);
        
        if(m_bEnabled) {
            if(m_bFired)
                dc.SetPen(wxPen(*wxRED, 2));
            else
                dc.SetPen(wxPen(*wxGREEN, 2));
        } else
            dc.SetPen(wxPen(wxColour(128, 192, 0, 128), 2, wxPENSTYLE_LONG_DASH));
         
        dc.DrawCircle( r1.x, r1.y, hypot(r1.x-r2.x, r1.y-r2.y) );
    }

    wxWindow *OpenPanel(wxWindow *parent) {
        AnchorPanel *panel = new AnchorPanel(parent);
        panel->m_tLatitude->SetValue(wxString::Format(_T("%f"), m_Latitude));
        panel->m_tLongitude->SetValue(wxString::Format(_T("%f"), m_Longitude));
        panel->m_sRadius->SetValue(m_Radius);
        panel->m_cbAutoSync->SetValue(m_bAutoSync);
        return panel;
    }

    void SavePanel(wxWindow *p) {
        AnchorPanel *panel = (AnchorPanel*)p;
        panel->m_tLatitude->GetValue().ToDouble(&m_Latitude);
        panel->m_tLongitude->GetValue().ToDouble(&m_Longitude);
        m_Radius = panel->m_sRadius->GetValue();
        m_bAutoSync = panel->m_cbAutoSync->GetValue();
    }

    void LoadConfig(TiXmlElement *e) {
        e->Attribute("Latitude", &m_Latitude);
        e->Attribute("Longitude", &m_Longitude);
        e->Attribute("Radius", &m_Radius);
        if(e->QueryBoolAttribute("AutoSync", &m_bAutoSync) != TIXML_SUCCESS)
            m_bAutoSync = false;
    }

    void SaveConfig(TiXmlElement *c) {
        c->SetAttribute("Type", "Anchor");
        c->SetDoubleAttribute("Latitude", m_Latitude);
        c->SetDoubleAttribute("Longitude", m_Longitude);
        c->SetAttribute("Radius", m_Radius);
        c->SetAttribute("AutoSync", m_bAutoSync);
    }

private:
    double Distance() {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();

        double anchordist;
        DistanceBearingMercator_Plugin(lastfix.Lat, lastfix.Lon,
                                       m_Latitude, m_Longitude,
                                       0, &anchordist);
        anchordist *= 1853.248; /* in meters */
        return anchordist;
    }

    PlugIn_Position_Fix_Ex minoldfix;

    double m_Latitude, m_Longitude, m_Radius;
    bool m_bWasEnabled, m_bAutoSync;
};

class CourseAlarm : public Alarm
{
public:
    CourseAlarm() : Alarm(true), m_Mode(BOTH), m_Tolerance(20) {
        m_Course = g_watchdog_pi->m_cog;
    }

    wxString Type() { return _("Course"); }
    wxString Options() {
        wxString s;
        switch(m_Mode) {
        case PORT: s += _("Port") + wxString(_T(" ")); break;
        case STARBOARD: s += _("Starboard") + wxString(_T(" ")); break;
        default: break;
        }
        return s + _("course") + wxString::Format(_T(" %f"), m_Course);
    }

    bool Test() {
        return CourseError() > m_Tolerance;
    }

    wxString GetStatus() {
        double courseerror = CourseError();
        wxString s;
        if(isnan(courseerror))
            s = _T("N/A");
        else {
            wxString fmt(_T("%.0f "));
            s = wxString::Format(fmt + _("degrees(s)"), courseerror);
        }

        if(m_Mode == STARBOARD)
            s += wxString(_T(" ")) + _("Starboard");
        else if(m_Mode == PORT)
            s += wxString(_T(" ")) + _("Port");

        return s;
    }

    void Render(wdDC &dc, PlugIn_ViewPort &vp) {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();

        double lat1 = lastfix.Lat, lon1 = lastfix.Lon, lat2, lon2, lat3, lon3;
        double dist = lastfix.Sog;

        if(isnan(dist))
            return;

        PositionBearingDistanceMercator_Plugin(lat1, lon1, m_Course+m_Tolerance,
                                               dist, &lat2, &lon2);
        PositionBearingDistanceMercator_Plugin(lat1, lon1, m_Course-m_Tolerance,
                                               dist, &lat3, &lon3);
        wxPoint r1, r2, r3;
        GetCanvasPixLL(&vp, &r1, lat1, lon1);
        GetCanvasPixLL(&vp, &r2, lat2, lon2);
        GetCanvasPixLL(&vp, &r3, lat3, lon3);

        if(m_bFired)
            dc.SetPen(wxPen(*wxRED, 2));
        else
            dc.SetPen(wxPen(*wxGREEN, 2));

        dc.DrawLine( r1.x, r1.y, r2.x, r2.y );
        dc.DrawLine( r1.x, r1.y, r3.x, r3.y );
    }

    wxWindow *OpenPanel(wxWindow *parent) {
        CoursePanel *panel = new CoursePanel(parent);
        panel->m_cMode->SetSelection((int)m_Mode);
        panel->m_sCourse->SetValue(m_Course);
        panel->m_sTolerance->SetValue(m_Tolerance);
        return panel;
    }

    void SavePanel(wxWindow *p) {
        CoursePanel *panel = (CoursePanel*)p;
        m_Mode = (Mode)panel->m_cMode->GetSelection();
        m_Course = panel->m_sCourse->GetValue();
        m_Tolerance = panel->m_sTolerance->GetValue();
    }

    void LoadConfig(TiXmlElement *e) {
        const char *mode = e->Attribute("Mode");
        if(!strcasecmp(mode, "Port")) m_Mode = PORT;
        else if(!strcasecmp(mode, "Starboard")) m_Mode = STARBOARD;
        else if(!strcasecmp(mode, "Starboard")) m_Mode = BOTH;
        else wxLogMessage(_T("Watchdog: ") + wxString(_("invalid Course mode")) + _T(": ")
                         + wxString::FromUTF8(mode));

        e->Attribute("Tolerance", &m_Tolerance);
        e->Attribute("Course", &m_Course);
    }

    void SaveConfig(TiXmlElement *c) {
        c->SetAttribute("Type", "Course");
        switch(m_Mode) {
        case PORT: c->SetAttribute("Mode", "Port");
        case STARBOARD: c->SetAttribute("Mode", "Starboard");
        case BOTH: c->SetAttribute("Mode", "Both");
        }

        c->SetDoubleAttribute("Tolerance", m_Tolerance);
        c->SetDoubleAttribute("Course", m_Course);
    }

private:
    double CourseError() {
        double error = heading_resolve(g_watchdog_pi->m_cog - m_Course);

        switch(m_Mode) {
        case PORT:      return -error;
        case STARBOARD: return  error;
        default:        return fabs(error);
        }
    }
    
    enum Mode { PORT, STARBOARD, BOTH } m_Mode;
    double m_Tolerance, m_Course;

};

class SpeedAlarm : public Alarm
{
public:
    SpeedAlarm() : Alarm(true), m_Mode(UNDERSPEED), m_Speed(1) {}

    wxString Type() { return _("Speed"); }
    wxString Options() {
        wxString s;
        switch(m_Mode) {
        case UNDERSPEED: s += _("UnderSpeed") + wxString(_T(" ")); break;
        case OVERSPEED: s += _("OverSpeed") + wxString(_T(" ")); break;
        }
        return s + wxString::Format(_T(" %f"), m_Speed);
    }

    wxString GetStatus() {
        wxString s;
        if(isnan(g_watchdog_pi->m_sog))
            s = _T("N/A");
        else {
            wxString fmt(_T("%.1f"));
            double knots = Knots();
            s = wxString::Format(fmt + (g_watchdog_pi->m_sog < knots ?
                                        _T(" < ") : _T(" > "))
                                 + fmt, g_watchdog_pi->m_sog, knots);
        }

        return s;
    }

    void Render(wdDC &dc, PlugIn_ViewPort &vp) {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();

        double knots = Knots();

        double lat1 = lastfix.Lat, lon1 = lastfix.Lon, lat2, lon2;
        PositionBearingDistanceMercator_Plugin(lat1, lon1, 0, knots, &lat2, &lon2);

        wxPoint r1, r2;
        GetCanvasPixLL(&vp, &r1, lat1, lon1);
        GetCanvasPixLL(&vp, &r2, lat2, lon2);

        if(m_bFired)
            dc.SetPen(wxPen(*wxRED, 2));
        else
            dc.SetPen(wxPen(*wxBLUE, 2));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawCircle( r1.x, r1.y, hypot(r1.x-r2.x, r1.y-r2.y) );
    }

    bool Test() {
        if(m_Mode == UNDERSPEED)
            return g_watchdog_pi->m_sog < Knots();
        else
            return g_watchdog_pi->m_sog > Knots();
    }

    wxWindow *OpenPanel(wxWindow *parent) {
        SpeedPanel *panel = new SpeedPanel(parent);
        panel->m_cMode->SetSelection((int)m_Mode);
        panel->m_tSpeed->SetValue(wxString::Format(_T("%f"), m_Speed));
        return panel;
    }

    void SavePanel(wxWindow *p) {
        SpeedPanel *panel = (SpeedPanel*)p;
        m_Mode = (Mode)panel->m_cMode->GetSelection();
        panel->m_tSpeed->GetValue().ToDouble(&m_Speed);
    }

    void LoadConfig(TiXmlElement *e) {
        const char *mode = e->Attribute("Mode");
        if(!strcasecmp(mode, "Underspeed")) m_Mode = UNDERSPEED;
        else if(!strcasecmp(mode, "Overspeed")) m_Mode = OVERSPEED;
        else wxLogMessage(_T("Watchdog: ") + wxString(_("invalid Speed mode")) + _T(": ")
                         + wxString::FromUTF8(mode));

        e->Attribute("Speed", &m_Speed);
    }

    void SaveConfig(TiXmlElement *c) {
        c->SetAttribute("Type", "Speed");
        switch(m_Mode) {
        case UNDERSPEED: c->SetAttribute("Mode", "Underspeed");
        case OVERSPEED: c->SetAttribute("Mode", "Overspeed");
        }

        c->SetDoubleAttribute("Speed", m_Speed);
    }

private:
    double Knots() {
        return g_watchdog_pi->m_sog;
    }

    enum Mode { UNDERSPEED, OVERSPEED } m_Mode;
    double m_Speed;
};

////////// Alarm Base Class /////////////////
std::vector<Alarm*> Alarm::s_Alarms;

void Alarm::RenderAll(wdDC &dc, PlugIn_ViewPort &vp)
{
    for(unsigned int i=0; i<s_Alarms.size(); i++)
        if(s_Alarms[i]->m_bgfxEnabled)
            s_Alarms[i]->Render(dc, vp);
}

void Alarm::LoadConfigAll()
{
    wxString configuration = watchdog_pi::StandardPath() + _T("WatchdogConfiguration.xml");
    TiXmlDocument doc;

    if(!doc.LoadFile(configuration.mb_str())) {
        wxLogMessage(_T("Watchdog: ") + wxString(_("Failed to read")) + _T(": ") + configuration);
        return;
    }

    TiXmlHandle root(doc.RootElement());
    for(TiXmlElement* e = root.FirstChild().Element(); e; e = e->NextSiblingElement()) {
        if(!strcasecmp(e->Value(), "Alarm")) {
            const char *type = e->Attribute("Type");
            Alarm *alarm;
            if(!strcasecmp(type, "LandFall")) alarm = Alarm::NewAlarm(LANDFALL);
            else if(!strcasecmp(type, "Boundary")) alarm = Alarm::NewAlarm(BOUNDARY);
            else if(!strcasecmp(type, "NMEAData")) alarm = Alarm::NewAlarm(NMEADATA);
            else if(!strcasecmp(type, "Deadman")) alarm = Alarm::NewAlarm(DEADMAN);
            else if(!strcasecmp(type, "Anchor")) alarm = Alarm::NewAlarm(ANCHOR);
            else if(!strcasecmp(type, "Course")) alarm = Alarm::NewAlarm(COURSE);
            else if(!strcasecmp(type, "Speed")) alarm = Alarm::NewAlarm(SPEED);
            else {
                wxLogMessage(_T("Watchdog: ") + wxString(_("invalid alarm type")) + _T(": ") + wxString::FromUTF8(type));
                continue;
            }

            alarm->LoadConfigBase(e);
            alarm->LoadConfig(e);
        }
    }
}

void Alarm::SaveConfigAll()
{
    TiXmlDocument doc;

    TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "utf-8", "" );
    doc.LinkEndChild( decl );

    TiXmlElement * root = new TiXmlElement( "OpenCPNWatchdogConfiguration" );
    doc.LinkEndChild( root );

    char version[24];
    sprintf(version, "%d.%d", PLUGIN_VERSION_MAJOR, PLUGIN_VERSION_MINOR);
    root->SetAttribute("version", version);
    root->SetAttribute("creator", "Opencpn Watchdog plugin");
    root->SetAttribute("author", "Sean D'Epagnier");

    for(unsigned int i=0; i<s_Alarms.size(); i++) {
        TiXmlElement *c = new TiXmlElement( "Alarm" );
        Alarm *alarm = s_Alarms[i];
        alarm->SaveConfig(c);
        alarm->SaveConfigBase(c);
        root->LinkEndChild(c);
    }

    wxString configuration = watchdog_pi::StandardPath() + _T("WatchdogConfiguration.xml");
    if(!doc.SaveFile(configuration.mb_str()))
        wxLogMessage(_T("Watchdog: ") + wxString(_("failed to save")) + _T(": ") + configuration);
}

void Alarm::DeleteAll()
{
    for(unsigned int i=0; i<s_Alarms.size(); i++)
        delete s_Alarms[i];
}

void Alarm::ResetAll()
{
    for(unsigned int i=0; i<s_Alarms.size(); i++)
        s_Alarms[i]->m_bFired = false;
}

void Alarm::NMEAStringAll(const wxString &sentence)
{
    for(unsigned int i=0; i<s_Alarms.size(); i++)
        s_Alarms[i]->NMEAString(sentence);
}

Alarm *Alarm::NewAlarm(enum AlarmType type)
{
    Alarm *alarm;
    switch(type) {
    case LANDFALL: alarm = new LandFallAlarm; break;
    case BOUNDARY: alarm = new BoundaryAlarm; break;
    case NMEADATA: alarm = new NMEADataAlarm; break;
    case DEADMAN:  alarm = new DeadmanAlarm;  break;
    case ANCHOR:   alarm = new AnchorAlarm;   break;
    case COURSE:   alarm = new CourseAlarm;   break;
    case SPEED:    alarm = new SpeedAlarm;    break;
    default:  wxLogMessage(_T("Invalid Alarm Type")); return NULL;
    }

    s_Alarms.push_back(alarm);
    return alarm;
}

Alarm::Alarm(bool gfx, int interval)
    : m_bHasGraphics(gfx), m_bEnabled(true), m_bgfxEnabled(false), m_bFired(false),
      m_bSound(true), m_bCommand(false), m_bMessageBox(false), m_bRepeat(false),
      m_bAutoReset(false),
      m_sSound(*GetpSharedDataLocation() + _T("sounds/2bells.wav")),
      m_iRepeatSeconds(60),
      m_interval(interval)
{
    m_Timer.Connect(wxEVT_TIMER, wxTimerEventHandler( Alarm::OnTimer ), NULL, this);
    m_Timer.Start(m_interval * 1000, wxTIMER_CONTINUOUS);
}

wxString Alarm::Action()
{
    wxString s;
    if(m_bSound)      s += _("Sound")      + wxString(_T(" "));
    if(m_bCommand)    s += _("Command")    + wxString(_T(" "));
    if(m_bMessageBox) s += _("MessageBox") + wxString(_T(" "));
    if(m_bRepeat)     s += _("Repeat")     + wxString(_T(" "));
    if(m_bAutoReset)  s += _("Auto Reset") + wxString(_T(" "));
    return s;
}

void Alarm::Run()
{
    if(m_bSound)
        PlugInPlaySound(m_sSound);

    if(m_bCommand)
        if(!wxProcess::Open(m_sCommand)) {
            wxMessageDialog mdlg(GetOCPNCanvasWindow(),
                                 Type() + _T(" ") +
                                 _("Failed to execute command: ") + m_sCommand,
                                 _("Watchdog"), wxOK | wxICON_ERROR);
            mdlg.ShowModal();
            m_bCommand = false;
        }

    if(m_bMessageBox) {
        wxMessageDialog mdlg(GetOCPNCanvasWindow(), Type() + _T(" ") + _("ALARM!"),
                             _("Watchman"), wxOK | wxICON_WARNING);
        mdlg.ShowModal();
    }
}

void Alarm::LoadConfigBase(TiXmlElement *e)
{
    e->QueryBoolAttribute("Enabled", &m_bEnabled);
    e->QueryBoolAttribute("GraphicsEnabled", &m_bgfxEnabled);
    e->QueryBoolAttribute("Sound", &m_bSound);
    m_sSound = wxString::FromUTF8(e->Attribute("SoundFile"));
    e->QueryBoolAttribute("Command", &m_bCommand);
    m_sCommand = wxString::FromUTF8(e->Attribute("CommandFile"));
    e->QueryBoolAttribute("MessageBox", &m_bMessageBox);
    e->QueryBoolAttribute("Repeat", &m_bRepeat);
    e->Attribute("RepeatSeconds", &m_iRepeatSeconds);
    e->QueryBoolAttribute("AutoReset", &m_bAutoReset);
}

void Alarm::SaveConfigBase(TiXmlElement *c)
{
    c->SetAttribute("Enabled", m_bEnabled);
    c->SetAttribute("GraphicsEnabled", m_bgfxEnabled);
    c->SetAttribute("Sound", m_bSound);
    c->SetAttribute("SoundFile", m_sSound.mb_str());
    c->SetAttribute("Command", m_bCommand);
    c->SetAttribute("CommandFile", m_sCommand.mb_str());
    c->SetAttribute("MessageBox", m_bMessageBox);
    c->SetAttribute("Repeat", m_bRepeat);
    c->SetAttribute("RepeatSeconds", m_iRepeatSeconds);
    c->SetAttribute("AutoReset", m_bAutoReset);
}

void Alarm::OnTimer( wxTimerEvent & )
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T( "/Settings/Watchdog" ) );

    int enabled = pConf->Read ( _T ( "Enabled" ), 1L );

    if(enabled == 2 && !g_watchdog_pi->m_WatchdogDialog)
        enabled = 0;

    if(enabled == 3 && (!g_watchdog_pi->m_WatchdogDialog || !g_watchdog_pi->m_WatchdogDialog->IsShown()))
       enabled = 0;

    if(enabled && m_bEnabled) {
        if(Test()) {        
            wxDateTime now = wxDateTime::Now();
            if(m_bFired) {
                if((now - m_LastAlarmTime).GetSeconds() > m_iRepeatSeconds && m_bRepeat) {
                    Run();
                    m_LastAlarmTime = now;
                }
            } else {
                m_bFired = true;
                Run();
                m_LastAlarmTime = now;
            }
        } else if(m_bAutoReset)
            m_bFired = false;
    }

    if(g_watchdog_pi->m_WatchdogDialog && g_watchdog_pi->m_WatchdogDialog->IsShown())
        for(unsigned int i=0; i<Alarm::s_Alarms.size(); i++)
            if(Alarm::s_Alarms[i] == this)
                g_watchdog_pi->m_WatchdogDialog->UpdateStatus(i);
}
