/*
 * File:   VM.cpp
 * Author: amz
 *
 * Created on 11 marzec 2014, 22:33
 */


#include "VM.h"

VM::VM() {
    this->vlp = NULL;
    this->config = new wxFileConfig(wxT("VLP"));
    wxMilliClock_t now = wxGetLocalTimeMillis();
    temporaryDirectory = wxFileName::GetTempDir().Append(wxString::Format(wxT("%svlpvm_%lu"), wxFileName::GetPathSeparators(), now));
    wxFileName::Mkdir(temporaryDirectory.GetFullPath());
}

bool VM::OnInit() {
    if (!wxAppConsole::OnInit()) {
        return false;
    }
    if (this->verbose) {
        wxLog::SetVerbose();
    }
    //This call enables us to use wxSocket calls in secondary threads
    wxSocketBase::Initialize();
    Connect(wxID_ANY, wxEVT_END_SESSION, (wxObjectEventFunction) &VM::OnClose, NULL, this);
    //void (VM::*OnSigTerm)(int) = &::add;

    #if defined (__LINUX__)
    if(SetSignalHandler(SIGINT,&VM::OnSigTerm)|| SetSignalHandler(SIGTERM,&VM::OnSigTerm) ) {
        wxLogVerbose( _( "Successfully handler installed." ) );
    } else {
        wxLogError ( _( "Failed to install handler." ) );
    }
	#endif

    wxLog::SetActiveTarget(new wxLogStderr);

    m_checker = new wxSingleInstanceChecker;
    if (m_checker->IsAnotherRunning()) {
        wxLogError(_("Another program instance is already running, aborting."));
        delete m_checker; // OnExit() won't be called if we return false
        m_checker = NULL;
        return false;
    }

    this->config->Read("NodeNumber", &this->nodeNumber, 0);

    wxIPV4address address;
    address.Hostname("0.0.0.0");
    address.Service(VM_PORT);

    server = new wxSocketServer(address, wxSOCKET_REUSEADDR);
    server->SetEventHandler(*this, SERVER_EVENT_ID);
    server->SetNotify(wxSOCKET_CONNECTION_FLAG);
    server->Notify(true);

    this->executablesDir = wxPathOnly(wxStandardPaths::Get().GetExecutablePath());
    wxLogVerbose(this->executablesDir.GetFullPath());
    return true;
}

int VM::OnExit() {
    wxLogVerbose(_ ("[VM] OnExit"));

    disconnect_seq();

    this->configuration.CloseConnections();
    this->server->Close();
    this->config->Write("NodeNumber", wxString::Format(_T("%d"), this->nodeNumber));
    this->config->Flush();
    delete m_checker;
    wxLogVerbose(wxT("Remove directory"));
    wxLogVerbose(temporaryDirectory.GetFullPath());
    wxFileName::Rmdir(temporaryDirectory.GetFullPath(), wxPATH_RMDIR_RECURSIVE);
    return 0;
}

void VM::OnServerEvent(wxSocketEvent &event) {
    wxLogVerbose(wxString::Format(_("Server Event %d"), event.GetEventType()));

    // Accept the new connection and get the socket pointer
    wxSocketBase *sock = server->Accept(false);

    // Tell the new socket how and where to process its events
    sock->SetEventHandler(*this, SOCKET_EVENT_ID);
    sock->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG | wxSOCKET_OUTPUT_FLAG | wxSOCKET_CONNECTION_FLAG);
    sock->Notify(true);

    wxLogVerbose(_ ("\nAccepted incoming connection.\n"));
}

