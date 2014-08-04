/* 
 * File:   Launcher.h
 * Author: amz
 *
 * Created on 12 marzec 2014, 00:47
 */

#ifndef LAUNCHER_H
#define	LAUNCHER_H

#include "wx/app.h"
#include "VLPMainWindow.h"

class Launcher: public wxApp {

    virtual bool OnInit();
    virtual int OnExit();
    private:
        VLPMainWindow *mainWindow;

};

#endif	/* LAUNCHER_H */

