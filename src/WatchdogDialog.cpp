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

#include <wx/imaglist.h>

#include "watchdog_pi.h"
#include "WatchdogDialog.h"
#include "EditAlarmDialog.h"

/* XPM */
const char * box_xpm[] = {
"20 20 3 1",
" 	c None",
".	c #FFFFFF",
"+	c #333333",
"....................",
".++++++++++++++++++.",
".+................+.",
".+................+.",
".+................+.",
".+................+.",
".+................+.",
".+................+.",
".+................+.",
".+................+.",
".+................+.",
".+................+.",
".+................+.",
".+................+.",
".+................+.",
".+................+.",
".+................+.",
".+................+.",
".++++++++++++++++++.",
"...................."};

/* XPM */
const char * check_xpm[] = {
"20 20 3 1",
" 	c None",
".	c #FFFFFF",
"+	c #333333",
"....................",
".++++++++++++++++++.",
".+................+.",
".+.++..........++.+.",
".+.+++........+++.+.",
".+..+++......+++..+.",
".+...+++....+++...+.",
".+....+++..+++....+.",
".+.....++++++.....+.",
".+......++++......+.",
".+......++++......+.",
".+.....++++++.....+.",
".+....+++..+++....+.",
".+...+++....+++...+.",
".+..+++......+++..+.",
".+.+++........+++.+.",
".+.++..........++.+.",
".+................+.",
".++++++++++++++++++.",
"...................."};


enum AlarmStatus { ALARM_ENABLED, ALARM_TYPE, ALARM_STATUS };

WatchdogDialog::WatchdogDialog( watchdog_pi &_watchdog_pi, wxWindow* parent)
    : WatchdogDialogBase( parent ), m_watchdog_pi(_watchdog_pi)
{
    wxFileConfig *pConf = GetOCPNConfigObject();

    pConf->SetPath ( _T( "/Settings/Watchdog" ) );

#ifdef __WXGTK__
    Move(0, 0);        // workaround for gtk autocentre dialog behavior
#endif
    Move(pConf->Read ( _T ( "DialogPosX" ), 20L ), pConf->Read ( _T ( "DialogPosY" ), 20L ));

    wxImageList *imglist = new wxImageList(20, 20, true, 1);
    imglist->Add(wxBitmap(box_xpm));
    imglist->Add(wxBitmap(check_xpm));
    m_lStatus->AssignImageList(imglist, wxIMAGE_LIST_SMALL);

    m_lStatus->InsertColumn(ALARM_ENABLED, _T("X"));
    m_lStatus->InsertColumn(ALARM_TYPE, _("Type"));
    m_lStatus->InsertColumn(ALARM_STATUS, _("Status"));
    m_lStatus->SetColumnWidth(ALARM_ENABLED, wxLIST_AUTOSIZE);
    m_lStatus->SetColumnWidth(ALARM_TYPE, wxLIST_AUTOSIZE);
    m_lStatus->SetColumnWidth(ALARM_STATUS, wxLIST_AUTOSIZE);

    this->GetSizer()->Fit( this );
    this->Layout();
    this->SetSizeHints( GetSize().x, GetSize().y );
}

WatchdogDialog::~WatchdogDialog()
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    pConf->SetPath ( _T ( "/Settings/Watchdog" ) );

    wxPoint p = GetPosition();

    pConf->Write ( _T ( "DialogPosX" ), p.x );
    pConf->Write ( _T ( "DialogPosY" ), p.y );
}

void WatchdogDialog::UpdateAlarms()
{
    while((int)Alarm::s_Alarms.size() > m_lStatus->GetItemCount()) {
        wxListItem item;
        m_lStatus->InsertItem(0, item);
    }

    while(m_lStatus->GetItemCount() > (int)Alarm::s_Alarms.size())
        m_lStatus->DeleteItem(0);

    for(unsigned int i=0; i<Alarm::s_Alarms.size(); i++)
        UpdateStatus(i);
    
}

void WatchdogDialog::UpdateStatus(int index)
{
    Alarm *alarm = Alarm::s_Alarms[index];
    m_lStatus->SetItemImage(index, alarm->m_bEnabled ? 1 : 0);

//    m_lAlarms->SetItem(index, ALARM_ENABLED, alarm->m_bEnabled ? _T("X") : _T(""));
    m_lStatus->SetColumnWidth(ALARM_ENABLED, wxLIST_AUTOSIZE);
    m_lStatus->SetItem(index, ALARM_TYPE, alarm->Type());
    m_lStatus->SetColumnWidth(ALARM_TYPE, wxLIST_AUTOSIZE);
    m_lStatus->SetItem(index, ALARM_STATUS, alarm->GetStatus());
    if(alarm->m_bSpecial)
        m_lStatus->SetItemTextColour(index, *wxBLUE);
    else
        m_lStatus->SetItemTextColour(index, alarm->m_bFired ? *wxRED: *wxBLACK);
    m_lStatus->SetColumnWidth(ALARM_STATUS, wxLIST_AUTOSIZE);

}

void WatchdogDialog::OnLeftDown( wxMouseEvent& event )
{
    if(event.GetX() >= m_lStatus->GetColumnWidth(0))
        return;

    wxPoint pos = event.GetPosition();
    int flags = 0;
    long index = m_lStatus->HitTest(pos, flags);
    if(index < 0)
        return;

    Alarm *alarm = Alarm::s_Alarms[index];
    alarm->m_bEnabled = !alarm->m_bEnabled;
    if(!alarm->m_bEnabled) {
        alarm->m_bFired = false;
        RequestRefresh(GetOCPNCanvasWindow());
    }
    UpdateStatus(index);
}

void WatchdogDialog::OnDoubleClick( wxMouseEvent& event )
{
    if(event.GetX() < m_lStatus->GetColumnWidth(0))
        return;

    wxPoint pos = event.GetPosition();
    int flags = 0;
    long index = m_lStatus->HitTest(pos, flags);
    if(index < 0)
        return;

    Alarm *alarm = Alarm::s_Alarms[index];
    EditAlarmDialog dlg(this, alarm);
    if(dlg.ShowModal() == wxID_OK)
        dlg.Save();
}

void WatchdogDialog::OnConfiguration( wxCommandEvent& event )
{
    m_watchdog_pi.ShowConfigurationDialog( this );
}

void WatchdogDialog::OnReset( wxCommandEvent& event )
{
    Alarm::ResetAll();
    UpdateAlarms();
}
