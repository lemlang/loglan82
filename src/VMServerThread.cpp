/*
 * Loglan VM Machine
 * Copyright (C) 2014  Andrzej Zadrożny azadrozny@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "VMServerThread.h"


void VMServerThread::AllocateGraphResource(wxSocketBase* socket) {
    wxIPV4address address;
    wxLogMessage(wxString::Format("Socket address %lu",(intptr_t)socket));
    socket->GetPeer(address);
    wxLogMessage ( wxString::Format(_( "Allocating graph resource for peer address: %s:%d" ),address.Hostname(), address.Service() ) );
    wxString wxString1 = wxString::Format("%s%s",
            this->m_pServer->getExecutablesDir()->GetFullPath(),
            wxFileName::GetPathSeparators());
    wxString graphcsCommand = wxString::Format("%svlpgr %d", wxString1, address.Service());
    wxExecute(graphcsCommand, wxEXEC_ASYNC);
    unsigned short int i = this->m_pServer->getConfiguration()->GetGraphicalResource(address.Service());
    int count = 60;
    while(i == 0 && count > 0) {
        i = m_pServer->getConfiguration()->GetGraphicalResource(address.Service());
        wxSleep(1);
        count--;
    }
    MESSAGE responseMessage;
    responseMessage.msg_type = MSG_GRAPH;
    if(i > 0 ) {
        responseMessage.param.pword[0] = GRAPH_ALLOCATED;
    } else {
        responseMessage.param.pword[0] = GRAPH_INACCESSIBLE;
    }
    wxLogMessage(wxString::Format("Write GRAPH_ALLOCATED/GRAPH_INACCESSIBLE message"));
    socket->Write(&responseMessage,sizeof(MESSAGE));
}


void* VMServerThread::Entry() {
    wxLogMessage(wxString::Format("Socket address %lu",(intptr_t)this->m_pSocket));
     AllocateGraphResource(this->m_pSocket);
     wxLogMessage(wxString::Format("thread ending"));
}
VMServerThread::VMServerThread(VM* pServer, wxSocketBase* pSocket): wxThread()
{
    m_pServer = pServer;
    m_pSocket = pSocket;
}
VMServerThread::~VMServerThread()
{

}
