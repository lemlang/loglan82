/*
 * Virtual Loglan Processor
*  Copyright (C) 2014  Andrzej Zadrozny <azadrozny@gmail.com>
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

#include "Configurations.h"


class iterator;

Configurations::Configurations() {
    lastIndex = 1;
}

Configurations::~Configurations() {

}
/**
 *   Add local interpreter instane to index
 *
 *   @param[in]     interpreter_port port number of instance
 *   @return interprete identifier - same as interpreter_port in our implementation.
 */

int Configurations::AddLocalInstance(int node_id, unsigned short interpreter_port, wxSocketBase* socket) {
    li.get<1>().insert(local_entry(node_id, interpreter_port,0,socket,0));
    return interpreter_port;
}

void Configurations::ChangeLocalInstance ( int node_id, unsigned  short  interpreter_port, short unsigned int graphic_resource_port, wxSocketBase* socket ) {

    //LocalIndexByIntepreter li_byi_it = li;
    LocalIndexByIntepreter::iterator it0;
    it0 = get<local_entry::ByInterpreter>(li).find(interpreter_port);
    if(it0 == get<local_entry::ByInterpreter>(li).end() ) {
        throw "[Configurations::ChangeLocalInstance] cannot find instance, cannot update?  ";
    } else {
        // found, update
        wxLogMessage(  wxString::Format("updating instance %d %d\n",(it0)->interpreter_port,(it0)->graphic_resource_port));
        get<local_entry::ByInterpreter>(li).replace(it0,local_entry(node_id, interpreter_port,graphic_resource_port,(it0)->interpreter_socket,socket));
    }

}
short unsigned int Configurations::GetGraphicalResource ( short unsigned int interpreter_port ) {
    LocalIndexByIntepreter::iterator it0;
    it0 = get<local_entry::ByInterpreter>(li).find(interpreter_port);
    wxLogMessage (  wxString::Format("get graphical resource %d for interpreter_port %d\n",(it0)->graphic_resource_port, interpreter_port));
    return (it0)->graphic_resource_port;
}
wxSocketBase* Configurations::GetGraphicalSocket( short unsigned int interpreter_port ) {
    LocalIndexByIntepreter::iterator it0;
    it0 = get<local_entry::ByInterpreter>(li).find(interpreter_port);
    wxLogMessage (  wxString::Format("get Graphical Socket %d for interpreter_port %d\n",(it0)->graphic_resource_port, interpreter_port));
    return (it0)->graphic_socket;
}
wxSocketBase* Configurations::GetIntSocket( short unsigned int graphical_port ) {
    LocalIndexByGraphics::iterator it0;
    it0 = get<local_entry::ByGraphics>(li).find(graphical_port);
    wxLogMessage (  wxString::Format("get int socket %lu for graphical_port %d\n",(intptr_t)((it0)->interpreter_socket), graphical_port));
    return (it0)->interpreter_socket;
}
void Configurations::CloseConnections()
{
 wxLogError(wxS("Close connections not implemented"));
}


std::size_t hash_value(const wxIPV4address &ipv4address)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, ipv4address.IPAddress().ToStdString());
    boost::hash_combine(seed, ipv4address.Service());
    return seed;
}

wxSocketBase *Configurations::GetRemoteSocketById(int node_id) {
    RemoteIndexByNodeId::iterator it1;
    it1 = get<remote_entry::ByNodeId>(ri).find(node_id);
    wxLogMessage (  wxString::Format("get remote socket %lu for node number  %d\n",(it1)->socket, node_id));

    return (it1)->socket;
}
wxSocketBase *Configurations::GetIntSocketById(int node_id) {
    LocalIndexByNodeId::iterator it1;
    it1 = get<local_entry::ByNodeId>(li).find(node_id);
    wxLogMessage (  wxString::Format("get int socket %lu for node number  %d\n",(it1)->interpreter_socket, node_id));

    return (it1)->interpreter_socket;
}

int Configurations::AddRemoteInstance(int node_id, wxSocketBase *base, unsigned  short remote_id ) {
    ri.get<1>().insert(remote_entry(node_id, remote_id, base, wxFalse  ));
    return remote_id;
}

void Configurations::CloseRemoteConnections(int MyNodeId) {
    MESSAGE message;

    message.msg_type = MSG_NET;
    message.param.pword[0] = NET_DISCONNECT;
    message.param.pword[1] = MyNodeId;

    RemoteIndexByNodeId::iterator rit;

    rit = get<remote_entry::ByNodeId>(ri).begin();

    while (rit != get<remote_entry::ByNodeId>(ri).end() ) {
        printf("%d\n",(rit)->interpreter_port);
        (rit)->socket->Write(&message, sizeof(MESSAGE));
        (rit)->socket->Close();
        get<remote_entry::ByNodeId>(ri).erase(rit);
        ++rit;
    }
}

void Configurations::RemoveRemote(int interpreter_port) {
    RemoteIndexByInterpreterPort::iterator rit;
    rit = get<remote_entry::ByInterpreterPort>(ri).find(interpreter_port);
    while (rit != get<remote_entry::ByInterpreterPort>(ri).end() )
    {
        wxLogMessage(wxString::Format("remove remote socket %lu for node number  %d\n", (rit)->socket, interpreter_port));
        (rit)->socket->Close();
        get<remote_entry::ByInterpreterPort>(ri).erase(rit);
        ++rit;
    }
}
