/* 
 * File:   Launcher.h
 * Author: amz
 *
 * Created on 12 marzec 2014, 00:47
 */

#ifndef LAUNCHER_H
#define	LAUNCHER_H

#include <wx/app.h>
#include <wx/socket.h>

#include "VLPMainWindow.h"
#include "../head/comm.h"


enum
{
    // id for sockets
    CLIENT_EVENT_ID = 3000,
    SOCKET_EVENT_ID
};
class VLPMainWindow;
class Launcher: public wxApp {
public:
    virtual bool OnInit();
    virtual int OnExit();
    void OnClientEvent(wxSocketEvent& event);
    void OnSocketEvent(wxSocketEvent& event);
    static void OnSigTerm(int sig);
    wxSocketClient* getSocketClient();
private:
        VLPMainWindow *mainWindow;
        wxSocketClient* client;
        DECLARE_EVENT_TABLE()

};


DECLARE_APP(Launcher)
#endif	/* LAUNCHER_H */

