#include <wx/wx.h>
#include <wx/panel.h>
#include "FiniteStateExtractor.h"

class LeftPanel : public wxPanel
{
public:
	LeftPanel(wxPanel *parent, FiniteStateExtractor *finiteStateExtractor);

	void OnUpload(wxCommandEvent & event);
	void OnGenerate(wxCommandEvent & event);
	wxTextCtrl *tc;
	wxButton *m_Upload;
	wxButton *m_Generate;
	wxPanel *m_parent;
	std::vector<std::string> data;
	FiniteStateExtractor *m_finiteStateExtractor;
	int count;

};

class ErrorPanel : public wxPanel
{
public:
	ErrorPanel(wxPanel *parent, FiniteStateExtractor *finiteStateExtractor);

	void OnSetText(wxCommandEvent & event);

	wxStaticText *m_text;

};

class StatesPanel : public wxPanel
{
public:
	StatesPanel(wxPanel *parent, FiniteStateExtractor *finiteStateExtractor);

	void OnSetText(wxCommandEvent & event);

	wxStaticText *m_text;

};

class TransitionsPanel : public wxPanel
{
public:
	TransitionsPanel(wxPanel *parent, FiniteStateExtractor *finiteStateExtractor);

	void OnSetText(wxCommandEvent & event);

	wxStaticText *m_text;

};

const int ID_PLUS = 101;
const int ID_MINUS = 102;