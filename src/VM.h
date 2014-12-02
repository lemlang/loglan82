/*
 * File:   VM.h
 * Author: amz
 *
 * Created on 11 marzec 2014, 22:33
 */

#ifndef VM_H
#define	VM_H
class VM;
#include "wx/app.h"
#include "../head/comm.h"
#include "Configurations.h"
#include "VMServerThread.h"
#include <wx/snglinst.h>
#include <wx/socket.h>
#include <wx/hashset.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include "backward.hpp"
enum
{
    // id for sockets
    SERVER_EVENT_ID = 1000,
    SOCKET_EVENT_ID
};

WX_DECLARE_HASH_SET( unsigned short int,
                     wxIntegerHash,
                     wxIntegerEqual,
                     PeerHashSet );


class VM : public wxAppConsole{
public:
    VM();
    virtual bool OnInit();
    virtual int OnExit();

    void OnServerEvent(wxSocketEvent& event);
    void OnSocketEvent(wxSocketEvent& event);
    void OnClose( wxCloseEvent& event );
    void static OnSigTerm(int sig);
    int getNodeNumber();
    Configurations* getConfiguration();
    wxFileName* getExecutablesDir();
private:
    int nodeNumber;
    wxSingleInstanceChecker *m_checker;
    wxSocketServer *server;
    DECLARE_EVENT_TABLE()
    void ForwardToGraphModule ( MESSAGE*,wxSocketBase*);
    void ProcessMessageInt ( MESSAGE*, wxSocketBase*);
    void ProcessMessageGraph ( MESSAGE*, wxSocketBase*);
    void ProcessMessageNet ( MESSAGE*, wxSocketBase*);
    void ProcessMessageVLP ( MESSAGE*, wxSocketBase*);
    unsigned short port;
    Configurations configuration;
    wxFileName executablesDir;
};

DECLARE_APP(VM)
#endif
