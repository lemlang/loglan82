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

int Configurations::AddLocalInstance(int node_id, unsigned short interpreter_port, wxSocketBase* socket, wxString * filename) {
    li.get<1>().insert(LocalEntry(node_id, interpreter_port,0,socket,0, filename));
    return interpreter_port;
}

void Configurations::ChangeLocalInstance ( int node_id, unsigned  short  interpreter_port, short unsigned int graphic_resource_port, wxSocketBase* socket ) {

    //LocalIndexByIntepreter li_byi_it = li;
    LocalIndexByIntepreter::iterator it0;
    it0 = get<LocalEntry::ByInterpreter>(li).find(interpreter_port);
    if(it0 == get<LocalEntry::ByInterpreter>(li).end() ) {
        throw "[Configurations::ChangeLocalInstance] cannot find instance, cannot update?  ";
    } else {
        // found, update
        wxLogMessage(  wxString::Format("updating instance %d %d\n",(it0)->interpreter_port,(it0)->graphic_resource_port));
        get<LocalEntry::ByInterpreter>(li).replace(it0, LocalEntry(node_id, interpreter_port,graphic_resource_port,(it0)->interpreter_socket,socket, (it0)->filename));
    }

}
short unsigned int Configurations::GetGraphicalResource ( short unsigned int interpreter_port ) {
    LocalIndexByIntepreter::iterator it0;
    it0 = get<LocalEntry::ByInterpreter>(li).find(interpreter_port);
    wxLogMessage (  wxString::Format("get graphical resource %d for interpreter_port %d\n",(it0)->graphic_resource_port, interpreter_port));
    return (it0)->graphic_resource_port;
}
wxSocketBase* Configurations::GetGraphicalSocket( short unsigned int interpreter_port ) {
    LocalIndexByIntepreter::iterator it0;
    it0 = get<LocalEntry::ByInterpreter>(li).find(interpreter_port);
    wxLogMessage (  wxString::Format("get Graphical Socket %d for interpreter_port %d\n",(it0)->graphic_resource_port, interpreter_port));
    return (it0)->graphic_socket;
}
wxSocketBase* Configurations::GetIntSocket( short unsigned int graphical_port ) {
    LocalIndexByGraphics::iterator it0;
    it0 = get<LocalEntry::ByGraphics>(li).find(graphical_port);
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
    it1 = get<RemoteEntry::ByNodeId>(ri).find(node_id);
    wxLogMessage (  wxString::Format("get remote socket %lu for node number  %d\n",(it1)->socket, node_id));
    if(it1 !=  get<RemoteEntry::ByNodeId>(ri).end()) {
        return (it1)->socket;
    } else {
        return NULL;
    }
}
wxSocketBase *Configurations::GetIntSocketById(int node_id) {
    LocalIndexByNodeId::iterator it1;
    it1 = get<LocalEntry::ByNodeId>(li).find(node_id);
    wxLogMessage (  wxString::Format("get int socket %lu for node number  %d\n",(it1)->interpreter_socket, node_id));

    return (it1)->interpreter_socket;
}

int Configurations::AddRemoteInstance(int node_id, wxSocketBase *base, unsigned  short remote_id ) {
    ri.get<1>().insert(RemoteEntry(node_id, remote_id, base, wxFalse  ));
    return remote_id;
}

void Configurations::CloseRemoteConnections(int MyNodeId) {
    MESSAGE message;

    message.msg_type = MSG_NET;
    message.param.pword[0] = NET_DISCONNECT;
    message.param.pword[1] = MyNodeId;

    RemoteIndexByNodeId::iterator rit;

    rit = get<RemoteEntry::ByNodeId>(ri).begin();

    while (rit != get<RemoteEntry::ByNodeId>(ri).end() ) {
        printf("%d\n",(rit)->interpreter_port);
        (rit)->socket->Write(&message, sizeof(MESSAGE));
        (rit)->socket->Close();
        get<RemoteEntry::ByNodeId>(ri).erase(rit);
        ++rit;
    }
}

