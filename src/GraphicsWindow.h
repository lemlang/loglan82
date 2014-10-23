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
    void PutChar(char ch);
    void onKeyDown(wxKeyEvent &aEvent);
    void onKeyUp(wxKeyEvent &aEvent);
    void onMouseClick(wxMouseEvent &aEvent);
    void onChar(wxKeyEvent &aEvent);
    DECLARE_EVENT_TABLE()
private:
    int showQuitDialog();
    wxTextCtrl *text; // the main text area



};

enum  {
    TEXT_Main = wxID_HIGHEST + 1
};

#endif	/* VLPMAINWINDOW_H */

