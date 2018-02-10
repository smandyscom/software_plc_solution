// standAlonePvm.cpp : main project file.

#include "stdafx.h"
#include "standAlonePvm.h"

using namespace _standAlonePvm;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	//----------------------------
	//	Function to Start EPCIO
	//----------------------------
	//startMachine();
	ldePathSetup();			// set-up working path on run-time
	startMachine();			// Hsien 2013/5/2 , start EPCIO card

	// Create the main window and run it
	System::Windows::Forms::Form^		formHandle	= gcnew standAlonePvm();
	//System::Threading::Timer^	timerHandle	= 
	//	gcnew System::Threading::Timer(					// would try to seperatre their data-scope in the future
														// make develope-work could be run individually
														// Hsien , 2013.01.11	

	Application::Run(formHandle/*gcnew standAlonePvm()*/);

	//closeMachine();

	return 0;
}
