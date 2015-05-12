//
// Created by amz on 07.05.15.
//

#include "MessageDialog.h"
#include "App.h"

MessageDialog::MessageDialog(wxWindow *parent, wxWindowID id, const wxString &title) : wxDialog(parent, id, title,
        wxDefaultPosition) {
    // Setup the Panel and Widgets.

    nodeId= new wxTextCtrl(this, wxID_ANY, "0");
    nodeId->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
    message = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(300, wxDefaultCoord));

    // Put the widgets in a sizer.
    wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );

    sizer->Add(CreateTextSizer( "Node ID:"));
    sizer->Add(nodeId);
    //sizer->AddStretchSpacer(1);

    sizer->Add(CreateTextSizer("Message:") );
    sizer->Add(message);
    //sizer->AddStretchSpacer(1);

    wxSizer *buttonSizer = CreateSeparatedButtonSizer((wxOK | wxCANCEL));
    if ( buttonSizer )
    {
        sizer->Add(buttonSizer);
    }

    SetAutoLayout( true );
    SetSizer( sizer );

    sizer->SetSizeHints( this );
    sizer->Fit( this );

    if (  wxCENTRE )
        Centre( wxBOTH );
}

MessageDialog::~MessageDialog() {}

void MessageDialog::onCancel(wxCommandEvent& WXUNUSED(pEvent))    {
    EndModal(wxID_CANCEL);
    Destroy();
}

void MessageDialog::onOk(wxCommandEvent& WXUNUSED(pEvent))    {
    EndModal(wxID_OK);
    Destroy();
}

