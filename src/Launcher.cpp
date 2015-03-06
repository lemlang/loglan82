/*
 * File:   Launcher.cpp
 * Author: amz
 *
 * Created on 12 marzec 2014, 00:47
 */


#include "Launcher.h"


bool Launcher::OnInit() {
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

    }

    MESSAGE msg;
    msg.msg_type = MSG_VLP;
    msg.param.pword[0] = VLP_CONNECT;
    wxLogMessage(wxString::Format("[VLP::OnInit] Connecting VM\n"));
    this->client->Write(&msg, sizeof(MESSAGE));

    mainWindow = new VLPMainWindow(wxT("Virtual Loglan Processor"));
    mainWindow->Show(true);
    return true;
}

int Launcher::OnExit() {
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
                if (readValue.msg_type == MSG_GRAPH) {
                    switch (readValue.param.pword[0]) {
                        //case GRAPH_SET_TITLE:
                            //wxLogMessage(wxString::Format("[VLP:%d::OnSocketEvent] GRAPH_SET_TITLE %ld", __LINE__, (long)&event));
                        //    this->window->SetTitle( wxString(readValue.param.pstr));
                        //    break;
                        default:
                            wxLogMessage(wxString::Format("[VLP::OnSocketEvent] Got unhandled event %ld MSG_GRAPH type: %d", (long)&event, readValue.param.pword[0]));
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

BEGIN_EVENT_TABLE(Launcher, wxApp)
                EVT_SOCKET(CLIENT_EVENT_ID, Launcher::OnClientEvent)
                EVT_SOCKET(SOCKET_EVENT_ID, Launcher::OnSocketEvent)
END_EVENT_TABLE()
