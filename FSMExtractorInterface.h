#include "Panels.h"
#include "FiniteStateExtractor.h"
#include <wx/wxprec.h>


class FSMExtractorInterface : public wxFrame
{
public:
	FSMExtractorInterface(const wxString& title);

	FiniteStateExtractor finiteStateExtractor;
	LeftPanel *m_lp;
	ErrorPanel *m_errorPanel;
	StatesPanel *m_statesPanel;
	TransitionsPanel *m_transitionsPanel;
	wxPanel *m_parent;

};

