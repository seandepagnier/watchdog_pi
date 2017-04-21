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

#ifndef _AISTARGETINFO_H_
#define _AISTARGETINFO_H_

//----------------------------------
//    The AIS Info Class Definition
//----------------------------------

class AIS_Target_Info
{
public:
    double      m_dLat;
    double      m_dLon;
    double      m_dSOG;
    double      m_dCOG;
    double      m_dHDG;
    int         m_iMMSI;
    int         m_iClass;
    bool        m_bOwnship;
    bool        m_bActive;
    bool        m_bLost;
    wxString    m_sShipName;
    wxString    m_sCallSign;
};

#endif
