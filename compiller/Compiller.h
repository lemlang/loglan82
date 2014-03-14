/*
 * File:   Compiller.h
 * Author: amz
 *
 * Created on 14 marzec 2014, 00:41
 */
#define __WXDEBUG__ 1
#include <wx/app.h>
#include <wx/cmdline.h>
#include "IncludeFile.h"

class Compiller : public wxAppConsole {
public:
    bool OnInit();
    int OnExit();
    IncludeFile* findTrueLine(long line, long *trueline);
    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
private:
    IncludeFileList includeList;
    int result;
    bool noCleanUp;
    wxArrayString files;
};

static const wxCmdLineEntryDesc g_cmdLineDesc [] ={
    { wxCMD_LINE_SWITCH, "h", "help", "displays help on the command line parameters",
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP},
    { wxCMD_LINE_SWITCH, "n", "no-cleanup", "don't cleanup temporary files",
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_SWITCH},
    { wxCMD_LINE_PARAM, NULL, NULL, "input file", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM},
    { wxCMD_LINE_NONE}
};

DECLARE_APP(Compiller)