#include"stdafx.h"

using namespace _standAlonePvm;
using namespace procInterface;

System::Void standAlonePvm::btClick(System::Object ^sender, System::EventArgs ^e)
{
	int senderCode
		= Int32::Parse(safe_cast<Windows::Forms::ToolStripButton^>(sender)->Tag->ToString());
	
	//----------------
	//	Switch the Correspoding Handler
	//----------------
	switch(senderCode){
		case SAP_TAGINDEX_START:	procStart();	break;
		case SAP_TAGINDEX_STOP:		procStop();		break;
//		case SAP_TAGINDEX_SETUP:	devSetup();		break;
		default:
			break;
	}
}

static int procLoad(PVM*	pvmInstance,procInterfaceControl^ e)
{
	//-------------------------------------------
	//	Load Process , Report if any error occurs
	//-------------------------------------------
	std::string stdFilename;
	
	MarshalString(e->filename,stdFilename);
	return pvmLoadProcFromFile(
		pvmInstance,
		stdFilename.c_str(),
		//e->timeInterval,
		e->pid);
}

int	standAlonePvm::procStart()
{
	//----------------------
	//	Start Sequence:
	//		1. start Device
	//		2. start PVM
	//		3. Loading Processes
	//		4. Release Processes
	//		5. Start Timer
	//----------------------
	// Hsien 2013/5/15
	int errorCode;

	//-------------------
	//	Start The Device
	//-------------------
	if(!startDevice()){				
		//isAnyError = true;
		return 0;
	}

	//----------------------
	//	Restart PVM
	//----------------------
	pvmInitialize(
		this->pvmInstance,
		SAP_TIME_BASE,
		(int(*)(void*))startWithRs,		// forced-casting
		*_db/*(void*)&filename*/);						// the start script


	//----------------------------------
	//	Load Instructions and Start Timer
	//----------------------------------
	errorCode = procLoad(this->pvmInstance,this->procInterfaceControlLevel1);
	if(errorCode!= PVM_SUCCESS){
			//--------------------------
			// Load the Leve1-1 Process
			// Hsien , 2013.01.28
			//--------------------------
		this->textBoxRenew(L"Error:" + errorCode.ToString());
		return 0;
	}
	//--------------------------
	// Load the Leve1-2 Process
	// Hsien , 2013.01.28
	//--------------------------
	for each(procInterfaceControl^ e in this->tlpLevel2->Controls){
		errorCode = procLoad(this->pvmInstance,e);
		if(errorCode != PVM_SUCCESS){
			//-------------
			//	Error Output
			//-------------
			//isAnyError = true;
			this->textBoxRenew(L"Error:" + errorCode.ToString());
			return 0;
		}
	}
	
	//---------------------
	//	no any error , timer engaged
	//---------------------
	pvmProcApplyAll(this->pvmInstance,procRelease);		// release all process and ready to launch , added by Hsien , 2013.01.11

	//----------------------
	//	Read-Out Keep Relay Contents
	//----------------------
	// Hsien 2013/5/15
	//FRAME*	keepRelay		= FSMI::queryFrame(24);
	//FILE*	fp				= fopen("keepRelayData.kp","rb");
	//size_t	readOutCounts	= fread(keepRelay->baseAdr,sizeof(char),keepRelay->frmSize,fp);
	//fclose(fp);

	//----------------------
	//	Start Timer
	//----------------------
	this->timerPvm->Change(0,SAP_TIME_BASE);			// start the timer immediatly
	this->tsPvmStart->Enabled = false;					// disable the butto

	//if(readOutCounts != keepRelay->frmSize)
	//	return 0;	// Hsien 2013/5/15 , FILE-IO fail

	return 1;
}

int standAlonePvm::procStop()
{
	//----------------------
	//	Stop Sequence:
	//		5. Stop Device
	//		4. Stop PVM
	//		3. Unloading Processes
	//		2. Inhibit	 Processes
	//		1. Stop Timer
	//----------------------
	// Hsien 2013/5/15


	//-------------------------------
	//	STOP THE TIMER
	//-------------------------------
	this->timerPvm->Change(
		Threading::Timeout::Infinite,
		Threading::Timeout::Infinite);
	this->tsPvmStart->Enabled = true;

	//---------------------
	//	no any error , timer engaged
	//---------------------
	pvmProcApplyAll(this->pvmInstance,procInhibit);		// release all process and ready to launch , added by Hsien , 2013.01.11

	//----------------
	//	Unload Level 
	//----------------
	pvmUnloadProc(this->pvmInstance,this->procInterfaceControlLevel1->pid);				// unload the LEVEL1
	//----------------
	// Unload Level2
	//----------------
	for each(procInterface::procInterfaceControl^ e in this->tlpLevel2->Controls){
		pvmUnloadProc(this->pvmInstance,e->pid);
	}

	//----------------------
	//	Finalize PVM
	//----------------------
	pvmFinalize(this->pvmInstance
		,(int(*)(void*))stopWithFileOut
		,(void*)NULL);

	////--------------------------------
	////	Store-Back Keep-Relay Contents
	//// Hsien 2013/5/15
	////--------------------------------
	//FRAME*	keepRelay	= FSMI::queryFrame(24);
	//FILE*	fp			= fopen("keepRelayData.kp","wb+");	// Hsien 2013/5/15 , re-generating file
	//size_t writeCounts	= fwrite(keepRelay->baseAdr,sizeof(char),keepRelay->frmSize,fp);
	//fclose(fp);

	//----------------
	//	Frame Reset
	//----------------
	//FSMI::resetFrame();		// added by Hsien , 2013.01.14

	//----------------
	//	Device Reset
	//----------------
	stopDevice();
	this->textBoxRenew(L"SAP Stopped");
	virOutRenew(nullptr);	// Hsien 2013/5/15 , reset Virtual Device GUI

	//if(writeCounts != keepRelay->frmSize)
	//	return 0;	// Hsien 2013/5/15	, FILE-IO fail

	return 1;
}

//int	standAlonePvm::devSetup()
//{
//	//-------------------------------
//	//	OPEN THE DIALOG
//	//-------------------------------
//	//if(this->_setupForm->ShowDialog()
//	//	== Windows::Forms::DialogResult::OK){
//	//		driverAssign(this->_deviceStartScrips[this->_setupForm->deviceCode],_DEVICE_START);
//	//		this->_deviceState = this->_setupForm->deviceCode;
//	//}
//
//	return 1;
//}