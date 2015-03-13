/*
 * File:   VM.cpp
 * Author: amz
 *
 * Created on 11 marzec 2014, 22:33
 */

#include "VM.h"
#include "../head/comm.h"

VM::VM() {
    this->port = 3600;

}

bool VM::OnInit() {
    if (!wxAppConsole::OnInit()) {
        return false;
    }
    if( verbose ) {
        wxLog::SetVerbose();
    }
    //This call enables us to use wxSocket calls in secondary threads
    wxSocketBase::Initialize();
    Connect(wxID_ANY,wxEVT_END_SESSION,(wxObjectEventFunction)&VM::OnClose, NULL, this);
    //void (VM::*OnSigTerm)(int) = &::add;


    if(SetSignalHandler(SIGINT,&VM::OnSigTerm)|| SetSignalHandler(SIGTERM,&VM::OnSigTerm) ) {
        wxLogVerbose( _( "Successfully handler installed." ) );
    } else {
        wxLogError ( _( "Failed to install handler." ) );
    }

    wxLog::SetActiveTarget ( new wxLogStderr );

    m_checker = new wxSingleInstanceChecker;
    if ( m_checker->IsAnotherRunning() ) {
        wxLogError ( _( "Another program instance is already running, aborting." ) );
        delete m_checker; // OnExit() won't be called if we return false
        m_checker = NULL;
        return false;
    }
    wxIPV4address address;
    address.Hostname ( "localhost" );
    address.Service ( 3600 );

    server = new wxSocketServer ( address, wxSOCKET_REUSEADDR);
    server->SetEventHandler ( *this, SERVER_EVENT_ID );
    server->SetNotify ( wxSOCKET_CONNECTION_FLAG );
    server->Notify ( true );

    this->executablesDir = wxPathOnly( wxStandardPaths::Get().GetExecutablePath() );
    wxLogVerbose( this->executablesDir.GetFullPath() );
    return true;
}

int VM::OnExit() {
    wxLogVerbose ( _ ( "[VM] OnExit" ) );
    this->configuration.CloseConnections();
    this->server->Close();
    delete m_checker;
    return 0;
}

void VM::OnServerEvent ( wxSocketEvent& event ) {
    wxLogVerbose ( wxString::Format ( _( "Server Event %d" ),event.GetEventType()) );

    // Accept the new connection and get the socket pointer
    wxSocketBase* sock = server->Accept ( false );

    // Tell the new socket how and where to process its events
    sock->SetEventHandler ( *this, SOCKET_EVENT_ID );
    sock->SetNotify ( wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG | wxSOCKET_OUTPUT_FLAG| wxSOCKET_CONNECTION_FLAG );
    sock->Notify ( true );

    wxLogVerbose ( _ ( "\nAccepted incoming connection.\n" ) );
}
void VM::OnSocketEvent ( wxSocketEvent& event ) {
    wxSocketBase *sock = event.GetSocket();

    // Process the event
    switch ( event.GetSocketEvent() ) {
    case wxSOCKET_INPUT: {
        MESSAGE readValue;
        // Read the data
        sock->Read ( &readValue, sizeof ( MESSAGE ) );

        switch ( readValue.msg_type ) {
            case MSG_VLP:
                wxLogVerbose ( wxString::Format ( _ ( "[VM::ProcessMessageVLP]  %lu"), (intptr_t)sock));
            this->ProcessMessageVLP ( &readValue,sock );
            break;
        case MSG_NET:
            wxLogVerbose ( wxString::Format ( _ ( "[VM::ProcessMessageNet]  %lu"), (intptr_t)sock));
            this->ProcessMessageNet ( &readValue,sock );
            break;
        case MSG_GRAPH:
            wxLogVerbose ( wxString::Format ( _ ( "[VM::ProcessMessageGraph] %lu"), (intptr_t)sock));
            this->ProcessMessageGraph ( &readValue,sock );
            break;
        case MSG_INT:
            wxLogVerbose ( wxString::Format ( _ ( "[VM::ProcessMessageInt]  %lu"), (intptr_t)sock));
            this->ProcessMessageInt ( &readValue,sock );
            break;
        default:
            wxLogVerbose ( wxString::Format ( _ ( "[VM::Received from client] %d" ), readValue.msg_type ) );
            break;
        }
        break;
    }
    case wxSOCKET_LOST: {
        wxLogVerbose ( wxString::Format( _ ( "[VM::Socket lost] %lu" ), (intptr_t)sock) );
        break;
    }
    case wxSOCKET_OUTPUT: {
        wxLogVerbose ( wxString::Format( _ ( "wxSOCKET_OUTPUT event: %d" ),event.GetEventType()) );
        break;
    }
    default: {
        wxLogVerbose ( wxString::Format( _ ( "Socket other event: %d" ),event.GetSocketEvent()) );
        break;
    }
    }
}

