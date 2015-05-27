/*
 * File:   Launcher.cpp
 * Author: amz
 *
 * Created on 12 marzec 2014, 00:47
 */


#include "App.h"


bool App::OnInit() {
    #if defined (__LINUX__)
    if( SetSignalHandler(SIGINT,&App::OnSigTerm) || SetSignalHandler(SIGTERM,&App::OnSigTerm) ) {
        wxLogVerbose( _( "Successfully handler installed." ) );
    } else {
        wxLogError ( _( "Failed to install handler." ) );
    }
	#endif
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
        boost::this_thread::sleep(boost::posix_time::millisec(2000));

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

    mainWindow = new MainWindow(wxT("Virtual Loglan Processor"), this);
    return true;
}

int App::OnExit() {
    MESSAGE msg;
    msg.msg_type = MSG_VLP;
    msg.param.pword[0] = VLP_DISCONNECT;
    this->client->Write(&msg, sizeof(MESSAGE));
    wxLogMessage(wxString::Format("[VLP::OnExit]\n"));
    return 0;
}

void App::OnClientEvent(wxSocketEvent &event) {
    wxLogMessage(wxString::Format("[VLP::OnClientEvent] Proccesing event %d", event.GetId()));
}

void App::OnSocketEvent(wxSocketEvent &event) {
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
                        case NET_PROPAGATE:
                            switch(readValue.param.pword[1]) {
                                case MSG_INT:
                                    /*  pom = find_link_by_ID(msg.param.pword[5]);
                                     msg.msg_type = MSG_NET;
                                     msg.param.pword[0] = NET_PROPAGATE;
                                     send_int(pom,&msg);
                                     todo what to do here? */
                                    break;
                                case MSG_VLP:
                                    switch(readValue.param.pword[6])
                                    {
                                        case VLP_WRITE:
                                            this->mainWindow->text->AppendText("### Incoming Messsage ###\n");
                                            this->mainWindow->text->AppendText(wxString::Format("Mesg from Node %d: %s\n",readValue.param.pword[2],wxString(readValue.param.pstr, wxConvUTF8)));
                                            break;
                                        /*
                                        case VLP_CLOSE_INSTANCE:
                                            msg.msg_type = MSG_INT;
                                            msg.param.pword[0] = INT_CLOSE_INSTANCE;
                                            pom = findINTbyID(msg.param.pword[7]);
                                            if (pom!=NULL)
                                            {
                                                write(pom->sock,&msg,sizeof(MESSAGE));
                                                MESSAGE m1;
                                                m1.msg_type = MSG_VLP;
                                                m1.param.pword[0] = VLP_INTERPRETER_DOWN;
                                                m1.param.pword[1] = pom->ID;
                                                write(net_sock,&m1,sizeof(MESSAGE));
                                            } else WriteMessage("Instance not found");
                                            break;*/
                                    } /* VLP switch */
                            }
                            break;
                        default:
                            wxLogMessage(wxString::Format("[VLP::OnSocketEvent] Got unhandled event %ld MSG_NET: %d %d %d %d",   (long) &event, readValue.param.pword[0], readValue.param.pword[2],  readValue.param.pword[4], readValue.param.pword[6]));
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

void App::OnSigTerm(int sig) {;
    wxLogError(wxString::Format("got signal %d",sig));
    if(sig == SIGTERM || sig == SIGINT) {
        wxGetApp().mainWindow->Close(true);
        wxGetApp().Exit();
    }
}

wxSocketClient *App::getSocketClient() {
    return client;
}

BEGIN_EVENT_TABLE(App, wxApp)
                EVT_SOCKET(CLIENT_EVENT_ID, App::OnClientEvent)
                EVT_SOCKET(SOCKET_EVENT_ID, App::OnSocketEvent)
END_EVENT_TABLE()

