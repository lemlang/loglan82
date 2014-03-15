/*
 * File:   Compiller.cpp
 * Author: amz
 *
 * Created on 14 marzec 2014, 00:41
 */

#include "Compiller.h"
#include "wx/filename.h"
#include <wx/list.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/stdpaths.h>

void Compiller::OnInitCmdLine(wxCmdLineParser& parser) {
    parser.SetDesc(g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths

    parser.SetSwitchChars(wxT("-"));
}

bool Compiller::OnCmdLineParsed(wxCmdLineParser& parser) {
    if (parser.Found("h")) {
        return false;
    }
    this->noCleanUp = parser.Found("n");

    for (int i = 0; i < parser.GetParamCount(); i++) {
        files.Add(parser.GetParam(i));
    }
    if (files.GetCount() == 1) {
        return true;
    } else {
        parser.Usage();
        return false;
    }

}

bool Compiller::OnInit() {
	delete wxLog::SetActiveTarget(new wxLogStderr(NULL));
    this->result = 0;
    if (!wxAppConsole::OnInit()) {
        this->result = 1;
        return false;
    }
    size_t t;
    wxString systemCommand;
    wxString poms, poms1, poms2;
    wxFileName file(this->files.Last());
    wxString filename = file.GetFullPath();
    file.SetExt("lcd");
    wxString filenameLcd = file.GetFullPath();
    file.SetExt("pcd");
    wxString filenamePcd = file.GetFullPath();


    wxString filenameNoExt = wxFileName::StripExtension(filename);
    wxFileName cwd(wxGetCwd());
    wxFileName executablesDir(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()));

    wxFileName temporaryDirectory(wxFileName::CreateTempFileName("loglan"));
    wxLogMessage("cwd: %s executable dir: %s filename: %s filename without ext: %s tempfilename: %s",
            cwd.GetFullPath(), executablesDir.GetFullPath(), filename, filenameNoExt, temporaryDirectory.GetFullPath());
	temporaryDirectory.SetExt("");
    wxString temporaryNoEXt = temporaryDirectory.GetFullPath();
    temporaryDirectory.SetExt("log");
    wxString temporaryLog = temporaryDirectory.GetFullPath();
    temporaryDirectory.SetExt("lcd");
    wxString temporaryLcd = temporaryDirectory.GetFullPath();
    temporaryDirectory.SetExt("err");
    wxString temporaryErr = temporaryDirectory.GetFullPath();
    temporaryDirectory.SetExt("pcd");
    wxString temporaryPcd = temporaryDirectory.GetFullPath();

    wxString fileToInclude;
    wxString fileNameToInclude;
    int currentline = 1;
    int i, j;
    long line, tline;


    includeList.clear();

    wxFFileOutputStream compfile(temporaryLog, "w");
    wxFileInputStream srcfile(filename);

    if (!compfile.IsOk()) {
        wxLogError(_("Cannot open temp file to write %s"), temporaryLog);
        this->result = 1;
        return false;
    }

    if (!srcfile.IsOk()) {
        wxLogError(_("Cannot open file %s"), filename);
        this->result = 1;
        return false;
    }

    wxTextOutputStream comps(compfile);
    wxTextInputStream src(srcfile);

    wxString readedLine;
    while (!srcfile.Eof()) {
        readedLine = src.ReadLine();
        i = readedLine.find("#include");
        if (i != -1) { // get include file
            i = readedLine.find('"');
            if (i != -1) {
                j = readedLine.find('"', i + 1);
            }
            if ((i != -1) && (j != -1)) {
                IncludeFile *includeFile;
                fileNameToInclude = readedLine.Mid(i + 1, j - i - 1);
                includeFile = new IncludeFile(fileNameToInclude.ToAscii(), currentline);
                fileToInclude.sprintf("%s%s%s", file.GetPath(wxPATH_GET_VOLUME & wxPATH_GET_SEPARATOR),
                        wxFileName::GetPathSeparators(), fileToInclude);
                wxFFileInputStream fileToIncludeStream(fileToInclude.ToAscii());
                if (!fileToIncludeStream.IsOk()) {
                    wxLogError(_("Cannot open include file: %s"), fileToInclude.ToAscii());
                    this->result = 1;
                    return false;
                }

                wxTextInputStream fileToIncludeTextStream(fileToIncludeStream);
                wxString readedLineOfInclude;
                while (!fileToIncludeStream.Eof()) {
                    readedLineOfInclude = fileToIncludeTextStream.ReadLine();
                    comps << readedLineOfInclude;
                    comps << "\n";
                    includeFile->len++;
                    currentline++;
                }
                includeList.Append(includeFile);
            }
        } else {
            comps << readedLine.ToAscii();
            comps << "\n";
        }
        currentline++;
    }// eof

    compfile.Close();

    systemCommand.sprintf("%s%cloglan %s > %s",
            executablesDir.GetFullPath(),
            wxFileName::GetPathSeparator(),
            temporaryLog, temporaryErr);
    wxLogMessage(_("System command: %s"),systemCommand);
    if (wxExecute(systemCommand, wxEXEC_SYNC) == -1) {
        wxLogError(_("Cannot execute pass1"));
        this->result = 1;
        return false;
    }

    if (wxFile::Exists(temporaryLcd.ToAscii())) {
        wxLogMessage(_("Compile ok"));
        systemCommand.sprintf("%s%cloglangen %s",
                executablesDir.GetFullPath(),
                wxFileName::GetPathSeparator(),
                temporaryNoEXt);
        wxLogMessage(_("System command: %s"),systemCommand);
        if (wxExecute(systemCommand, wxEXEC_SYNC) == -1) {
            wxLogError(_("Cannot execute pass2"));
            this->result = 1;
            return false;
        }
        wxCopyFile(temporaryLcd, filenameLcd);
        wxCopyFile(temporaryPcd, filenamePcd);
        //todo remote temporary files
    } else {

        wxLogError(_("File doesn't exist: %s"), temporaryLcd);
        this->result = 1;

        wxFFileInputStream err(temporaryErr.ToAscii());
        if (!err.IsOk()) {
            wxLogError(_("Cannot open error file %s"), temporaryErr);
            this->result = 1;
            return false;
        }
        wxTextInputStream errstream(err);
        IncludeFile *fl;
        wxString readedLine;
        errstream >> readedLine;
        i = readedLine.find("LOGLAN-82");
        while ((!err.Eof()) && (i == -1)) {
            errstream >> readedLine;
            i = poms.find("LOGLAN-82");
        }

        while (!err.Eof()) {
            //Found LOGLAN-82 header, and err not eof
            errstream >> readedLine;
            i = readedLine.find("ERROR");
            if (i != -1) {
                //Found error message
                i = i - 2;
                j = readedLine.rfind(' ', i);
                wxString errroCode = readedLine.Mid(j + 1, i - j);
                //todo check conversion
                errroCode.ToLong(&line);
                fl = this->findTrueLine(line, &tline);
                wxString errorMessage = readedLine.Right(poms.length() - i - 1);
                if (fl != NULL) {
                    wxLogError("%s: %d  %s", fl->filename, tline, errorMessage);
                } else {
                    wxLogError("%d  %s", tline, errorMessage);
                }
            }
        }
    }
    if (!this->noCleanUp) {
        wxRemoveFile(temporaryLcd.ToAscii());
        wxRemoveFile(temporaryPcd.ToAscii());
        if( wxFileExists(temporaryErr.ToAscii()) ) {
			wxRemoveFile(temporaryErr.ToAscii());
		}
        //todo clear all temporary files
    }
    return false;
}

