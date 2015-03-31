/*
 * File:   Launcher.cpp
 * Author: amz
 *
 * Created on 12 marzec 2014, 00:47
 */


#include "Launcher.h"


bool Launcher::OnInit() {
     ;
    if( SetSignalHandler(SIGINT,&Launcher::OnSigTerm) || SetSignalHandler(SIGTERM,&Launcher::OnSigTerm) ) {
        wxLogVerbose( _( "Successfully handler installed." ) );
    } else {
        wxLogError ( _( "Failed to install handler." ) );
    }
    wxLog::SetActiveTarget(new wxLogStderr);

    this->client = new wxSocketClient();;
    this->client->SetEventHandler(*this, SOCKET_EVENT_ID);
    this->client->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
    this->client->Notify(true);

    wxIPV4address address;
    address.Hostname("localhost");
    address.Service(3600);
    if( this->client->Connect(address, true) == false) {
        //cannot connect; try to 
        wxLogMessage(wxString::Format("[VLP::OnInit] Cannot connect to VM, trying to start one\n"));
        wxFileName executablesDir = wxPathOnly( wxStandardPaths::Get().GetExecutablePath() );
        wxString graphcsCommand = wxString::Format("%s%svlpvm",
                executablesDir.GetFullPath(),
                wxFileName::GetPathSeparators());
        wxExecute(graphcsCommand, wxEXEC_ASYNC);
        sleep(2);

        if( this->client->Connect(address, true) == false) {
            wxLogError(wxString::Format("[VLP::OnInit] Cannot connect to VM\n"));
            return false;
        }
    }

    MESSAGE msg;
    msg.msg_type = MSG_VLP;
    msg.param.pword[0] = VLP_CONNECT;
    wxLogMessage(wxString::Format("[VLP::OnInit] Connecting VM\n"));
    this->client->Write(&msg, sizeof(MESSAGE));

    mainWindow = new VLPMainWindow(wxT("Virtual Loglan Processor"), this);
    return true;
}

int Launcher::OnExit() {
    MESSAGE msg;
    msg.msg_type = MSG_VLP;
    msg.param.pword[0] = VLP_DISCONNECT;
    this->client->Write(&msg, sizeof(MESSAGE));
    wxLogMessage(wxString::Format("[VLP::OnExit]\n"));
    return 0;
}

void Launcher::OnClientEvent(wxSocketEvent &event) {
    wxLogMessage(wxString::Format("[VLP::OnClientEvent] Proccesing event %d", event.GetId()));
}

void Launcher::OnSocketEvent(wxSocketEvent &event) {
    try {
        wxLogMessage(wxString::Format("[VLP:%d::OnSocketEvent] Proccesing event %ld", __LINE__, (long)&event));
        switch (event.GetSocketEvent()) {
            case wxSOCKET_INPUT:
                MESSAGE readValue, writeValue;
                event.GetSocket()->Read(&readValue, sizeof(MESSAGE));
                if (readValue.msg_type == MSG_VLP) {
                    switch (readValue.param.pword[0]) {
                        case VLP_CONNECTED:
                            wxLogMessage(_("[VLP] Connected to VM"));
                            mainWindow->Show(true);
                            break;
                        case VLP_CONNECTION_FAILED:
                            wxLogMessage(_("[VLP] Failed to connect to VM"));
                            mainWindow->Close(true);
                            this->Exit();
                            break;
                        default:
                            wxLogMessage(wxString::Format("[VLP::OnSocketEvent] Got unhandled event %ld MSG_VLP type: %d", (long) &event, readValue.param.pword[0]));
                            break;
                    }
                } else if (readValue.msg_type == MSG_NET) {
                    switch (readValue.param.pword[0]) {
                        case NET_CSWRITELN:
                            this->mainWindow->text->AppendText(wxString::FromUTF8(readValue.param.pstr)+"\n");
                            break;
                        default:
                            wxLogMessage(wxString::Format("[VLP::OnSocketEvent] Got unhandled event %ld MSG_NET type: %d", (long) &event, readValue.param.pword[0]));
                            break;
                    }
                } else {
                    //wxLogMessage(wxString::Format("[VLP:%d::OnSocketEvent] Got event %ld non MSG_GRAPH mesg_type %d",  __LINE__, (long)&event, readValue.msg_type ));
                }
                break;

                        case wxSOCKET_LOST:
                            wxLogMessage(wxString(_ ("[VLP::OnSocketEvent] Socket lost")));
                            break;

                        case wxSOCKET_OUTPUT:
                            wxLogMessage(wxString::Format(_ ("[VLP::OnSocketEvent] wxSOCKET_OUTPUT event: %ld"), (long)&event));
                            break;

                        default:
                            wxLogMessage(wxString::Format(_ ("[VLP::OnSocketEvent] Socket other event: %ld"), (long)&event));
                            break;

        }
        wxLogMessage(wxString::Format("[VLP::OnSocketEvent] Proccessed event %ld", (long)&event));

    } catch (int e) {
        wxLogMessage(wxString::Format("[VLP::OnSocketEvent] Exception when procceing event %ld", (long)&event));
    }
}

void Launcher::OnSigTerm(int sig) {;
    wxLogError(wxString::Format("got signal %d",sig));
    if(sig == SIGTERM || sig == SIGINT) {
        wxGetApp().mainWindow->Close(true);
        wxGetApp().Exit();
    }
}

wxSocketClient *Launcher::getSocketClient() {
    return client;
}

BEGIN_EVENT_TABLE(Launcher, wxApp)
                EVT_SOCKET(CLIENT_EVENT_ID, Launcher::OnClientEvent)
                EVT_SOCKET(SOCKET_EVENT_ID, Launcher::OnSocketEvent)
END_EVENT_TABLE()

