/*
 * File:   IncludeFile.h
 * Author: amz
 *
 * Created on 14 marzec 2014, 01:07
 */
#include <wx/object.h>
#include <wx/list.h>
class IncludeFile: public wxObject {
public:
public:
    char filename[255];
    int start, len;
    IncludeFile(const char *s, int st);
private:

};

WX_DECLARE_LIST(IncludeFile, IncludeFileList);

