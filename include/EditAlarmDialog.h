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

#ifndef _EDITALARMDIALOG_H_
#define _EDITALARMDIALOG_H_

#include "WatchdogUI.h"

#ifdef __WXMSW__
#include "GL/gl.h"            // local copy for Windows
#include <GL/glu.h>
#else

#ifndef __OCPN__ANDROID__
//#include <GL/gl.h>
//#include <GL/glu.h>
#else
//#include "qopengl.h"                  // this gives us the qt runtime gles2.h
//#include "GL/gl_private.h"
#endif
#endif

class Alarm;

void GetODVersion( void );
bool ODVersionNewerThan(int major, int minor, int patch);

class EditAlarmDialog : public EditAlarmDialogBase
{
public:
    EditAlarmDialog(wxWindow* parent, Alarm *alarm);

    void Save();
    void OnTestAlarm( wxCommandEvent& event );
    void OnInformation( wxCommandEvent& event );

private:
    Alarm *m_alarm;
};

#endif //_EDITALARMDIALOG_H_