void VM::ProcessMessageInt ( MESSAGE*message, wxSocketBase*socket){
    MESSAGE responseMessage;
    wxLogVerbose ( wxString::Format( _ ( "[VM::ProcessMessageInt]int message type %d" ),message->param.pword[0] ) );
    switch(message->param.pword[0]) {
        case INT_CTX_REQ:
            wxIPV4address address;
            socket->GetPeer(address);

            responseMessage.param.pword[2] = configuration.AddLocalInstance(address.Service(),socket);
            responseMessage.msg_type = MSG_INT;
            responseMessage.param.pword[0] = INT_CTX;
            responseMessage.param.pword[1] = this->getNodeNumber();
            /** if (e->remote)
            {
                msg.param.pword[3] = e->p_ctx.node;
                msg.param.pword[4] = e->p_ctx.program_id;
            }*/
            socket->Write(&responseMessage,sizeof(MESSAGE));
            //socket->Close();
            //socket->Destroy();
            break;
    }
}
void VM::ProcessMessageGraph ( MESSAGE*message, wxSocketBase*socket){
    if (message->param.pword[0]==GRAPH_ALLOCATE) {
        wxLogVerbose(wxString::Format("[VM::Socket address] %lu",(intptr_t)socket));
        VMServerThread* pThread = new VMServerThread(this, socket);
        pThread->Create();
        pThread->Run();
    } else if (message->param.pword[0]== GRAPH_ALLOCATING) {
        wxIPV4address address;
        socket->GetPeer(address);
        configuration.ChangeLocalInstance(message->param.pword[1],address.Service(),socket);
        wxLogVerbose(wxString::Format("[VM::MessageGraph Processed] %lu",(intptr_t)socket));
    } else if  (message->param.pword[0]== GRAPH_INKEY_RESPONSE) {
        this->ForwardToIntModule(message,socket);
    } else {
        this->ForwardToGraphModule(message,socket);
    }
}
void VM::ProcessMessageNet ( MESSAGE* message, wxSocketBase* socket) {
    wxLogVerbose(wxString::Format("ProcessMessageNet %d %l",message->msg_type,socket->GetTimeout()));
}

void VM::ProcessMessageVLP ( MESSAGE* message, wxSocketBase* socket) {
    MESSAGE responseMessage;

    switch(message->param.pword[0]) {
        case VLP_CONNECT:
            if( vlp != NULL&& vlp->IsConnected() ) {
                //todo reply VLP_CONNECTION_FAILED
                responseMessage.msg_type = MSG_VLP;
                responseMessage.param.pword[0] = VLP_CONNECTION_FAILED;
                socket->Write(&responseMessage, sizeof(MESSAGE));
            } else {
                vlp = socket;
                responseMessage.msg_type = MSG_VLP;
                responseMessage.param.pword[0] = VLP_CONNECTED;
                vlp->Write(&responseMessage, sizeof(MESSAGE));
            }
            break;
        case VLP_DISCONNECT:
            //disconnect client
            if(vlp == socket) {
                socket->Close();
                vlp = NULL;
                wxLogVerbose ( _ ( "[VM::ProcessMessageVLP] Disconnected client"));
            }
        case VLP_INTERPRETER_DOWN:
            //forget interpreter
            wxLogVerbose ( _ ( "[VM::ProcessMessageVLP] Interpreter down - NOT implemented"));
            //todo close graph module?
            default:
                wxLogVerbose ( wxString::Format( _ ( "[VM::ProcessMessageVLP]VLP message type %d" ),message->param.pword[0] ) );
    }
}

void VM::ForwardToGraphModule( MESSAGE *message,wxSocketBase*socket) {
    wxIPV4address address;
    socket->GetPeer(address);
    wxSocketBase* graph_socket = this->configuration.GetGraphicalSocket(address.Service());
    graph_socket->Write(message,sizeof(MESSAGE));
    wxLogVerbose(wxString::Format("Message forwarded to GraphModule"));
}
void VM::ForwardToIntModule  ( MESSAGE *message,wxSocketBase*socket) {
    wxIPV4address address;
    socket->GetPeer(address);
    wxSocketBase* int_socket = this->configuration.GetIntSocket(address.Service());
    int_socket->Write(message,sizeof(MESSAGE));
    if( int_socket->Error() ) {
        wxLogVerbose(wxString::Format("Message forwarding to InthModule failed"));
    } else {
        wxLogVerbose(wxString::Format("Message forwarded to InthModule"));
    }
}
int VM::getNodeNumber() {
    return nodeNumber;
}
Configurations* VM::getConfiguration() {
    return &configuration;
}
wxFileName* VM::getExecutablesDir() {
    return &executablesDir;
}


void VM::OnClose(wxCloseEvent &event) {
    if (event.GetEventType() == wxEVT_CLOSE_WINDOW)
        wxMessageBox(L"got wxEVT_CLOSE_WINDOW");
    ExitMainLoop();

}

void VM::OnInitCmdLine(wxCmdLineParser& parser) {
    parser.SetDesc(g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars(wxT("-"));
}

bool VM::OnCmdLineParsed(wxCmdLineParser& parser) {
    if (parser.Found("h")) {
        return false;
    }
    this->verbose = parser.Found("v");
    return true;
}

BEGIN_EVENT_TABLE ( VM, wxAppConsole )
    EVT_SOCKET ( SERVER_EVENT_ID,  VM::OnServerEvent )
    EVT_SOCKET ( SOCKET_EVENT_ID,  VM::OnSocketEvent )
END_EVENT_TABLE()

void VM::OnSigTerm(int sig) {;
    wxLogVerbose(wxString::Format("got signal %d",sig));
    if(sig == SIGTERM || sig == SIGINT) {
        wxLogError("NOT IMPLEMENTED GRACEFUL SHUTDOWN");
        //todo properrly close all connections, send shutdown notification for all loglanint etc
        wxGetApp().Exit();
    }
}
