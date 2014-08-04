/* 
 * File:   VLPMainWindow.cpp
 * Author: amz
 * 
 * Created on 12 marzec 2014, 10:48
 */


#include "VLPMainWindow.h"
#include <wx/wfstream.h>

VLPMainWindow::VLPMainWindow(const wxString& title)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(650, 350)) {
    menubar = new wxMenuBar;
    file = new wxMenu;
    file->Append(wxID_EXECUTE, wxT("&Execute"));
    file->Append(wxID_ABORT, wxT("&Kill"));
    file->AppendSeparator();
    file->Append(wxID_EXIT, wxT("&Quit"));
    menubar->Append(file, wxT("&Program"));
    SetMenuBar(menubar);

    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(VLPMainWindow::OnQuit));
    
    Connect(wxID_EXECUTE, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(VLPMainWindow::OnExecute));
    Centre();
}

void VLPMainWindow::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    Close(FALSE);
}

void VLPMainWindow::OnClose(wxCloseEvent& event) {
    if (showQuitDialog() == wxID_YES) {
        this->Destroy();
    } else {
        if (event.CanVeto()) {
            event.Veto();
        }
    }
}

int VLPMainWindow::showQuitDialog() {
    wxMessageDialog *dial = new wxMessageDialog(NULL,
            wxT("Are you sure to quit?"), wxT("Question"),
            wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    int ret = dial->ShowModal();
    dial->Destroy();
    wxLogDebug(wxString::Format(_("Show Dialog returns %d."), ret));
    return ret;
}

void VLPMainWindow::OnExecute(wxCommandEvent& WXUNUSED(event)) {
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
        
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        wxLog::SetActiveTarget(target);
        return;
    }

}

BEGIN_EVENT_TABLE(VLPMainWindow, wxFrame)
EVT_CLOSE(VLPMainWindow::OnClose)
END_EVENT_TABLE()