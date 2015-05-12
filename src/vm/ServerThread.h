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

#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H


#include "wx/app.h"
#include <wx/thread.h>
#include <wx/process.h>
#include "VM.h"

class ServerThread : public wxThread {
public:
    VM*   m_pServer;  //Our server
    wxSocketBase* m_pSocket;  //The client socket for read/write ops
    ServerThread();
    ServerThread(VM* pServer, wxSocketBase* pSocket);
    ~ServerThread();
    virtual void* Entry();
private:
    void AllocateGraphResource(wxSocketBase*);

};

#endif // SERVERTHREAD_H
