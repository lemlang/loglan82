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

struct LocalEntry {
    int node_id;
    unsigned short interpreter_port;
    unsigned short graphic_resource_port;
    wxSocketBase* graphic_socket;
    wxSocketBase* interpreter_socket;
    wxString* filename;
    struct ByNodeId {};
    struct ByInterpreter {};
    struct ByGraphics{};
    LocalEntry(
        int node_id,
        unsigned short interpreter_port,
        unsigned short graphic_resource_port,
        wxSocketBase* interpreter_socket,
        wxSocketBase* graphic_socket,
        wxString* filename):
        node_id(node_id),
        interpreter_port(interpreter_port),
        graphic_resource_port(graphic_resource_port),
        interpreter_socket(interpreter_socket),
        graphic_socket(graphic_socket),
        filename(filename)
        {}

};

/* structure to store remote VM instances */
struct RemoteEntry {
    int node_id;
    unsigned short interpreter_port;
    wxSocketBase* socket;
    bool connected;
    struct ByNodeId {};
    struct ByInterpreterPort {};
    struct BySocket{};
    RemoteEntry(
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
        LocalEntry,
    indexed_by<
        hashed_unique<tag<LocalEntry::ByNodeId>, member<LocalEntry,int,&LocalEntry::node_id> >,
        hashed_unique<tag<LocalEntry::ByInterpreter>, member<LocalEntry,unsigned short,&LocalEntry::interpreter_port> >,
        hashed_unique<tag<LocalEntry::ByGraphics>, member<LocalEntry,unsigned short,&LocalEntry::graphic_resource_port> >
    >
> LocalIndex;

typedef LocalIndex::index<LocalEntry::ByNodeId>::type LocalIndexByNodeId;
typedef LocalIndex::index<LocalEntry::ByInterpreter>::type LocalIndexByIntepreter;
typedef LocalIndex::index<LocalEntry::ByGraphics>::type LocalIndexByGraphics;


using namespace boost::multi_index;
typedef multi_index_container<
        RemoteEntry,
    indexed_by<
        hashed_non_unique<tag<RemoteEntry::ByNodeId>, member<RemoteEntry,int,&RemoteEntry::node_id> >,
        hashed_non_unique<tag<RemoteEntry::ByInterpreterPort>, member<RemoteEntry,unsigned short,&RemoteEntry::interpreter_port> >,
        hashed_unique<tag<RemoteEntry::BySocket>, member<RemoteEntry,wxSocketBase*,&RemoteEntry::socket> >
    >
> RemoteIndex;


typedef RemoteIndex::index<RemoteEntry::ByNodeId>::type RemoteIndexByNodeId;
typedef RemoteIndex::index<RemoteEntry::ByInterpreterPort>::type RemoteIndexByInterpreterPort;
typedef RemoteIndex::index<RemoteEntry::BySocket>::type RemoteIndexBySocket;


class Configurations {
public:
    Configurations();
    ~Configurations();
    int AddLocalInstance(int node_id, unsigned short interpreter_port, wxSocketBase* socket, wxString* filename);
    void ChangeLocalInstance(int node_id, unsigned short interpreter_port,
                                  unsigned short graphic_resource_port,wxSocketBase*);
    unsigned short int GetGraphicalResource(unsigned short int interpreter_port);

    wxSocketBase* GetGraphicalSocket(unsigned short int interpreter_port);

    wxSocketBase *GetIntSocket(unsigned short int graphical_port);

    wxSocketBase *GetIntSocketById(int node_id);

    wxSocketBase* GetRemoteSocketById(int node_id);
    const LocalEntry*GetLocalEntry(int node_id);


    int AddRemoteInstance(int node_id, wxSocketBase *base, unsigned  short remote_id );
    //void ChangeRemoteInstance(wxSocketBase*);



    void CloseRemoteConnections(int MyNodeId);

    void CloseConnections();

    void RemoveRemote(int interpreter_port);

    void RemoveInt(int id);

    void CloseRemote(unsigned short local_port);

private:
    LocalIndex li;
    RemoteIndex ri;
    int lastIndex;

};

#endif // CONFIGURATIONS_H
