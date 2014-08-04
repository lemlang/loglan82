/*
 * File:   Runner.cpp
 * Author: amz
 *
 * Created on 11 marzec 2014, 22:33
 */

#include "Runner.h"

Runner::Runner() {
    this->port = 3600;
}

bool Runner::OnInit(){
    wxLog::SetActiveTarget(new wxLogStderr);

    m_checker = new wxSingleInstanceChecker;
    if ( m_checker->IsAnotherRunning() )
    {
        wxLogError(_("Another program instance is already running, aborting."));
        delete m_checker; // OnExit() won't be called if we return false
        m_checker = NULL;
        return false;
    }
    wxIPV4address address;
    address.Hostname("localhost");
    address.Service(3600);
    server = new wxSocketServer(address);
    server->SetEventHandler(*this, SERVER_ID);
    server->SetNotify(wxSOCKET_CONNECTION_FLAG);
    server->Notify(true);
    return true;
};

int Runner::OnExit()
{
    delete m_checker;
    return 0;
}

void Runner::OnServerEvent ( wxSocketEvent& event ) {
    // Accept the new connection and get the socket pointer
    wxSocketBase* sock = server->Accept(false);

    // Tell the new socket how and where to process its events
    sock->SetEventHandler(*this, SOCKET_ID);
    sock->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
    sock->Notify(true);

    wxLogMessage(_("\nAccepted incoming connection.\n"));
}
void Runner::OnSocketEvent ( wxSocketEvent& event ) {
    wxSocketBase *sock = event.GetSocket();

    // Process the event
    switch(event.GetSocketEvent())
    {
        case wxSOCKET_INPUT:
        {
            char buf[10];

            // Read the data
            sock->Read(buf, sizeof(buf));

            wxLogMessage(wxString(_("Received from client: ")) + wxString(buf, wxConvUTF8, 10) + _("\n"));

            // Write it back
            sock->Write(buf, sizeof(buf));

            wxLogMessage(_("Wrote string back to client.\n"));

            // We are done with the socket, destroy it
            sock->Destroy();

            break;
        }
        case wxSOCKET_LOST:
        {
            sock->Destroy();
            break;
        }
    }
}

BEGIN_EVENT_TABLE(Runner, wxAppConsole)
EVT_SOCKET(SERVER_EVENT_ID,  Runner::OnServerEvent)
EVT_SOCKET(SOCKET_EVENT_ID,  Runner::OnSocketEvent)
END_EVENT_TABLE()

