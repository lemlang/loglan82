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

struct local_entry {
    unsigned short interpreter_port;
    unsigned short graphic_resource_port;
    wxSocketBase* graphic_socket;
    wxSocketBase* interpreter_socket;
    struct ByInterpreter {}; struct ByGraphics{};
    local_entry(
        unsigned short interpreter_port,
        unsigned short graphic_resource_port,
        wxSocketBase* interpreter_socket,
        wxSocketBase* graphic_socket):
        interpreter_port(interpreter_port),
        graphic_resource_port(graphic_resource_port),
        interpreter_socket(interpreter_socket),
        graphic_socket(graphic_socket)
        {}

};

struct remote_entry {
    unsigned short interpreter_port;
    wxIPV4address remote_address;
    unsigned short remote_port;
    unsigned short remote_id;
    wxSocketBase* socket;
    remote_entry(
        unsigned short interpreter_port,
        unsigned short remote_id,
        wxIPV4address remote_address,
        unsigned short remote_port,
        wxSocketBase*socket):
        interpreter_port(interpreter_port),
        remote_address(remote_address),
        remote_port(remote_port),
        remote_id(remote_id),
        socket(socket)
        {}
};

using namespace boost::multi_index;
typedef boost::multi_index_container<
    local_entry,
    indexed_by<
    //non-unique as some subscribers might have more than one number
    hashed_unique<
    tag<local_entry::ByInterpreter>, member<local_entry,unsigned short,&local_entry::interpreter_port>
        >,
        hashed_unique<

        tag<local_entry::ByGraphics>, member<local_entry,unsigned short,&local_entry::graphic_resource_port>
        >
    >
> LocalIndex;

typedef LocalIndex::index<local_entry::ByInterpreter>::type LocalIndexByIntepreter;
typedef LocalIndex::index<local_entry::ByGraphics>::type LocalIndexByGraphics;


using namespace boost::multi_index;
typedef boost::multi_index_container<
remote_entry,
indexed_by<
hashed_unique<
composite_key<
remote_entry,
member<remote_entry,unsigned short,&remote_entry::interpreter_port>,
    member<remote_entry,unsigned short,&remote_entry::remote_id>
    >
>,
hashed_unique< // unique as numbers belong to only one subscriber
composite_key<
remote_entry,
member<remote_entry,wxIPV4address,&remote_entry::remote_address>,
member<remote_entry,unsigned short,&remote_entry::remote_port>
>
>
>
> RemoteIndex;

class Configurations {
public:
    Configurations();
    ~Configurations();
    int AddLocalInstance(unsigned short interpreter_port,wxSocketBase*);
    void ChangeLocalInstance(unsigned short interpreter_port,
                                  unsigned short graphic_resource_port,wxSocketBase*);
    unsigned short int GetGraphicalResource(unsigned short int interpreter_port);

    wxSocketBase* GetGraphicalSocket(unsigned short int interpreter_port);

    wxSocketBase *GetIntSocket(unsigned short int graphical_port);
    void CloseConnections();

    std::list<wxSocketBase*> vlp;

private:
    LocalIndex li;

};

#endif // CONFIGURATIONS_H
