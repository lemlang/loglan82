//
// Created by amz on 07.05.15.
//

#ifndef PROJECT_MESSAGEDIALOG_H
#define PROJECT_MESSAGEDIALOG_H

#include <wx/wx.h>
#include "MessageDialog.h"
#include "MainWindow.h"
#include <wx/sizer.h>

class MainWindow;
class MessageDialog: public wxDialog   {

public:
    MessageDialog(wxWindow *parent, wxWindowID id, const wxString &wxString);
    virtual ~MessageDialog();
    void onCancel(wxCommandEvent& pEvent);
    void onOk(wxCommandEvent& pEvent);
    void onCheck(wxCommandEvent& pEvent);

    short getNode()    {
        long log;

        nodeId->GetValue().ToLong(&log, 0);
        return  log;
    }
    const wxString getValue()    {
        return message->GetValue();    }

private:
    wxTextCtrl* nodeId;
    wxTextCtrl* message;
};


#endif //PROJECT_MESSAGEDIALOG_H
