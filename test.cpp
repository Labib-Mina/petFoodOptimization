#include <wx/wx.h>

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    wxTextCtrl* outputBox;
    void OnRunButton(wxCommandEvent& event);
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Run main.cpp", wxDefaultPosition, wxSize(500, 400))
{
    wxPanel* panel = new wxPanel(this);

    wxButton* runButton = new wxButton(panel, wxID_ANY, "Run main.exe", wxPoint(10, 10), wxSize(120, 30));
    outputBox = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(10, 50), wxSize(460, 300),
                               wxTE_MULTILINE | wxTE_READONLY);

    runButton->Bind(wxEVT_BUTTON, &MyFrame::OnRunButton, this);
}

void MyFrame::OnRunButton(wxCommandEvent& event)
{
    wxArrayString output, errors;

    // Run main.exe and capture output
    long result = wxExecute("main.exe", output, errors);

    if (result != -1)
    {
        wxString text;
        for (auto& line : output)
            text += line + "\n";
        for (auto& err : errors)
            text += "ERR: " + err + "\n";

        outputBox->SetValue(text);
    }
    else
    {
        outputBox->SetValue("Failed to execute main.exe");
    }
}