void VM::OnSocketEvent(wxSocketEvent &event) {
    wxSocketBase *sock = event.GetSocket();

    // Process the event
    switch (event.GetSocketEvent()) {
        case wxSOCKET_INPUT: {
            MESSAGE readValue;
            // Read the data
            sock->Read(&readValue, sizeof(MESSAGE));

            switch (readValue.msg_type) {
                case MSG_VLP:
                    wxLogVerbose(wxString::Format(_ ("[VM::ProcessMessageVLP]  %lu"), (intptr_t) sock));
                    this->ProcessMessageVLP(&readValue, sock);
                    break;
                case MSG_NET:
                    wxLogVerbose(wxString::Format(_ ("[VM::ProcessMessageNet]  %lu"), (intptr_t) sock));
                    this->ProcessMessageNet(&readValue, sock);
                    break;
                case MSG_GRAPH:
                    wxLogVerbose(wxString::Format(_ ("[VM::ProcessMessageGraph] %lu"), (intptr_t) sock));
                    this->ProcessMessageGraph(&readValue, sock);
                    break;
                case MSG_INT:
                    wxLogVerbose(wxString::Format(_ ("[VM::ProcessMessageInt]  %lu"), (intptr_t) sock));
                    this->ProcessMessageInt(&readValue, sock);
                    break;
                default:
                    wxLogVerbose(wxString::Format(_ ("[VM::Received from client] %d"), readValue.msg_type));
                    break;
            }
            break;
        }
        case wxSOCKET_LOST: {
            wxLogVerbose(wxString::Format(_ ("[VM::Socket lost] %lu"), (intptr_t) sock));
            break;
        }
        case wxSOCKET_OUTPUT: {
            wxLogVerbose(wxString::Format(_ ("wxSOCKET_OUTPUT event: %d"), event.GetEventType()));
            break;
        }
        default: {
            wxLogVerbose(wxString::Format(_ ("Socket other event: %d"), event.GetSocketEvent()));
            break;
        }
    }
}

void VM::ProcessMessageInt(MESSAGE *message, wxSocketBase *socket) {
    MESSAGE responseMessage;
    wxLogVerbose(wxString::Format(_ ("[VM::ProcessMessageInt]int message type %d"), message->param.pword[0]));

    wxIPV4address address;
    socket->GetPeer(address);
    wxString log_message;
    switch (message->param.pword[0]) {
        case INT_EXITING:
            //check if from local int module
            socket->Close();
            this->configuration.CloseRemote(address.Service());
            log_message = (wxString::Format(_("%s : End of program execution"), message->param.pstr));
            WriteAtConsole(&log_message);
            break;
        case INT_CTX_REQ:

            responseMessage.param.pword[2] = configuration.AddLocalInstance(this->nodeNumber, address.Service(),
                                                            socket, new wxString( message->param.pstr) );
            responseMessage.msg_type = MSG_INT;
            responseMessage.param.pword[0] = INT_CTX;
            responseMessage.param.pword[1] = this->nodeNumber;
            /** if (e->remote)
            {
                msg.param.pword[3] = e->p_ctx.node;
                msg.param.pword[4] = e->p_ctx.program_id;
            }*/
            socket->Write(&responseMessage, sizeof(MESSAGE));
            //socket->Close();
            //socket->Destroy();
            break;
    }
}

void VM::ProcessMessageGraph(MESSAGE *message, wxSocketBase *socket) {
    if (message->param.pword[0] == GRAPH_ALLOCATE) {
        wxLogVerbose(wxString::Format("[VM::Socket address] %lu", (intptr_t) socket));
        ServerThread *pThread = new ServerThread(this, socket);
        pThread->Create();
        pThread->Run();
    } else if (message->param.pword[0] == GRAPH_ALLOCATING) {
        wxIPV4address address;
        socket->GetPeer(address);
        configuration.ChangeLocalInstance(this->nodeNumber, message->param.pword[1], address.Service(), socket);
        wxLogVerbose(wxString::Format("[VM::MessageGraph Processed] %lu", (intptr_t) socket));
    } else if (message->param.pword[0] == GRAPH_INKEY_RESPONSE || message->param.pword[0] == GRAPH_READLN_RESPONSE ||
               message->param.pword[0] == GRAPH_READCHAR_RESPONSE ||
               message->param.pword[0] == GRAPH_READSTR_RESPONSE) {
        this->ForwardToIntModule(message, socket);
    } else {
        this->ForwardToGraphModule(message, socket);
    }
}

