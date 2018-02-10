// testForm3.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace testForm3;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());
	GC::Collect(0,System::GCCollectionMode::Forced);

	 _CrtDumpMemoryLeaks();
	return 0;
}
