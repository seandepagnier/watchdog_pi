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


#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "wx/process.h"

#include "watchman_pi.h"
#include "WatchmanDialog.h"
#include "icons.h"

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new watchman_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

//---------------------------------------------------------------------------------------------------------
//
//    Watchman PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

watchman_pi::watchman_pi(void *ppimgr)
    : opencpn_plugin_18(ppimgr)
{
    // Create the PlugIn icons
    initialize_images();
}

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

int watchman_pi::Init(void)
{
      AddLocaleCatalog( _T("opencpn-watchman_pi") );

      // Set some default private member parameters
      m_watchman_dialog_x = 0;
      m_watchman_dialog_y = 0;

      ::wxDisplaySize(&m_display_width, &m_display_height);

      //    Get a pointer to the opencpn display canvas, to use as a parent for the POI Manager dialog
      m_parent_window = GetOCPNCanvasWindow();

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();

      //    And load the configuration items
      LoadConfig();

      m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_watchman, _img_watchman, wxITEM_NORMAL,
            _("Watchman"), _T(""), NULL,
             WATCHMAN_TOOL_POSITION, 0, this);

      m_pWatchmanDialog = NULL;

      m_DeadmanTimer.Connect(wxEVT_TIMER, wxTimerEventHandler
                             ( watchman_pi::OnDeadmanTimer ), NULL, this);
      m_DeadmanUpdateTime = wxDateTime::Now();

      return (WANTS_TOOLBAR_CALLBACK    |
              WANTS_CURSOR_LATLON       |
              INSTALLS_TOOLBAR_TOOL     |
              WANTS_PREFERENCES         |
              WANTS_NMEA_EVENTS         |
              WANTS_CONFIG
           );
}

bool watchman_pi::DeInit(void)
{
      //    Record the dialog position
      if (NULL != m_pWatchmanDialog)
      {
            wxPoint p = m_pWatchmanDialog->GetPosition();
            SetWatchmanDialogX(p.x);
            SetWatchmanDialogY(p.y);

            m_pWatchmanDialog->Close();
            delete m_pWatchmanDialog;
            m_pWatchmanDialog = NULL;
      }
      SaveConfig();

      RemovePlugInTool(m_leftclick_tool_id);

      return true;
}

int watchman_pi::GetAPIVersionMajor()
{
      return MY_API_VERSION_MAJOR;
}

int watchman_pi::GetAPIVersionMinor()
{
      return MY_API_VERSION_MINOR;
}

int watchman_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int watchman_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

wxBitmap *watchman_pi::GetPlugInBitmap()
{
      return _img_watchman;
}

wxString watchman_pi::GetCommonName()
{
      return _("Watchman");
}


wxString watchman_pi::GetShortDescription()
{
      return _("Watchman PlugIn for OpenCPN");
}

wxString watchman_pi::GetLongDescription()
{
      return _("Watchman Fax PlugIn for OpenCPN\n\
Alarm user of possible dangerous situations. \n\
Currently only supports coastline detection. \n\
\n\
The Weather Fax plugin was written by Sean D'Epagnier\n\
");
}

int watchman_pi::GetToolbarToolCount(void)
{
      return 1;
}

void watchman_pi::SetColorScheme(PI_ColorScheme cs)
{
      if (NULL == m_pWatchmanDialog)
            return;
      DimeWindow(m_pWatchmanDialog);
}

void watchman_pi::RearrangeWindow()
{
      if (NULL == m_pWatchmanDialog)
            return;

      SetColorScheme(PI_ColorScheme());

      m_pWatchmanDialog->Fit();
}

void watchman_pi::OnToolbarToolCallback(int id)
{
      if(!m_pWatchmanDialog)
      {
            m_pWatchmanDialog = new WatchmanDialog(*this, m_parent_window);
            m_pWatchmanDialog->Move(wxPoint(m_watchman_dialog_x, m_watchman_dialog_y));
      }

      RearrangeWindow();
      m_pWatchmanDialog->Show(!m_pWatchmanDialog->IsShown());

      wxPoint p = m_pWatchmanDialog->GetPosition();
      m_pWatchmanDialog->Move(0,0);        // workaround for gtk autocentre dialog behavior
      m_pWatchmanDialog->Move(p);
}

void watchman_pi::OnDeadmanTimer( wxTimerEvent & )
{
    if(m_bDeadman)
        Alarm();
}

bool watchman_pi::LoadConfig(void)
{
      wxFileConfig *pConf = m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T( "/Settings/Watchman" ) );

            m_watchman_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 20L );
            m_watchman_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 20L );

            pConf->Read ( _T ( "LandFall" ), &m_bLandFall, 0 );
            pConf->Read ( _T ( "LandFallDistance" ), &m_dLandFallDistance, 3 );

            pConf->Read ( _T ( "Deadman" ), &m_bDeadman, 0 );
            int deadmanminutes;
            pConf->Read ( _T ( "DeadmanMinutes" ), &deadmanminutes, 20 );
            m_DeadmanSpan = wxTimeSpan::Minutes(deadmanminutes);

            pConf->Read ( _T ( "MessageBox" ), &m_bMessageBox, 0);

            pConf->Read ( _T ( "SoundEnabled" ), &m_bSound, 0 );
            pConf->Read ( _T ( "SoundFilepath" ), &m_sSound, _T("") );
            pConf->Read ( _T ( "CommandEnabled" ), &m_bCommand, 0 );
            pConf->Read ( _T ( "CommandFilepath" ), &m_sCommand, _T("") );
            return true;
      } else
            return false;
}

