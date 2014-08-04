/*
 * File:   Launcher.cpp
 * Author: amz
 *
 * Created on 12 marzec 2014, 00:47
 */

#include "Launcher.h"


bool Launcher::OnInit() {
    wxLog::SetActiveTarget(new wxLogStderr);
    mainWindow = new VLPMainWindow(wxT("Virtual Loglan Processor"));
    mainWindow->Show(true);
    return true;
}

int Launcher::OnExit() {
    return 0;
}
