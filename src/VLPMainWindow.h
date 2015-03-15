/* 
 * File:   VLPMainWindow.h
 * Author: amz
 *
 * Created on 12 marzec 2014, 10:48
 */

#ifndef VLPMAINWINDOW_H
#define	VLPMAINWINDOW_H

#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/socket.h>
#include "Launcher.h"


class Launcher;
class VLPMainWindow: public wxFrame  {
public:
    VLPMainWindow(const wxString& title, Launcher*parent);
    void OnQuit(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnExecute(wxCommandEvent& WXUNUSED(event));
    void OnKill(wxCommandEvent& WXUNUSED(event));
    void OnConnect(wxCommandEvent& WXUNUSED(event));

    DECLARE_EVENT_TABLE()
private:
    wxMenuBar *menubar;
    wxMenu *file;
    wxMenu *network;
    wxTextCtrl *text;
    int showQuitDialog();
    int showKillDialog();
    wxString showConnectDialog();
    Launcher*parent;
};

enum  {
    TEXT_Main = wxID_HIGHEST + 1,
    VLPMenu_Connect = TEXT_Main+ 1
};
#endif	/* VLPMAINWINDOW_H */

