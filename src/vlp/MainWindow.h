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
#include <wx/textfile.h>
#include <wx/txtstrm.h>
#include <wx/process.h>
#include "App.h"
#include "MessageDialog.h"


class App;
class MessageDialog;
class MainWindow : public wxFrame  {
public:
    MainWindow(const wxString& title, App *parent);
    void OnQuit(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnExecute(wxCommandEvent& WXUNUSED(event));
    void OnCompile(wxCommandEvent& WXUNUSED(event));
    void OnCompiled(wxProcessEvent& event);
    void OnKill(wxCommandEvent& WXUNUSED(event));
    void OnConnect(wxCommandEvent& WXUNUSED(event));
    void OnDisconnect(wxCommandEvent& WXUNUSED(event));
    void OnSendMessage(wxCommandEvent& WXUNUSED(event));

    wxTextCtrl *text;
    DECLARE_EVENT_TABLE()
private:
    wxString lastIpConnected;
    wxMenuBar *menubar;
    wxMenu *file;
    wxMenu *network;
    int showQuitDialog();
    int showKillDialog();
    wxString showConnectDialog();
    App *parent;
    MessageDialog*messageDialog;

    bool ReadOutput(wxInputStream& s);
};

enum  {
    TEXT_Main = wxID_HIGHEST + 1,
    VLPMenu_Connect = TEXT_Main+ 1,
    VLPMenu_Disconnect = TEXT_Main+ 2,
    VLPMenu_SendMessage = TEXT_Main+ 3,
    VLPexecCompilled = wxID_HIGHEST+ 2
};
#endif	/* VLPMAINWINDOW_H */