int Compiller::OnExit() {
    return this->result;
}

IncludeFile* Compiller::findTrueLine(long ln, long *trueline) {
    IncludeFile *pom, *pom1;
    IncludeFileList pl;
    int c1 = 0, c2 = 0;
    IncludeFileList::iterator listIterator;


    if (includeList.IsEmpty()) {
        *trueline = ln;
        return (NULL);
    }
    pl.clear();
    listIterator = includeList.begin();
    pom = *listIterator;
    if (pom->start > ln) {
        *trueline = ln;
        return (NULL);
    }

    while ((listIterator != includeList.end()) && (ln > pom->start)) {

        if (ln < pom->start + pom->len) {
            *trueline = ln - pom->start;
            return (pom);
        }
        pl.Append(pom);
        listIterator++;
        pom = *listIterator;
    }

    if (pom != NULL) {
        listIterator = pl.begin();
        pom1 = *listIterator;
        c1 = 0;
        while (listIterator != pl.end()) {
            c1 += pom1->len;
            listIterator++;
            pom1 = *listIterator;
        }
        *trueline = ln - c1;
        return (NULL);
    } else {
        listIterator = includeList.begin();
        pom1 = *listIterator;
        c1 = 0;
        while (listIterator != includeList.end()) {
            c1 += pom1->len;
            listIterator++;
            pom1 = *listIterator;
        }
        *trueline = ln - c1;
        return (NULL);
    }
    return (NULL);
}


