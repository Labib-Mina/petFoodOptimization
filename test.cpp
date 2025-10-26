#include <wx/wx.h>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include "pet.h"
#include "Product.h"

using namespace std;

// -------------------- App --------------------
class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

// -------------------- Frame --------------------
class MyFrame : public wxFrame {
public:
    MyFrame();

private:
    bool m_darkMode = false;

    // UI Controls
    wxChoice* weightClassChoice;
    wxTextCtrl* ageInput;
    wxTextCtrl* breedInput;
    wxTextCtrl* mealsInput;
    wxCheckBox* darkModeCheckbox;

    void OnStartButton(wxCommandEvent& event);
    void OnDarkModeToggle(wxCommandEvent& event);

    // Theme helpers
    void ApplyTheme(wxWindow* win, bool dark);
    wxColour GetBgColor(bool dark) { return dark ? wxColour(30, 30, 30) : wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW); }
    wxColour GetFgColor(bool dark) { return dark ? wxColour(220, 220, 220) : wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT); }
    wxColour GetCtrlBgColor(bool dark) { return dark ? wxColour(50, 50, 50) : wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW); }

    // Dialog helpers
    void ShowResultDialog(const wxString& title, const wxString& message);
    wxDialog* CreateThemedDialog(const wxString& title, bool resizable = false);
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame() : wxFrame(NULL, wxID_ANY, "Pet Food Optimizer", wxDefaultPosition, wxSize(520, 450)) {
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // -------------------- Pet Info --------------------
    wxStaticBoxSizer* petSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Pet Information");
    petSizer->Add(new wxStaticText(panel, wxID_ANY, "Weight Class:"), 0, wxALL, 5);
    weightClassChoice = new wxChoice(panel, wxID_ANY);
    weightClassChoice->Append("Small (1)");
    weightClassChoice->Append("Medium (2)");
    weightClassChoice->Append("Large (3)");
    weightClassChoice->SetSelection(0);
    petSizer->Add(weightClassChoice, 0, wxEXPAND | wxALL, 5);

    petSizer->Add(new wxStaticText(panel, wxID_ANY, "Age:"), 0, wxALL, 5);
    ageInput = new wxTextCtrl(panel, wxID_ANY);
    petSizer->Add(ageInput, 0, wxEXPAND | wxALL, 5);

    petSizer->Add(new wxStaticText(panel, wxID_ANY, "Breed:"), 0, wxALL, 5);
    breedInput = new wxTextCtrl(panel, wxID_ANY);
    petSizer->Add(breedInput, 0, wxEXPAND | wxALL, 5);

    petSizer->Add(new wxStaticText(panel, wxID_ANY, "Meals per day:"), 0, wxALL, 5);
    mealsInput = new wxTextCtrl(panel, wxID_ANY);
    petSizer->Add(mealsInput, 0, wxEXPAND | wxALL, 5);

    sizer->Add(petSizer, 0, wxEXPAND | wxALL, 10);

    // -------------------- Dark Mode Toggle --------------------
    darkModeCheckbox = new wxCheckBox(panel, wxID_ANY, "Dark Mode");
    sizer->Add(darkModeCheckbox, 0, wxALIGN_LEFT | wxLEFT | wxRIGHT | wxBOTTOM, 15);
    darkModeCheckbox->Bind(wxEVT_CHECKBOX, &MyFrame::OnDarkModeToggle, this);

    wxButton* startButton = new wxButton(panel, wxID_ANY, "Start Calculation");
    sizer->Add(startButton, 0, wxALIGN_CENTER | wxALL, 10);

    panel->SetSizer(sizer);
    ApplyTheme(panel, m_darkMode);
    startButton->Bind(wxEVT_BUTTON, &MyFrame::OnStartButton, this);
}

void MyFrame::OnDarkModeToggle(wxCommandEvent& event) {
    m_darkMode = darkModeCheckbox->GetValue();
    // Re-apply theme to main panel
    wxPanel* panel = dynamic_cast<wxPanel*>(GetChildren()[0]);
    if (panel) ApplyTheme(panel, m_darkMode);
}

