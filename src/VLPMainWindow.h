/* 
 * File:   VLPMainWindow.h
 * Author: amz
 *
 * Created on 12 marzec 2014, 10:48
 */

#ifndef VLPMAINWINDOW_H
#define	VLPMAINWINDOW_H
#include <wx/wx.h>

class VLPMainWindow: public wxFrame  {
public:
    VLPMainWindow(const wxString& title);
    void OnQuit(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnExecute(wxCommandEvent& WXUNUSED(event));
    wxMenuBar *menubar;
    wxMenu *file;
    DECLARE_EVENT_TABLE()
private:
    int showQuitDialog();

};

#endif	/* VLPMAINWINDOW_H */

