/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  watch man Plugin
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

#ifndef _WATCHMANPI_H_
#define _WATCHMANPI_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <wx/fileconf.h>

#define     PLUGIN_VERSION_MAJOR    0
#define     PLUGIN_VERSION_MINOR    4

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

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define WATCHMAN_TOOL_POSITION    -1          // Request default positioning of toolbar tool

class ocpnDC;
class WatchmanDialog;

class watchman_pi : public wxEvtHandler, public opencpn_plugin_18
{
public:
      watchman_pi(void *ppimgr);

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

      bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp);
      bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);
      void Render(ocpnDC &dc, PlugIn_ViewPort &vp);

      void OnTimer( wxTimerEvent & );

//    Optional plugin overrides
      void SetColorScheme(PI_ColorScheme cs);

//    Other public methods
      void SetWatchmanDialogX    (int x){ m_watchman_dialog_x = x;}
      void SetWatchmanDialogY    (int x){ m_watchman_dialog_y = x;}

      void OnWatchmanDialogClose();
      void ShowPreferencesDialog( wxWindow* parent );

      void    RunAlarm(wxString sound, wxString command, bool mb);

      wxWindow         *m_parent_window;

      double m_dLandFallDistance;

      bool m_bLandFall;
      wxDateTime LastLandFallCheck;

      bool m_bDeadman;
      wxTimeSpan m_DeadmanSpan;

      bool m_bAnchor;
      double m_dAnchorLatitude, m_dAnchorLongitude;
      double m_iAnchorRadius;

      bool m_bGPSAlarm;
      double m_dGPSSeconds;
      bool m_bAISAlarm;
      double m_dAISSeconds;

      bool m_bOffCourseAlarm;
      double m_dOffCourseDegrees, m_dCourseDegrees;

      bool m_bSound;
      wxString m_sSound;
      bool m_bCommand;
      wxString m_sCommand;
      bool m_bMessageBox;

      wxDateTime m_DeadmanUpdateTime;

      wxFileConfig     *m_pconfig;
      PlugIn_Position_Fix_Ex m_lastfix;

      bool     m_bLandFallAlarmed, m_bAnchorAlarmed, m_bDeadmanAlarmed;
      bool     m_bGPSAlarmed, m_bAISAlarmed, m_bOffCourseAlarmed;

      bool m_dCurrentCourse;

private:
      void ResetDeadman();

      bool    LoadConfig(void);
      bool    SaveConfig(void);

      void    Alarm();
      void    SetCursorLatLon(double lat, double lon);
      void    SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix);
      void    SetAISSentence(wxString &);

      WatchmanDialog   *m_pWatchmanDialog;
      int               m_watchman_dialog_x, m_watchman_dialog_y;
      int               m_display_width, m_display_height;

      int               m_leftclick_tool_id;
      int               m_anchor_menu_id;

      void              RearrangeWindow();

      double m_cursor_lat, m_cursor_lon;

      wxDateTime m_LastLandFallCheck;
      wxDateTime m_LastAlarmTime;

      wxDateTime m_LastPositionFix, m_LastAISSentence;

      wxTimer m_Timer;
};

#endif