void MyFrame::ApplyTheme(wxWindow* win, bool dark) {
    win->SetBackgroundColour(GetBgColor(dark));
    win->SetForegroundColour(GetFgColor(dark));

    // Recursively apply to children
    wxWindowList& children = win->GetChildren();
    for (wxWindow* child : children) {
        if (child->IsKindOf(CLASSINFO(wxTextCtrl)) ||
            child->IsKindOf(CLASSINFO(wxStaticText)) ||
            child->IsKindOf(CLASSINFO(wxChoice))) {
            child->SetBackgroundColour(GetCtrlBgColor(dark));
            child->SetForegroundColour(GetFgColor(dark));
        } else if (child->IsKindOf(CLASSINFO(wxButton)) ||
                   child->IsKindOf(CLASSINFO(wxCheckBox))) {
            // Buttons/checkboxes usually auto-adapt, but we can force if needed
        }
        child->Refresh();
    }
    win->Refresh();
}

wxDialog* MyFrame::CreateThemedDialog(const wxString& title, bool resizable) {
    long style = wxDEFAULT_DIALOG_STYLE | (resizable ? (wxRESIZE_BORDER | wxMAXIMIZE_BOX) : 0);
    wxDialog* dialog = new wxDialog(this, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, style);
    ApplyTheme(dialog, m_darkMode);
    return dialog;
}

void MyFrame::ShowResultDialog(const wxString& title, const wxString& message) {
    wxDialog* dialog = CreateThemedDialog(title, true); // resizable = true
    dialog->SetSize(550, 350);
    dialog->SetMinSize(wxSize(400, 200));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    wxTextCtrl* text = new wxTextCtrl(dialog, wxID_ANY, message,
        wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL | wxVSCROLL);
    ApplyTheme(text, m_darkMode);
    sizer->Add(text, 1, wxEXPAND | wxALL, 10);

    wxButton* okBtn = new wxButton(dialog, wxID_OK, "OK");
    ApplyTheme(okBtn, m_darkMode);
    sizer->Add(okBtn, 0, wxALIGN_CENTER | wxALL, 5);

    dialog->SetSizer(sizer);
    dialog->ShowModal();
    dialog->Destroy();
}

