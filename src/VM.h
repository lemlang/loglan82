/*
 * File:   VM.h
 * Author: amz
 *
 * Created on 11 marzec 2014, 22:33
 */

#ifndef VM_H
#define	VM_H
class VM;

#define __WXDEBUG__ 1
#include "wx/app.h"
#include "../head/comm.h"
#include "Configurations.h"
#include "VMServerThread.h"
#include <wx/msgdlg.h>
#include <wx/snglinst.h>
#include <wx/cmdline.h>
#include <wx/socket.h>
#include <wx/hashset.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/fileconf.h>
#include "backward.hpp"
#if defined (__WIDOWS__)
#include <windows.h>
#include <signal.h>
#include <stdio.h>
#endif
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
    static void OnSigTerm(int sig);
    int getNodeNumber();
    Configurations* getConfiguration();
    wxFileName* getExecutablesDir();
    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
private:
    int nodeNumber;
    wxSingleInstanceChecker *m_checker;
    wxSocketServer *server;
    wxSocketBase *vlp;
    DECLARE_EVENT_TABLE()
    void ForwardToGraphModule ( MESSAGE*,wxSocketBase*);
    void ProcessMessageInt ( MESSAGE*, wxSocketBase*);
    void ProcessMessageGraph ( MESSAGE*, wxSocketBase*);
    void ProcessMessageNet ( MESSAGE*, wxSocketBase*);
    void ProcessMessageVLP ( MESSAGE*, wxSocketBase*);
    unsigned short port;
    Configurations configuration;
    wxFileName executablesDir;

    void ForwardToIntModule(MESSAGE *message, wxSocketBase *socket);

    bool verbose;

    void initialize_remote_connection(char string[]);

    void send_connect_info(wxSocketClient *pClient);

    void write_at_console(wxString *data);
    wxFileConfig* config;

    void send_accept_info(wxSocketBase *pClient);

    void disconnect_seq();

    void exit_sequence();
};

static const wxCmdLineEntryDesc g_cmdLineDesc [] ={
        { wxCMD_LINE_SWITCH, "h", "help", "displays help on the command line parameters",
                wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP},
        { wxCMD_LINE_SWITCH, "v", "verbose", "verbose messages output",
                wxCMD_LINE_VAL_NONE, wxCMD_LINE_SWITCH},
        { wxCMD_LINE_NONE}
};

DECLARE_APP(VM)
#endif
