/*
 * File:   GraphicsWindow.cpp
 * Author: amz
 *
 * Created on 12 marzec 2014, 10:48
 */


#include "GraphicsWindow.h"
#include <wx/wfstream.h>
#include <wx/textctrl.h>

GraphicsWindow::GraphicsWindow(const wxString& title)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(650, 350)) {
    text = new wxTextCtrl(this, TEXT_Main, "", wxDefaultPosition, wxDefaultSize,
            wxTE_MULTILINE , wxTextValidator(wxFILTER_ALPHANUMERIC), wxTextCtrlNameStr);
    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(GraphicsWindow::OnQuit));
    Connect(wxID_EXECUTE, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(GraphicsWindow::OnExecute));
    text->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(GraphicsWindow::onKeyDown));
    text->Connect(wxEVT_KEY_UP,   wxKeyEventHandler(GraphicsWindow::onKeyUp));
    text->Connect(wxEVT_CHAR,   wxKeyEventHandler(GraphicsWindow::onChar));
    text->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(GraphicsWindow::onMouseClick));
    text->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(GraphicsWindow::onMouseClick));
    text->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(GraphicsWindow::onMouseClick));
    text->Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(GraphicsWindow::onMouseClick));
    text->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(GraphicsWindow::onMouseClick));
    text->Connect(wxEVT_RIGHT_DCLICK, wxMouseEventHandler(GraphicsWindow::onMouseClick));
    Centre();
}


void GraphicsWindow::onMouseClick(wxMouseEvent& aEvent)
{
    wxLogMessage(wxString::Format(_("GraphicsWindow::onMouseClick\n")));
    //aEvent.ResumePropagation(1);
    //aEvent.Skip();
    aEvent.StopPropagation();
}

void GraphicsWindow::onKeyDown(wxKeyEvent& aEvent)
{

    wxLogMessage(wxString::Format(_("GraphicsWindow::onKeyDown\n")));
    aEvent.ResumePropagation(1);
    aEvent.Skip();
    //aEvent.StopPropagation();
}

void GraphicsWindow::onKeyUp(wxKeyEvent& aEvent)
{

    wxLogMessage(wxString::Format(_("GraphicsWindow::onKeyUp \"%c\" %d %d\n"),aEvent.GetUnicodeKey(), aEvent.GetKeyCode(), aEvent.GetModifiers() ));
    aEvent.ResumePropagation(1);
    aEvent.Skip();
    //aEvent.StopPropagation();
}

void GraphicsWindow::onChar(wxKeyEvent& aEvent)
{
    wxLogMessage(wxString::Format(_("GraphicsWindow::onChar \"%c\" %d\n"),aEvent.GetUnicodeKey(), aEvent.GetKeyCode() ));
    //aEvent.ResumePropagation(1);
    //aEvent.Skip();
    aEvent.StopPropagation();
}
void GraphicsWindow::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    Close(FALSE);
}

void GraphicsWindow::OnClose(wxCloseEvent& event) {
    this->Destroy();return;
    if (showQuitDialog() == wxID_YES) {
        this->Destroy();
    } else {
        if (event.CanVeto()) {
            event.Veto();
        }
    }
}

int GraphicsWindow::showQuitDialog() {
    wxMessageDialog *dial = new wxMessageDialog(NULL,
            wxT("Are you sure to quit?"), wxT("Question"),
            wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    int ret = dial->ShowModal();
    dial->Destroy();
    wxLogMessage(wxString::Format(_("[GraphicsWindow::showQuitDialog] Show Dialog returns %d."), ret));
    return ret;
}

void GraphicsWindow::OnExecute(wxCommandEvent& WXUNUSED(event)) {
    wxFileDialog
    openFileDialog(this, _("Open Loglan file"), "", "",
            "log files (*.log)|*.log", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return; // the user changed idea...

    // proceed loading the file chosen by the user;
    // this can be done with e.g. wxWidgets input streams:
    wxFileInputStream input_stream(openFileDialog.GetPath());
    if (!input_stream.IsOk()) {
        wxLog* target = wxLog::GetActiveTarget();
        wxLog::SetActiveTarget(NULL);

        wxLogMessage("[GraphicsWindow::showQuitDialog] Cannot open file '%s'.", openFileDialog.GetPath());
        wxLog::SetActiveTarget(target);
        return;
    }

}
GraphicsWindow::~GraphicsWindow(){
}

BEGIN_EVENT_TABLE(GraphicsWindow, wxFrame)
EVT_CLOSE(GraphicsWindow::OnClose)
END_EVENT_TABLE()

void GraphicsWindow::PutChar(char ch) {
    (*text) << ch;
}