void MyFrame::OnStartButton(wxCommandEvent& event) {
    try {
        // --- Step 1: Get Pet Info ---
        int weightClass = weightClassChoice->GetSelection() + 1;
        int age = wxAtoi(ageInput->GetValue()); if (age <= 0) age = 1;
        wxString breedWx = breedInput->GetValue();
        string breed = string(breedWx.mb_str());
        int meals = wxAtoi(mealsInput->GetValue()); if (meals <= 0) meals = 1;

        Pet pet(weightClass, age, breed);
        pet.setMealsPerDay(meals);
        vector<double> ideal = pet.idealDiet();

        stringstream ss;
        ss << fixed << setprecision(2);
        ss << "Breed: " << pet.getBreed() << "\n";
        ss << "Age: " << pet.getAge() << " years\n";
        ss << "Weight Class: " << pet.getWeightClass() << "\n";
        ss << "Meals per day: " << pet.getMealsPerDay() << "\n\n";
        ss << "Ideal daily nutrients:\n";
        ss << "  Protein: " << ideal[0] << " g/day\n";
        ss << "  Fat:     " << ideal[1] << " g/day\n";

        ShowResultDialog("Ideal Intake", ss.str());

        // --- Step 2: Get Initial Product Info ---
        wxDialog* prodDialog = CreateThemedDialog("Enter Initial Product Info");
        prodDialog->SetSize(400, 200);

        wxBoxSizer* prodSizer = new wxBoxSizer(wxVERTICAL);
        wxTextCtrl* fatInput = new wxTextCtrl(prodDialog, wxID_ANY);
        wxTextCtrl* proteinInput = new wxTextCtrl(prodDialog, wxID_ANY);
        ApplyTheme(fatInput, m_darkMode);
        ApplyTheme(proteinInput, m_darkMode);

        prodSizer->Add(new wxStaticText(prodDialog, wxID_ANY, "Fat per meal (g):"), 0, wxALL, 5);
        prodSizer->Add(fatInput, 0, wxEXPAND | wxALL, 5);
        prodSizer->Add(new wxStaticText(prodDialog, wxID_ANY, "Protein per meal (g):"), 0, wxALL, 5);
        prodSizer->Add(proteinInput, 0, wxEXPAND | wxALL, 5);

        wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton* okBtn = new wxButton(prodDialog, wxID_OK, "OK");
        wxButton* cancelBtn = new wxButton(prodDialog, wxID_CANCEL, "Cancel");
        ApplyTheme(okBtn, m_darkMode);
        ApplyTheme(cancelBtn, m_darkMode);
        btnSizer->Add(okBtn, 0, wxALL, 5);
        btnSizer->Add(cancelBtn, 0, wxALL, 5);
        prodSizer->Add(btnSizer, 0, wxALIGN_RIGHT | wxALL, 5);

        prodDialog->SetSizerAndFit(prodSizer);

        if (prodDialog->ShowModal() != wxID_OK) {
            prodDialog->Destroy();
            return;
        }

        double fat = wxAtof(fatInput->GetValue());
        double protein = wxAtof(proteinInput->GetValue());
        prodDialog->Destroy();

        Product product(fat, protein);
        double actualProtein = product.getProtein() * pet.getMealsPerDay();
        double actualFat = product.getFat() * pet.getMealsPerDay();
        double score = product.computeScore(pet);

        stringstream initOut;
        initOut << fixed << setprecision(2);
        initOut << "--- Initial Product ---\n\n";
        initOut << "Protein: " << actualProtein << " g/day (ideal " << ideal[0]
                << ", diff " << (actualProtein - ideal[0]) << ")\n";
        initOut << "Fat:     " << actualFat << " g/day (ideal " << ideal[1]
                << ", diff " << (actualFat - ideal[1]) << ")\n";
        initOut << "Computed Score: " << score << " / 10\n";

        ShowResultDialog("Initial Product Result", initOut.str());

        // --- Step 3: Get Adjusted Product Info ---
        wxDialog* adjustDialog = CreateThemedDialog("Enter Adjusted Product Info");
        adjustDialog->SetSize(400, 200);

        wxBoxSizer* adjSizer = new wxBoxSizer(wxVERTICAL);
        wxTextCtrl* newFatInput = new wxTextCtrl(adjustDialog, wxID_ANY, wxString::Format("%.2f", fat));
        wxTextCtrl* newProteinInput = new wxTextCtrl(adjustDialog, wxID_ANY, wxString::Format("%.2f", protein));
        ApplyTheme(newFatInput, m_darkMode);
        ApplyTheme(newProteinInput, m_darkMode);

        adjSizer->Add(new wxStaticText(adjustDialog, wxID_ANY, "New Fat per meal (g):"), 0, wxALL, 5);
        adjSizer->Add(newFatInput, 0, wxEXPAND | wxALL, 5);
        adjSizer->Add(new wxStaticText(adjustDialog, wxID_ANY, "New Protein per meal (g):"), 0, wxALL, 5);
        adjSizer->Add(newProteinInput, 0, wxEXPAND | wxALL, 5);

        wxBoxSizer* adjBtnSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton* adjOkBtn = new wxButton(adjustDialog, wxID_OK, "OK");
        wxButton* adjCancelBtn = new wxButton(adjustDialog, wxID_CANCEL, "Cancel");
        ApplyTheme(adjOkBtn, m_darkMode);
        ApplyTheme(adjCancelBtn, m_darkMode);
        adjBtnSizer->Add(adjOkBtn, 0, wxALL, 5);
        adjBtnSizer->Add(adjCancelBtn, 0, wxALL, 5);
        adjSizer->Add(adjBtnSizer, 0, wxALIGN_RIGHT | wxALL, 5);

        adjustDialog->SetSizerAndFit(adjSizer);

        if (adjustDialog->ShowModal() != wxID_OK) {
            adjustDialog->Destroy();
            return;
        }

        double newFat = wxAtof(newFatInput->GetValue());
        double newProtein = wxAtof(newProteinInput->GetValue());
        adjustDialog->Destroy();

        product.setFat(newFat);
        product.setProtein(newProtein);

        actualProtein = product.getProtein() * pet.getMealsPerDay();
        actualFat = product.getFat() * pet.getMealsPerDay();
        score = product.computeScore(pet);

        stringstream adjOut;
        adjOut << fixed << setprecision(2);
        adjOut << "--- Adjusted Product ---\n\n";
        adjOut << "Protein: " << actualProtein << " g/day (ideal " << ideal[0]
               << ", diff " << (actualProtein - ideal[0]) << ")\n";
        adjOut << "Fat:     " << actualFat << " g/day (ideal " << ideal[1]
               << ", diff " << (actualFat - ideal[1]) << ")\n";
        adjOut << "New Computed Score: " << score << " / 10\n";

        ShowResultDialog("Adjusted Product Result", adjOut.str());

    } catch (const exception& e) {
        wxMessageBox("Error: " + wxString(e.what()), "Error", wxOK | wxICON_ERROR);
    }
}