/*
 * File:   GraphicsWindow.cpp
 * Author: amz
 *
 * Created on 12 marzec 2014, 10:48
 */


#include "GraphicsWindow.h"


GraphicsWindow::GraphicsWindow(const wxString& title, Graphics*parent)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(650, 350)) {
    input_queue_nl_count = 0;
    this->parent = parent;
    wxTextValidator validator(wxFILTER_INCLUDE_LIST );
    text = new wxTextCtrl(this, TEXT_Main, "", wxDefaultPosition, wxDefaultSize,
            wxTE_MULTILINE ,validator , wxTextCtrlNameStr);
    wxLogMessage(wxString::Format(_("GraphicsWindow::this->text %lu \n"),(long unsigned int) text));
    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(GraphicsWindow::OnQuit));
    Connect(wxID_EXECUTE, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(GraphicsWindow::OnExecute));
    //text->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(GraphicsWindow::onKeyDown));
    text->Connect(wxEVT_CHAR_HOOK,   wxKeyEventHandler(GraphicsWindow::onChar));
    text->Connect(wxEVT_KEY_UP,   wxKeyEventHandler(GraphicsWindow::onKeyUp), NULL, this);
    text->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(GraphicsWindow::onMouseClick), NULL, this);
    text->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(GraphicsWindow::onMouseClick), NULL, this);
    text->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(GraphicsWindow::onMouseClick), NULL, this);
    text->Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(GraphicsWindow::onMouseClick), NULL, this);
    text->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(GraphicsWindow::onMouseClick), NULL, this);
    text->Connect(wxEVT_RIGHT_DCLICK, wxMouseEventHandler(GraphicsWindow::onMouseClick), NULL, this);
    Centre();
}


void GraphicsWindow::onMouseClick(wxMouseEvent& aEvent)
{
    wxLogMessage(wxString::Format(_("GraphicsWindow::onMouseClick\n")));
    //aEvent.ResumePropagation(1);
    //aEvent.Skip();
    aEvent.StopPropagation();
}

void GraphicsWindow::onKeyDown(wxKeyEvent& aEvent)
{

    //wxLogMessage(wxString::Format(_("GraphicsWindow::onKeyDown\n")));
    aEvent.ResumePropagation(wxEVENT_PROPAGATE_MAX);
    //aEvent.Skip();
    //aEvent.StopPropagation();
}

void GraphicsWindow::onKeyUp(wxKeyEvent& aEvent)
{
//aEvent.GetUnicodeKey(),
    wxLogMessage(wxString::Format(_("GraphicsWindow::onKeyUp %d %d\n"),aEvent.GetKeyCode(), aEvent.GetModifiers() ));
    //aEvent.ResumePropagation(wxEVENT_PROPAGATE_MAX);
    //aEvent.Skip();
    //aEvent.StopPropagation();
}

