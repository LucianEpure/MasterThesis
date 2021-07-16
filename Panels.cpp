#include <wx/stattext.h>
#include "FSMExtractorInterface.h"

LeftPanel::LeftPanel(wxPanel * parent, FiniteStateExtractor * finiteStateExtractor)
	: wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN)
{
	count = 0;
	m_parent = parent;
	m_finiteStateExtractor = finiteStateExtractor;
	m_Upload = new wxButton(this, ID_PLUS, wxT("Upload"),
		wxPoint(10, 10));
	m_Generate = new wxButton(this, ID_MINUS, wxT("Generate"),
		wxPoint(10, 80));
	tc = new wxTextCtrl(this, -1, wxT(""),
		wxPoint(10, 40));

	Connect(ID_PLUS, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(LeftPanel::OnUpload));
	Connect(ID_MINUS, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(LeftPanel::OnGenerate));
}

void LeftPanel::OnUpload(wxCommandEvent & WXUNUSED(event))
{
	count++;
	std::string fileName = this->tc->GetValue().ToStdString();
	FSMExtractorInterface *comm = (FSMExtractorInterface *)m_parent->GetParent();
	data = m_finiteStateExtractor->getData(fileName);
	if (data.size() == 0)
	{
		comm->m_errorPanel->m_text->SetLabel("Error, no data");
	}
	else
	{
		comm->m_errorPanel->m_text->SetLabel("Uploaded!");
	}
}

void LeftPanel::OnGenerate(wxCommandEvent & WXUNUSED(event))
{
	std::string fileName = this->tc->GetValue().ToStdString();
	data = m_finiteStateExtractor->getData(fileName);
	std::vector<FiniteStateMachine> finiteStateMachines = m_finiteStateExtractor->getFiniteStateMachines(data);
	//take just the first one
	std::string statesToPrint;
	std::string transitionsToPrint;
	if (finiteStateMachines.size() > 0)
	{
		for (int i = 0; i < finiteStateMachines[0].states.size(); i++)
		{
			statesToPrint = statesToPrint + finiteStateMachines[0].states[i].stateName + "\n";
		}
		for (int j = 0; j < finiteStateMachines[0].transitionFunctions.size(); j++)
		{
			transitionsToPrint = transitionsToPrint + "FROM "+ finiteStateMachines[0].transitionFunctions[j].startState.stateName + " TO " + finiteStateMachines[0].transitionFunctions[j].finishState.stateName + "\n";
		}
	}
	FSMExtractorInterface *comm = (FSMExtractorInterface *)m_parent->GetParent();
	comm->m_statesPanel->m_text->SetLabel(statesToPrint);
	comm->m_transitionsPanel->m_text->SetLabel(transitionsToPrint);
}


ErrorPanel::ErrorPanel(wxPanel * parent, FiniteStateExtractor *finiteStateExtractor)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition,
		wxSize(100, 100), wxBORDER_SUNKEN)
{
	m_text = new wxStaticText(this, -1, wxT("Errors"), wxPoint(10, 10));
}

StatesPanel::StatesPanel(wxPanel * parent, FiniteStateExtractor *finiteStateExtractor)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition,
		wxSize(100, 100), wxBORDER_SUNKEN)
{
	m_text = new wxStaticText(this, -1, wxT("States"), wxPoint(10, 10));
}

TransitionsPanel::TransitionsPanel(wxPanel * parent, FiniteStateExtractor *finiteStateExtractor)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition,
		wxSize(200, 100), wxBORDER_SUNKEN)
{
	m_text = new wxStaticText(this, -1, wxT("Transitions"), wxPoint(10, 10));
}