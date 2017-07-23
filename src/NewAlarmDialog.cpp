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

#include "watchdog_pi.h"
#include "NewAlarmDialog.h"

NewAlarmDialog::NewAlarmDialog(wxWindow* parent)
    : NewAlarmDialogBase(parent)
{
    m_lAlarmType->InsertColumn(0, _T(""));
    m_lAlarmType->InsertItem(LANDFALL, _("Landfall"));
    m_lAlarmType->InsertItem(BOUNDARY, _("Boundary Alarm"));
    m_lAlarmType->InsertItem(NMEADATA, _("NMEA Data"));
    m_lAlarmType->InsertItem(DEADMAN,  _("Deadman"));
    m_lAlarmType->InsertItem(ANCHOR,   _("Anchor"));
    m_lAlarmType->InsertItem(COURSE,   _("Course"));
    m_lAlarmType->InsertItem(SPEED,    _("Speed"));
    m_lAlarmType->InsertItem(WIND,     _("Wind"));
    m_lAlarmType->InsertItem(WEATHER,  _("Weather"));
    m_lAlarmType->SetColumnWidth(0, wxLIST_AUTOSIZE);
}