bool watchman_pi::SaveConfig(void)
{
      wxFileConfig *pConf = m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T ( "/Settings/Watchman" ) );

            pConf->Write ( _T ( "DialogPosX" ),   m_watchman_dialog_x );
            pConf->Write ( _T ( "DialogPosY" ),   m_watchman_dialog_y );

            pConf->Write ( _T ( "LandFall" ), m_bLandFall );
            pConf->Write ( _T ( "LandFallDistance" ), m_dLandFallDistance);

            pConf->Write ( _T ( "Deadman" ), m_bDeadman );
            pConf->Write ( _T ( "DeadmanMinutes" ), m_DeadmanSpan.GetMinutes() );

            pConf->Write ( _T ( "MessageBox" ), m_bMessageBox);

            pConf->Write ( _T ( "SoundEnabled" ), m_bSound);
            pConf->Write ( _T ( "SoundFilepath" ), m_sSound);
            pConf->Write ( _T ( "CommandEnabled" ), m_bCommand);
            pConf->Write ( _T ( "CommandFilepath" ), m_sCommand);
            return true;
      }
      else
            return false;
}

void watchman_pi::Alarm()
{
    if(m_bSound) {
        // play sound
    }
    if(m_bCommand) {
        wxProcess::Open(m_sCommand);
    }

    if(m_bMessageBox) {
        wxMessageDialog mdlg(m_parent_window, _("ALARM!!!!"),
                             wxString(_("Watchman"), wxOK | wxICON_ERROR));
        mdlg.ShowModal();
    }
}

void watchman_pi::SetCursorLatLon(double, double)
{
    m_DeadmanTimer.Start(m_DeadmanSpan.GetMilliseconds().ToLong(), true);
    m_DeadmanUpdateTime = wxDateTime::Now();
}

extern "C" void ll_gc_ll(double lat, double lon, double crs, double dist, double *dlat, double *dlon);
extern bool gshhsCrossesLand(double lat1, double lon1, double lat2, double lon2);
void watchman_pi::SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix)
{
    if(!m_bLandFall)
        return;

    wxDateTime now = wxDateTime::Now();
    wxTimeSpan diff = now - m_LastLandFallCheck;
    if(diff.GetSeconds() < 15)
        return;

    m_LastLandFallCheck = now;

    for(double t = 0; t<360; t+=18) {
        double dlat, dlon;
        ll_gc_ll(pfix.Lat, pfix.Lon, t, m_dLandFallDistance, &dlat, &dlon);

        if(gshhsCrossesLand(pfix.Lat, pfix.Lon, dlat, dlon)) {
            Alarm();
            return;
        }
    }

    if(m_pWatchmanDialog)
        m_pWatchmanDialog->UpdateLandFallTime(pfix);
}

void watchman_pi::ShowPreferencesDialog( wxWindow* parent )
{
    WatchmanPrefsDialog *dialog = new WatchmanPrefsDialog( parent );

    dialog->m_cbLandFall->SetValue(m_bLandFall);
    dialog->m_tcLandFallDistance->SetValue(wxString::Format(_T("%.2f"),
                                                            m_dLandFallDistance));

    dialog->m_cbDeadman->SetValue(m_bDeadman);
    dialog->m_sDeadmanMinutes->SetValue(m_DeadmanSpan.GetMinutes());

    dialog->m_cbMessageBox->SetValue(m_bMessageBox);

    dialog->m_cbSound->SetValue(m_bSound);
    dialog->m_fpSound->SetPath(m_sSound);
    dialog->m_cbCommand->SetValue(m_bCommand);
    dialog->m_tCommand->SetValue(m_sCommand);
    
    dialog->Fit();
    wxColour cl;
    GetGlobalColor(_T("DILG1"), &cl);
    dialog->SetBackgroundColour(cl);
    
    if(dialog->ShowModal() == wxID_OK)
    {
        m_bLandFall = dialog->m_cbLandFall->GetValue();
        dialog->m_tcLandFallDistance->GetValue().ToDouble(&m_dLandFallDistance);

        m_bDeadman = dialog->m_cbDeadman->GetValue();
        m_DeadmanSpan = wxTimeSpan::Minutes(dialog->m_sDeadmanMinutes->GetValue());

        m_bMessageBox = dialog->m_cbMessageBox->GetValue();

        m_bSound = dialog->m_cbSound->GetValue();
        m_sSound = dialog->m_fpSound->GetPath();
        m_bCommand = dialog->m_cbCommand->GetValue();
        m_sCommand = dialog->m_tCommand->GetValue();
        SaveConfig();
    }
    delete dialog;
}
