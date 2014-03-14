#include "IncludeFile.h"

    IncludeFile::IncludeFile(const char *s, int st) {
        strcpy(filename, s);
        start = st;
        len = 0;
    };
#include <wx/listimpl.cpp>
WX_DEFINE_LIST(IncludeFileList);
