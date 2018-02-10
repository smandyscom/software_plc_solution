#include"stdafx.h"
#include "ldeSimulator.h"


using namespace _ldEditor;

int ldSimulatorControl::load(FILE*	ircFile)
{
	IRC_UNIT*				ircInstructions = NULL;
	size_t					ircSize;

	if((ircSize = ircFread(ircFile,&ircInstructions)) < 0)
		return IRC_ERR_OVERSIZE;

	return this->load(ircInstructions,ircSize);;
}

int ldSimulatorControl::load(void *ircInstructions,const size_t insLength)
{
	SIMU_DATA_COMMAND		localCommand;
	int						errorCode;

	//-----
	//	Debug use
	//-----													// Load instruction
	errorCode = pvmLoadProcFromBuffer(this->pvmSimulator
		,ircInstructions
		,insLength
	//	,10
		,SIMU_PROC_PID);

	if(errorCode != PVM_SUCCESS)
		return errorCode;

	//--------------------
	//	Prepare the first command
	//--------------------
	memset(&localCommand,0,sizeof(SIMU_DATA_COMMAND));
//	localCommand.simuPid = errorCode;
	localCommand.simuSignal = SIMU_SIGNAL_NULL;

	//-------------------
	//	Write-in command  , Clear all contents in frame and ready to launch
	//-------------------
	hsWrite((HS_SIGNAL*)this->sharedCommand,&localCommand);
	pvmProcApplySingle(this->pvmSimulator,SIMU_PROC_PID,procRelease);// Release Forbidden Signal 
																	// and ready to launch
	//------------
	//	Start BackgroundWorker and set status as waitForSignal
	//------------
	this->backgroundWorkerPvm->RunWorkerAsync();

	return 1;
}

int ldSimulatorControl::load(const char*	filename)
{
	FILE*	fp;

	fp = fopen(filename,"r");
	if(!fp)
		return 0;

	return this->load(fp);
}

int ldSimulatorControl::terminate()
{
	SIMU_DATA_COMMAND		localCommand;
	localCommand.simuSignal = SIMU_SIGNAL_STOP;

	//-------------------
	//	Write-in command 
	//-------------------
	while(hsWrite((HS_SIGNAL*)this->sharedCommand,&localCommand) == HS_WAIT);

	//---------------
	//	Wait For BackgroundWorker to Stop and Unload Process
	//---------------
	pvmUnloadProc(this->pvmSimulator,SIMU_PROC_PID);

	//----------
	//	then Reset the values in frame , Hsien 2013.01.04
	//	not implement yet
	//----------
	FSMI::resetFrame();

	return 1;
}
//-----------------
//	SINGAL STEP DEBUGGING
//-----------------
int	ldSimulatorControl::singalStep(void*	simuInfo)
{
	SIMU_DATA_COMMAND		localCommand;
	localCommand.simuSignal = SIMU_SIGNAL_STEP;
	//--------
	//	Keep Writing Until Write-In successfully
	//--------
	while(hsWrite((HS_SIGNAL*)this->sharedCommand,&localCommand) == HS_WAIT);
	//--------
	//	To Access Current Status 
	//--------
	this->queryInfo(simuInfo);
	//--------
	//	To Trigger Watch Table
	//--------
	this->triggerWatchTable();

	return 1;
}

//------------------------------------
//	CONTINOUS DEBUGGING
//------------------------------------
//int	ldSimulatorControl::startDebugging()
//{
//	return 1;
//}
//int	ldSimulatorControl::setBreakPoint(const int rungSn)
//{
//	return 1;
//}
//int	ldSimulatorControl::rstBreakPoint(const int rungSn)
//{
//	return 1;
//}
//int	ldSimulatorControl::clrBreakPoint(void)
//{
//	return 1;
//}

//-----------------------
//	Query
//-----------------------
int ldSimulatorControl::queryInfo		(void*	simuInfo)
{
	//-------------
	//	Keep Reading Until Read-Out successfully
	//-------------
	while(hsRead((HS_SIGNAL*)this->sharedInfo,(SIMU_DATA_INFORMATION*)simuInfo) == HS_WAIT);

	return 1;
}


int ldSimulatorControl::triggerWatchTable()
{
	//--------------------------------------
	//	Trigger the Watch Table simoutanously , to refresh to value of requested variable
	//--------------------------------------
	Windows::Forms::KeyEventArgs^	eKd = gcnew Windows::Forms::KeyEventArgs(Windows::Forms::Keys::Enter);
	Windows::Forms::TableLayoutPanelCellPosition^	cellPos;

	for each(Control^ e in this->tlpWatchTable->Controls){
		cellPos = this->tlpWatchTable->GetPositionFromControl(e);
		if(cellPos->Column == 0){
			//-------
			//	Except the banner
			//-------
			this->kdEnterVariable(e,eKd);
		}
	}

	return 1;
}