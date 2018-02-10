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
	//		3. Loading Processes ( within method of Tab-Form2
	//		4. Release Processes ( ignore this action if method in Tab-Form2
	//		5. Start Timer
	//----------------------
	// Hsien 2013/5/15
	int errorCode;

	//----------------------
	//	Stop Service
	//----------------------
	this->stopService();
	//----------------------------------
	//	Load Instructions and Start Timer
	//----------------------------------
	//----------------------
	//	Tab-Form2 Method
	// Hsien 2013/12/2
	//----------------------
	//rewind(this->defaultFp);
	System::IO::File::WriteAllText("default.tab",this->tbTab->Text);
	this->defaultFp = fopen("default.tab","rb");
	errorCode = pvmLoadFromTab2(this->pvmInstance,this->defaultFp);
	fclose(this->defaultFp);
	if(errorCode != PVM_SUCCESS){
			//--------------------------
			// Load the Leve1-1 Process
			// Hsien , 2013.01.28
			//--------------------------
		this->textBoxRenew(L"Error:" + errorCode.ToString());
		return 0;
	}

//	errorCode = procLoad(this->pvmInstance,this->procInterfaceControlLevel1);

	//if(errorCode!= PVM_SUCCESS){
	//		//--------------------------
	//		// Load the Leve1-1 Process
	//		// Hsien , 2013.01.28
	//		//--------------------------
	//	this->textBoxRenew(L"Error:" + errorCode.ToString());
	//	return 0;
	//}
	//--------------------------
	// Load the Leve1-2 Process
	// Hsien , 2013.01.28
	//--------------------------
	//for each(procInterfaceControl^ e in this->tlpLevel2->Controls){
	//	errorCode = procLoad(this->pvmInstance,e);
	//	if(errorCode != PVM_SUCCESS){
	//		//-------------
	//		//	Error Output
	//		//-------------
	//		//isAnyError = true;
	//		this->textBoxRenew(L"Error:" + errorCode.ToString());
	//		return 0;
	//	}
	//}
	
	//---------------------
	//	no any error , timer engaged
	//---------------------
//	pvmProcApplyAll(this->pvmInstance,procRelease);		// release all process and ready to launch , added by Hsien , 2013.01.11

	this->tsPvmStart->Enabled = false;					// disable the butto
	this->cbOverride->Enabled = false;					// Hsien 2013/6/4

	//----------------------
	//	Start Service
	//----------------------
	this->startService();
	this->textBoxRenew(L"SAP Started");								//

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
	//	STOP Service
	//-------------------------------
	this->stopService();
	//---------------------
	//	no any error , timer engaged
	//---------------------
	//pvmProcApplyAll(this->pvmInstance,procInhibit);		// release all process and ready to launch , added by Hsien , 2013.01.11
	//----------------------
	//	Unload
	//----------------------
	pvmProcApplyAll2(this->pvmInstance,procFinalize);

	//----------------
	//	Unload Level 
	//----------------
//	pvmUnloadProc(this->pvmInstance,this->procInterfaceControlLevel1->pid);				// unload the LEVEL1
	//----------------
	// Unload Level2
	//----------------
	//for each(procInterface::procInterfaceControl^ e in this->tlpLevel2->Controls){
	//	pvmUnloadProc(this->pvmInstance,e->pid);	// Hsien 2013/12/2 , unload specific proc by pid
	//}


	this->tsPvmStart->Enabled = true;
	this->cbOverride->Enabled = true;

	//----------------------
	//	Start Timer
	//----------------------
	this->startService();
	this->textBoxRenew(L"SAP Stopped");

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