void GraphicsWindow::onChar(wxKeyEvent& aEvent) {
    int key_char = 11;
    if( aEvent.GetUnicodeKey() == WXK_NONE) {
        wxLogMessage(wxString::Format(_("GraphicsWindow::onChar %d\n"),aEvent.GetKeyCode() ));
        key_char = aEvent.GetKeyCode();

    } else {
        wxLogMessage(wxString::Format(_("GraphicsWindow::onChar unicode %d\n"),aEvent.GetUnicodeKey()));
        key_char = aEvent.GetUnicodeKey();
        if( ! aEvent.ShiftDown() ) {
            key_char = tolower(key_char);
        }
    }
    if ( read_queue.size() > 0 ) {
        int key = read_queue.front();
        MESSAGE writeValue;
        switch(key) {
            case GRAPH_INKEY:
                writeValue.msg_type = MSG_GRAPH;
                writeValue.param.pword[0] = GRAPH_INKEY_RESPONSE;
                writeValue.param.pword[3] = key_char;
                this->parent->getSocketClient()->Write(&writeValue, sizeof(MESSAGE));
                wxLogMessage(wxString::Format(_("[Graphicsd::OnSocketEvent] GRAPH_INKEY wrote response %c"),key_char));
                read_queue.pop();
                return;
            case GRAPH_READLN:
                if( 13 == key_char) {
                    writeValue.msg_type = MSG_GRAPH;
                    writeValue.param.pword[0] = GRAPH_READLN_RESPONSE;
                    this->parent->getSocketClient()->Write(&writeValue, sizeof(MESSAGE));

                    wxLogMessage(_("[Graphicsd::OnSocketEvent] GRAPH_READLN_RESPONSE wrote response"));
                    read_queue.pop();
                    return;
                }
            break;
            case GRAPH_READCHAR:
                writeValue.msg_type = MSG_GRAPH;
                writeValue.param.pword[0] = GRAPH_READCHAR_RESPONSE;
                writeValue.param.pword[3] = key_char;
                this->parent->getSocketClient()->Write(&writeValue, sizeof(MESSAGE));
                wxLogMessage(_("[Graphicsd::OnSocketEvent] GRAPH_READCHAR wrote response"));
                read_queue.pop();
                return;
            case GRAPH_READSTR:
                if( 13 == key_char) {
                    writeValue.msg_type = MSG_GRAPH;
                    writeValue.param.pword[0] = GRAPH_READSTR_RESPONSE;
                    strcpy(writeValue.param.pstr,input_buffer.c_str());
                    this->parent->getSocketClient()->Write(&writeValue, sizeof(MESSAGE));

                    wxLogMessage(_("[Graphicsd::OnSocketEvent] GRAPH_READSTR_RESPONSE wrote response"));
                    read_queue.pop();
                    input_queue.clear();
                    input_queue_nl_count=0;
                } else if( 8 == key_char) {
                    input_buffer.substr(0,input_buffer.size()-1);
                    //this->text->Remove(this->text->GetValue().size()-1, this->text->GetValue().size());
                    return;
                } else {
                    wxLogMessage(_("[Graphicsd::OnSocketEvent] GRAPH_READSTR"));
                    input_buffer.append((wxChar)key_char);
                    //(*text) << (wxChar)key_char;

                    return;
                }
            break;
        }
    }
    if( 13 == key_char) {
        input_queue_nl_count++;
        wxLogMessage(_("[Graphicsd::OnSocketEvent] append newline"));
    } else
        wxLogMessage("[Graphicsd::OnSocketEvent] appending %d", key_char);
    input_queue.push_back(key_char);
}
void GraphicsWindow::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    Close(FALSE);
}

void GraphicsWindow::OnClose(wxCloseEvent& event) {
    /*this->Destroy();return;
    if (showQuitDialog() == wxID_YES) {
        this->Destroy();
    } else {
        if (event.CanVeto()) {
            event.Veto();
        }
    }*/
}

int GraphicsWindow::showQuitDialog() {
    wxMessageDialog *dial = new wxMessageDialog(NULL,
            wxT("Are you sure to quit?"), wxT("Question"),
            wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    int ret = dial->ShowModal();
    dial->Destroy();
    wxLogMessage(wxString::Format(_("[GraphicsWindow::showQuitDialog] Show Dialog returns %d."), ret));
    return ret;
}

void GraphicsWindow::OnExecute(wxCommandEvent& WXUNUSED(event)) {
    wxFileDialog
    openFileDialog(this, _("Open Loglan file"), "", "",
            "log files (*.log)|*.log", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return; // the user changed idea...

    // proceed loading the file chosen by the user;
    // this can be done with e.g. wxWidgets input streams:
    wxFileInputStream input_stream(openFileDialog.GetPath());
    if (!input_stream.IsOk()) {
        wxLog* target = wxLog::GetActiveTarget();
        wxLog::SetActiveTarget(NULL);

        wxLogMessage("[GraphicsWindow::showQuitDialog] Cannot open file '%s'.", openFileDialog.GetPath());
        wxLog::SetActiveTarget(target);
        return;
    }
}
GraphicsWindow::~GraphicsWindow(){
}

void GraphicsWindow::PutChar(char ch) {
    (*text) << ch;
}

void GraphicsWindow::WriteText(char string[]) {
    (*text) << string;
}

void GraphicsWindow::WaitRead(int messageType) {
    read_queue.push(messageType);
}

bool GraphicsWindow::PopInputQueue(int *pInt) {
    if( input_queue.size() > 0) {
        *pInt = input_queue.front();
        input_queue.pop_front();
        return true;
    }
    return false;
}

bool GraphicsWindow::GetLine() {

    if( input_queue_nl_count>0) {
        input_queue_nl_count--;
        std::deque<int>::iterator it = input_queue.begin();
        while (it != input_queue.end()) {
            if( 13 == *it) {
                input_queue.erase(it);

                wxLogMessage(_("[Graphics::GetLine] true"));
                return true;
            }
            it++;
        }
    }

    wxLogMessage(wxString::Format(_("[Graphics::GetLine] false %lu"),input_queue.size() ) );
    return false;
}

void GraphicsWindow::ClearAll() {
    text->Clear();
    //todo clear painted screen
}

BEGIN_EVENT_TABLE(GraphicsWindow, wxFrame)
EVT_CLOSE(GraphicsWindow::OnClose)
END_EVENT_TABLE()
