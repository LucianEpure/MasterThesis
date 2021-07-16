#include "Main.h"
IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	FSMExtractorInterface *fsmExtractorInterface = new FSMExtractorInterface(wxT("FSM Extraction"));
	fsmExtractorInterface->Show(true);

	return true;
}