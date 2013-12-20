/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  watch dog Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2013 by Sean D'Epagnier                                 *
 *   sean at depagnier dot com                                             *
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

#ifndef _WATCHMANPI_H_
#define _WATCHMANPI_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <wx/fileconf.h>

#define     PLUGIN_VERSION_MAJOR    0
#define     PLUGIN_VERSION_MINOR    8

#define     MY_API_VERSION_MAJOR    1
#define     MY_API_VERSION_MINOR    8

#include "../../../include/ocpn_plugin.h"

#ifdef __MSVC__
#include <float.h>
#include <iostream>
#include <limits>

# if !defined(M_PI)
# define M_PI		3.14159265358979323846	/* pi */
# endif

# if !defined(NAN)
# define NAN std::numeric_limits<double>::quiet_NaN ()
# endif

# if !defined(INFINITY)
# define INFINITY std::numeric_limits<double>::infinity ()
# endif

#define isnan _isnan
#define isinf(x) (!_finite(x) && !_isnan(x))

inline double trunc(double d){ return (d>0) ? floor(d) : ceil(d) ; }
inline double round(double n) { return n < 0.0 ? ceil(n - 0.5) : floor(n + 0.5); }

# if !defined(snprintf)
# define snprintf _snprintf
# endif
#define vsnprintf _vsnprintf
#define strcasecmp _stricmp
#define strncasecmp _strnicmp

#define strtok_r strtok_s

#endif

double heading_resolve(double degrees);

//----------------------------------
//    The PlugIn Class Definition
//----------------------------------

#define WATCHMAN_TOOL_POSITION    -1          // Request default positioning of toolbar tool

class ocpnDC;
class WatchdogDialog;
class WatchdogPrefsDialog;

class watchdog_pi : public wxEvtHandler, public opencpn_plugin_18
{
public:

    Alarm Alarms[ALARMCOUNT];

    watchdog_pi(void *ppimgr);

//    The required PlugIn Methods
      int Init(void);
      bool DeInit(void);

      int GetAPIVersionMajor();
      int GetAPIVersionMinor();
      int GetPlugInVersionMajor();
      int GetPlugInVersionMinor();
      wxBitmap *GetPlugInBitmap();
      wxString GetCommonName();
      wxString GetShortDescription();
      wxString GetLongDescription();

      int GetToolbarToolCount(void);

      void OnToolbarToolCallback(int id);
      void OnContextMenuItemCallback(int id);

      wxColour Color(enum Alarm alarm_mask);

      bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp);
      bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);
      void Render(ocpnDC &dc, PlugIn_ViewPort &vp);

      void OnTimer( wxTimerEvent & );

//    Optional plugin overrides
      void SetColorScheme(PI_ColorScheme cs);

//    Other public methods
      void SetWatchdogDialogX    (int x){ m_watchdog_dialog_x = x;}
      void SetWatchdogDialogY    (int x){ m_watchdog_dialog_y = x;}

      void OnWatchdogDialogClose();
      void ShowPreferencesDialog( wxWindow* parent );

      void Alarm(enum AlarmName alarm);

      wxWindow         *m_parent_window;

      wxDateTime m_DeaddogUpdateTime;

      wxFileConfig     *m_pconfig;
      PlugIn_Position_Fix_Ex m_lastfix, m_lasttimerfix;

      int m_iAlarm, m_iLastAlarm;

      double sog, cog;

private:
      void ResetDeaddog();

      bool    LoadConfig(void);
      bool    SaveConfig(void);

      void    Alarm();
      void    SetCursorLatLon(double lat, double lon);
      void    SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix);

      WatchdogDialog   *m_pWatchdogDialog;
      WatchdogPrefsDialog *m_pWatchdogPrefsDialog;
      int               m_watchdog_dialog_x, m_watchdog_dialog_y;
      int               m_display_width, m_display_height;

      int               m_leftclick_tool_id;
      int               m_anchor_menu_id;

      void              RearrangeWindow();

      double m_cursor_lat, m_cursor_lon;

      wxDateTime m_LastLandFallCheck;
      wxDateTime m_LastAlarmTime;

      wxDateTime m_LastPositionFix;

      wxDateTime m_LastTimerFix;

      wxTimer m_Timer;
};

#endif
