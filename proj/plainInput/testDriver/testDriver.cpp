// testDriver.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
using namespace testDriver;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());

	_CrtDumpMemoryLeaks();		// leakage detector
								// test ok , no memory leakage
								// after store/rebuild no leakage happend
	return 0;
}
