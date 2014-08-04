/*
 * File:   Runner.cpp
 * Author: amz
 *
 * Created on 11 marzec 2014, 22:33
 */

#include "VM.h"

VM::VM() {
    this->port = 3600;
}

bool VM::OnInit() {
    //This call enables us to use wxSocket calls in secondary threads
    wxSocketBase::Initialize();

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

    server = new wxSocketServer ( address);
    server->SetEventHandler ( *this, SERVER_EVENT_ID );
    server->SetNotify ( wxSOCKET_CONNECTION_FLAG );
    server->Notify ( true );

    this->executablesDir = wxPathOnly( wxStandardPaths::Get().GetExecutablePath() );
    wxLogMessage( this->executablesDir.GetFullPath() );
    return true;
}

int VM::OnExit() {
    wxLogMessage ( _ ( "[VM] OnExit" ) );
    this->configuration.CloseConnections();
    this->server->Close();
    delete m_checker;
    return 0;
}

void VM::OnServerEvent ( wxSocketEvent& event ) {
    wxLogMessage ( wxString::Format ( _( "Server Event %d" ),event.GetEventType()) );

    // Accept the new connection and get the socket pointer
    wxSocketBase* sock = server->Accept ( false );

    // Tell the new socket how and where to process its events
    sock->SetEventHandler ( *this, SOCKET_EVENT_ID );
    sock->SetNotify ( wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG | wxSOCKET_OUTPUT_FLAG| wxSOCKET_CONNECTION_FLAG );
    sock->Notify ( true );

    wxLogMessage ( _ ( "\nAccepted incoming connection.\n" ) );
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
                wxLogMessage ( wxString::Format ( _ ( "ProcessMessageVLP ")));
            this->ProcessMessageVLP ( &readValue,sock );
            break;
        case MSG_NET:
            wxLogMessage ( wxString::Format ( _ ( "ProcessMessageNet ")));
            this->ProcessMessageNet ( &readValue,sock );
            break;
        case MSG_GRAPH:
            wxLogMessage ( wxString::Format ( _ ( "ProcessMessageGraph ")));
            this->ProcessMessageGraph ( &readValue,sock );
            break;
        case MSG_INT:
            wxLogMessage ( wxString::Format ( _ ( "ProcessMessageInt ")));
            this->ProcessMessageInt ( &readValue,sock );
            break;
        default:
            wxLogMessage ( wxString::Format ( _ ( "Received from client: %d\n" ), readValue.msg_type ) );
            break;
        }
        break;
    }
    case wxSOCKET_LOST: {
        wxLogMessage ( wxString( _ ( "Socket lost" )) );
        break;
    }
    case wxSOCKET_OUTPUT: {
        wxLogMessage ( wxString::Format( _ ( "wxSOCKET_OUTPUT event: %d" ),event.GetEventType()) );
        break;
    }
    default: {
        wxLogMessage ( wxString::Format( _ ( "Socket other event: %d" ),event.GetSocketEvent()) );
        break;
    }
    }
}

void VM::ProcessMessageInt ( MESSAGE*message, wxSocketBase*socket){
    MESSAGE responseMessage;
    wxLogMessage ( wxString::Format( _ ( "[VM::ProcessMessageInt]int message type %d" ),message->param.pword[0] ) );
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
        wxLogMessage(wxString::Format("Socket address %lu",(intptr_t)socket));
        VMServerThread* pThread = new VMServerThread(this, socket);
        pThread->Create();
        pThread->Run();
    } else if (message->param.pword[0]== GRAPH_ALLOCATING) {
        wxIPV4address address;
        socket->GetPeer(address);
        configuration.ChangeLocalInstance(message->param.pword[1],address.Service(),socket);
        wxLogMessage(wxString::Format("MessageGraph Processed"));
    } else {
        this->ForwardToGraphModule(message,socket);
    }
}
void VM::ProcessMessageNet ( MESSAGE* message, wxSocketBase* socket){
    wxLogMessage(wxString::Format("ProcessMessageNet %d %d",message->msg_type,socket->GetTimeout()));
}
void VM::ProcessMessageVLP ( MESSAGE* message, wxSocketBase* socket){
    wxLogMessage(wxString::Format("ProcessMessageVLP %d %d",message->msg_type,socket->GetTimeout()));
}

void VM::ForwardToGraphModule ( MESSAGE *message,wxSocketBase*socket) {
    wxIPV4address address;
    socket->GetPeer(address);
    wxSocketBase* graph_socket = this->configuration.GetGraphicalSocket(address.Service());
    graph_socket->Write(message,sizeof(MESSAGE));
    wxLogMessage(wxString::Format("Message forwarded to GraphModule"));
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


BEGIN_EVENT_TABLE ( VM, wxAppConsole )
    EVT_SOCKET ( SERVER_EVENT_ID,  VM::OnServerEvent )
    EVT_SOCKET ( SOCKET_EVENT_ID,  VM::OnSocketEvent )
END_EVENT_TABLE()

