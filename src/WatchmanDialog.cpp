/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  watchman Plugin
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

#include "watchman_pi.h"
#include "WatchmanDialog.h"

WatchmanDialog::WatchmanDialog( watchman_pi &_watchman_pi, wxWindow* parent)
    : WatchmanDialogBase( parent ), m_watchman_pi(_watchman_pi)
{
}

WatchmanDialog::~WatchmanDialog()
{
}

extern "C" void ll_gc_ll(double lat, double lon, double crs, double dist, double *dlat, double *dlon);
extern bool gshhsCrossesLand(double lat1, double lon1, double lat2, double lon2);
void WatchmanDialog::UpdateLandFallTime(PlugIn_Position_Fix_Ex &pfix)
{
    double lat1 = pfix.Lat, lon1 = pfix.Lon, lat2, lon2;
    double dist = 0, dist1 = 1000;
    wxTimeSpan time;
    int count = 0;
    while(count < 10) {
        ll_gc_ll(pfix.Lat, pfix.Lon, pfix.Cog, dist + dist1, &lat2, &lon2);
        if(gshhsCrossesLand(lat1, lon1, lat2, lon2)) {
            if(dist < 1) {
                time = wxTimeSpan::Seconds(3600.0 * dist / pfix.Sog);
                wxString s;
                s += wxString::Format(_("%d Days"), time.GetDays());
                time -= wxTimeSpan::Days(time.GetDays());
                s += wxString::Format(_(" %d Hours"), time.GetHours());
                time -= wxTimeSpan::Hours(time.GetHours());
                s += wxString::Format(_(" %d Minutes"), time.GetMinutes());
                time -= wxTimeSpan::Minutes(time.GetMinutes());
                s += wxString::Format(_(" %d Seconds"), time.GetSeconds());
                m_stLandFallTime->SetLabel(s);
                return;
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
    m_stLandFallTime->SetLabel(_("LandFall not Detected"));
}