void Configurations::RemoveRemote(int interpreter_port) {
    RemoteIndexByInterpreterPort::iterator rit;
    rit = get<RemoteEntry::ByInterpreterPort>(ri).find(interpreter_port);
    while (rit != get<RemoteEntry::ByInterpreterPort>(ri).end() )
    {
        wxLogMessage(wxString::Format("remove remote socket %lu for node number  %d\n", (rit)->socket, interpreter_port));
        (rit)->socket->Close();
        get<RemoteEntry::ByInterpreterPort>(ri).erase(rit);
        ++rit;
    }
}

const LocalEntry *Configurations::GetLocalEntry(int node_id) {
    LocalIndexByNodeId::iterator it1;
    it1 = get<LocalEntry::ByNodeId>(li).find(node_id);
    wxLogMessage (  wxString::Format("get int socket %lu for node number  %d\n",(it1)->interpreter_socket, node_id));

    return &(* it1);
}

void Configurations::RemoveInt(int node_id) {
    LocalIndexByNodeId::iterator it  = get<LocalEntry::ByNodeId>(li).find(node_id);
    wxLogMessage (  wxString::Format("get int %d for node number  %d\n",(it)->node_id, node_id));
    (it)->interpreter_socket->Close();
    get<LocalEntry::ByNodeId>(li).erase(it);
}

void Configurations::CloseRemote(unsigned short interpreter_port) {
MESSAGE msg;
            msg.msg_type = MSG_NET;
            msg.param.pword[0] = NET_PROPAGATE;
            msg.param.pword[1] = MSG_VLP;
    //todo src node
            msg.param.pword[2] = INT_MAX;
            //todo what is it?
            //msg.param.pword[4] = i;
            msg.param.pword[6] = VLP_CLOSE_INSTANCE;

    RemoteIndexByInterpreterPort::iterator rit = get<RemoteEntry::ByInterpreterPort>(ri).find(interpreter_port);
    while (rit != get<RemoteEntry::ByInterpreterPort>(ri).end() )
    {
        wxLogMessage(wxString::Format("close remote interpreter %lu for node number  %d\n", (rit)->socket, interpreter_port));
        msg.param.pword[7] = (rit)->node_id;
        (rit)->socket->Write(&msg,sizeof(MESSAGE));
        (rit)->socket->Close();
        get<RemoteEntry::ByInterpreterPort>(ri).erase(rit);
        ++rit;
    }
}

const RemoteVM* Configurations::GetRemoteVMByNodeId(int node_id) {
    RemoteVMIndexByNodeId::iterator rvmit = get<RemoteVM::ByNodeId>(vmi).find(node_id);
    if ( rvmit == get<RemoteVM::ByNodeId>(vmi).end()) {
        wxLogMessage("Remote VM not found\n");
        return NULL;
    }
    wxLogMessage(wxString::Format("number of remote nodes: %d\n", this->GetRemoteVMCount()));;
    return &(*rvmit);
}

void Configurations::AddRemoteVM(int node_id, wxSocketBase *socket) {
    get<RemoteVM::ByNodeId>(vmi).insert(RemoteVM(node_id, socket));
}

void Configurations::RemoveRemoteVM(int node_id) {

    RemoteVMIndexByNodeId::iterator rvmit = get<RemoteVM::ByNodeId>(this->vmi).find(node_id);
    get<RemoteVM::ByNodeId>(this->vmi).erase(rvmit);

}

int Configurations::GetRemoteVMCount() {
    return get<RemoteVM::ByNodeId>(this->vmi).size();
}

RemoteVMIndexByNodeId::iterator Configurations::GetRemoteVMIterator() {
    return get<RemoteVM::ByNodeId>(this->vmi).begin();
}

RemoteVMIndexByNodeId::iterator Configurations::GetRemoteVMIIndexEnd() {
    return get<RemoteVM::ByNodeId>(this->vmi).end();
}
