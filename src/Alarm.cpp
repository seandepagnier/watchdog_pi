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

#include <map>

#include <wx/wx.h>
#include "wddc.h"

#include <wx/process.h>

#include "json/json.h"
#include "tinyxml.h"
#include "signalk_client.h"

#include "watchdog_pi.h"
#include "WatchdogDialog.h"
#include "WindPanel.h"
#include "WeatherPanel.h"

#include "AIS_Target_Info.h"
#include "nmea0183.h"
#include "ODAPI.h"

static double deg2rad(double deg)
{
    return (deg * M_PI / 180.0);
}

static double rad2deg(double rad)
{
    return (rad * 180.0 / M_PI);
}

///////// The Alarm classes /////////
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

    bool Test() {
        if(wxIsNaN(g_watchdog_pi->m_sog))
            return m_bNoData;
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
        if(wxIsNaN(anchordist))
            s = "N/A";
        else {
            wxString fmt("%.0f ");
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
        
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
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
        panel->m_tLatitude->SetValue(wxString::Format("%f", m_Latitude));
        panel->m_tLongitude->SetValue(wxString::Format("%f", m_Longitude));
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
        if(wxIsNaN(g_watchdog_pi->m_cog))
            return NAN;
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
    CourseAlarm() : Alarm(true), m_Mode(BOTH), m_Tolerance(20), m_bGPSCourse(true) {
        m_Course = g_watchdog_pi->m_cog;
    }

    wxString Type() { return _("Course"); }

    bool Test() {
        double error = CourseError();
        if(wxIsNaN(error))
            return m_bNoData;
            
        return error > m_Tolerance;
    }

    wxString GetStatus() {
        double courseerror = CourseError();
        wxString s;
        if(wxIsNaN(courseerror))
            s = "N/A";
        else {
            wxString fmt("%.0f ");
            s = wxString::Format(fmt + _("degrees(s)"), courseerror);
        }

        if(m_Mode == STARBOARD)
            s += wxString(" ") + _("Starboard");
        else if(m_Mode == PORT)
            s += wxString(" ") + _("Port");

        return s;
    }

    void Render(wdDC &dc, PlugIn_ViewPort &vp) {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();

        double lat1 = lastfix.Lat, lon1 = lastfix.Lon, lat2, lon2, lat3, lon3;
        double dist = lastfix.Sog;

        if(wxIsNaN(dist))
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
        panel->m_rbGPSCourse->SetValue(m_bGPSCourse);
        panel->m_rbHeading->SetValue(!m_bGPSCourse);
        return panel;
    }

    void SavePanel(wxWindow *p) {
        CoursePanel *panel = (CoursePanel*)p;
        m_Mode = (Mode)panel->m_cMode->GetSelection();
        m_Course = panel->m_sCourse->GetValue();
        m_Tolerance = panel->m_sTolerance->GetValue();
        m_bGPSCourse = panel->m_rbGPSCourse->GetValue();
    }

    void LoadConfig(TiXmlElement *e) {
        const char *mode = e->Attribute("Mode");
        if(!strcasecmp(mode, "Port")) m_Mode = PORT;
        else if(!strcasecmp(mode, "Starboard")) m_Mode = STARBOARD;
        else if(!strcasecmp(mode, "Starboard")) m_Mode = BOTH;
        else wxLogMessage("Watchdog: " + wxString(_("invalid Course mode")) + ": "
                         + wxString::FromUTF8(mode));

        e->Attribute("Tolerance", &m_Tolerance);
        e->Attribute("Course", &m_Course);
        e->QueryBoolAttribute("GPSCourse", &m_bGPSCourse);
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
        c->SetAttribute("GPSCourse", m_bGPSCourse);
    }

private:
    double CourseError() {
        double error = heading_resolve((m_bGPSCourse ?
                                        g_watchdog_pi->m_cog :
                                        g_watchdog_pi->m_hdm) - m_Course);
        switch(m_Mode) {
        case PORT:      return -error;
        case STARBOARD: return  error;
        default:        return fabs(error);
        }
    }
    
    enum Mode { PORT, STARBOARD, BOTH } m_Mode;
    double m_Tolerance, m_Course;
    bool m_bGPSCourse;
};

class SpeedAlarm : public Alarm
{
public:
    SpeedAlarm() : Alarm(true), m_Mode(UNDERSPEED), m_dSpeed(1) {}

    wxString Type() { return _("Speed"); }

    wxString GetStatus() {
        wxString s;
        if(wxIsNaN(g_watchdog_pi->m_sog))
            s = "N/A";
        else {
            wxString fmt("%.1f");
            s = wxString::Format(fmt + (Knots() < m_dSpeed ?
                                        " < " : " > ")
                                 + fmt, Knots(), m_dSpeed);
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
        double knots = Knots();
        if(wxIsNaN(knots))
            return m_bNoData;

        if(m_Mode == UNDERSPEED)
            return m_dSpeed > knots;

        return m_dSpeed < knots;
    }

    wxWindow *OpenPanel(wxWindow *parent) {
        SpeedPanel *panel = new SpeedPanel(parent);
        panel->m_cMode->SetSelection((int)m_Mode);
        panel->m_tSpeed->SetValue(wxString::Format("%f", m_dSpeed));
        panel->m_sliderSOGAverageNumber->SetValue(m_iAverageTime);
        return panel;
    }

    void SavePanel(wxWindow *p) {
        SpeedPanel *panel = (SpeedPanel*)p;
        m_Mode = (Mode)panel->m_cMode->GetSelection();
        panel->m_tSpeed->GetValue().ToDouble(&m_dSpeed);
        m_iAverageTime = panel->m_sliderSOGAverageNumber->GetValue();
    }

    void LoadConfig(TiXmlElement *e) {
        const char *mode = e->Attribute("Mode");
        if(!strcasecmp(mode, "Underspeed")) m_Mode = UNDERSPEED;
        else if(!strcasecmp(mode, "Overspeed")) m_Mode = OVERSPEED;
        else wxLogMessage("Watchdog: " + wxString(_("invalid Speed mode")) + ": "
                         + wxString::FromUTF8(mode));

        e->Attribute("Speed", &m_dSpeed);
        m_iAverageTime = 10;
        e->Attribute("AverageTime", &m_iAverageTime);
    }

    void SaveConfig(TiXmlElement *c) {
        c->SetAttribute("Type", "Speed");
        switch(m_Mode) {
        case UNDERSPEED: 
            c->SetAttribute("Mode", "Underspeed");
            break;
        case OVERSPEED: 
            c->SetAttribute("Mode", "Overspeed");
            break;
        }

        c->SetDoubleAttribute("Speed", m_dSpeed);
        c->SetAttribute("AverageTime", m_iAverageTime);
    }

    void OnTimer( wxTimerEvent &tEvent )
    {
        Alarm::OnTimer( tEvent );
        double sog = g_watchdog_pi->LastFix().Sog;
        if(!wxIsNaN(sog))
            m_SOGqueue.push_front(sog);
        while((int)m_SOGqueue.size() > m_iAverageTime)
            m_SOGqueue.pop_back();
    }
    
private:
    double Knots() {
        if(m_SOGqueue.size() == 0)
            return g_watchdog_pi->LastFix().Sog;
        // average speed in list
        double l_avSpeed = 0.0;
        for(std::list<double>::iterator it = m_SOGqueue.begin(); it != m_SOGqueue.end(); it++)
            l_avSpeed += *it;

        l_avSpeed /= m_SOGqueue.size();
        return l_avSpeed;
    }

    enum Mode { UNDERSPEED, OVERSPEED } m_Mode;
    double  m_dSpeed;
    int     m_iAverageTime;
    std::list<double> m_SOGqueue;
};

void TrueWind(double VA, double A, double VB, double &VW, double &W)
{
/* law of cosine:
           ________________________
          /  2    2
   VW =  / VA + VB - 2 VA VB cos(A)
       \/

    2    2    2
  VA = VW + VB + 2 VW VB cos(W)
                
    2    2    2
  VW + VB - VA  = 2 VW VB cos(W)

          /   2    2    2\
W = acos |  VW + VB - VA  |
         |  ------------  |
          \   2 VW VB    /
*/
    A = deg2rad(A);
    VW = sqrt(VA*VA + VB*VB - 2*VA*VB*cos(A));
    W = rad2deg(acos((VW*VW + VB*VB - VA*VA) / (2*VW*VB)));
}    

class WindAlarm : public Alarm
{
public:
    WindAlarm() : Alarm(true), m_Mode(UNDERSPEED), m_Type(APPARENT), m_dVal(5), m_dRange(15), m_speed(NAN), m_direction(NAN), m_gps_speed(0), m_bearing(NAN), m_WindDataTime(wxDateTime::Now()) {}

    wxString Type() { return _("Wind"); }

    wxString GetStatus() {
        wxString s;
        wxString fmt("%.1f");
        switch(m_Mode) {
        case UNDERSPEED:
        case OVERSPEED:
            if(wxIsNaN(m_speed))
                return "N/A";
            else
                return wxString::Format(fmt + (m_Mode == UNDERSPEED ? " < " : " > ") + fmt, m_speed, m_dVal);
        case DIRECTION:
            if(wxIsNaN(m_direction))
                return "N/A";
            else
                return wxString::Format(fmt + " < " + fmt + " < " + fmt,
                                        heading_resolve(m_dVal - m_dRange), m_direction,
                                        heading_resolve(m_dVal + m_dRange));
        }
        return "";
    }

    void Render(wdDC &dc, PlugIn_ViewPort &vp) {
        if(m_Mode != DIRECTION)
            return;
        if(wxIsNaN(m_direction))
            return;
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();

        double lat[4] = {lastfix.Lat}, lon[4] = {lastfix.Lon};
        double dist = lastfix.Sog;
        double bearing = m_bearing, direction = m_direction, speed;
        switch(m_Type) {
        case TRUE_ABSOLUTE:
            bearing = 0;
        case TRUE_RELATIVE:
            TrueWind(m_speed, m_direction, m_gps_speed, speed, direction);
        case APPARENT:
            break;
        }
        PositionBearingDistanceMercator_Plugin(lat[0], lon[0], bearing + direction,
                                               dist, &lat[1], &lon[1]);
        PositionBearingDistanceMercator_Plugin(lat[0], lon[0], bearing + m_dVal - m_dRange,
                                               dist, &lat[2], &lon[2]);
        PositionBearingDistanceMercator_Plugin(lat[0], lon[0], bearing + m_dVal + m_dRange,
                                               dist, &lat[2], &lon[2]);

        wxPoint r[4];
        for(int i=0; i<4; i++)
            GetCanvasPixLL(&vp, &r[i], lat[i], lon[i]);

        if(m_bFired)
            dc.SetPen(wxPen(*wxRED, 2));
        else
            dc.SetPen(wxPen(*wxGREEN, 2));

        for(int i=1; i<4; i++) {
            dc.DrawLine( r[0].x, r[0].y, r[i].x, r[i].y );
            dc.SetPen(wxPen(*wxBLUE, 2));
        }
    }

    bool Test() {
        // no data for 3 seconds..  Is this correct?
        if((wxDateTime::Now() - m_WindDataTime).GetSeconds() > 3)
            return m_bNoData;
        
        switch(m_Mode) {
        case UNDERSPEED: return m_dVal > m_speed;
        case OVERSPEED:  return m_dVal < m_speed;
        default: break;
        }

        double dir = heading_resolve(m_direction, m_dVal);
        switch(m_Mode) {
        case DIRECTION: return  dir < m_dVal - m_dRange || dir > m_dVal + m_dRange;
        default: break;
        }

        m_gps_speed = g_watchdog_pi->LastFix().Sog * .1 + m_gps_speed * .9;
        return 0;
    }

    wxWindow *OpenPanel(wxWindow *parent) {
        WindPanel *panel = new WindPanel(parent, m_direction);
        panel->m_cMode->SetSelection((int)m_Mode);
        panel->m_cType->SetSelection((int)m_Type);
        panel->m_sRange->Enable(m_Type == (int)DIRECTION);
        panel->m_sValue->SetValue(m_dVal);
        panel->m_sRange->SetValue(m_dRange);
        return panel;
    }

    void SavePanel(wxWindow *p) {
        WindPanel *panel = (WindPanel*)p;
        m_Mode = (Mode)panel->m_cMode->GetSelection();
        m_Type = (WindType)panel->m_cType->GetSelection();
        m_dVal = panel->m_sValue->GetValue();
        m_dRange = panel->m_sRange->GetValue();
    }

    void LoadConfig(TiXmlElement *e) {
        const char *mode = e->Attribute("Mode");
        if(!strcasecmp(mode, "Underspeed")) m_Mode = UNDERSPEED;
        else if(!strcasecmp(mode, "Overspeed")) m_Mode = OVERSPEED;
        else if(!strcasecmp(mode, "Direction")) {
            m_Mode = DIRECTION;
            e->Attribute("Range", &m_dRange);
        } else wxLogMessage("Watchdog: " + wxString(_("invalid Wind mode")) + ": "
                         + wxString::FromUTF8(mode));

        const char *type = e->Attribute("Type");
        if(!strcasecmp(mode, "Apparent")) m_Type = APPARENT;
        else if(!strcasecmp(mode, "True Relative")) m_Type = TRUE_RELATIVE;
        else if(!strcasecmp(mode, "True Absolute")) m_Type = TRUE_ABSOLUTE;
        else wxLogMessage("Watchdog: " + wxString(_("invalid Wind type")) + ": "
                         + wxString::FromUTF8(type));

        e->Attribute("Value", &m_dVal);
    }

    void SaveConfig(TiXmlElement *c) {
        c->SetAttribute("Type", "Wind");
        switch(m_Mode) {
        case UNDERSPEED: c->SetAttribute("Mode", "Underspeed"); break;
        case OVERSPEED:  c->SetAttribute("Mode", "Overspeed");  break;
        case DIRECTION:
            c->SetAttribute("Mode", "Direction");
            c->SetDoubleAttribute("Range", m_dRange);
            break;
        }

        c->SetDoubleAttribute("Value", m_dVal);
    }
    
private:
    void NMEAString(const wxString &string) {
        wxString str = string;
        NMEA0183 nmea;
        wxString LastSentenceIDReceived;
        nmea << str;
        if(!nmea.PreParse())
            return;
        if(nmea.LastSentenceIDReceived == "HDM" && nmea.Parse()) {
            m_bearing = nmea.Hdm.DegreesMagnetic + g_watchdog_pi->Declination();
        } else
        if(nmea.LastSentenceIDReceived == "MWV" &&
           nmea.Parse() && nmea.Mwv.IsDataValid == NTrue ) {
            double m_wSpeedFactor = 1.0; //knots ("N")
            if (nmea.Mwv.WindSpeedUnits == "K" ) m_wSpeedFactor = 0.53995 ; //km/h > knots
            if (nmea.Mwv.WindSpeedUnits == "M" ) m_wSpeedFactor = 1.94384;
            if(nmea.Mwv.Reference == "R" && m_Type == APPARENT) {
                ProcessWind(nmea.Mwv.WindSpeed * m_wSpeedFactor, nmea.Mwv.WindAngle);
            } else if(nmea.Mwv.Reference == "T" && m_Type == TRUE_RELATIVE) {
                // should we accept true wind nmea sentences rather than calculate ourself??
            }
        }
    }

    void ProcessWind(double apparent_speed, double apparent_direction)
    {
        m_WindDataTime = wxDateTime::Now();
        if(m_Type == APPARENT) {
            m_speed = apparent_speed;
            m_direction = apparent_direction;
        } else {
            TrueWind(m_speed, m_direction, m_gps_speed, m_speed, m_direction);
            if(m_Type == TRUE_ABSOLUTE) {
                m_direction += m_bearing;
                if(m_direction > 360)
                    m_direction -= 360;
            }
        }
        
    }

    enum Mode { UNDERSPEED, OVERSPEED, DIRECTION } m_Mode;
    enum WindType { APPARENT, TRUE_RELATIVE, TRUE_ABSOLUTE } m_Type;
    double  m_dVal, m_dRange;

    double m_speed, m_direction;
    double m_gps_speed, m_bearing;

    wxDateTime m_WindDataTime;
};

class WeatherAlarm : public Alarm
{
public:
    WeatherAlarm() : Alarm(false), m_Variable(BAROMETER), m_Mode(BELOW), m_dVal(1004), m_iRatePeriod(0),
                     m_curvalue(NAN), m_currate(NAN), m_WeatherDataTime(wxDateTime::Now()) {}

    wxString Type() { return _("Weather"); }

    wxString GetStatus() {
        wxString s = StrVariable();
        s += " ";
        
        double val = Value();
        if(wxIsNaN(val))
            s += "N/A";
        else {
            wxString fmt("%.2f");
            double aval = m_Mode == DECREASING ? -val : val;
            s += wxString::Format(fmt + (aval < m_dVal ?
                                        " < " : " > ")
                                 + fmt, aval, m_dVal);
            if(m_Mode == INCREASING || m_Mode == DECREASING)
                s += " " + _("In") + wxString::Format(" %d ", m_iRatePeriod) + _("Seconds");
        }

        return s;
    }

    bool Test() {
        // no data for 10 seconds..  Is this correct?
        if((wxDateTime::Now() - m_WeatherDataTime).GetSeconds() > 3)
            return m_bNoData;

        switch(m_Mode) {
        case ABOVE: case INCREASING: return m_dVal < Value();
        case BELOW: return m_dVal > Value();
        case DECREASING: return m_dVal < -Value();
        }
        return 0;
    }

    wxWindow *OpenPanel(wxWindow *parent) {
        WeatherPanel *panel = new WeatherPanel(parent);
        panel->m_cVariable->SetSelection((int)m_Variable);
        panel->m_rbRate->SetValue((bool)(m_Mode / 2));
        panel->m_cType->SetSelection((bool)(m_Mode % 2));
        panel->m_tValue->SetValue(wxString::Format("%f", m_dVal));
        panel->m_sRatePeriod->SetValue(m_iRatePeriod);
        panel->SetupControls();
        return panel;
    }

    void SavePanel(wxWindow *p) {
        WeatherPanel *panel = (WeatherPanel*)p;
        m_Variable = (WeatherAlarmVariable)panel->m_cVariable->GetSelection();
        m_Mode = (Mode)(panel->m_cType->GetSelection() + 2*panel->m_rbRate->GetValue());
        panel->m_tValue->GetValue().ToDouble(&m_dVal);
        m_iRatePeriod = panel->m_sRatePeriod->GetValue();
    }

    void LoadConfig(TiXmlElement *e) {
        const char *variable = e->Attribute("Variable");
        if(!strcasecmp(variable, "Barometer")) m_Variable = BAROMETER;
        else if(!strcasecmp(variable, "AirTemperature")) m_Variable = AIR_TEMPERATURE;
        else if(!strcasecmp(variable, "SeaTemperature")) m_Variable = SEA_TEMPERATURE;
        else if(!strcasecmp(variable, "RelativeHumidity")) m_Variable = RELATIVE_HUMIDITY;
        else {
            wxLogMessage("Watchdog: " + wxString(_("invalid Weather variable")) +
                         ": " + wxString::FromUTF8(variable));
            m_Variable = BAROMETER;
        }

        const char *mode = e->Attribute("Mode");
        if(!strcasecmp(mode, "Above")) m_Mode = ABOVE;
        else if(!strcasecmp(mode, "Below")) m_Mode = BELOW;
        else if(!strcasecmp(mode, "Increasing")) m_Mode = INCREASING;
        else if(!strcasecmp(mode, "Decreasing")) m_Mode = DECREASING;
        else {
            wxLogMessage("Watchdog: " + wxString(_("invalid Weather mode")) +
                         ": " + wxString::FromUTF8(mode));
            m_Mode = ABOVE;
        }

        e->Attribute("Value", &m_dVal);
        e->Attribute("RatePeriod", &m_iRatePeriod);
    }

    void SaveConfig(TiXmlElement *c) {
        c->SetAttribute("Type", "Weather");
        switch(m_Mode) {
            case ABOVE: c->SetAttribute("Mode", "Above"); break;
            case BELOW: c->SetAttribute("Mode", "Below"); break;
            case INCREASING: c->SetAttribute("Mode", "Increasing"); break;
            case DECREASING: c->SetAttribute("Mode", "Decreasing"); break;
        }

        const char *variable = "";
        switch(m_Variable) {
        case BAROMETER: variable = "Barometer"; break;
        case AIR_TEMPERATURE: variable = "AirTemperature"; break;
        case SEA_TEMPERATURE: variable = "SeaTemperature"; break;
        case RELATIVE_HUMIDITY: variable = "RelativeHumidity"; break;
        }
        c->SetAttribute("Variable", variable);

        c->SetDoubleAttribute("Value", m_dVal);
        c->SetAttribute("RatePeriod", m_iRatePeriod);
    }
    
private:
    double Value() {
        if(m_Mode == INCREASING || m_Mode == DECREASING)
            return m_currate;
        return m_curvalue;
    }

    wxString StrVariable() {
        switch(m_Variable) {
        case BAROMETER: return _("Barometer"); break;
        case AIR_TEMPERATURE: return _("Air Temperature"); break;
        case SEA_TEMPERATURE: return _("Sea Temperature"); break;
        case RELATIVE_HUMIDITY: return _("Humidity"); break;
        }
        return "";
    }

    void NMEAString(const wxString &string) {
        wxString str = string;
        NMEA0183 nmea;
        nmea << str;
        if(!nmea.PreParse())
            return;

        double value = NAN;
        switch(m_Variable) {
        case BAROMETER:
            if(nmea.LastSentenceIDReceived == "MDA" && nmea.Parse())
                value = nmea.Mda.Pressure * 1000;
            break;
        case RELATIVE_HUMIDITY:
            if(nmea.LastSentenceIDReceived == "MDA" && nmea.Parse())
                value = nmea.Mda.RelativeHumidity;
            break;
        case AIR_TEMPERATURE:
            if(nmea.LastSentenceIDReceived == "MTA" && nmea.Parse())
                value = nmea.Mta.Temperature;
            break;
        case SEA_TEMPERATURE:
            if(nmea.LastSentenceIDReceived == "MTW" && nmea.Parse())
                value = nmea.Mtw.Temperature;
            break;
        }
        if(wxIsNaN(value))
            return;

        m_WeatherDataTime = wxDateTime::Now();
        if(m_Mode == INCREASING || m_Mode == DECREASING) {
            wxDateTime now = wxDateTime::Now();
            if(!valuetime.IsValid()) {
                m_curvalue = value;
                valuetime = now;
            } else
            if((now - valuetime).GetSeconds() >= m_iRatePeriod) {
                m_currate = value - m_curvalue;
                m_curvalue = value;
                valuetime = now;
            }
        } else
            m_curvalue = value;
    }

    WeatherAlarmVariable m_Variable;
    enum Mode { ABOVE, BELOW, INCREASING, DECREASING } m_Mode;
    double m_dVal;
    int m_iRatePeriod;
    double m_curvalue, m_currate;
    wxDateTime valuetime;
    wxDateTime m_WeatherDataTime;
};

class DeadmanAlarm : public Alarm
{
public:
    DeadmanAlarm() : m_Minutes(20) {}

    wxString Type() { return _("Deadman"); }

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
        wxString d, fmt("%d ");
        if(days)
            d = wxString::Format(fmt + "days ", days);
        return d + wxString::Format("%02d:%02d:%02d",
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

class NMEADataAlarm : public Alarm
{
public:
    NMEADataAlarm() :
        start(wxDateTime::Now()),
        m_sentences("$GPGGA"),
        m_seconds(10)
        {}

    wxString Type() { return _("NMEA Data"); }

    bool Test() {
        return ElapsedSeconds() > m_seconds;
    }

    wxString GetStatus() {
        return wxString::Format("%d " + _("second(s)"), ElapsedSeconds());
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

class LandFallAlarm : public Alarm
{
public:
    LandFallAlarm() : Alarm(true, 5 /* seconds */),
                      m_Mode(TIME),
                      m_TimeMinutes(20),
                      m_Distance(3)
        {
            if(!PlugIn_GSHHS_CrossesLand(0, 0, 60, 60)) {
                wxLogMessage("Watchdog: " + wxString(_("landfall alarm without gshhs data")));
                m_bData = false;
            } else
                m_bData = true;
        }

    wxString Type() { return _("LandFall"); }

    bool Test() {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();

        if(wxIsNaN(lastfix.Lat))
            return m_bNoData;

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
                        m_LandFallTime = wxTimeSpan::Seconds(3600.0 * (dist + dist1) / lastfix.Sog);
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
        if(!m_bData)
            return _("No GSHHS Data");
        
        switch(m_Mode) {
        case TIME:
        {
            if(m_LandFallTime.IsNull())
                return _("LandFall Time Invalid");

            wxString s, fmt(" %d ");
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
            return wxString::Format(" " + wxString(_("Distance")) +
                                    (m_bFired ? " <" : " >") +
                                    " %.2f nm", m_Distance);
        } break;
        }
        return "";
    }

    void Render(wdDC &dc, PlugIn_ViewPort &vp) {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();
        if(wxIsNaN(m_crossinglat1))
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
        panel->m_tDistance->SetValue(wxString::Format("%f", m_Distance));
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
        else wxLogMessage("Watchdog: " + wxString(_("invalid LandFall mode")) + ": "
                         + wxString::FromUTF8(mode));

        e->Attribute("TimeMinutes", &m_TimeMinutes);
        e->Attribute("Distance", &m_Distance);
    }

    void SaveConfig(TiXmlElement *c) {
        c->SetAttribute("Type", "LandFall");
        switch(m_Mode) {
        case TIME: c->SetAttribute("Mode", "Time"); break;
        case DISTANCE: c->SetAttribute("Mode", "Distance"); break;
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

    bool m_bData;
};

extern Json::Value g_ReceivedBoundaryTimeJSONMsg;
extern wxString    g_ReceivedBoundaryTimeMessage;
extern Json::Value g_ReceivedBoundaryDistanceJSONMsg;
extern wxString    g_ReceivedBoundaryDistanceMessage;
extern Json::Value g_ReceivedBoundaryAnchorJSONMsg;
extern wxString    g_ReceivedBoundaryAnchorMessage;
extern Json::Value g_ReceivedGuardZoneJSONMsg;
extern wxString    g_ReceivedGuardZoneMessage;
extern Json::Value g_ReceivedODVersionJSONMsg;
extern wxString    g_ReceivedODVersionMessage;
extern wxString    g_GuardZoneName;
extern AIS_Target_Info g_AISTarget;
extern wxString    g_BoundaryName;
extern wxString    g_BoundaryDescription;
extern wxString    g_BoundaryGUID;
extern Json::Value g_ReceivedODAPIJSONMsg;
extern wxString    g_ReceivedODAPIMessage;
extern Json::Value g_ReceivedPathGUIDJSONMsg;
extern wxString    g_ReceivedPathGUIDMessage;


enum 
{   
    ID_BOUNDARY_EXCLUSION = 0,
    ID_BOUNDARY_INCLUSION,
    ID_BOUNDARY_NEITHER,
    ID_BOUNDARY_ANY,
    
    ID_BOUNDARY_TYPE_LAST
};
enum
{
    ID_BOUNDARY_STATE_ANY = 0,
    ID_BOUNDARY_STATE_ACTIVE,
    ID_BOUNDARY_STATE_INACTIVE,
    
    ID_BOUNDARY_STATE_LAST
};

class BoundaryAlarm : public Alarm
{
public:
    BoundaryAlarm() : Alarm(true, 3 /* seconds */),
                      m_Mode(TIME),
                      m_TimeMinutes(20),
                      m_Distance(3),
                      m_BoundaryType(ID_BOUNDARY_ANY),
                      m_BoundaryState(ID_BOUNDARY_STATE_ACTIVE),
                      m_bAnchorOutside(false),
                      m_bGuardZoneFired(false),
                      m_bGZFound(false)
        {
            g_GuardZoneName = wxEmptyString;
            m_baTimer.Connect(wxEVT_TIMER, wxTimerEventHandler( BoundaryAlarm::OnFlashTimer ), NULL, this);
            m_baTimer.Start(1000, wxTIMER_CONTINUOUS);
            m_iCheckFrequency = 3;
            m_bWasEnabled = false;
            GetODVersion();
        }

    wxString Type() { 
        switch(m_Mode) {
            case TIME: return _("Boundary Time");
            case DISTANCE: return _("Boundary Distance");
            case ANCHOR: return _("Inclusion Alarm");
            case GUARD: return _("Guard Zone");
            default: return _("Boundary Distance");
        }
    }
    
    wxString Options() {
        switch(m_Mode) {
        case TIME: return _("Time") + wxString::Format(_T(" < %f"), m_TimeMinutes);
        case DISTANCE: return _("Distance") + wxString::Format(_T(" < %f nm"), m_Distance);
        case ANCHOR: {
            if(m_BoundaryName == wxEmptyString)
                return _("Boundary GUID") + wxString(_T(": ")) + m_BoundaryGUID;
            else
                return _("Boundary Name") + wxString(_T(": ")) + m_BoundaryName;
        }
        case GUARD: {
            if(m_GuardZoneName == wxEmptyString)
                return _("Guard Zone GUID") + wxString(_T(": ")) + m_GuardZoneGUID;
            else
                return _("Guard Zone Name") + wxString(_T(": ")) + m_GuardZoneName;
        }
        default: return _T("");
        }
    }

    bool Test() {
        PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();

        if(wxIsNaN(lastfix.Lat))
            return m_bNoData;

        double lat, lon;
        double dist = 0;
                
        m_BoundaryTime = wxTimeSpan();

//        bool l_bUseODAPI_FPIAB = false;
        bool l_bUseODAPI_FCBLC = false;
        OD_FindPointInAnyBoundary pOD_FindPointInAnyBoundary = NULL;
        OD_FindClosestBoundaryLineCrossing pODFindClosestBoundaryLineCrossing = NULL;
        
        if(ODVersionNewerThan( 1, 1, 15)) {
            Json::Value jMsg;
            Json::FastWriter writer;

            jMsg["Source"] = "WATCHDOG_PI";
            jMsg["Type"] = "Request";
            jMsg["Msg"] = "GetAPIAddresses";
            jMsg["MsgId"] = "GetAPIAddresses";
            SendPluginMessage( "OCPN_DRAW_PI", writer.write( jMsg ) );
            if(g_ReceivedODAPIMessage != wxEmptyString &&  g_ReceivedODAPIJSONMsg["MsgId"].asString() == "GetAPIAddresses") {
                wxString sptr = g_ReceivedODAPIJSONMsg["OD_FindPointInAnyBoundary"].asString();
                if(sptr != "null") {
                    sscanf(sptr.To8BitData().data(), "%p", &pOD_FindPointInAnyBoundary);
                    //l_bUseODAPI_FPIAB = true;
                }
                sptr = g_ReceivedODAPIJSONMsg["OD_FindClosestBoundaryLineCrossing"].asString();
                if(sptr != "null") {
                    sscanf(sptr.To8BitData().data(), "%p", &pODFindClosestBoundaryLineCrossing);
                    l_bUseODAPI_FCBLC = true;
                }
            }
        }
        
        switch(m_Mode) {
            case TIME: {
                if(wxIsNaN(lastfix.Lat) || wxIsNaN(lastfix.Lon) ||wxIsNaN(lastfix.Cog) || wxIsNaN(lastfix.Sog)) break;
                if(ODVersionNewerThan( 1, 1, 1)) {
                    dist = lastfix.Sog * ( m_TimeMinutes / 60 );
                    PositionBearingDistanceMercator_Plugin(lastfix.Lat, lastfix.Lon, lastfix.Cog, dist, &lat, &lon);
                    if(l_bUseODAPI_FCBLC) {
                        FindClosestBoundaryLineCrossing_t *pFCPIAB = new FindClosestBoundaryLineCrossing_t;
                        pFCPIAB->dStartLat = lastfix.Lat;
                        pFCPIAB->dStartLon = lastfix.Lon;
                        pFCPIAB->dEndLat = lat;
                        pFCPIAB->dEndLon = lon;
                        switch (m_BoundaryType) {
                            case ID_BOUNDARY_ANY:
                                pFCPIAB->sBoundaryType = "Any";
                                break;
                            case ID_BOUNDARY_EXCLUSION:
                                pFCPIAB->sBoundaryType = "Exclusion";
                                break;
                            case ID_BOUNDARY_INCLUSION:
                                pFCPIAB->sBoundaryType = "Inclusion";
                                break;
                            case ID_BOUNDARY_NEITHER:
                                pFCPIAB->sBoundaryType = "Neither";
                                break;
                            default:
                                pFCPIAB->sBoundaryType = "Any";
                                break;
                        }
                        switch (m_BoundaryState) {
                            case ID_BOUNDARY_STATE_ANY:
                                pFCPIAB->sBoundaryState = "Any";
                                break;
                            case ID_BOUNDARY_STATE_ACTIVE:
                                pFCPIAB->sBoundaryState = "Active";
                                break;
                            case ID_BOUNDARY_STATE_INACTIVE:
                                pFCPIAB->sBoundaryState = "Inactive";
                                break;
                        }
                        if((*pODFindClosestBoundaryLineCrossing)(pFCPIAB)) {
                            if(m_bCurrentBoatPos)
                                m_BoundaryTime = wxTimeSpan();
                            else
                                m_BoundaryTime = wxTimeSpan::Seconds(3600.0 * dist / lastfix.Sog);
                            m_BoundaryAtLat = pFCPIAB->dCrossingLat;
                            m_BoundaryAtLon = pFCPIAB->dCrossingLon;
                            m_BoundaryDistance = pFCPIAB->dCrossingDistance;
                            m_BoundaryName = pFCPIAB->sName;
                            m_BoundaryDescription = pFCPIAB->sDescription;
                            m_BoundaryGUID = pFCPIAB->sGUID;
                            delete pFCPIAB;
                            return true;
                        }
                        delete pFCPIAB;
                        return false;
                        
                    } else {
                        Json::Value jMsg;
                        Json::FastWriter writer;
                        wxString    MsgString;
                        jMsg["Source"] = "WATCHDOG_PI";
                        jMsg["Type"] = "Request";
                        jMsg["Msg"] = "FindClosestBoundaryLineCrossing";
                        jMsg["MsgId"] = "time";
                        jMsg["StartLat"] = lastfix.Lat;
                        jMsg["StartLon"] = lastfix.Lon;
                        jMsg["EndLat"] = lat;
                        jMsg["EndLon"] = lon;
                        switch (m_BoundaryType) {
                            case ID_BOUNDARY_ANY:
                                jMsg["BoundaryType"] = "Any";
                                break;
                            case ID_BOUNDARY_EXCLUSION:
                                jMsg["BoundaryType"] = "Exclusion";
                                break;
                            case ID_BOUNDARY_INCLUSION:
                                jMsg["BoundaryType"] = "Inclusion";
                                break;
                            case ID_BOUNDARY_NEITHER:
                                jMsg["BoundaryType"] = "Neither";
                                break;
                            default:
                                jMsg["BoundaryType"] = "Any";
                                break;
                        }
                        switch (m_BoundaryState) {
                            case ID_BOUNDARY_STATE_ANY:
                                jMsg["BoundaryState"] = "Any";
                                break;
                            case ID_BOUNDARY_STATE_ACTIVE:
                                jMsg["BoundaryState"] = "Active";
                                break;
                            case ID_BOUNDARY_STATE_INACTIVE:
                                jMsg["BoundaryState"] = "Inactive";
                                break;
                        }
                        MsgString = writer.write( jMsg );
                        SendPluginMessage( "OCPN_DRAW_PI", MsgString );
                        if(g_ReceivedBoundaryTimeMessage != wxEmptyString &&
                           g_ReceivedBoundaryTimeJSONMsg["MsgId"].asString() == "time" &&
                            g_ReceivedBoundaryTimeJSONMsg["Found"].asBool() == true ) {
                            // This is our message
                            int l_BoundaryType = ID_BOUNDARY_ANY;
                            if(g_ReceivedBoundaryTimeJSONMsg["BoundaryType"].asString() == "Exclusion") l_BoundaryType = ID_BOUNDARY_EXCLUSION;
                            else if(g_ReceivedBoundaryTimeJSONMsg["BoundaryType"].asString() == "Inclusion") l_BoundaryType = ID_BOUNDARY_INCLUSION;
                            else if(g_ReceivedBoundaryTimeJSONMsg["BoundaryType"].asString() == "Neither") l_BoundaryType = ID_BOUNDARY_NEITHER;
                            if(m_BoundaryType == ID_BOUNDARY_ANY || m_BoundaryType == l_BoundaryType ) {
                                if(m_bCurrentBoatPos)
                                    m_BoundaryTime = wxTimeSpan();
                                else
                                    m_BoundaryTime = wxTimeSpan::Seconds(3600.0 * dist / lastfix.Sog);
                                m_BoundaryAtLat = g_ReceivedBoundaryTimeJSONMsg["CrossingLat"].asDouble();
                                m_BoundaryAtLon = g_ReceivedBoundaryTimeJSONMsg["CrossingLon"].asDouble();
                                m_BoundaryDistance = g_ReceivedBoundaryTimeJSONMsg["CrossingDist"].asDouble();
                                m_BoundaryName = g_ReceivedBoundaryTimeJSONMsg["Name"].asString();
                                m_BoundaryDescription = g_ReceivedBoundaryTimeJSONMsg["Description"].asString();
                                m_BoundaryGUID = g_ReceivedBoundaryTimeJSONMsg["GUID"].asString();
                                g_ReceivedBoundaryTimeMessage = wxEmptyString;
                                return true;
                            }
                        }
                    }    
                } else {
                    for(int count = 11; count > 0; count--) {
                        if(count == 11) {
                            lat = lastfix.Lat;
                            lon = lastfix.Lon;
                            m_bCurrentBoatPos = true;
                        } else {
                            dist = lastfix.Sog * ( m_TimeMinutes / 60 ) / count;
                            PositionBearingDistanceMercator_Plugin(lastfix.Lat, lastfix.Lon, lastfix.Cog, dist , &lat, &lon);
                            m_bCurrentBoatPos = false;
                        }
                        
                        // Do JSON message to OD Plugin to check if boundary m_crossinglat
                        Json::Value jMsg;
                        Json::FastWriter writer;
                        wxString    MsgString;
                        jMsg["Source"] = "WATCHDOG_PI";
                        jMsg["Type"] = "Request";
                        jMsg["Msg"] = "FindPointInAnyBoundary";
                        jMsg["MsgId"] = "time";
                        jMsg["lat"] = lat;
                        jMsg["lon"] = lon;
                        switch (m_BoundaryType) {
                            case ID_BOUNDARY_ANY:
                                jMsg["BoundaryType"] = "Any";
                                break;
                            case ID_BOUNDARY_EXCLUSION:
                                jMsg["BoundaryType"] = "Exclusion";
                                break;
                            case ID_BOUNDARY_INCLUSION:
                                jMsg["BoundaryType"] = "Inclusion";
                                break;
                            case ID_BOUNDARY_NEITHER:
                                jMsg["BoundaryType"] = "Neither";
                                break;
                            default:
                                jMsg["BoundaryType"] = "Any";
                                break;
                        }
                        switch (m_BoundaryState) {
                            case ID_BOUNDARY_STATE_ANY:
                                jMsg["BoundaryState"] = "Any";
                                break;
                            case ID_BOUNDARY_STATE_ACTIVE:
                                jMsg["BoundaryState"] = "Active";
                                break;
                            case ID_BOUNDARY_STATE_INACTIVE:
                                jMsg["BoundaryState"] = "Inactive";
                                break;
                        }
                        MsgString = writer.write( jMsg );
                        SendPluginMessage( "OCPN_DRAW_PI", MsgString );
                        if(g_ReceivedBoundaryTimeMessage != wxEmptyString &&
                           g_ReceivedBoundaryTimeJSONMsg["MsgId"].asString() == "time" &&
                           g_ReceivedBoundaryTimeJSONMsg["Found"].asBool() == true ) {
                            // This is our message
                            int l_BoundaryType = ID_BOUNDARY_ANY;
                            if(g_ReceivedBoundaryTimeJSONMsg["BoundaryType"].asString() == "Exclusion") l_BoundaryType = ID_BOUNDARY_EXCLUSION;
                            else if(g_ReceivedBoundaryTimeJSONMsg["BoundaryType"].asString() == "Inclusion") l_BoundaryType = ID_BOUNDARY_INCLUSION;
                            else if(g_ReceivedBoundaryTimeJSONMsg["BoundaryType"].asString() == "Neither") l_BoundaryType = ID_BOUNDARY_NEITHER;
                            if(m_BoundaryType == ID_BOUNDARY_ANY || m_BoundaryType == l_BoundaryType ) {
                                if(m_bCurrentBoatPos)
                                    m_BoundaryTime = wxTimeSpan();
                                else
                                    m_BoundaryTime = wxTimeSpan::Seconds(3600.0 * dist / lastfix.Sog);
                                m_BoundaryName = g_ReceivedBoundaryTimeJSONMsg["Name"].asString();
                                m_BoundaryDescription = g_ReceivedBoundaryTimeJSONMsg["Description"].asString();
                                m_BoundaryGUID = g_ReceivedBoundaryTimeJSONMsg["GUID"].asString();
                                g_ReceivedBoundaryTimeMessage = wxEmptyString;
                                return true;
                            }
                        }
                        g_ReceivedBoundaryTimeMessage = wxEmptyString;
                    }
                }
                break;
            }
            case DISTANCE: {
                if(wxIsNaN(lastfix.Lat) || wxIsNaN(lastfix.Lon)) break;
                // check OD version to see which lookup to use
                if( ODVersionNewerThan( 1, 1, 1)) {
                    BoundaryCrossingList.clear();
                    for(double t = 0; t<360; t+=10) {
                        PositionBearingDistanceMercator_Plugin(lastfix.Lat, lastfix.Lon, t, m_Distance , &lat, &lon);
                        if(l_bUseODAPI_FCBLC) {
                            FindClosestBoundaryLineCrossing_t *pFCPIAB = new FindClosestBoundaryLineCrossing_t;
                            pFCPIAB->dStartLat = lastfix.Lat;
                            pFCPIAB->dStartLon = lastfix.Lon;
                            pFCPIAB->dEndLat = lat;
                            pFCPIAB->dEndLon = lon;
                            switch (m_BoundaryType) {
                                case ID_BOUNDARY_ANY:
                                    pFCPIAB->sBoundaryType = "Any";
                                    break;
                                case ID_BOUNDARY_EXCLUSION:
                                    pFCPIAB->sBoundaryType = "Exclusion";
                                    break;
                                case ID_BOUNDARY_INCLUSION:
                                    pFCPIAB->sBoundaryType = "Inclusion";
                                    break;
                                case ID_BOUNDARY_NEITHER:
                                    pFCPIAB->sBoundaryType = "Neither";
                                    break;
                                default:
                                    pFCPIAB->sBoundaryType = "Any";
                                    break;
                            }
                            switch (m_BoundaryState) {
                                case ID_BOUNDARY_STATE_ANY:
                                    pFCPIAB->sBoundaryState = "Any";
                                    break;
                                case ID_BOUNDARY_STATE_ACTIVE:
                                    pFCPIAB->sBoundaryState = "Active";
                                    break;
                                case ID_BOUNDARY_STATE_INACTIVE:
                                    pFCPIAB->sBoundaryState = "Inactive";
                                    break;
                            }
                            if((*pODFindClosestBoundaryLineCrossing)(pFCPIAB)) {
                                BOUNDARYCROSSING l_BoundaryCrossing;
                                l_BoundaryCrossing.Name = pFCPIAB->sName;
                                l_BoundaryCrossing.Description = pFCPIAB->sDescription;
                                l_BoundaryCrossing.GUID = pFCPIAB->sGUID;
                                l_BoundaryCrossing.Lon = pFCPIAB->dCrossingLon;
                                l_BoundaryCrossing.Lat = pFCPIAB->dCrossingLat;
                                l_BoundaryCrossing.Len = pFCPIAB->dCrossingDistance;
                                BoundaryCrossingList.push_back(l_BoundaryCrossing);
                            }
                            delete pFCPIAB;
                        } else {
                            Json::Value jMsg;
                            Json::FastWriter writer;
                            wxString    MsgString;
                            jMsg["Source"] = "WATCHDOG_PI";
                            jMsg["Type"] = "Request";
                            jMsg["Msg"] = "FindClosestBoundaryLineCrossing";
                            jMsg["MsgId"] = "distance";
                            jMsg["StartLat"] = lastfix.Lat;
                            jMsg["StartLon"] = lastfix.Lon;
                            jMsg["EndLat"] = lat;
                            jMsg["EndLon"] = lon;
                            switch (m_BoundaryType) {
                                case ID_BOUNDARY_ANY:
                                    jMsg["BoundaryType"] = "Any";
                                    break;
                                case ID_BOUNDARY_EXCLUSION:
                                    jMsg["BoundaryType"] = "Exclusion";
                                    break;
                                case ID_BOUNDARY_INCLUSION:
                                    jMsg["BoundaryType"] = "Inclusion";
                                    break;
                                case ID_BOUNDARY_NEITHER:
                                    jMsg["BoundaryType"] = "Neither";
                                    break;
                                default:
                                    jMsg["BoundaryType"] = "Any";
                                    break;
                            }
                            switch (m_BoundaryState) {
                                case ID_BOUNDARY_STATE_ANY:
                                    jMsg["BoundaryState"] = "Any";
                                    break;
                                case ID_BOUNDARY_STATE_ACTIVE:
                                    jMsg["BoundaryState"] = "Active";
                                    break;
                                case ID_BOUNDARY_STATE_INACTIVE:
                                    jMsg["BoundaryState"] = "Inactive";
                                    break;
                            }
                            MsgString = writer.write( jMsg );
                            SendPluginMessage( "OCPN_DRAW_PI", MsgString );
                            if(g_ReceivedBoundaryDistanceMessage != wxEmptyString &&
                               g_ReceivedBoundaryDistanceJSONMsg["MsgId"].asString() == "distance" &&
                                g_ReceivedBoundaryDistanceJSONMsg["Found"].asBool() == true ) {
                                // This is our message
                                int l_BoundaryType = ID_BOUNDARY_ANY;
                                if(g_ReceivedBoundaryDistanceJSONMsg["BoundaryType"].asString() == "Exclusion") l_BoundaryType = ID_BOUNDARY_EXCLUSION;
                                else if(g_ReceivedBoundaryDistanceJSONMsg["BoundaryType"].asString() == "Inclusion") l_BoundaryType = ID_BOUNDARY_INCLUSION;
                                else if(g_ReceivedBoundaryDistanceJSONMsg["BoundaryType"].asString() == "Neither") l_BoundaryType = ID_BOUNDARY_NEITHER;
                                if(m_BoundaryType == ID_BOUNDARY_ANY || m_BoundaryType == l_BoundaryType ) {
                                    BOUNDARYCROSSING l_BoundaryCrossing;
                                    l_BoundaryCrossing.Name = g_ReceivedBoundaryDistanceJSONMsg["Name"].asString();
                                    l_BoundaryCrossing.Description = g_ReceivedBoundaryDistanceJSONMsg["Description"].asString();
                                    l_BoundaryCrossing.GUID = g_ReceivedBoundaryDistanceJSONMsg["GUID"].asString();
                                    l_BoundaryCrossing.Lon = g_ReceivedBoundaryDistanceJSONMsg["CrossingLon"].asDouble();
                                    l_BoundaryCrossing.Lat = g_ReceivedBoundaryDistanceJSONMsg["CrossingLat"].asDouble();
                                    l_BoundaryCrossing.Len = g_ReceivedBoundaryDistanceJSONMsg["CrossingDist"].asDouble();
                                    BoundaryCrossingList.push_back(l_BoundaryCrossing);
                                    g_ReceivedBoundaryDistanceMessage = wxEmptyString;
                                }
                            }
                        }
                    }
                    if(!BoundaryCrossingList.empty()) {
                        std::list<BOUNDARYCROSSING>::iterator it = BoundaryCrossingList.begin();
                        double l_dLen = it->Len;
                        m_BoundaryName = it->Name;
                        m_BoundaryDescription = it->Description;
                        m_BoundaryGUID = it->GUID;
                        m_BoundaryAtLat = it->Lat;
                        m_BoundaryAtLon = it->Lon;
                        m_BoundaryDistance = it->Len;
                        it++;
                        while( it != BoundaryCrossingList.end() ) {
                            if( l_dLen > it->Len ) {
                                l_dLen = it->Len;
                                m_BoundaryName = it->Name;
                                m_BoundaryDescription = it->Description;
                                m_BoundaryGUID = it->GUID;
                                m_BoundaryAtLat = it->Lat;
                                m_BoundaryAtLon = it->Lon;
                                m_BoundaryDistance = it->Len;
                            }
                            it++;
                        }
                        return true;
                    }
                } else {
                    int step_count = 0;
                    for(int count = 11; count > 0; count--) {
                        double t_step;
                        if(count == 11 || step_count < 6) t_step = 45;      // increase the angle between checks when looking close to boat
                        else t_step = 360 / (8 + ((32/6) * (step_count - 5)));
                        step_count++;
                        
                        for(double t = 0; t<360; t+=t_step) {
                            if(count == 11) {
                                lat = lastfix.Lat;
                                lon = lastfix.Lon;
                                t = 360; // no point looking around if we are checking current boat position
                                m_bCurrentBoatPos = true;
                            } else {
                                dist = m_Distance / count;      // be more accurate the closer to the boundary the boat is.
                                PositionBearingDistanceMercator_Plugin(lastfix.Lat, lastfix.Lon, t, dist , &lat, &lon);
                                m_bCurrentBoatPos = false;
                            }
                            
                            Json::Value jMsg;
                            Json::FastWriter writer;
                            wxString    MsgString;
                            jMsg["Source"] = "WATCHDOG_PI";
                            jMsg["Type"] = "Request";
                            jMsg["Msg"] = "FindPointInAnyBoundary";
                            jMsg["MsgId"] = "distance";
                            jMsg["lat"] = lat;
                            jMsg["lon"] = lon;
                            switch (m_BoundaryType) {
                                case ID_BOUNDARY_ANY:
                                    jMsg["BoundaryType"] = "Any";
                                    break;
                                case ID_BOUNDARY_EXCLUSION:
                                    jMsg["BoundaryType"] = "Exclusion";
                                    break;
                                case ID_BOUNDARY_INCLUSION:
                                    jMsg["BoundaryType"] = "Inclusion";
                                    break;
                                case ID_BOUNDARY_NEITHER:
                                    jMsg["BoundaryType"] = "Neither";
                                    break;
                                default:
                                    jMsg["BoundaryType"] = "Any";
                                    break;
                            }
                            switch (m_BoundaryState) {
                                case ID_BOUNDARY_STATE_ANY:
                                    jMsg["BoundaryState"] = "Any";
                                    break;
                                case ID_BOUNDARY_STATE_ACTIVE:
                                    jMsg["BoundaryState"] = "Active";
                                    break;
                                case ID_BOUNDARY_STATE_INACTIVE:
                                    jMsg["BoundaryState"] = "Inactive";
                                    break;
                            }
                            MsgString = writer.write( jMsg );
                            SendPluginMessage( "OCPN_DRAW_PI", MsgString );
                            if(g_ReceivedBoundaryDistanceMessage != wxEmptyString &&
                                g_ReceivedBoundaryDistanceJSONMsg["MsgId"].asString() == "distance" &&
                                g_ReceivedBoundaryDistanceJSONMsg["Found"].asBool() == true ) {
                                // This is our message
                                int l_BoundaryType = ID_BOUNDARY_ANY;
                            if(g_ReceivedBoundaryDistanceJSONMsg["BoundaryType"].asString() == "Exclusion") l_BoundaryType = ID_BOUNDARY_EXCLUSION;
                            else if(g_ReceivedBoundaryDistanceJSONMsg["BoundaryType"].asString() == "Inclusion") l_BoundaryType = ID_BOUNDARY_INCLUSION;
                            else if(g_ReceivedBoundaryDistanceJSONMsg["BoundaryType"].asString() == "Neither") l_BoundaryType = ID_BOUNDARY_NEITHER;
                            if(m_BoundaryType == ID_BOUNDARY_ANY || m_BoundaryType == l_BoundaryType ) {
                                if(m_bCurrentBoatPos)
                                    m_BoundaryDistance = 0;
                                else
                                    m_BoundaryDistance = dist;
                                m_BoundaryDirection = t;
                                m_BoundaryAtLat = lat;
                                m_BoundaryAtLon = lon;
                                m_BoundaryName = g_ReceivedBoundaryDistanceJSONMsg["Name"].asString();
                                m_BoundaryDescription = g_ReceivedBoundaryDistanceJSONMsg["Description"].asString();
                                m_BoundaryGUID = g_ReceivedBoundaryDistanceJSONMsg["GUID"].asString();
                                g_ReceivedBoundaryDistanceMessage = wxEmptyString;
                                return true;
                            }
                                }
                                g_ReceivedBoundaryDistanceMessage = wxEmptyString;
                        }
                    }
                }
                break;
            }
            case ANCHOR: {
                if(wxIsNaN(lastfix.Lat) || wxIsNaN(lastfix.Lon)) break;
                if(m_BoundaryName == wxEmptyString)
                    m_BoundaryName = g_BoundaryName;
                if(m_BoundaryDescription == wxEmptyString)
                    m_BoundaryDescription = g_BoundaryDescription;
                
                Json::Value jMsg;
                Json::FastWriter writer;
                jMsg["Source"] = "WATCHDOG_PI";
                jMsg["Type"] = "Request";
                jMsg["Msg"] = "FindPointInBoundary";
                jMsg["MsgId"] = "anchor";
                jMsg["GUID"] = (std::string)m_BoundaryGUID;
                jMsg["lat"] = lastfix.Lat;
                jMsg["lon"] = lastfix.Lon;
                
                SendPluginMessage( "OCPN_DRAW_PI", writer.write( jMsg ));
                if(g_ReceivedBoundaryAnchorMessage != wxEmptyString &&
                    g_ReceivedBoundaryAnchorJSONMsg["MsgId"].asString() == "anchor" &&
                    g_ReceivedBoundaryAnchorJSONMsg["Found"].asBool() == false ) {

                    // This is our message
                    g_ReceivedBoundaryDistanceMessage = wxEmptyString;
                    m_bAnchorOutside = true;
                    return true;
                }
                m_bAnchorOutside = false;
                g_ReceivedBoundaryDistanceMessage = wxEmptyString;
                break;
            }
            case GUARD: {
                if(wxIsNaN(g_AISTarget.m_dLat) || wxIsNaN(g_AISTarget.m_dLat)) break;
                Json::Value jMsg;
                Json::FastWriter writer;
                jMsg["Source"] = "WATCHDOG_PI";
                jMsg["Type"] = "Request";
                jMsg["Msg"] = "FindPointInGuardZone";
                jMsg["MsgId"] = "guard";
                jMsg["GUID"] = (std::string)m_GuardZoneGUID;
                jMsg["lat"] = g_AISTarget.m_dLat;
                jMsg["lon"] = g_AISTarget.m_dLon;
                switch (m_BoundaryType) {
                    case ID_BOUNDARY_ANY:
                        jMsg["BoundaryType"] = "Any";
                        break;
                    case ID_BOUNDARY_EXCLUSION:
                        jMsg["BoundaryType"] = "Exclusion";
                        break;
                    case ID_BOUNDARY_INCLUSION:
                        jMsg["BoundaryType"] = "Inclusion";
                        break;
                    case ID_BOUNDARY_NEITHER:
                        jMsg["BoundaryType"] = "Neither";
                        break;
                    default:
                        jMsg["BoundaryType"] = "Any";
                        break;
                }
                switch (m_BoundaryState) {
                    case ID_BOUNDARY_STATE_ANY:
                        jMsg["BoundaryState"] = "Any";
                        break;
                    case ID_BOUNDARY_STATE_ACTIVE:
                        jMsg["BoundaryState"] = "Active";
                        break;
                    case ID_BOUNDARY_STATE_INACTIVE:
                        jMsg["BoundaryState"] = "Inactive";
                        break;
                }
                
                SendPluginMessage( "OCPN_DRAW_PI", writer.write( jMsg ));
                if(g_ReceivedGuardZoneMessage != wxEmptyString &&
                   g_ReceivedGuardZoneJSONMsg["MsgId"].asString() == "guard" &&
                   g_ReceivedGuardZoneJSONMsg["Found"].asBool() == true ) { 

                    g_ReceivedGuardZoneMessage = wxEmptyString;
                    m_GuardZoneName = g_ReceivedGuardZoneJSONMsg["Name"].asString();
                    m_GuardZoneDescription = g_ReceivedGuardZoneJSONMsg["Description"].asString();
                    m_GuardZoneGUID = g_ReceivedGuardZoneJSONMsg["GUID"].asString();
                    return true;
                }
                g_ReceivedGuardZoneMessage = wxEmptyString;
                if(m_bSpecial) return true;
                break;
            }
        }
       
        return false;
    }

    wxString GetStatus() {
        switch(m_Mode) {
            case TIME:
            {
                wxString l_s;
                if(!m_bWasEnabled && m_bEnabled) {
                    GetODVersion();
                }
                
                m_bWasEnabled = m_bEnabled;
                
                if(m_BoundaryName != wxEmptyString)
                    l_s = " " + wxString(_("Boundary name")) + ": " + m_BoundaryName;
                else
                    l_s =  " " + wxString(_("Boundary GUID")) + ": " + m_BoundaryGUID;

                if(m_BoundaryTime.IsNull()) {
                    if(!m_bCurrentBoatPos) {
                        l_s = _("No Boundary within ");
                        l_s << m_TimeMinutes;
                        l_s += _(" minutes");
                    }
                    else {
                        switch (m_BoundaryState) {
                            case ID_BOUNDARY_STATE_ANY:
                                l_s.append(_(" - inside any boundary"));
                                break;
                            case ID_BOUNDARY_STATE_ACTIVE:
                                l_s.append(_(" - inside active boundary"));
                                break;
                            case ID_BOUNDARY_STATE_INACTIVE:
                                l_s.append(_(" - inside inactive boundary"));
                                break;
                        }
                    }
                } else {
                    l_s.append(_(" in ") + TimeBoundaryMsg());
                }
                return l_s;
                break;
            }
            case DISTANCE:
            {
                wxString l_s;
                if(!m_bWasEnabled && m_bEnabled) {
                    GetODVersion();
                }
                
                m_bWasEnabled = m_bEnabled;
                
                if(m_bFired) {
                    if(m_BoundaryName != wxEmptyString)
                        l_s = " " + wxString(_("Boundary name")) + ": " + m_BoundaryName;
                    else
                        l_s =  " " + wxString(_("Boundary GUID")) + ": " + m_BoundaryGUID;
                    if(m_bCurrentBoatPos) {
                        switch (m_BoundaryState) {
                            case ID_BOUNDARY_STATE_ANY:
                                l_s.append(_(" - inside any boundary"));
                                break;
                            case ID_BOUNDARY_STATE_ACTIVE:
                                l_s.append(_(" - inside active boundary"));
                                break;
                            case ID_BOUNDARY_STATE_INACTIVE:
                                l_s.append(_(" - inside inactive boundary"));
                                break;
                        }
                    }
                    else {
                        l_s.append(" <= ");
                        l_s << m_BoundaryDistance;
                        l_s.append(" nm");
                    }
                } else {
                    switch (m_BoundaryState) {
                        case ID_BOUNDARY_STATE_ANY:
                            l_s = _("Any Boundary Distance") + " >";
                            break;
                        case ID_BOUNDARY_STATE_ACTIVE:
                            l_s = _("Active Boundary Distance") + " >";
                            break;
                        case ID_BOUNDARY_STATE_INACTIVE:
                            l_s = _("Inactive Boundary Distance") + " >";
                            break;
                    }
                    l_s.append(wxString::Format(" %.2f nm", m_Distance));
                }
                return l_s;
                break;
            } 
            case ANCHOR:
            {
                if(m_BoundaryName != wxEmptyString) {
                    return " " + wxString(_("Boat")) + " " +
                    (m_bAnchorOutside ? _("outside") : _("inside")) +
                    wxString(_(" boundary area: ")) + m_BoundaryName;
                } else {
                return " " + wxString(_("Boat")) + " " +
                    (m_bAnchorOutside ? wxString(_("outside")) : wxString(_("inside"))) +
                    wxString(_(" boundary area: ")) + m_BoundaryGUID;
                }
                break;
            }
            case GUARD:
            {
                Json::Value jMsg;
                Json::FastWriter writer;
                wxString    MsgString;
                
                jMsg["Source"] = "WATCHDOG_PI";
                jMsg["Type"] = "Request";
                jMsg["Msg"] = "FindPathByGUID";
                jMsg["MsgId"] = "guard";
                jMsg["GUID"] = (std::string)m_GuardZoneGUID;
                MsgString = writer.write( jMsg );
                g_ReceivedPathGUIDMessage = wxEmptyString;
                SendPluginMessage( "OCPN_DRAW_PI", MsgString );
                if(g_ReceivedPathGUIDMessage != wxEmptyString && 
                   g_ReceivedPathGUIDJSONMsg["MsgId"].asString() == "guard" && 
                   g_ReceivedPathGUIDJSONMsg["Found"].asBool() == true ) {
                    g_GuardZoneName = g_ReceivedPathGUIDJSONMsg["Name"].asString();
                    m_bSpecial = false;
                } else {
                    m_bSpecial = true;
                }
                if(m_bSpecial)
                    return " " + wxString(_("Guard Zone")) + ": " + m_GuardZoneName + ": Not found";
                else
                    return " " + wxString(_("Guard Zone")) + ": " + m_GuardZoneName + ": " +
                    (m_bGuardZoneFired ? wxString(_("AIS Target in zone")) : wxString(_("NO AIS tagets found in zone")));
                break;
            }
        }
        return "";
    }

    void Render(wdDC &dc, PlugIn_ViewPort &vp) {
        if(m_bFired) {
            PlugIn_Position_Fix_Ex lastfix = g_watchdog_pi->LastFix();
            wxPoint r1, r2;

            GetCanvasPixLL(&vp, &r1, lastfix.Lat, lastfix.Lon);
            GetCanvasPixLL(&vp, &r2, m_BoundaryAtLat, m_BoundaryAtLon);
            if(m_bHighlight) {
                dc.SetPen(wxPen(*wxRED, 3));
            } else {
                dc.SetPen(wxPen(*wxLIGHT_GREY, 2));
            }
            dc.DrawLine( r1.x, r1.y, r2.x, r2.y );
        }
    }
    
    wxWindow *OpenPanel(wxWindow *parent) {
        BoundaryPanel *panel = new BoundaryPanel(parent);
        panel->m_rbTime->SetValue(m_Mode == TIME);
        panel->m_rbDistance->SetValue(m_Mode == DISTANCE);
        panel->m_sliderCheckFrequency->SetValue(m_iCheckFrequency);
        panel->m_rbAnchor->SetValue(m_Mode == ANCHOR);
        panel->m_rbGuard->SetValue(m_Mode == GUARD);
        panel->m_sTimeMinutes->SetValue(m_TimeMinutes);
        panel->m_tDistance->SetValue(wxString::Format("%f", m_Distance));
        switch (m_BoundaryType) {
            case ID_BOUNDARY_ANY:
                panel->m_radioBoxBoundaryType->SetSelection(0);
                break;
            case ID_BOUNDARY_EXCLUSION:
                panel->m_radioBoxBoundaryType->SetSelection(1);
                break;
            case ID_BOUNDARY_INCLUSION:
                panel->m_radioBoxBoundaryType->SetSelection(2);
                break;
            case ID_BOUNDARY_NEITHER:
                panel->m_radioBoxBoundaryType->SetSelection(3);
                break;
            default:
                panel->m_radioBoxBoundaryType->SetSelection(0);
                break;
        }
        switch (m_BoundaryState) {
            case ID_BOUNDARY_STATE_ANY:
                panel->m_radioBoxBoundaryState->SetSelection(0);
                break;
            case ID_BOUNDARY_STATE_ACTIVE:
                panel->m_radioBoxBoundaryState->SetSelection(1);
                break;
            case ID_BOUNDARY_STATE_INACTIVE:
                panel->m_radioBoxBoundaryState->SetSelection(2);
                break;
            default:
                panel->m_radioBoxBoundaryState->SetSelection(0);
                break;
        }
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
        m_iCheckFrequency = panel->m_sliderCheckFrequency->GetValue();
        m_Timer.Start(m_iCheckFrequency * 1000, wxTIMER_CONTINUOUS);
        
        switch (panel->m_radioBoxBoundaryType->GetSelection()) {
            case 0:
                m_BoundaryType = ID_BOUNDARY_ANY;
                break;
            case 1:
                m_BoundaryType = ID_BOUNDARY_EXCLUSION;
                break;
            case 2:
                m_BoundaryType = ID_BOUNDARY_INCLUSION;
                break;
            case 3:
                m_BoundaryType = ID_BOUNDARY_NEITHER;
                break;
            default:
                m_BoundaryType = ID_BOUNDARY_ANY;
                break;
        }
        switch (panel->m_radioBoxBoundaryState->GetSelection()) {
            case 0:
                m_BoundaryState = ID_BOUNDARY_STATE_ANY;
                break;
            case 1:
                m_BoundaryState = ID_BOUNDARY_STATE_ACTIVE;
                break;
            case 2:
                m_BoundaryState = ID_BOUNDARY_STATE_INACTIVE;
                break;
            default:
                m_BoundaryState = ID_BOUNDARY_STATE_ANY;
                break;
        }
        wxString l_sName;
        m_BoundaryGUID = panel->m_tBoundaryGUID->GetValue();
        if(m_BoundaryGUID != wxEmptyString) {
            l_sName = GetPathNameByGUID(m_BoundaryGUID);
            if(l_sName != wxEmptyString) {
                m_BoundaryName = l_sName;
            }
        }
        if(g_BoundaryDescription != wxEmptyString) {
            m_BoundaryDescription = g_BoundaryDescription;
            g_BoundaryDescription = wxEmptyString;
        }
        m_GuardZoneGUID = panel->m_tGuardZoneGUID->GetValue();
        if(m_GuardZoneGUID != wxEmptyString) {
            l_sName = GetPathNameByGUID(m_GuardZoneGUID);
            if(l_sName != wxEmptyString)
                m_GuardZoneName = l_sName;
        }
    }

    void LoadConfig(TiXmlElement *e) {
        const char *mode = e->Attribute("Mode");
        if(!strcasecmp(mode, "Time")) m_Mode = TIME;
        else if(!strcasecmp(mode, "Distance")) m_Mode = DISTANCE;
        else if(!strcasecmp(mode, "Anchor")) m_Mode = ANCHOR;
        else if(!strcasecmp(mode, "Guard")) m_Mode = GUARD;
        else wxLogMessage("Watchdog: " + wxString(_("invalid Boundary mode")) + ": "
                         + wxString::FromUTF8(mode));

        e->Attribute("TimeMinutes", &m_TimeMinutes);
        e->Attribute("Distance", &m_Distance);
        e->Attribute("CheckFrequency", &m_iCheckFrequency);
        e->Attribute("BoundaryType", &m_BoundaryType);
        e->Attribute("BoundaryState", &m_BoundaryState);
        m_BoundaryGUID = wxString::FromUTF8(e->Attribute("BoundaryGUID"));
        m_BoundaryDescription = wxString::FromUTF8(e->Attribute("BoundaryDescription"));
        m_BoundaryName = wxString::FromUTF8(e->Attribute("BoundaryName"));
        m_GuardZoneGUID = wxString::FromUTF8(e->Attribute("GuardZoneGUID"));
        m_GuardZoneName = wxString::FromUTF8(e->Attribute("GuardZoneName"));
        if(m_GuardZoneName == wxEmptyString) m_GuardZoneName = m_GuardZoneGUID;
    }

    void SaveConfig(TiXmlElement *c) {
        c->SetAttribute("Type", "Boundary");
        c->SetAttribute(("BoundaryType"), m_BoundaryType);
        c->SetAttribute(("BoundaryState"), m_BoundaryState);
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

        c->SetAttribute("CheckFrequency", m_iCheckFrequency);
        c->SetAttribute("TimeMinutes", m_TimeMinutes);
        c->SetDoubleAttribute("Distance", m_Distance);
        c->SetAttribute("BoundaryGUID", m_BoundaryGUID.mb_str());
        c->SetAttribute("BoundaryName", m_BoundaryName.mb_str());
        c->SetAttribute("BoundaryDescription", m_BoundaryDescription.mb_str());
        c->SetAttribute("GuardZoneGUID", m_GuardZoneGUID.mb_str());
        c->SetAttribute("GuardZoneName", m_GuardZoneName.mb_str());
    }

    wxString MessageBoxText()
    {
        if(m_bMessageBox) {
            switch (m_Mode) {
            case TIME: {
                wxString  l_s = "\n"
                    + _("Name") + ": " + m_BoundaryName + "\n"
                    + _("Description") + ": " + m_BoundaryDescription + "\n"
                    + _("GUID") + ": " + m_BoundaryGUID + "\n";
                if(m_bCurrentBoatPos)
                    l_s.append(wxString(_("inside boundary")));
                else
                    l_s.append(wxString(_("in")) + ": " + TimeBoundaryMsg());
                return l_s;
            }
            case DISTANCE: {
                wxString l_s = "\n"
                    + _("Name") + ": " + m_BoundaryName + "\n"
                    + _("Description") + ": " + m_BoundaryDescription + "\n"
                    + _("GUID") + ": " + m_BoundaryGUID + "\n";
                if(m_bCurrentBoatPos)
                    l_s.append(wxString(_("inside boundary")));
                else {
                    l_s += wxString(_("in")) + ": ";
                    l_s << m_BoundaryDistance;
                    l_s += " nm";
                }
                return l_s;
            }
            case ANCHOR: {
                wxString l_s = "\n" 
                    + _("Outside") + "\n"
                    + _("Name") + ": " + m_BoundaryName + "\n"
                    + _("Description") + ": " + m_BoundaryDescription + "\n"
                    + _("GUID") + ": " + m_BoundaryGUID;
                wxMessageDialog mdlg(GetOCPNCanvasWindow(), l_s, _("Watchdog"), wxOK | wxICON_WARNING);
                return l_s;
            }
            case GUARD: {
                wxString l_s = "\n" 
                    + _("Guard Zone Name") + ": " + m_GuardZoneName + "\n"
                    + _("Description") + ": " + m_GuardZoneDescription + "\n"
                    + _("GUID") + ": " + m_GuardZoneGUID + "\n" 
                    + _("Time") + ": " + wxDateTime::Now().FormatISOCombined(' ') + "\n";
                if(m_bSpecial) {
                    l_s.append("Guard Zone not Found");
                    m_bFired = false;
                    m_bEnabled = false;
                } else {
                    l_s.append(_("Ship Name") + ": " + g_AISTarget.m_sShipName + "\n"
                               + _("Ship MMSI") + ": " + wxString::Format("%i", g_AISTarget.m_iMMSI));
                }
                return l_s;
            }
            }
        } else {
            if(m_bSpecial && m_Mode == GUARD) {
                wxString l_s = "\n" 
                    + _("Guard Zone Name") + ": " + m_GuardZoneName + "\n"
                    + _("Description") + ": " + m_GuardZoneDescription + "\n"
                    + _("GUID") + ": " + m_GuardZoneGUID + "\n" + _("Guard Zone not Found");
                wxMessageDialog mdlg(GetOCPNCanvasWindow(), l_s, _("Watchdog"), wxOK | wxICON_WARNING);
                mdlg.ShowModal();
                m_bFired = false;
                m_bEnabled = false;
            }       
        }
        return "";
    }
    
    void OnAISMessage (int iAlarmIndex) 
    {
        bool l_bFoundMMSI = false;
        AISMMSITIME l_AISMsgInfo;
        
        if(m_bEnabled) {
            std::list<AISMMSITIME>::iterator it = AISMsgInfoList.begin();
            while(it != AISMsgInfoList.end()) {
                wxFileConfig *l_pConf = GetOCPNConfigObject();
                l_pConf->SetPath (  "/Settings/AIS"  );
                
                int l_iLostMins = l_pConf->Read ( "MarkLost_Minutes", 8L );
                
                if((wxDateTime::Now() - it->MsgTime).GetSeconds() > (l_iLostMins * 60)) {
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

    void OnTimer( wxTimerEvent &tEvent )
    {
        switch (m_Mode) {
            case TIME:
            case DISTANCE:
            case ANCHOR:
                Alarm::OnTimer( tEvent );
                break;
            case GUARD:
//                Alarm::OnTimer( tEvent );
                if(g_watchdog_pi->m_WatchdogDialog && g_watchdog_pi->m_WatchdogDialog->IsShown())
                    for(unsigned int i=0; i<Alarm::s_Alarms.size(); i++)
                        if(Alarm::s_Alarms[i] == this)
                            g_watchdog_pi->m_WatchdogDialog->UpdateStatus(i);
                break;
        }
        return;
    }
    
    void OnFlashTimer( wxTimerEvent &tEvent )
    {
        if(m_bFired) {
            switch (m_Mode) {
                case TIME:
                    break;
                case DISTANCE:
                    m_bHighlight = (m_bHighlight ? false : true);
                    RequestRefresh(GetOCPNCanvasWindow());
                    break;
                case ANCHOR:
                    break;
                case GUARD:
                    break;
            }
        }
    }
    
    wxString TimeBoundaryMsg()
    {
        wxString s, fmt(" %d ");
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
        wxString    MsgString;
        if(g_ReceivedODVersionMessage != wxEmptyString) return;
        jMsg["Source"] = "WATCHDOG_PI";
        jMsg["Type"] = "Request";
        jMsg["Msg"] = "Version";
        jMsg["MsgId"] = "version";
        MsgString = writer.write( jMsg );
        SendPluginMessage( "OCPN_DRAW_PI", MsgString );
    }
    
    wxString GetPathNameByGUID(wxString GUID)
    {
        Json::Value jMsg;
        Json::FastWriter writer;
        wxString    MsgString;
        wxString    l_sName = wxEmptyString;

        jMsg["Source"] = "WATCHDOG_PI";
        jMsg["Type"] = "Request";
        jMsg["Msg"] = "FindPathByGUID";
        jMsg["MsgId"] = "general";
        jMsg["GUID"] = (std::string)GUID;
        MsgString = writer.write( jMsg );
        g_ReceivedPathGUIDMessage = wxEmptyString;
        SendPluginMessage( "OCPN_DRAW_PI", MsgString );
        if(g_ReceivedPathGUIDMessage != wxEmptyString &&
           g_ReceivedPathGUIDJSONMsg["MsgId"].asString() == "general" &&
           g_ReceivedPathGUIDJSONMsg["Found"].asBool() == true ) {
            l_sName = g_ReceivedPathGUIDJSONMsg["Name"].asString();
            }
            return l_sName;
    }

private:

    enum Mode { TIME, DISTANCE, ANCHOR, GUARD } m_Mode;
    double      m_TimeMinutes, m_Distance;
    wxTimeSpan  m_BoundaryTime;
    double      m_BoundaryDistance;
    int         m_BoundaryDirection;
    double      m_BoundaryAtLat;
    double      m_BoundaryAtLon;
    int         m_BoundaryType;
    int         m_BoundaryState;
    bool        m_AISMoving;
    bool        m_AISStationary;
    bool        m_bAnchorOutside;
    wxString    m_BoundaryGUID;
    wxString    m_BoundaryName;
    wxString    m_BoundaryDescription;
    wxString    m_GuardZoneName;
    wxString    m_GuardZoneDescription;
    wxString    m_GuardZoneGUID;
    bool        m_bGuardZoneFired;
    bool        m_bCurrentBoatPos;
    bool        m_bHighlight;
    int         m_iCheckFrequency;
    bool        m_bWasEnabled;
    bool        m_bGZFound;
    
    struct AISMMSITIME {
        int MMSI;
        wxDateTime MsgTime;
    };
    
    std::list<AISMMSITIME> AISMsgInfoList;
    
    struct BOUNDARYCROSSING {
        wxString    Name;
        wxString    Description;
        wxString    GUID;
        double      Len;
        double      Lon;
        double      Lat;
    };
    
    std::list<BOUNDARYCROSSING> BoundaryCrossingList;
    
    wxTimer    m_baTimer;
    
};


class pypilotAlarm : virtual public Alarm, virtual public SignalKClient
{
public:
    pypilotAlarm() : SignalKClient(false, false),
                     m_bNoConnection(true),
                     m_bOverTemperature(true), m_bOverCurrent(false),
                     m_bNoIMU(true), m_bNoMotorController(true),
                     m_bNoRudderFeedback(false), m_bNoMotorTemperature(false),
                     m_bDriverTimeout(true),
                     m_bEndOfTravel(false), m_bLostMode(false),
                     m_bServoSaturated(false),
                     m_bPowerConsumption(false), m_dPowerConsumption(10),
                     m_bCourseError(false), m_dCourseError(20),
                     m_host("192.168.14.1")
        {
            // give 10 seconds to at startup before connection failure
            m_startTime = wxDateTime::UNow() + wxTimeSpan::Seconds(10);
            m_lastMessageTime = wxDateTime::UNow();
        }

    wxString Type() { return _("pypilot"); }

    wxString GetStatus() {
        return m_status;
    }

    bool Test() {
        wxString status;
        double d;
        // in order of importance
        wxDateTime unow = wxDateTime::UNow();
        if(m_bNoConnection && unow > m_startTime && (!connected() || (unow - m_lastMessageTime).GetMilliseconds() > 3000))
            status = "no connection";
        if(m_bNoIMU && lastvalue("imu.loopfreq") == "0")
            status = "no imu";
        else if(m_bNoMotorController && lastvalue("servo.controller") == "none")
            status = "no controller";
        else if(m_bEndOfTravel && (lastvalue("servo.flags").Contains("FAULT") ||
                                   lastvalue("servo.flags").Contains("RUDDER")))
            status = "end of travel";
        else if(m_bOverTemperature && lastvalue("servo.flags").Contains("OVERTEMP"))
            status = "over temperature";
        else if(m_bOverCurrent && lastvalue("servo.flags").Contains("OVERCURRENT"))
            status = "over current";
        else if(m_bNoRudderFeedback && lastvalue("servo.rudder") == "False")
            status = "no rudder feedback";
        else if(m_bNoMotorTemperature && lastvalue("servo.motor_temp") == "False")
            status = "no motor temperature";
        else if(m_bDriverTimeout && lastvalue("servo.flags").Contains("DRIVER_TIMEOUT"))
            status = "driver timeout (No Motor)";
        else if(m_bLostMode && lastvalue("ap.lostmode") == "True")
            status = "lost mode";
        else if(m_bServoSaturated && lastvalue("servo.flags").Contains("SATURATED"))
            status = "servo saturated";
        else if(m_bPowerConsumption && lastvalue("servo.watts").ToDouble(&d) && d > m_dPowerConsumption)
            status = "power consumption " + wxString::Format("%.2f > %.2f", d, m_dPowerConsumption);
        else if(m_bCourseError && lastvalue("ap.heading_error").ToDouble(&d) && d > m_dCourseError)
            status = "course error " + wxString::Format("%.2f > %.2f", d, m_dCourseError);
        if(m_status != status)
            m_DelayTime = wxDateTime(); // invalid

        if(!status)
            return false;
        
        m_status = status;
        return true;
    }

    void OnTimer(wxTimerEvent &tEvent) {
        Alarm::OnTimer( tEvent );
        if(!connected()) {
            connect(m_host);
            return;
        }

        std::string name;
        Json::Value data;
        while(receive(name, data)) {
            Json::Value value = data.get("value", "");
            if(value.isConvertibleTo(Json::stringValue))
               m_lastvalues[name] = value.asString();
            m_lastMessageTime = wxDateTime::UNow();
        }
    }

    void OnConnected() {
        UpdateWatchlist();
    }

    void OnDisconnected() {
        m_lastvalues.clear();
    }

    void UpdateWatchlist() {
        if(!connected())
            return;

        std::map<std::string, bool> watchlist;
        if(m_bNoConnection)              watchlist["imu.loopfreq"]     = true;
        if(m_bOverTemperature || m_bOverCurrent || m_bDriverTimeout
           || m_bEndOfTravel || m_bServoSaturated)
            watchlist["servo.flags"] = true;
        if(m_bNoIMU)              watchlist["imu.loopfreq"]     = true;
        if(m_bNoMotorController)       watchlist["servo.controller"] = true;
        if(m_bNoRudderFeedback)   watchlist["servo.rudder"]     = true;
        if(m_bNoMotorTemperature) watchlist["servo.motor_temp"] = true;
        if(m_bLostMode)           watchlist["ap.lost_mode"]     = true;
        if(m_bPowerConsumption)   watchlist["servo.watts"]      = true;
        if(m_bCourseError)        watchlist["ap.heading_error"] = true;

        update_watchlist(watchlist);
    }

    wxString lastvalue(std::string name)
    {
        if(m_lastvalues.find(name) == m_lastvalues.end())
            return "";
        return m_lastvalues[name];
    }

    wxWindow *OpenPanel(wxWindow *parent) {
        pypilotPanel *panel = new pypilotPanel(parent);
        panel->m_cbNoConnection->SetValue(m_bNoConnection);
        panel->m_cbOverTemperature->SetValue(m_bOverTemperature);
        panel->m_cbOverCurrent->SetValue(m_bOverCurrent);
        panel->m_cbNoIMU->SetValue(m_bNoIMU);
        panel->m_cbNoMotorController->SetValue(m_bNoMotorController);
        panel->m_cbNoRudderFeedback->SetValue(m_bNoRudderFeedback);
        panel->m_cbNoMotorTemperature->SetValue(m_bNoMotorTemperature);
        panel->m_cbDriverTimeout->SetValue(m_bDriverTimeout);
        panel->m_cbEndOfTravel->SetValue(m_bEndOfTravel);
        panel->m_cbLostMode->SetValue(m_bLostMode);
        panel->m_cbServoSaturated->SetValue(m_bServoSaturated);
        panel->m_cbPowerConsumption->SetValue(m_bPowerConsumption);
        panel->m_sPowerConsumption->SetValue(m_dPowerConsumption);
        panel->m_cbCourseError->SetValue(m_bCourseError);
        panel->m_sCourseError->SetValue(m_dCourseError);
        panel->m_cHost->SetValue(m_host);
        return panel;
    }

    void SavePanel(wxWindow *p) {
        pypilotPanel *panel = (pypilotPanel*)p;
        m_bNoConnection = panel->m_cbNoConnection->GetValue();
        m_bOverTemperature = panel->m_cbOverTemperature->GetValue();
        m_bOverCurrent = panel->m_cbOverCurrent->GetValue();
        m_bNoIMU = panel->m_cbNoIMU->GetValue();
        m_bNoMotorController = panel->m_cbNoMotorController->GetValue();
        m_bNoRudderFeedback = panel->m_cbNoRudderFeedback->GetValue();
        m_bNoMotorTemperature = panel->m_cbNoMotorTemperature->GetValue();
        m_bDriverTimeout = panel->m_cbDriverTimeout->GetValue();
        m_bEndOfTravel = panel->m_cbEndOfTravel->GetValue();
        m_bLostMode = panel->m_cbLostMode->GetValue();
        m_bServoSaturated = panel->m_cbServoSaturated->GetValue();
        m_bPowerConsumption = panel->m_cbPowerConsumption->GetValue();
        m_dPowerConsumption = panel->m_sPowerConsumption->GetValue();
        m_bCourseError = panel->m_cbCourseError->GetValue();
        m_dCourseError = panel->m_sCourseError->GetValue();
        m_host = panel->m_cHost->GetValue().BeforeFirst(' ');

        UpdateWatchlist();
    }

    void LoadConfig(TiXmlElement *e) {
        e->QueryBoolAttribute("NoConnection", &m_bNoConnection);
        e->QueryBoolAttribute("OverTemperature", &m_bOverTemperature);
        e->QueryBoolAttribute("OverCurrent", &m_bOverCurrent);
        e->QueryBoolAttribute("NoIMU", &m_bNoIMU);
        e->QueryBoolAttribute("NoMotorController", &m_bNoMotorController);
        e->QueryBoolAttribute("NoRudderFeedback", &m_bNoRudderFeedback);
        e->QueryBoolAttribute("NoMotorTemperature", &m_bNoMotorTemperature);
        e->QueryBoolAttribute("DriverTimeout", &m_bDriverTimeout);
        e->QueryBoolAttribute("EndOfTravel", &m_bEndOfTravel);
        e->QueryBoolAttribute("LostMode", &m_bLostMode);
        e->QueryBoolAttribute("ServoSaturated", &m_bServoSaturated);
        e->QueryBoolAttribute("PowerConsumption", &m_bPowerConsumption);
        e->Attribute("PowerConsumptionWatts", &m_dPowerConsumption);
        e->QueryBoolAttribute("CourseError", &m_bCourseError);
        e->Attribute("CourseErrorDegrees", &m_dCourseError);
        m_host = e->Attribute("Host");

        UpdateWatchlist();
    }

    void SaveConfig(TiXmlElement *c) {
        c->SetAttribute("Type", "pypilot");
        c->SetAttribute("NoConnection", m_bNoConnection);
        c->SetAttribute("OverTemperature", m_bOverTemperature);
        c->SetAttribute("OverCurrent", m_bOverCurrent);
        c->SetAttribute("NoIMU", m_bNoIMU);
        c->SetAttribute("NoMotorController", m_bNoMotorController);
        c->SetAttribute("NoRudderFeedback", m_bNoRudderFeedback);
        c->SetAttribute("NoMotorTemperature", m_bNoMotorTemperature);
        c->SetAttribute("DriverTimeout", m_bDriverTimeout);
        c->SetAttribute("EndOfTravel", m_bEndOfTravel);
        c->SetAttribute("LostMode", m_bLostMode);
        c->SetAttribute("ServoSaturated", m_bServoSaturated);
        c->SetAttribute("PowerConsumption", m_bPowerConsumption);
        c->SetDoubleAttribute("PowerConsumptionWatts", m_dPowerConsumption);
        c->SetAttribute("CourseError", m_bCourseError);
        c->SetDoubleAttribute("CourseErrorDegrees", m_dCourseError);
        c->SetAttribute("Host", m_host);
    }
    
private:
    std::map<std::string, std::string> m_lastvalues;
    wxString m_status;

    bool m_bNoConnection, m_bOverTemperature, m_bOverCurrent;
    bool m_bNoIMU, m_bNoMotorController, m_bNoRudderFeedback,
        m_bNoMotorTemperature, m_bDriverTimeout;
    bool m_bEndOfTravel, m_bLostMode, m_bServoSaturated;
    bool m_bPowerConsumption;
    double m_dPowerConsumption;
    bool m_bCourseError;
    double m_dCourseError;
    wxString m_host;

    wxDateTime m_startTime, m_lastMessageTime;
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
    wxString configuration = watchdog_pi::StandardPath() + "WatchdogConfiguration.xml";
    TiXmlDocument doc;

    if(!doc.LoadFile(configuration.mb_str())) {
        wxLogMessage("Watchdog: " + wxString(_("Failed to read")) + ": " + configuration);
        return;
    }

    TiXmlHandle root(doc.RootElement());
    for(TiXmlElement* e = root.FirstChild().Element(); e; e = e->NextSiblingElement()) {
        if(!strcasecmp(e->Value(), "Alarm")) {
            const char *type = e->Attribute("Type");
            if(!type)
                continue;
            Alarm *alarm;
            if(!strcasecmp(type, "Anchor")) alarm = Alarm::NewAlarm(ANCHOR);
            else if(!strcasecmp(type, "Course")) alarm = Alarm::NewAlarm(COURSE);
            else if(!strcasecmp(type, "Speed")) alarm = Alarm::NewAlarm(SPEED);
            else if(!strcasecmp(type, "Wind")) alarm = Alarm::NewAlarm(WIND);
            else if(!strcasecmp(type, "Weather")) alarm = Alarm::NewAlarm(WEATHER);
            else if(!strcasecmp(type, "Deadman")) alarm = Alarm::NewAlarm(DEADMAN);
            else if(!strcasecmp(type, "NMEAData")) alarm = Alarm::NewAlarm(NMEADATA);
            else if(!strcasecmp(type, "LandFall")) alarm = Alarm::NewAlarm(LANDFALL);
            else if(!strcasecmp(type, "Boundary")) alarm = Alarm::NewAlarm(BOUNDARY);
            else if(!strcasecmp(type, "pypilot")) alarm = Alarm::NewAlarm(PYPILOT);
            else {
                wxLogMessage("Watchdog: " + wxString(_("invalid alarm type")) + ": " + wxString::FromUTF8(type));
                continue;
            }

            alarm->LoadConfigBase(e);
            alarm->LoadConfig(e);
            Alarm::s_Alarms.push_back(alarm);
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

    wxString configuration = watchdog_pi::StandardPath() + "WatchdogConfiguration.xml";
    if(!doc.SaveFile(configuration.mb_str()))
        wxLogMessage("Watchdog: " + wxString(_("failed to save")) + ": " + configuration);
}

void Alarm::DeleteAll()
{
    for(unsigned int i=0; i<s_Alarms.size(); i++)
        delete s_Alarms[i];
    s_Alarms.clear();
}

void Alarm::ResetAll()
{
    for(unsigned int i=0; i<s_Alarms.size(); i++)
        s_Alarms[i]->Reset();
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
    case ANCHOR:   alarm = new AnchorAlarm;   break;
    case COURSE:   alarm = new CourseAlarm;   break;
    case SPEED:    alarm = new SpeedAlarm;    break;
    case WIND:     alarm = new WindAlarm;     break;
    case WEATHER:  alarm = new WeatherAlarm; break;
    case DEADMAN:  alarm = new DeadmanAlarm;  break;
    case NMEADATA: alarm = new NMEADataAlarm; break;
    case LANDFALL: alarm = new LandFallAlarm; break;
    case BOUNDARY: alarm = new BoundaryAlarm; break;
    case PYPILOT: alarm = new pypilotAlarm; break;
    default:  wxLogMessage("Invalid Alarm Type"); return NULL;
    }

    return alarm;
}

Alarm::Alarm(bool gfx, int interval)
    : m_bHasGraphics(gfx), m_bEnabled(false), m_bgfxEnabled(false), m_bFired(false), m_bSpecial(false),
      m_bSound(true), m_bCommand(false), m_bMessageBox(false), m_bNoData(true), m_bRepeat(false), m_bAutoReset(true),
      m_sSound(*GetpSharedDataLocation() + "sounds/2bells.wav"),
      m_LastAlarmTime(wxDateTime::Now()),
      m_iRepeatSeconds(60), m_iDelay(0),
      m_interval(interval), m_count(0)
{
    m_Timer.Connect(wxEVT_TIMER, wxTimerEventHandler( Alarm::OnTimer ), NULL, this);
    m_Timer.Start(m_interval * 1000, wxTIMER_CONTINUOUS);
}

wxString Alarm::Action()
{
    wxString s;
    if(m_bSound)      s += _("Sound")      + wxString(" ");
    if(m_bCommand)    s += _("Command")    + wxString(" ");
    if(m_bMessageBox) s += _("MessageBox") + wxString(" ");
    if(m_bRepeat)     s += _("Repeat")     + wxString(" ");
    if(m_bAutoReset)  s += _("Auto Reset") + wxString(" ");
    return s;
}

void Alarm::Run()
{
    if(m_bSound)
        PlugInPlaySound(m_sSound);

    if(m_bCommand)
        if(!wxProcess::Open(m_sCommand)) {
            wxMessageDialog mdlg(GetOCPNCanvasWindow(),
                                 Type() + " " +
                                 _("Failed to execute command: ") + m_sCommand,
                                 _("Watchdog"), wxOK | wxICON_ERROR);
            mdlg.ShowModal();
            m_bCommand = false;
        }

    if(m_bMessageBox) {
        wxMessageDialog mdlg(GetOCPNCanvasWindow(), Type() + " " + _("ALARM!")
                             + MessageBoxText(),
                             _("Watchdog"), wxOK | wxICON_WARNING);
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
    e->QueryBoolAttribute("NoData", &m_bNoData);
    e->QueryBoolAttribute("Repeat", &m_bRepeat);
    e->Attribute("RepeatSeconds", &m_iRepeatSeconds);
    e->Attribute("Delay", &m_iDelay);
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
    c->SetAttribute("NoData", m_bNoData);
    c->SetAttribute("Repeat", m_bRepeat);
    c->SetAttribute("RepeatSeconds", m_iRepeatSeconds);
    c->SetAttribute("Delay", m_iDelay);
    c->SetAttribute("AutoReset", m_bAutoReset);
}

void Alarm::OnTimer( wxTimerEvent & )
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath (  "/Settings/Watchdog"  );

    int enabled = pConf->Read ( "Enabled", 1L );

    if(enabled == 2 && !g_watchdog_pi->m_bWatchdogDialogShown)
        enabled = 0;

    if(enabled == 3 && (!g_watchdog_pi->m_WatchdogDialog || !g_watchdog_pi->m_WatchdogDialog->IsShown()))
       enabled = 0;

    if(enabled && m_bEnabled) {
        if(Test()) {
            wxDateTime now = wxDateTime::Now();
            if(!m_DelayTime.IsValid())
                m_DelayTime = now;
            if((now - m_DelayTime).GetSeconds() >= m_iDelay) {
                if(m_bFired) {
                    if((now - m_LastAlarmTime).GetSeconds() > m_iRepeatSeconds && m_bRepeat) {
                        Run();
                        m_LastAlarmTime = now;
                    }
                } else {
                    m_bFired = true;
                    m_count++;
                    Run();
                    m_LastAlarmTime = now;
                }
            }
        } else {
            if(m_bAutoReset && m_bFired) {
                m_bFired = false;
                RequestRefresh(GetOCPNCanvasWindow());
            }
            m_DelayTime = wxDateTime(); // invalid
        }
    }

    if(g_watchdog_pi->m_WatchdogDialog && g_watchdog_pi->m_WatchdogDialog->IsShown())
        for(unsigned int i=0; i<Alarm::s_Alarms.size(); i++)
            if(Alarm::s_Alarms[i] == this)
                g_watchdog_pi->m_WatchdogDialog->UpdateStatus(i);
}
