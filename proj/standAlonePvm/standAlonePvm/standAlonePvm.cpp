// standAlonePvm.cpp : main project file.

#include "stdafx.h"
#include "standAlonePvm.h"

using namespace _standAlonePvm;

#ifndef ON_SETUPPACK
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

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
	ldePathSetup(args);			// set-up working path on run-time
//	startMachine();			// Hsien 2013/5/2 , start EPCIO card

	// Create the main window and run it
	System::Windows::Forms::Form^		formHandle	= gcnew standAlonePvm();
	//System::Threading::Timer^	timerHandle	= 
	//	gcnew System::Threading::Timer(					// would try to seperatre their data-scope in the future
														// make develope-work could be run individually
														// Hsien , 2013.01.11	

	Application::Run(formHandle/*gcnew standAlonePvm()*/);

#ifndef ON_SETUPPACK
	_CrtDumpMemoryLeaks();
#endif
	//closeMachine();

	return 0;
}

#include<PLC_FSMI.h>
#include<STC_DQI.h>

using namespace FSMI;
using namespace RSMI;
using namespace _virtualDevice;
int standAlonePvm::monitorInit(void* arg)
{
	//----------------------
	//	Monitor Initializing:
	//		0. rallocating override-use frame
	//		1. gcnew Monitor
	//		2. link to image
	//		3. register name
	//		4. adding into TLP
	//----------------------

	//----------------------
	//	read-out db and allocating monitor GUI and override-use frame
	//----------------------
	FRAME*		frmMonitor = NULL;
	FRAME*		frmOvrride = NULL;
	FRMATTR*	ref;
	sqlite3*	_db = (sqlite3*)arg;
	virtualMaintainerControl^	vmMaintain;
	
	int			errorCode;
	size_t		counts;

	Windows::Forms::TabPage^			tp	= nullptr;	// Hsien 2013/6/4 , used to loading tlp
	Windows::Forms::TableLayoutPanel^	tlp = nullptr;	


	errorCode = dqiLoadFrameAttr(_db,&ref,&counts);		// Hsien , 2013.04.25
	if(errorCode != DQI_SUCCESS)
		return 0;


	for(size_t i=0;i<counts;i++){

		//----------------------
		//	GCNEW monitor
		//----------------------
		tp	= gcnew TabPage(String::Format("{0:G}",gcnew String(ref[i].frameString)));
		tp->AutoScroll = true;
		tlp = gcnew TableLayoutPanel(/*this->tlpMonitor*/);
		tlp->RowStyles->Add(gcnew Windows::Forms::RowStyle(Windows::Forms::SizeType::Absolute,30));
		tlp->AutoSize	= true;
		tp->Controls->Add(tlp);

		if(ref[i].isOverridable){
			//----------------------
			//	Allocating SHADOW FRAMEs
			//----------------------
/*			if(!addFrame(ref[i].frameId + SHADOW_FRAME_OFFSET_ORIGIN,ref[i].totalSize))
				return 0;*/			// fail to allocating , Hsien 2013/6/4
			if(!addFrame(ref[i].frameId + SHADOW_FRAME_OFFSET_OVRRIDE,ref[i].totalSize))
				return 0;			// fail to allocating , Hsien 2013/6/4
		}

		frmMonitor = queryFrame(ref[i].frameId + ref[i].monitorFrameOffset);
		frmOvrride = queryFrame(ref[i].frameId + ref[i].overrideFrameOffset);

		for(int j=0;j < frmMonitor->frmSize/2 /*counts in WORD*/;j++){
			vmMaintain = gcnew virtualMaintainerControl();
			vmMaintain->registerName	(
				String::Format("{0:G}{1:D2}"
				,gcnew String(ref[i].frameString)
				,/*i*/j));	// Hsien 2013/6/1

			vmMaintain->registerImage	(&(((short*)frmMonitor->baseAdr)[/*i*/j]),0);
			if(ref[i].isOverridable){
				//----------------------
				//	Activate override function
				// Hsien 2013/6/4
				//----------------------
				vmMaintain->registerImage	(&(((short*)frmOvrride/*frmMonitor*/->baseAdr)[/*i*/j]),1);
				vmMaintain->enableOverride(0);
			}

			//this->tlpMonitor->Controls->Add(vmMaintain,-1,-1);
			tlp->Controls->Add(vmMaintain,-1,-1);
		}//for

		this->tcMonitor->Controls->Add(tp);
	}// for

	free(ref);		// release buffer

	return 1;
}

int standAlonePvm::startService()
{
	//----------------------
	//	Service include:
	//		Timer PVM
	//		Timer Monitor
	// Hsien 2013/6/5
	//----------------------
	FRAME*	keepRelay			= queryFrame(24);
	FRAME*	keepRelayOveride	= queryFrame(24+100);


	keepRelayReadout();			// Hsien 2013/6/5 
	memcpy(keepRelayOveride->baseAdr,keepRelay->baseAdr,keepRelay->frmSize);

	//----------------------
	//	Start Timer  , Resume Device
	//----------------------
	this->timerPvm->Change		(0,SAP_TIME_BASE);			// start the timer immediatly
	this->timerMonitor->Change	(0,SAP_MONITOR_TIME_BASE);

	//this->winFormTimerPvm->Enabled = true;
	//this->winFormTimerMonitor->Enabled = true;

	return 1;
}

int standAlonePvm::stopService()
{
	//----------------------
	//	Service include:
	//		Timer PVM
	//		Timer Monitor
	// Hsien 2013/6/5
	//----------------------

	//-------------------------------
	//	STOP THE TIMER , Clean FSMI(Excluding K-Frame
	//-------------------------------
	this->timerPvm->Change(
		Threading::Timeout::Infinite,
		Threading::Timeout::Infinite);
	this->timerMonitor->Change(
		Threading::Timeout::Infinite,
		Threading::Timeout::Infinite);
	//this->winFormTimerPvm->Enabled = false;
	//this->winFormTimerMonitor->Enabled = false;

	keepRelayWriteIn();				// Hsien 2013/6/5
	FSMI::resetFrame();

	return 1;
}