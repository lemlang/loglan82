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

Configurations::Configurations() {

}

Configurations::~Configurations() {

}
/**
 *   Add local interpreter instane to index
 *
 *   @param[in]     interpreter_port port number of instance
 *   @return interprete identifier - same as interpreter_port in our implementation.
 */

int Configurations::AddLocalInstance ( short unsigned int interpreter_port, wxSocketBase* socket) {
    li.get<1>().insert(local_entry(interpreter_port,0,0,0));
    return interpreter_port;
}

void Configurations::ChangeLocalInstance ( short unsigned int interpreter_port, short unsigned int graphic_resource_port, wxSocketBase* socket ) {

    //LocalIndexByIntepreter li_byi_it = li;
    LocalIndexByIntepreter::iterator it0;
    it0 = get<local_entry::ByInterpreter>(li).find(interpreter_port);
    if(it0 == li.end() ) {
        throw "[Configurations::ChangeLocalInstance] cannot find instance, cannot update?  ";
    } else {
        // found, update
        wxLogMessage(  wxString::Format("updating instance %d %d\n",(it0)->interpreter_port,(it0)->graphic_resource_port));
        li.replace(it0,local_entry(interpreter_port,graphic_resource_port,(it0)->interpreter_socket,socket));
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
    wxLogMessage (  wxString::Format("get graphical resource %d for interpreter_port %d\n",(it0)->graphic_resource_port, interpreter_port));
    return (it0)->graphic_socket;
}
void Configurations::CloseConnections()
{

}


std::size_t hash_value(const wxIPV4address &ipv4address)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, ipv4address.IPAddress().ToStdString());
    boost::hash_combine(seed, ipv4address.Service());
    return seed;
}
