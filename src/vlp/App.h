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

#include "MainWindow.h"
#include "../../head/comm.h"
#if defined (__WINDOWS__)
#include <windows.h>
#include <signal.h>
#include <stdio.h>
#endif

#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

enum
{
    // id for sockets
    CLIENT_EVENT_ID = 3000,
    SOCKET_EVENT_ID
};
class MainWindow;
class App : public wxApp {
public:
    virtual bool OnInit();
    virtual int OnExit();
    void OnClientEvent(wxSocketEvent& event);
    void OnSocketEvent(wxSocketEvent& event);
    static void OnSigTerm(int sig);
    wxSocketClient* getSocketClient();
private:
        MainWindow *mainWindow;
        wxSocketClient* client;
        DECLARE_EVENT_TABLE()

};


DECLARE_APP(App)
#endif	/* LAUNCHER_H */