void VM::ProcessMessageNet(MESSAGE *message, wxSocketBase *socket) {
    wxIPV4address address;
    socket->GetPeer(address);
    wxLogVerbose(wxString::Format("ProcessMessageNet %d %s", message->msg_type, address.IPAddress()));
    wxString log_message;
    switch (message->param.pword[0]) {
        case NET_GET_INFO:
            this->ConnectionInfo(socket);
            break;
        case NET_NODES_NUM:
            message->param.pword[0]=NET_NODES_NUM_RESPONSE;
            message->param.pword[1]=this->configuration.GetRemoteVMCount();
            socket->Write ( message, sizeof ( MESSAGE ) );
            break;
        case NET_CONNECT_TO:
            this->initialize_remote_connection(message->param.pstr);
            break;
        case NET_CONNECT:
            configuration.AddRemoteInstance(this->nodeNumber, socket, message->param.pword[1]);
            log_message = (wxString::Format("New remote connection %d %s", message->param.pword[1],
                                            address.IPAddress()));
            WriteAtConsole(&log_message);
            wxLogVerbose(log_message);
            this->send_accept_info(socket);
            break;
        case NET_ACCEPT:
            configuration.AddRemoteInstance(this->nodeNumber, socket, message->param.pword[1]);
            log_message = (wxString::Format("New remote connection %d %s", message->param.pword[1],
                                            address.IPAddress()));
            WriteAtConsole(&log_message);
            wxLogVerbose(log_message);
            break;
        case NET_DISCONNECT:
            log_message = (wxString::Format("Node: %d (%s) disconnected", message->param.pword[1],
                                            address.IPAddress()));
            this->configuration.RemoveRemote(message->param.pword[1]);
            WriteAtConsole(&log_message);
            wxLogVerbose(log_message);
            break;
        case NET_CCD_START:
            socket->Notify(false);
            this->ReciveFile(socket,message->param.pstr,"ccd", message->param.pword[1]);
            socket->Notify(true);
            break;
        case NET_PCD_START:
            socket->Notify(false);
            this->ReciveFile(socket,message->param.pstr,"pcd", message->param.pword[1]);
            socket->Notify(true);
            break;
        case NET_NODE_EXIST:
            this->CheckNode( message->param.pword[1], socket);
            break;
        case NET_PROPAGATE:
            wxSocketBase *rsocket = this->configuration.GetRemoteSocketById(message->param.pword[4]);
            if (rsocket != NULL) {
                rsocket->Write(message, sizeof(message));
                log_message = (wxString::Format("Net propagate to node: %d", message->param.pword[4]));
            } else {
                /* maybe we are the target? */
                if (nodeNumber == message->param.pword[4]) {
                    if (message->param.pword[1] == MSG_VLP) {
                        switch(message->param.pword[6]) {
                            case VLP_CLOSE_INSTANCE:
                                rsocket =this->configuration.GetIntSocketById(message->param.pword[7]);
                                if( socket != NULL) {
                                    MESSAGE m1;
                                    m1.msg_type = MSG_INT;
                                    m1.param.pword[0] = INT_CLOSE_INSTANCE;
                                    socket->Write(&m1,sizeof(MESSAGE));
                                    this->configuration.RemoveInt(message->param.pword[7]);
                                }


                                break;
                            default:
                                SendToVlp(message);
                                break;
                        }
                    } else if (message->param.pword[1] == MSG_INT) SendToInt(message);
                    else {
                        /* todo what to do now? */
                    }
                } else {
                    log_message = (wxString::Format("Net propagate failed, canot find node: %d",
                                                    message->param.pword[4]));
                }
            }
            wxLogVerbose(log_message);
            break;
    }
}

