#include "Rtt_LinuxConsoleApp.h"

bool Rtt_LinuxConsoleApp::OnInit()
{
	wxInitAllImageHandlers();
	Solar2DTuxConsole = new Rtt_LinuxConsole(NULL, wxID_ANY, wxEmptyString);
	SetTopWindow(Solar2DTuxConsole);
	Solar2DTuxConsole->Show();

	return true;
}

void Rtt_LinuxConsoleApp::ClearLog()
{
	Solar2DTuxConsole->ClearLog();
}

void Rtt_LinuxConsoleApp::UpdateLog(wxString message)
{
	Solar2DTuxConsole->UpdateLog(message);
}

void Rtt_LinuxConsoleApp::UpdateLog(wxString message, int messageType)
{
	switch(messageType)
	{
		case MessageType::Warning:
			Solar2DTuxConsole->UpdateLogWarning(message);
			break;

		case MessageType::Error:
			Solar2DTuxConsole->UpdateLogError(message);
			break;

		default:
			Solar2DTuxConsole->UpdateLog(message);
			break;
	}
}

IMPLEMENT_APP(Rtt_LinuxConsoleApp);
