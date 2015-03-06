/* 
 * File:   VLPMainWindow.cpp
 * Author: amz
 * 
 * Created on 12 marzec 2014, 10:48
 */


#include "VLPMainWindow.h"
#include "../head/comm.h"
#include <wx/wfstream.h>
#include <wx/filename.h>

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
    Connect(wxID_ABORT, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(VLPMainWindow::OnKill));
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

int VLPMainWindow::showKillDialog() {
    wxTextEntryDialog *dial = new wxTextEntryDialog(NULL,
            wxT("Which proccess to kill?"), wxT("Question"),wxT(""),
            wxTextEntryDialogStyle);
    dial->SetTextValidator(wxFILTER_DIGITS );
    dial->ShowModal();
    wxString target = dial->GetValue ();
    int num = wxAtoi(target );
    dial->Destroy();
    wxLogDebug(wxString::Format(_("Kill Dialog returns %d."), num));
    return num;
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
    wxFileName executablesDir = wxPathOnly( wxStandardPaths::Get().GetExecutablePath() );
    wxString wxString1 = wxString::Format("%s%s",
            executablesDir.GetFullPath(),
            wxFileName::GetPathSeparators());
    wxString graphcsCommand = wxString::Format("%sloglanint %s", wxString1, openFileDialog.GetPath());
    wxExecute(graphcsCommand, wxEXEC_ASYNC);
}

void VLPMainWindow::OnKill(wxCommandEvent &event) {
    int id = showKillDialog();
    if( id > 0) {
        MESSAGE m;
        m.msg_type = MSG_INT;
        m.param.pword[0] = INT_KILL;
        m.param.pword[1] = id;

    }
}
BEGIN_EVENT_TABLE(VLPMainWindow, wxFrame)
EVT_CLOSE(VLPMainWindow::OnClose)
END_EVENT_TABLE()

