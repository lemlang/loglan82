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
    localEntryLastId = 1;
}

Configurations::~Configurations() {

}
/**
 *   Add local interpreter instane to index
 *
 *   @param[in]     interpreter_port port number of instance
 *   @return interprete identifier - same as interpreter_port in our implementation.
 */

int Configurations::AddLocalInstance(bool remote, unsigned short interpreter_port, wxSocketBase* socket, wxString * filename, ctx_struct p_ctx) {
    LocalEntry *entry = new LocalEntry(localEntryLastId++, remote, interpreter_port,0,socket,0, filename, p_ctx);
    li.get<1>().insert(*entry);
    return entry->entry_id;
}

void Configurations::ChangeLocalInstance ( int entryId, unsigned  short  interpreter_port,  wxSocketBase* interpreter_socket, short unsigned int graphic_resource_port, wxSocketBase* graphics_socket ) {

    //LocalIndexByIntepreter li_byi_it = li;
    LocalIndexByEntryId::iterator it0;
    it0 = get<LocalEntry::ByEntryId>(li).find(entryId);
    if(it0 == get<LocalEntry::ByEntryId>(li).end() ) {
        throw "[Configurations::ChangeLocalInstance] cannot find instance, cannot update?  ";
    } else {
        // found, update
        wxLogMessage(  wxString::Format("updating instance %d %d\n",(it0)->interpreter_port,(it0)->graphic_resource_port));
        get<LocalEntry::ByEntryId>(li).replace(it0, LocalEntry(entryId, (it0)->remote,  interpreter_port,graphic_resource_port,interpreter_socket,graphics_socket , (it0)->filename, (it0)->p_ctx ));
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
wxSocketBase *Configurations::GetIntSocketById(int entry_id) {
    LocalIndexByEntryId::iterator it1;
    it1 = get<LocalEntry::ByEntryId>(li).find(entry_id);
    wxLogMessage (  wxString::Format("get int socket %lu for node number  %d\n",(it1)->interpreter_socket, entry_id));

    return (it1)->interpreter_socket;
}

int Configurations::AddRemoteInstance(int node_id, int entry_id, wxSocketBase *base, unsigned  short remote_id ) {
    ri.get<1>().insert(RemoteEntry(node_id,entry_id, remote_id, base, wxFalse  ));
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

    RemoteVMIndexByNodeId::iterator itr = this->GetRemoteVMIterator();
    while(itr != this->GetRemoteVMIIndexEnd()) {
        (itr)->socket->Write(&message, sizeof(MESSAGE));
        (itr)->socket->Close();
        this->RemoveRemoteVM(itr->node_id);
        itr++;
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
    LocalIndexByEntryId::iterator it1;
    it1 = get<LocalEntry::ByEntryId>(li).find(node_id);
    wxLogMessage (  wxString::Format("get int socket for node number  %d\n",node_id));
    if( it1 != get<LocalEntry::ByEntryId>(li).end() ) {
        return &(* it1);
    }
    return NULL;
}

void Configurations::RemoveInt(int entry_id) {
    LocalIndexByEntryId::iterator it  = get<LocalEntry::ByEntryId>(li).find(entry_id);
    wxLogMessage (  wxString::Format("get int %d for node number  %d\n",(it)->entry_id, entry_id));
    (it)->interpreter_socket->Close();
    get<LocalEntry::ByEntryId>(li).erase(it);
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

const LocalEntry *Configurations::GetInterpreterByInterpreter(unsigned short interpreterPort) {
    LocalIndexByIntepreter::iterator lit = get<LocalEntry::ByInterpreter>(li).find(interpreterPort);
    if ( lit  == get<LocalEntry::ByInterpreter>(li).end()) {
        wxLogMessage("Local Entry not found\n");
        return NULL;
    }
    return &(*lit);
}

const RemoteEntry *Configurations::GetRemoteInterpreterBySocket(wxSocketBase *socket) {
    RemoteIndexBySocket::iterator rit = get<RemoteEntry::BySocket>(ri).find(socket);
    if ( rit  == get<RemoteEntry::BySocket>(ri).end()) {
        wxLogMessage("Remote Entry not found\n");
        return NULL;
    }
    return &(*rit);
}

const RemoteVM *Configurations::GetRemoteVMBySocket(wxSocketBase *socket) {
    RemoteVMIndexBySocket::iterator rvmit = get<RemoteVM::BySocket>(vmi).find(socket);
    if ( rvmit == get<RemoteVM::BySocket>(vmi).end()) {
        wxLogMessage("Remote VM not found\n");
        return NULL;
    }
    return &(*rvmit);
}

const LocalEntry *Configurations::GetInterpreterByGraphics(unsigned short interpreterPort) {
    LocalIndexByGraphics::iterator lit = get<LocalEntry::ByGraphics>(li).find(interpreterPort);
    if ( lit  == get<LocalEntry::ByGraphics>(li).end()) {
        wxLogMessage("Local Entry not found\n");
        return NULL;
    }
    return &(*lit);
}
