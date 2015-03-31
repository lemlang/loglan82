/* 
 * File:   VLPMainWindow.cpp
 * Author: amz
 * 
 * Created on 12 marzec 2014, 10:48
 */


#include "VLPMainWindow.h"

VLPMainWindow::VLPMainWindow(const wxString& title, Launcher* parent)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(650, 350)) {
    this->parent = parent;
    menubar = new wxMenuBar;
    file = new wxMenu;
    file->Append(wxID_EXECUTE, wxT("&Execute"));
    file->Append(wxID_ABORT, wxT("&Kill"));
    file->AppendSeparator();
    file->Append(wxID_EXIT, wxT("&Quit"));
    network = new wxMenu;
    menubar->Append(file, wxT("&Program"));
    file = new wxMenu;
    file->Append(VLPMenu_Connect, wxT("&Connect"));
    file->Append(VLPMenu_Disconnect, wxT("&Disconnect"));

    menubar->Append(file, wxT("&Network"));
    SetMenuBar(menubar);
    text = new wxTextCtrl(this, TEXT_Main, "", wxDefaultPosition, wxDefaultSize,
            wxTE_MULTILINE | wxTE_READONLY, wxTextValidator(wxFILTER_NONE), wxTextCtrlNameStr);


    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(VLPMainWindow::OnQuit));
    
    Connect(wxID_EXECUTE, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(VLPMainWindow::OnExecute));
    Connect(wxID_ABORT, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(VLPMainWindow::OnKill));
    Connect(VLPMenu_Connect, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(VLPMainWindow::OnConnect));
    Connect(VLPMenu_Disconnect, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(VLPMainWindow::OnDisconnect));
    Centre();
}

void VLPMainWindow::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    Close(FALSE);
}

void VLPMainWindow::OnClose(wxCloseEvent& event) {
    if (event.CanVeto()) {
        if (showQuitDialog() != wxID_YES) {

            event.Veto();
            return;
        }
    }
    this->Destroy();
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
}void VLPMainWindow::OnConnect(wxCommandEvent &event) {
    wxString address = showConnectDialog();
    MESSAGE m;
    m.msg_type = MSG_NET;
    m.param.pword[0] = NET_CONNECT_TO;
    strcpy(m.param.pstr,address);
    this->parent->getSocketClient()->Write(&m,sizeof(MESSAGE));
    //todo send connection signals
}


void VLPMainWindow::OnDisconnect(wxCommandEvent &event) {
    MESSAGE m;
    m.msg_type = MSG_VLP;
    m.param.pword[0] = NET_DISCONNECT;
    this->parent->getSocketClient()->Write(&m,sizeof(MESSAGE));
    this->text->AppendText(_("Disconnecting from remote machines\n"));
}

wxString VLPMainWindow::showConnectDialog() {
    wxTextEntryDialog *dial = new wxTextEntryDialog(NULL,
            wxT("Submit IP to connect?"), wxT("Question"),lastIpConnected,
            wxTextEntryDialogStyle);
    const wxString allow[] = { wxT("1"),  wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"), wxT("7"), wxT("8"),
            wxT("9"), wxT("0"), wxT(".")};

    wxArrayString* str = new wxArrayString(11, allow);
    wxTextValidator validTxt(wxFILTER_INCLUDE_CHAR_LIST);
    validTxt.SetIncludes(*str);

    dial->SetTextValidator(validTxt);
    dial->ShowModal();
    lastIpConnected = dial->GetValue ();
    dial->Destroy();
    wxLogDebug(wxString::Format(_("Connect Dialog returns %s."), lastIpConnected));
    return lastIpConnected;
}

BEGIN_EVENT_TABLE(VLPMainWindow, wxFrame)
EVT_CLOSE(VLPMainWindow::OnClose)
END_EVENT_TABLE()
