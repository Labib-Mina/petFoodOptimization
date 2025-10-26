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
    wxColour GetBgColor(bool dark) { return dark ? wxColour(30, 30, 46) : wxColour(248, 249, 250); } // #1e1e2e / #f8f9fa
    wxColour GetFgColor(bool dark) { return dark ? wxColour(240, 240, 255) : wxColour(30, 30, 40); } // light text / dark text
    wxColour GetCtrlBgColor(bool dark) { return dark ? wxColour(45, 45, 65) : wxColour(255, 255, 255); } // input bg
    wxColour GetAccentColor(bool dark) { return dark ? wxColour(44, 106, 181) : wxColour(93, 156, 236); } // #2c6ab5 / #5d9cec
    wxColour GetSecondaryAccent() { return wxColour(114, 9, 183); } // purple #7209b7
    wxColour GetHeaderColor(bool dark) { return dark ? wxColour(40, 40, 60) : wxColour(230, 230, 240); } // subtle header

    wxFont GetAppFont() {
        wxFont font = wxFont(11, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Cascadia Code");
        if (!font.IsOk()) {
            font = wxFont(11, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas");
        }
        if (!font.IsOk()) {
            font = wxFont(11, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Courier New");
        }
        return font;
    }

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

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Pet Food Optimizer", wxDefaultPosition, wxSize(640, 580),
              wxDEFAULT_FRAME_STYLE | wxRESIZE_BORDER) {
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // -------------------- Pet Info Header --------------------
    wxPanel* headerPanel = new wxPanel(panel, wxID_ANY);
    headerPanel->SetBackgroundColour(GetHeaderColor(m_darkMode));
    wxStaticText* headerLabel = new wxStaticText(headerPanel, wxID_ANY, "Pet Information");
    headerLabel->SetForegroundColour(GetFgColor(m_darkMode));
    headerLabel->SetFont(GetAppFont().Bold());

    wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);
    headerSizer->AddSpacer(12);
    headerSizer->Add(headerLabel, 0, wxALIGN_CENTER_VERTICAL);
    headerSizer->AddSpacer(12);
    headerPanel->SetSizer(headerSizer);

    sizer->Add(headerPanel, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 12);

    // -------------------- Pet Info Fields --------------------
    wxPanel* petPanel = new wxPanel(panel, wxID_ANY);
    petPanel->SetBackgroundColour(GetBgColor(m_darkMode));
    wxBoxSizer* petSizer = new wxBoxSizer(wxVERTICAL);

    auto AddLabeledControl = [&](const wxString& label, wxWindow* ctrl) {
        // Label ABOVE the control
        wxStaticText* lbl = new wxStaticText(petPanel, wxID_ANY, label + ":");
        lbl->SetForegroundColour(GetFgColor(m_darkMode));
        lbl->SetFont(GetAppFont().Bold()); // Optional: bold labels

        petSizer->Add(lbl, 0, wxLEFT | wxTOP | wxRIGHT, 10);
        petSizer->Add(ctrl, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 8);
    };

    weightClassChoice = new wxChoice(petPanel, wxID_ANY);
    weightClassChoice->Append("Small (1)");
    weightClassChoice->Append("Medium (2)");
    weightClassChoice->Append("Large (3)");
    weightClassChoice->SetSelection(0);
    AddLabeledControl("Weight Class", weightClassChoice);

    ageInput = new wxTextCtrl(petPanel, wxID_ANY);
    AddLabeledControl("Age", ageInput);

    breedInput = new wxTextCtrl(petPanel, wxID_ANY);
    AddLabeledControl("Breed", breedInput);

    mealsInput = new wxTextCtrl(petPanel, wxID_ANY);
    AddLabeledControl("Meals per day", mealsInput);

    petPanel->SetSizer(petSizer);
    sizer->Add(petPanel, 0, wxEXPAND | wxALL, 12);

    // -------------------- Dark Mode Toggle --------------------
    darkModeCheckbox = new wxCheckBox(panel, wxID_ANY, "Enable Dark Mode");
    sizer->Add(darkModeCheckbox, 0, wxALIGN_LEFT | wxLEFT | wxRIGHT | wxBOTTOM, 20);
    darkModeCheckbox->Bind(wxEVT_CHECKBOX, &MyFrame::OnDarkModeToggle, this);

    wxButton* startButton = new wxButton(panel, wxID_ANY, "Start Calculation");
    sizer->Add(startButton, 0, wxALIGN_CENTER | wxALL, 16);

    panel->SetSizer(sizer);

    ApplyTheme(panel, m_darkMode);
    startButton->Bind(wxEVT_BUTTON, &MyFrame::OnStartButton, this);

    this->SetMinSize(wxSize(520, 520)); // Slightly taller for vertical layout
}

void MyFrame::OnDarkModeToggle(wxCommandEvent& event) {
    m_darkMode = darkModeCheckbox->GetValue();
    wxPanel* panel = dynamic_cast<wxPanel*>(GetChildren()[0]);
    if (panel) ApplyTheme(panel, m_darkMode);
}

void MyFrame::ApplyTheme(wxWindow* win, bool dark) {
    win->SetBackgroundColour(GetBgColor(dark));
    win->SetForegroundColour(GetFgColor(dark));
    win->SetFont(GetAppFont());

    // Style controls
    if (win->IsKindOf(CLASSINFO(wxTextCtrl))) {
        win->SetBackgroundColour(GetCtrlBgColor(dark));
        win->SetForegroundColour(GetFgColor(dark));
    }
    else if (win->IsKindOf(CLASSINFO(wxChoice))) {
        win->SetBackgroundColour(GetCtrlBgColor(dark));
        win->SetForegroundColour(GetFgColor(dark));
    }
    else if (win->IsKindOf(CLASSINFO(wxButton))) {
        win->SetBackgroundColour(GetAccentColor(dark));
        win->SetForegroundColour(*wxWHITE);
        win->SetFont(GetAppFont().Bold());
    }
    else if (win->IsKindOf(CLASSINFO(wxCheckBox))) {
        win->SetForegroundColour(GetFgColor(dark));
    }
    else if (win->IsKindOf(CLASSINFO(wxStaticText))) {
        win->SetForegroundColour(GetFgColor(dark));
    }
    else if (win->IsKindOf(CLASSINFO(wxPanel))) {
        // Special case: header panel
        if (win == dynamic_cast<wxPanel*>(GetChildren()[0])) {
            // Skip root panel — we handle children separately
        }
        else if (win == dynamic_cast<wxPanel*>(GetChildren()[0]->GetChildren()[0])) {
            // Header panel
            win->SetBackgroundColour(GetHeaderColor(dark));
        }
    }

    // Recurse into all children
    wxWindowList& children = win->GetChildren();
    for (wxWindow* child : children) {
        ApplyTheme(child, dark);
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
    wxDialog* dialog = CreateThemedDialog(title, true);
    dialog->SetSize(580, 380);
    dialog->SetMinSize(wxSize(420, 220));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    wxTextCtrl* text = new wxTextCtrl(dialog, wxID_ANY, message,
        wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL | wxVSCROLL);

    text->SetBackgroundColour(GetCtrlBgColor(m_darkMode));
    text->SetForegroundColour(GetFgColor(m_darkMode));
    text->SetFont(GetAppFont());

    sizer->Add(text, 1, wxEXPAND | wxALL, 12);

    wxButton* okBtn = new wxButton(dialog, wxID_OK, "OK");
    okBtn->SetBackgroundColour(GetAccentColor(m_darkMode));
    okBtn->SetForegroundColour(*wxWHITE);
    okBtn->SetFont(GetAppFont().Bold());
    sizer->Add(okBtn, 0, wxALIGN_CENTER | wxBOTTOM, 12);

    dialog->SetSizer(sizer);
    dialog->ShowModal();
    dialog->Destroy();
}

void MyFrame::OnStartButton(wxCommandEvent& event) {
    try {
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
        prodDialog->SetSize(420, 200);

        wxBoxSizer* prodSizer = new wxBoxSizer(wxVERTICAL);
        wxTextCtrl* fatInput = new wxTextCtrl(prodDialog, wxID_ANY);
        wxTextCtrl* proteinInput = new wxTextCtrl(prodDialog, wxID_ANY);

        fatInput->SetBackgroundColour(GetCtrlBgColor(m_darkMode));
        fatInput->SetForegroundColour(GetFgColor(m_darkMode));
        proteinInput->SetBackgroundColour(GetCtrlBgColor(m_darkMode));
        proteinInput->SetForegroundColour(GetFgColor(m_darkMode));

        prodSizer->Add(new wxStaticText(prodDialog, wxID_ANY, "Fat per meal (g):"), 0, wxALL, 6);
        prodSizer->Add(fatInput, 0, wxEXPAND | wxALL, 6);
        prodSizer->Add(new wxStaticText(prodDialog, wxID_ANY, "Protein per meal (g):"), 0, wxALL, 6);
        prodSizer->Add(proteinInput, 0, wxEXPAND | wxALL, 6);

        wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton* okBtn = new wxButton(prodDialog, wxID_OK, "OK");
        wxButton* cancelBtn = new wxButton(prodDialog, wxID_CANCEL, "Cancel");
        okBtn->SetBackgroundColour(GetAccentColor(m_darkMode));
        okBtn->SetForegroundColour(*wxWHITE);
        cancelBtn->SetBackgroundColour(GetSecondaryAccent());
        cancelBtn->SetForegroundColour(*wxWHITE);
        btnSizer->Add(okBtn, 0, wxRIGHT, 8);
        btnSizer->Add(cancelBtn, 0);
        prodSizer->Add(btnSizer, 0, wxALIGN_RIGHT | wxTOP | wxBOTTOM, 10);

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
        adjustDialog->SetSize(420, 200);

        wxBoxSizer* adjSizer = new wxBoxSizer(wxVERTICAL);
        wxTextCtrl* newFatInput = new wxTextCtrl(adjustDialog, wxID_ANY, wxString::Format("%.2f", fat));
        wxTextCtrl* newProteinInput = new wxTextCtrl(adjustDialog, wxID_ANY, wxString::Format("%.2f", protein));

        newFatInput->SetBackgroundColour(GetCtrlBgColor(m_darkMode));
        newFatInput->SetForegroundColour(GetFgColor(m_darkMode));
        newProteinInput->SetBackgroundColour(GetCtrlBgColor(m_darkMode));
        newProteinInput->SetForegroundColour(GetFgColor(m_darkMode));

        adjSizer->Add(new wxStaticText(adjustDialog, wxID_ANY, "New Fat per meal (g):"), 0, wxALL, 6);
        adjSizer->Add(newFatInput, 0, wxEXPAND | wxALL, 6);
        adjSizer->Add(new wxStaticText(adjustDialog, wxID_ANY, "New Protein per meal (g):"), 0, wxALL, 6);
        adjSizer->Add(newProteinInput, 0, wxEXPAND | wxALL, 6);

        wxBoxSizer* adjBtnSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton* adjOkBtn = new wxButton(adjustDialog, wxID_OK, "OK");
        wxButton* adjCancelBtn = new wxButton(adjustDialog, wxID_CANCEL, "Cancel");
        adjOkBtn->SetBackgroundColour(GetAccentColor(m_darkMode));
        adjOkBtn->SetForegroundColour(*wxWHITE);
        adjCancelBtn->SetBackgroundColour(GetSecondaryAccent());
        adjCancelBtn->SetForegroundColour(*wxWHITE);
        adjBtnSizer->Add(adjOkBtn, 0, wxRIGHT, 8);
        adjBtnSizer->Add(adjCancelBtn, 0);
        adjSizer->Add(adjBtnSizer, 0, wxALIGN_RIGHT | wxTOP | wxBOTTOM, 10);

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