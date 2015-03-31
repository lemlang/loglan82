/*
 * File:   Launcher.h
 * Author: amz
 *
 * Created on 12 marzec 2014, 00:47
 */

#ifndef GRAPHICS_H
#define	GRAPHICS_H

#include <wx/socket.h>
#include <wx/cmdline.h>

#include "wx/app.h"
#include "../head/comm.h"
#include "GraphicsWindow.h"
#include "backward.hpp"

enum
{
    // id for sockets
    CLIENT_EVENT_ID = 2000,
    SOCKET_EVENT_ID
};
class GraphicsWindow;
class Graphics: public wxApp {
public:
    virtual bool OnInit();
    virtual int OnExit();

    void OnClientEvent(wxSocketEvent& event);
    void OnSocketEvent(wxSocketEvent& event);
    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
    wxSocketClient* getSocketClient();
private:
        wxSocketClient* client;
        GraphicsWindow* window;
        unsigned short int interpreter_identifier;
    void InkeyRespond();
    void ReadlnRespond();
    void ReadCharRespond();

        DECLARE_EVENT_TABLE()
};

static const wxCmdLineEntryDesc g_cmdLineDesc [] ={
    { wxCMD_LINE_SWITCH, "h", "help", "displays help on the command line parameters",
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP},
            { wxCMD_LINE_PARAM, NULL, NULL, "local node identifier", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM},
            { wxCMD_LINE_NONE}
};

DECLARE_APP(Graphics)

#endif	/* GRAPHICS_H */
