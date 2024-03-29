/***************************************************************************
*
* Project:  OpenCPN
* Purpose:  Watchdog Properties Dialog support
* Author:   Jon Gough
*
***************************************************************************
*   Copyright (C) 2010 by David S. Register                               *
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
**************************************************************************/

#ifndef __WatchdogPropertiesDialog__
#define __WatchdogPropertiesDialog__

#include "WatchdogUI.h"
#include <wx/bmpcbox.h>

#ifdef __WXMSW__
#include "GL/gl.h"            // local copy for Windows
#include <GL/glu.h>
#else
#endif


#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

class WatchdogPropertiesDialog : public WatchdogPropertiesDialogBase
{
public:
    WatchdogPropertiesDialog( wxWindow* parent );

protected:
    void OnWatchdogPropertiesOKClick( wxCommandEvent& event );
    void OnAboutAuthor( wxCommandEvent& event );
};

#endif // __WatchdogPropertiesDialog__
