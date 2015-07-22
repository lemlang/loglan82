/*
 * File:   VLPMainWindow.h
 * Author: amz
 *
 * Created on 12 marzec 2014, 10:48
 */

#ifndef GRAPHICSWINDOW_H
#define	GRAPHICSWINDOW_H
#include <wx/wx.h>
#include <wx/wfstream.h>
#include <wx/textctrl.h>
#include <wx/textbuf.h>
#include <queue>
#include <deque>
#include "../head/comm.h"
#include "Graphics.h"

class Graphics;
class GraphicsWindow: public wxFrame  {
public:
    ~GraphicsWindow();
    GraphicsWindow(const wxString& title, Graphics*graphics);
    void OnQuit(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnExecute(wxCommandEvent& WXUNUSED(event));
    void PutChar(char ch);
    int ReadChar();
    void onKeyDown(wxKeyEvent &aEvent);
    void onKeyUp(wxKeyEvent &aEvent);
    void onMouseClick(wxMouseEvent &aEvent);
    void onChar(wxKeyEvent &aEvent);
    void WriteText(char string[]);
    void WaitRead(int messageType);
    bool PopInputQueue(int *pInt);
    bool GetLine();
    void ClearAll();

    DECLARE_EVENT_TABLE()


private:
    int showQuitDialog();
    wxTextCtrl* text; // the main text area
    std::queue<wxChar> read_queue;
    std::deque<wxChar> input_queue;
    int input_queue_nl_count;
    wxString input_buffer;
    Graphics* parent; };

enum  {
    TEXT_Main = wxID_HIGHEST + 1
};

#endif	/* VLPMAINWINDOW_H */

