// ldeditor_expr.cpp : main project file.
#include "stdafx.h"
#include "ldEditor.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace _ldEditor;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 
	
	ldePathSetup(args);			// set-up working path on run-time
								// Hsien  ,2013.11.14
//	if(args->Length>0)
//	System::Diagnostics::Process^ p = System::Diagnostics::Process::GetCurrentProcess();
//	System::Windows::Forms::MessageBox::Show(p->MainModule->FileName,"an");
#if LDE_ON_DEVELOP
	//------------
	//	Allocate Console
	//------------
	AllocConsole();
	freopen("CONOUT$","w",stdout);
	//printf("%s",args[1]->ToCharArray());
#endif
	// Create the main window and run it
	_ldEditor::ldEditor	^ldeMain = gcnew _ldEditor::ldEditor();	
	if(args->Length>0)
		ldeMain->load(args[0]);							// throw lmds path to open it , Hsien , 2013.11.15
	Application::Run(ldeMain);

#if LDE_ON_DEVELOP
	FreeConsole();
	_CrtDumpMemoryLeaks();
#endif 

	return 0;
}

