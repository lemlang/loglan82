/* 
 * File:   VLPMainWindow.cpp
 * Author: amz
 * 
 * Created on 12 marzec 2014, 10:48
 */


#include "MainWindow.h"

MainWindow::MainWindow(const wxString& title, App * parent)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(650, 350)) {
    this->parent = parent;
    //(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = "dialogBox")
    this->messageDialog = new MessageDialog(this, wxWindow::NewControlId(), _("Send message to node") );
    menubar = new wxMenuBar;
    file = new wxMenu;
    file->Append(wxID_EXECUTE, wxT("&Execute"));
    file->Append(wxID_CONVERT, wxT("&Compile"));
    file->Append(wxID_ABORT, wxT("&Kill"));
    file->AppendSeparator();
    file->Append(wxID_EXIT, wxT("&Quit"));
    network = new wxMenu;
    menubar->Append(file, wxT("&Program"));
    file = new wxMenu;
    file->Append(VLPMenu_Connect, wxT("&Connect"));
    file->Append(VLPMenu_Disconnect, wxT("&Disconnect"));
    file->Append(VLPMenu_SendMessage, wxT("&Send message"));

    menubar->Append(file, wxT("&Network"));
    SetMenuBar(menubar);
    text = new wxTextCtrl(this, TEXT_Main, "", wxDefaultPosition, wxDefaultSize,
            wxTE_MULTILINE | wxTE_READONLY, wxTextValidator(wxFILTER_NONE), wxTextCtrlNameStr);


    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MainWindow::OnQuit));
    
    Connect(wxID_CONVERT, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MainWindow::OnCompile));
    Connect(wxID_EXECUTE, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MainWindow::OnExecute));
    Connect(wxID_ABORT, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MainWindow::OnKill));
    Connect(VLPMenu_Connect, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MainWindow::OnConnect));
    Connect(VLPMenu_Disconnect, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MainWindow::OnDisconnect));
    Connect(VLPMenu_SendMessage, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MainWindow::OnSendMessage));
    Centre();
}

void MainWindow::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    Close(FALSE);
}

void MainWindow::OnClose(wxCloseEvent& event) {
    if (event.CanVeto()) {
        if (showQuitDialog() != wxID_YES) {

            event.Veto();
            return;
        }
    }
    this->Destroy();
}

int MainWindow::showKillDialog() {
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
int MainWindow::showQuitDialog() {
    wxMessageDialog *dial = new wxMessageDialog(NULL,
            wxT("Are you sure to quit?"), wxT("Question"),
            wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    int ret = dial->ShowModal();
    dial->Destroy();
    wxLogDebug(wxString::Format(_("Show Dialog returns %d."), ret));
    return ret;
}

void MainWindow::OnExecute(wxCommandEvent& WXUNUSED(event)) {
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

void MainWindow::OnKill(wxCommandEvent &event) {
    int id = showKillDialog();
    if( id > 0) {
        MESSAGE m;
        m.msg_type = MSG_INT;
        m.param.pword[0] = INT_KILL;
        m.param.pword[1] = id;

    }
}void MainWindow::OnConnect(wxCommandEvent &event) {
    wxString address = showConnectDialog();
    MESSAGE m;
    m.msg_type = MSG_NET;
    m.param.pword[0] = NET_CONNECT_TO;
    strcpy(m.param.pstr,address);
    this->parent->getSocketClient()->Write(&m,sizeof(MESSAGE));
    //todo send connection signals
}


void MainWindow::OnDisconnect(wxCommandEvent &event) {
    MESSAGE m;
    m.msg_type = MSG_VLP;
    m.param.pword[0] = NET_DISCONNECT;
    this->parent->getSocketClient()->Write(&m,sizeof(MESSAGE));
    this->text->AppendText(_("Disconnecting from remote machines\n"));
}

wxString MainWindow::showConnectDialog() {
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

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_CLOSE(MainWindow::OnClose)
END_EVENT_TABLE()


void MainWindow::OnSendMessage(wxCommandEvent &event) {
    if ( this->messageDialog->ShowModal() == wxID_OK ) {
        wxLogDebug(_("Send Message dialog."));
        MESSAGE message;
        message.msg_type = MSG_NET;
        message.param.pword[0] = NET_PROPAGATE;
        message.param.pword[1] = MSG_VLP;
        message.param.pword[2] = 0;
        message.param.pword[4] = this->messageDialog->getNode();
        message.param.pword[6] = VLP_WRITE;
        strcpy(message.param.pstr,this->messageDialog->GetName().c_str());
        this->parent->getSocketClient()->Write(&message,sizeof(MESSAGE));
    } else {
        wxLogDebug(_("Send Message dialog cancel."));
    }

}

void MainWindow::OnCompile(wxCommandEvent &event) {
    wxFileDialog
            openFileDialog(this, _("Compile Loglan file"), "", "",
                           "log files (*.log)|*.log", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return; // the user changed idea...
    wxFileName executablesDir = wxPathOnly( wxStandardPaths::Get().GetExecutablePath() );
    wxString wxString1 = wxString::Format("%s%s",
                                          executablesDir.GetFullPath(),
                                          wxFileName::GetPathSeparators());
    wxString graphcsCommand = wxString::Format("%svlpc %s", wxString1, openFileDialog.GetPath());
    this->text->AppendText(graphcsCommand);

    wxScopedPtr<wxProcess> process(new wxProcess);
    process->Redirect();

    wxExecute(graphcsCommand, wxEXEC_BLOCK, process.get());

    this->ReadOutput(*(process->GetErrorStream()));
    this->ReadOutput(*(process->GetInputStream()));

}

bool MainWindow::ReadOutput(wxInputStream& s)
{
    // the stream could be already at EOF or in wxSTREAM_BROKEN_PIPE state
    s.Reset();
    wxTextInputStream tis(s, " ", wxConvUTF8);

    while (true)
    {
        wxString line = tis.ReadLine();
        if ( !line.empty() )
            this->text->AppendText(line);
        this->text->AppendText(wxTextFile::GetEOL());
            //this->text->AppendText("\n");
        if (s.Eof())
            break;
        if ( !s )
            return false;
    }

    return true;
}

