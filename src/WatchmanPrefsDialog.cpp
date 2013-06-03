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
#include "WatchmanPrefsDialog.h"

WatchmanPrefsDialog::WatchmanPrefsDialog( watchman_pi &_watchman_pi, wxWindow* parent)
    : WatchmanPrefsDialogBase( parent ), m_watchman_pi(_watchman_pi)
{
}

void WatchmanPrefsDialog::OnSyncToBoat( wxCommandEvent& event )
{
    m_tAnchorLatitude->SetValue(wxString::Format(_T("%f"), m_watchman_pi.m_lastfix.Lat));
    m_tAnchorLongitude->SetValue(wxString::Format(_T("%f"), m_watchman_pi.m_lastfix.Lon));
}

void WatchmanPrefsDialog::OnCurrentCourse( wxCommandEvent& event )
{
    m_sCourseDegrees->SetValue(m_watchman_pi.m_dCurrentCourse);
}
