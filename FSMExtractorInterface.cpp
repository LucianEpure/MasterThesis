#include "FSMExtractorInterface.h"

FSMExtractorInterface::FSMExtractorInterface(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 500))
{
	m_parent = new wxPanel(this, wxID_ANY);

	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

	m_lp = new LeftPanel(m_parent, &finiteStateExtractor);
	m_errorPanel = new ErrorPanel(m_parent, &finiteStateExtractor);
	m_statesPanel = new StatesPanel(m_parent, &finiteStateExtractor);
	m_transitionsPanel = new TransitionsPanel(m_parent, &finiteStateExtractor);

	hbox->Add(m_lp, 1, wxALL, 5);
	hbox->Add(m_errorPanel, 1, wxALL, 5);
	hbox->Add(m_statesPanel, 1, wxEXPAND | wxALL, 5);
	hbox->Add(m_transitionsPanel, 1, wxEXPAND | wxALL, 5);

	m_parent->SetSizer(hbox);

	this->Centre();
}