void VM::ProcessMessageVLP(MESSAGE *message, wxSocketBase *socket) {
    MESSAGE responseMessage;

    switch (message->param.pword[0]) {
        case NET_DISCONNECT:

            disconnect_seq();
            break;
        case VLP_CONNECT:
            if (vlp != NULL && vlp->IsConnected()) {
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
            if (vlp == socket) {
                socket->Close();
                vlp = NULL;
                wxLogVerbose(_ ("[VM::ProcessMessageVLP] Disconnected client"));
            }
            break;
        case VLP_INTERPRETER_DOWN:
            //forget interpreter
            wxLogVerbose(_ ("[VM::ProcessMessageVLP] Interpreter down - NOT implemented"));
            //todo close graph module?
            break;
        case VLP_REMOTE_INSTANCE: {
            wxString txtMessage(wxString::Format("Running remote program: %s", message->param.pstr));
            this->WriteAtConsole(&txtMessage);
            this->RunRemoteInt(new wxString(message->param.pstr));
            //todo copy parameters
            /*this->configuration.AddRemoteInstance();

                pom->p_ctx.node = message->param.pword[2];
                pom->p_ctx.program_id = message->param.pword[7];
                pom->RInstances[message->param.pword[2]] = message->param.pword[7];*/
            break;
        }
        case VLP_REMOTE_INSTANCE_PLEASE:
            this->AllocateRemoteInstance(this->nodeNumber, message->param.pword[2]);
            break;
        case VLP_REMOTE_INSTANCE_OK:
            //todo
            message->param.pword[2];
            wxLogVerbose(_("[VM::ProcessMessageVLP]VLP message type VLP_REMOTE_INSTANCE_OK"));

            responseMessage.msg_type = MSG_VLP;
            responseMessage.param.pword[0] = VLP_REMOTE_INSTANCE_HERE;
            responseMessage.param.pword[8] = message->param.pword[8];
            this->configuration.GetIntSocketById(this->nodeNumber)->Write(&responseMessage,sizeof(MESSAGE));
        default:
            wxLogVerbose(wxString::Format(_ ("[VM::ProcessMessageVLP]VLP message type %d"), message->param.pword[0]));
    }
}

void VM::ForwardToGraphModule(MESSAGE *message, wxSocketBase *socket) {
    wxIPV4address address;
    socket->GetPeer(address);
    wxSocketBase *graph_socket = this->configuration.GetGraphicalSocket(address.Service());
    graph_socket->Write(message, sizeof(MESSAGE));
    wxLogVerbose(wxString::Format("Message forwarded to GraphModule"));
}

void VM::ForwardToIntModule(MESSAGE *message, wxSocketBase *socket) {
    wxIPV4address address;
    socket->GetPeer(address);
    wxSocketBase *int_socket = this->configuration.GetIntSocket(address.Service());
    int_socket->Write(message, sizeof(MESSAGE));
    if (int_socket->Error()) {
        wxLogVerbose(wxString::Format("Message forwarding to InthModule failed"));
    } else {
        wxLogVerbose(wxString::Format("Message forwarded to InthModule"));
    }
}

int VM::getNodeNumber() {
    return nodeNumber;
}

Configurations *VM::getConfiguration() {
    return &configuration;
}

wxFileName *VM::getExecutablesDir() {
    return &executablesDir;
}


void VM::OnClose(wxCloseEvent &event) {
    if (event.GetEventType() == wxEVT_CLOSE_WINDOW)
        wxMessageBox(L"got wxEVT_CLOSE_WINDOW");
    ExitMainLoop();

}

void VM::OnInitCmdLine(wxCmdLineParser &parser) {
    parser.SetDesc(g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars(wxT("-"));
}

bool VM::OnCmdLineParsed(wxCmdLineParser &parser) {
    if (parser.Found("h")) {
        return false;
    }
    this->verbose = parser.Found("v");
    return true;
}

BEGIN_EVENT_TABLE (VM, wxAppConsole)
                EVT_SOCKET (SERVER_EVENT_ID, VM::OnServerEvent)
                EVT_SOCKET (SOCKET_EVENT_ID, VM::OnSocketEvent)
END_EVENT_TABLE()

void VM::OnSigTerm(int sig) {
    ;
    wxLogVerbose(wxString::Format("got signal %d", sig));
    if (sig == SIGTERM || sig == SIGINT) {
        wxLogError("NOT IMPLEMENTED GRACEFUL SHUTDOWN");
        //todo properrly close all connections, send shutdown notification for all loglanint etc
        wxGetApp().Exit();
    }
}

void VM::initialize_remote_connection(char string[]) {
    wxSocketClient *client;
    client = new wxSocketClient();;
    client->SetEventHandler(*this, SOCKET_EVENT_ID);
    client->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
    client->Notify(true);

    wxIPV4address address;
    address.Hostname(string);
    address.Service(VM_PORT);
    if (client->Connect(address, true) == false) {
        wxLogError(_("Remote not responding."));
        wxString s(wxT("Remote not responding"));
        WriteAtConsole(&s);
    } else {
        this->send_connect_info(client);
    }
}

void VM::send_connect_info(wxSocketClient *pClient) {
    MESSAGE message;
    message.param.pword[0] = NET_CONNECT;
    message.param.pword[1] = this->nodeNumber;
    message.msg_type = MSG_NET;
    if (pClient->IsConnected()) {
        pClient->Write(&message, sizeof(message));
        wxLogError(_("Send init message."));
    } else {

        wxLogError(_("Remote not connected."));
        wxString s(wxT("Connection failed"));
        WriteAtConsole(&s);
    }

}

void VM::WriteAtConsole(wxString *data) {
    if (this->vlp != NULL && this->vlp->IsConnected()) {
        MESSAGE message;

        message.msg_type = MSG_NET;
        message.param.pword[0] = NET_CSWRITELN;
        strncpy(message.param.pstr, (const char *) data->mb_str(wxConvUTF8), 254);
        this->vlp->Write(&message, sizeof(message));
    } else {
        wxLog(data);
    }
}

void VM::send_accept_info(wxSocketBase *pClient) {
    MESSAGE message;
    message.param.pword[0] = NET_ACCEPT;
    message.param.pword[1] = this->nodeNumber;
    message.msg_type = MSG_NET;
    if (pClient->IsConnected()) {
        pClient->Write(&message, sizeof(message));
        wxLogError(_("Send net accept message."));
    } else {

        wxLogError(_("Remote not connected."));
        wxString s(wxT("Connection failed"));
        WriteAtConsole(&s);
    }
}

void VM::disconnect_seq() {

    wxLogError(_("Disconnect sequence."));
    this->configuration.CloseRemoteConnections(this->nodeNumber);
}

void VM::exit_sequence() {

    wxLogError(_("Exit sequence."));
}

void VM::SendToVlp(MESSAGE *message) {
    if (this->vlp != NULL && this->vlp->IsConnected()) {
        this->vlp->Write(message, sizeof(message));
    } else {
        /* todo what to do */
    }
}

void VM::SendToInt(MESSAGE *message) {
    this->configuration.GetIntSocket(message->param.pword[5]);
}

void VM::AllocateRemoteInstance(int localNodeNumber, int remoteNodeNumber) {
    MESSAGE msg;
    char s[255];
    wxSocketBase * socketBase = this->configuration.GetRemoteSocketById(remoteNodeNumber);
    if( socketBase != NULL) {
        this->TransmitFiles(socketBase, remoteNodeNumber, this->configuration.GetLocalEntry(localNodeNumber)->filename,
                            localNodeNumber);
        msg.msg_type = MSG_VLP;
        msg.param.pword[0] = VLP_REMOTE_INSTANCE;
        msg.param.pword[2] = localNodeNumber;
        msg.param.pword[4] = remoteNodeNumber;
        msg.param.pword[7] = localNodeNumber;
        strcpy(msg.param.pstr,this->configuration.GetLocalEntry(localNodeNumber)->filename->mb_str());
        socketBase->Write(&msg,sizeof(MESSAGE));
    } else {
        //There is no such a node!
        wxString s(wxString::Format("Warning: Node number %d not found!",remoteNodeNumber));
        wxLogError(s);
        WriteAtConsole(&s);
        wxLogError(_("Allocating O-process on the local node"));
        msg.msg_type = MSG_VLP;
        msg.param.pword[0] = VLP_REMOTE_INSTANCE_HERE;
        msg.param.pword[1] = localNodeNumber;
        socketBase = this->configuration.GetIntSocketById(localNodeNumber);
        socketBase->Write(&msg,sizeof(MESSAGE));
    }

}

void VM::TransmitFiles(wxSocketBase*socket, int remoteNodeNumber, wxString *filename, int localNodeNumber) {

    MESSAGE msg;
    wxLogVerbose(wxT("Sending ccd file: %s"),filename);
    wxString ccdFileName = wxString::Format("%s.ccd", filename);
    msg.param.pword[1] = wxFileName::GetSize(ccdFileName).GetValue();
    msg.param.pword[0] = NET_CCD_START;
    strcpy(msg.param.pstr,filename->c_str());
    socket->Write(&msg, sizeof(MESSAGE));
    this->SendFile(ccdFileName, socket);
    wxLogVerbose(wxT("Sending pcd file: %s"),filename);
    wxString pcdFileName = wxString::Format("%s.pcd", filename);
    msg.param.pword[1] = wxFileName::GetSize(pcdFileName ).GetValue();
    msg.param.pword[0] = NET_PCD_START;
    strcpy(msg.param.pstr,filename->c_str());
    socket->Write(&msg, sizeof(MESSAGE));
    this->SendFile(pcdFileName, socket);
}

bool VM::SendFile(const wxString& fileName, wxSocketBase* socket)
{
    wxASSERT( socket );
    if ( !socket->IsOk() )
        return false; // should also report an error

    wxFFile inFile;
    if ( !inFile.Open(fileName, wxS("rb")) )
        return false;

    wxFileOffset bytesLeft = inFile.Length();
    char buffer[g_fileTransferChunkSize];
    size_t chunkSize = sizeof(buffer);

    // code for sending the file name and size should be added here

    while ( bytesLeft > 0 && !inFile.Error() && !socket->Error() )
    {
        if ( chunkSize > bytesLeft )
            chunkSize = bytesLeft;
        if ( inFile.Read(&buffer, chunkSize) != chunkSize )
            break; // error
        if ( socket->WriteMsg(&buffer, chunkSize).LastCount() != chunkSize )
            break; // error
        bytesLeft -= chunkSize;
    }

    if ( bytesLeft > 0 || inFile.Error() || socket->Error() )
    {
        return false; // should also report an error
    }
    return true;
}

void VM::RunRemoteInt(wxString *filename) {
    wxString wxString1 = wxString::Format("%s%s",
                                          executablesDir.GetFullPath(),
                                          wxFileName::GetPathSeparators());

    wxString graphcsCommand = wxString::Format("%sloglanint %s", wxString1, filename);
    wxExecute(graphcsCommand, wxEXEC_ASYNC);
}

bool VM::ReciveFile(wxSocketBase *socket, char filename[], const char *filetype, int fileSize) {
    wxString fileName = temporaryDirectory.GetFullPath().Append(wxString::Format(wxT("%s%s.%s"), wxFileName::GetPathSeparators(), filename, filetype));
    wxASSERT( socket );
    if ( !socket->IsOk() )
        return false; // should also report an error

    wxFFile outFile;
    if ( !outFile.Open(fileName, wxS("wb")) )
    {
        // it would be probably wise to notify the sender
        // it needn't bother to send the file any more
        return false;
    }

    wxFileOffset bytesLeft = fileSize;
    char buffer[g_fileTransferChunkSize];
    size_t chunkSize = sizeof(buffer);

    while ( bytesLeft > 0 && !outFile.Error() && !socket->Error() )
    {
        if ( chunkSize > bytesLeft )
            chunkSize = bytesLeft;
        if ( socket->ReadMsg(&buffer, chunkSize).LastCount() != chunkSize )
            break; // error
        if ( outFile.Write(&buffer, chunkSize) != chunkSize )
            break; // error
        bytesLeft -= chunkSize;
    }

    if ( bytesLeft > 0 || outFile.Error() || socket->Error() )
    {
        return false; // should also report an error and remove the incomplete received file
    }
    return true;
}

void VM::CheckNode(int nodeId, wxSocketBase *socket) {
    MESSAGE m;

    m.msg_type = MSG_NET;
    m.param.pword[0] = NET_NODE_EXIST;
    const RemoteVM* remoteVM = this->configuration.GetRemoteVMByNodeId(nodeId);
    if( remoteVM == NULL ) {
    m.param.pword[1] = 0;
    } else {
        m.param.pword[1] = 1;
        wxIPV4address address;
        remoteVM->socket->GetPeer(address);
        strcpy ( m.param.pstr,address.IPAddress().c_str() );
    }
    socket->Write ( &m, sizeof ( MESSAGE ) );
}

void VM::ConnectionInfo(wxSocketBase *socket) {
    MESSAGE m;
    int k = 0;
    m.msg_type = MSG_NET;
    m.param.pword[0] = NET_INFO;
    strcpy ( m.param.pstr,"" );
    RemoteVMIndexByNodeId::iterator rit =  this->configuration.GetRemoteVMIterator();
    while (rit != this->configuration.GetRemoteVMIIndexEnd() ) {

        wxIPV4address address;
        (rit)->socket->GetPeer(address);

        strcat ( m.param.pstr, wxString::Format("%d=%s;",(rit)->node_id, address.IPAddress()).c_str());
        k++;
        if ( k==12 ) {
            m.param.pword[1]=12;
            socket->Write ( &m,sizeof ( MESSAGE ) );
            k=0;
            strcpy ( m.param.pstr,"" );
        }
    }
    if(k >0) {
        m.param.pword[1]=k;
        socket->Write ( &m,sizeof ( MESSAGE ) );
    }

    m.param.pword[0] = NET_INFO_END;
    socket->Write ( &m,sizeof( MESSAGE ) );

}
