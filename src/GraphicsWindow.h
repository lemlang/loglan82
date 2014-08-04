/*
 * File:   VLPMainWindow.h
 * Author: amz
 *
 * Created on 12 marzec 2014, 10:48
 */

#ifndef GRAPHICSWINDOW_H
#define	GRAPHICSWINDOW_H
#include <wx/wx.h>

class GraphicsWindow: public wxFrame  {
public:
    ~GraphicsWindow();
    GraphicsWindow(const wxString& title);
    void OnQuit(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnExecute(wxCommandEvent& WXUNUSED(event));
    //virtual void SetTitle(const wxString&);
    DECLARE_EVENT_TABLE()
private:
    int showQuitDialog();

};

#endif	/* VLPMAINWINDOW_H */

