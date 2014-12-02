/*
 * File:   Launcher.cpp
 * Author: amz
 *
 * Created on 12 marzec 2014, 00:47
 */

#include "Graphics.h"
#include "../head/comm.h"


void Graphics::OnInitCmdLine(wxCmdLineParser &parser) {
    parser.SetDesc(g_cmdLineDesc);
    parser.SetSwitchChars(wxT("-"));
}

bool Graphics::OnCmdLineParsed(wxCmdLineParser &parser) {
    if (parser.Found("h")) {
        return false;
    }
    if (parser.GetParamCount() == 1) {
        this->interpreter_identifier = wxAtoi(parser.GetParam(0));
        return true;
    } else {
        parser.Usage();
        return false;
    }
}

bool Graphics::OnInit() {
    wxApp::OnInit();
    wxLog::SetActiveTarget(new wxLogStderr);
    this->client = new wxSocketClient();;
    this->client->SetEventHandler(*this, SOCKET_EVENT_ID);
    this->client->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
    this->client->Notify(true);

    wxIPV4address address;
    address.Hostname("localhost");
    address.Service(3600);
    this->client->Connect(address);
    MESSAGE msg;
    msg.msg_type = MSG_GRAPH;
    msg.param.pword[0] = GRAPH_ALLOCATING;
    wxLogMessage(wxString::Format("[Graphics::OnInit] interpreter_identifier: %d\n", this->interpreter_identifier));
    msg.param.pword[1] = this->interpreter_identifier;
    this->client->Write(&msg, sizeof(MESSAGE));


    this->window = new GraphicsWindow(wxT("VLP - Graphics Resource"));
    this->window->Show(true);
    return true;
}

int Graphics::OnExit() {
    wxLogMessage(wxString::Format("[Graphics::OnExit]"));

    return 0;
}

void Graphics::OnClientEvent(wxSocketEvent &event) {
    wxLogMessage(wxString::Format("[Graphics::OnClientEvent] Proccesing event %d", event.GetId()));
}

void Graphics::OnSocketEvent(wxSocketEvent &event) {
    try {
        wxLogMessage(wxString::Format("[Graphics:%d::OnSocketEvent] Proccesing event %ld", __LINE__, (long)&event));
        switch (event.GetSocketEvent()) {
            case wxSOCKET_INPUT:
                //wxLogMessage(wxString::Format("[Graphics::OnSocketEvent] Socket wxSOCKET_INPUT %ld", (long)&event));

                MESSAGE readValue,writeValue;
                event.GetSocket()->Read(&readValue, sizeof(MESSAGE));
                //wxLogMessage(wxString::Format("[Graphics::OnSocketEvent] Readed MESSAGE %ld", (long)&event));
                if (readValue.msg_type == MSG_GRAPH) {
                    switch (readValue.param.pword[0]) {
                        case GRAPH_SET_TITLE:
                            //wxLogMessage(wxString::Format("[Graphics:%d::OnSocketEvent] GRAPH_SET_TITLE %ld", __LINE__, (long)&event));
                            this->window->SetTitle( wxString(readValue.param.pstr));
                            break;
                        case GRAPH_PUTCHAR:
                            this->window->PutChar( readValue.param.pchar);
                            break;
                        case GRAPH_INKEY:
                            wxLogMessage(wxString::Format("[Graphics:%d::OnSocketEvent] GRAPH_INKEY %ld", __LINE__, (long)&event));
                            window->ReadChar();
                            break;
                        default:
                            wxLogMessage(wxString::Format("[Graphics:%d::OnSocketEvent] Got unhandled event %ld MSG_GRAPH type: %d", __LINE__, (long)&event, readValue.param.pword[0]));
                            break;
                    }
                } else {
                    //wxLogMessage(wxString::Format("[Graphics:%d::OnSocketEvent] Got event %ld non MSG_GRAPH mesg_type %d",  __LINE__, (long)&event, readValue.msg_type ));



                }
                break;

            case wxSOCKET_LOST:
                wxLogMessage(wxString(_ ("[Graphics::OnSocketEvent] Socket lost")));
                break;

            case wxSOCKET_OUTPUT:
                wxLogMessage(wxString::Format(_ ("[Graphics::OnSocketEvent] wxSOCKET_OUTPUT event: %ld"), (long)&event));
                break;

            default:
                wxLogMessage(wxString::Format(_ ("[Graphics::OnSocketEvent] Socket other event: %ld"), (long)&event));
                break;

        }
        wxLogMessage(wxString::Format("[Graphics::OnSocketEvent] Proccessed event %ld", (long)&event));

    } catch (int e) {
        wxLogMessage(wxString::Format("[Graphics::OnSocketEvent] Exception when procceing event %ld", (long)&event));

        //wxLogMessage ( wxString::Format( _ ( "[Graphics::OnSocketEvent] An exception occurred. ") ));
    }
}

BEGIN_EVENT_TABLE(Graphics, wxApp)
                EVT_SOCKET(CLIENT_EVENT_ID, Graphics::OnClientEvent)
                EVT_SOCKET(SOCKET_EVENT_ID, Graphics::OnSocketEvent)
END_EVENT_TABLE()
