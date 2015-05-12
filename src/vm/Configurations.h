/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2014  <copyright holder> <email>
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

#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H
#include <wx/socket.h>
#include <wx/log.h>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>
#include "../../head/comm.h"

struct local_entry {
    int node_id;
    unsigned short interpreter_port;
    unsigned short graphic_resource_port;
    wxSocketBase* graphic_socket;
    wxSocketBase* interpreter_socket;
    struct ByNodeId {};
    struct ByInterpreter {};
    struct ByGraphics{};
    local_entry(
        int node_id,
        unsigned short interpreter_port,
        unsigned short graphic_resource_port,
        wxSocketBase* interpreter_socket,
        wxSocketBase* graphic_socket):
        node_id(node_id),
        interpreter_port(interpreter_port),
        graphic_resource_port(graphic_resource_port),
        interpreter_socket(interpreter_socket),
        graphic_socket(graphic_socket)
        {}

};

struct remote_entry {
    int node_id;
    unsigned short interpreter_port;
    wxSocketBase* socket;
    bool connected;
    struct ByNodeId {};
    struct ByInterpreterPort {};
    struct BySocket{};
    remote_entry(
        int node_id,
        unsigned short interpreter_port,
        wxSocketBase*socket,
        bool connected):
            node_id(node_id),
            interpreter_port(interpreter_port),
            socket(socket),
            connected(connected)
        {}
};

using namespace boost::multi_index;
typedef multi_index_container<
    local_entry,
    indexed_by<
        hashed_unique<tag<local_entry::ByNodeId>, member<local_entry,int,&local_entry::node_id> >,
        hashed_unique<tag<local_entry::ByInterpreter>, member<local_entry,unsigned short,&local_entry::interpreter_port> >,
        hashed_unique<tag<local_entry::ByGraphics>, member<local_entry,unsigned short,&local_entry::graphic_resource_port> >
    >
> LocalIndex;

typedef LocalIndex::index<local_entry::ByNodeId>::type LocalIndexByNodeId;
typedef LocalIndex::index<local_entry::ByInterpreter>::type LocalIndexByIntepreter;
typedef LocalIndex::index<local_entry::ByGraphics>::type LocalIndexByGraphics;


using namespace boost::multi_index;
typedef multi_index_container<
    remote_entry,
    indexed_by<
        hashed_non_unique<tag<remote_entry::ByNodeId>, member<remote_entry,int,&remote_entry::node_id> >,
        hashed_non_unique<tag<remote_entry::ByInterpreterPort>, member<remote_entry,unsigned short,&remote_entry::interpreter_port> >,
        hashed_unique<tag<remote_entry::BySocket>, member<remote_entry,wxSocketBase*,&remote_entry::socket> >
    >
> RemoteIndex;


typedef RemoteIndex::index<remote_entry::ByNodeId>::type RemoteIndexByNodeId;
typedef RemoteIndex::index<remote_entry::ByInterpreterPort>::type RemoteIndexByInterpreterPort;
typedef RemoteIndex::index<remote_entry::BySocket>::type RemoteIndexBySocket;


class Configurations {
public:
    Configurations();
    ~Configurations();
    int AddLocalInstance(int node_id, unsigned short interpreter_port, wxSocketBase* socket);
    void ChangeLocalInstance(int node_id, unsigned short interpreter_port,
                                  unsigned short graphic_resource_port,wxSocketBase*);
    unsigned short int GetGraphicalResource(unsigned short int interpreter_port);

    wxSocketBase* GetGraphicalSocket(unsigned short int interpreter_port);

    wxSocketBase *GetIntSocket(unsigned short int graphical_port);

    wxSocketBase *GetIntSocketById(int node_id);

    wxSocketBase* GetRemoteSocketById(int node_id);


    int AddRemoteInstance(int node_id, wxSocketBase *base, unsigned  short remote_id );
    //void ChangeRemoteInstance(wxSocketBase*);



    void CloseRemoteConnections(int MyNodeId);

    void CloseConnections();

    void RemoveRemote(int interpreter_port);

private:
    LocalIndex li;
    RemoteIndex ri;
    int lastIndex;

};

#endif // CONFIGURATIONS_H
