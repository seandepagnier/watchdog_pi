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

      return (WANTS_TOOLBAR_CALLBACK    |
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

bool watchman_pi::LoadConfig(void)
{
      wxFileConfig *pConf = m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T( "/Settings/Watchman" ) );

            m_watchman_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 20L );
            m_watchman_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 20L );

            pConf->Read ( _T ( "LandFallDistance" ), &m_dLandFallDistance, 3 );
            pConf->Read ( _T ( "LandFallSoundEnabled" ), &m_bLandFallSound, 0 );
            pConf->Read ( _T ( "LandFallSoundFilepath" ), &m_sLandFallSound, _T("") );
            pConf->Read ( _T ( "LandFallCommandEnabled" ), &m_bLandFallCommand, 0 );
            pConf->Read ( _T ( "LandFallCommandFilepath" ), &m_sLandFallCommand, _T("") );
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

            pConf->Write ( _T ( "LandFallDistance" ), m_dLandFallDistance);
            pConf->Write ( _T ( "LandFallSoundEnabled" ), m_bLandFallSound);
            pConf->Write ( _T ( "LandFallSoundFilepath" ), m_sLandFallSound);
            pConf->Write ( _T ( "LandFallCommandEnabled" ), m_bLandFallCommand);
            pConf->Write ( _T ( "LandFallCommandFilepath" ), m_sLandFallCommand);
            return true;
      }
      else
            return false;
}

extern "C" void ll_gc_ll(double lat, double lon, double crs, double dist, double *dlat, double *dlon);
extern bool gshhsCrossesLand(double lat1, double lon1, double lat2, double lon2);
void watchman_pi::SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix)
{
    wxDateTime now = wxDateTime::Now();
    wxTimeSpan diff = now - m_LastLandFallCheck;
    if(diff.GetSeconds() < 15)
        return;

    m_LastLandFallCheck = now;

    for(double t = 0; t<360; t+=18) {
        double dlat, dlon;
        ll_gc_ll(pfix.Lat, pfix.Lon, t, m_dLandFallDistance, &dlat, &dlon);

        if(gshhsCrossesLand(pfix.Lat, pfix.Lon, dlat, dlon)) {
            if(m_bLandFallSound) {
                // play sound
            }
            if(m_bLandFallCommand) {
                wxProcess::Open(m_sLandFallCommand);
            }
            return;
        }
    }

    if(m_pWatchmanDialog)
        m_pWatchmanDialog->UpdateLandFallTime(pfix);
}

void watchman_pi::ShowPreferencesDialog( wxWindow* parent )
{
    WatchmanPrefsDialog *dialog = new WatchmanPrefsDialog( parent );

    dialog->m_tcLandFallDistance->SetValue(wxString::Format(_T("%.2f"),
                                                            m_dLandFallDistance));
    dialog->m_cbLandFallSound->SetValue(m_bLandFallSound);
    dialog->m_fpLandFallSound->SetPath(m_sLandFallSound);
    dialog->m_cbLandFallCommand->SetValue(m_bLandFallCommand);
    dialog->m_tLandFallCommand->SetValue(m_sLandFallCommand);
    
    dialog->Fit();
    wxColour cl;
    GetGlobalColor(_T("DILG1"), &cl);
    dialog->SetBackgroundColour(cl);
    
    if(dialog->ShowModal() == wxID_OK)
    {
        dialog->m_tcLandFallDistance->GetValue().ToDouble(&m_dLandFallDistance);
        m_bLandFallSound = dialog->m_cbLandFallSound->GetValue();
        m_sLandFallSound = dialog->m_fpLandFallSound->GetPath();
        m_bLandFallCommand = dialog->m_cbLandFallCommand->GetValue();
        m_sLandFallCommand = dialog->m_tLandFallCommand->GetValue();
        SaveConfig();
    }
    delete dialog;
}
