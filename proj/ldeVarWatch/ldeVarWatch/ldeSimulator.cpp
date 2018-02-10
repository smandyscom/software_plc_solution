// ldeVarWatch.cpp : main project file.

#include "stdafx.h"
#include "ldeSimulator.h"

using namespace _ldEditor;

System::Void ldSimulatorControl::bgworkerPvmThread(System::Object ^/*sender*/
													, System::ComponentModel::DoWorkEventArgs ^/*e*/)
{
	SIMU_DATA_COMMAND		localCommand;
	SIMU_DATA_INFORMATION	localInfo;

	//-----------------------------------------
	//	Initialization
	//-----------------------------------------
	localCommand.simuSignal = SIMU_SIGNAL_NULL;
	localInfo.simuState		= SIMU_STATUS_SIGNAL_WAIT;

	//-----------------------------------------
	//	Service Loop until stop-condition holds
	//-----------------------------------------
	while(localCommand.simuSignal != SIMU_SIGNAL_STOP /* the condition to stop */){
		//------------------------------------
		//	Data-Exchanging ( Shared to Local) , stay in wait-state when no further signal
		//------------------------------------
		if( hsRead((HS_SIGNAL*)this->sharedCommand,&localCommand) == HS_WAIT){
			if(localCommand.simuSignal !=  SIMU_SIGNAL_STOP)
				localCommand.simuSignal = SIMU_SIGNAL_NULL;
		}

		switch(localInfo.simuState){
			//------------------------
			//	
			//------------------------
			case SIMU_STATUS_SIGNAL_WAIT:
				switch(localCommand.simuSignal){
					case SIMU_SIGNAL_STEP:	localInfo.simuState = SIMU_STATUS_RUN_ONCE;	break;
					case SIMU_SIGNAL_START: localInfo.simuState = SIMU_STATUS_RUN_BREAK;break;
					case SIMU_SIGNAL_STOP:
						/* Close this bg worker*/
						break;
					case SIMU_SIGNAL_NULL:	break;		// stay here
				}
				break;
			//------------------------
			//	
			//------------------------
			case SIMU_STATUS_RUN_ONCE:
				pvmProcApplySingle(this->pvmSimulator,SIMU_PROC_PID,procResume);
				pvmProcApplySingle(this->pvmSimulator,SIMU_PROC_PID,procRun);
				localInfo.simuDebugInfo 
					= pvmProcApplySingle(this->pvmSimulator,SIMU_PROC_PID,procQueryDebugInfo);

				localInfo.simuState = SIMU_STATUS_INFO_WRITE_OUT;		// do wait signal
				break;
			//------------------------
			//	Break Point Debugging Mode , would not implement until the step-mode tested well.
			//		goal setted by Hsien , 2013.01.03
			//------------------------
	/*		case SIMU_STATUS_RUN_BREAK :
				pvmProcApplySingle(this->pvmSimulator,localInfo.simuPid,procResume);
				pvmProcApplySingle(this->pvmSimulator,localInfo.simuPid,procRun);
				localInfo.simuDebugInfo
					 = pvmProcApplySingle(this->pvmSimulator,localInfo.simuPid,procQueryDebugInfo);*/
				//--------------------------
				//	Try to Match Break-Table
				//--------------------------
				//searchResult
				//	 = System::Array::BinarySearch(this->breakInsTable,this->simuDebugInfo);	// need to verify how to use
				//---------------------------
				//	if matched , turns status
				//---------------------------
				//if(searchResult >= 0 )
				//	localInfo.simuState = SIMU_STATUS_SIGNAL_WAIT;
				//---------------------------
				//	ELSE : if no any matched , stay in this status
				//---------------------------
				/*break;*/
				//-----------
				//	Data-Exchanging (Local to Shared)
				//-----------
			case SIMU_STATUS_INFO_WRITE_OUT:
				if(hsWrite((HS_SIGNAL*)this->sharedInfo,&localInfo) == HS_SUCCESS)
					localInfo.simuState = SIMU_STATUS_SIGNAL_WAIT;
				break;
		}// state-machine switch
	}// thread , while-loop
}