/*
 * File:   Runner.h
 * Author: amz
 *
 * Created on 11 marzec 2014, 22:33
 */

#define	RUNNER_H

#include "wx/app.h"

#include <wx/snglinst.h>
#include <wx/socket.h>
enum
{
    // id for sockets
    SERVER_ID = 1000,
    SOCKET_ID
};

class Runner : public wxAppConsole{
public:
    Runner();
    virtual bool OnInit();
    virtual int OnExit();

    void OnServerEvent(wxSocketEvent& event);
    void OnSocketEvent(wxSocketEvent& event);
private:
    wxSingleInstanceChecker *m_checker;
    wxSocketServer *server;
    DECLARE_EVENT_TABLE()
    unsigned short port;
};

DECLARE_APP(Runner)
