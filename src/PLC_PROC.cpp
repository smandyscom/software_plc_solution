//---------------------------------------------------------
// FILENAME				: PLC_PROC.h
// ORIGIN AYTHOR		: Hsien , 2012.10.29
// EDITOR				: Hsien
// NOTE					: PLC-Process Context-Definition and Manipulator
//							with build-in thread-safe facility : hand-shake mechanism
//-------------------------------------------------------------------------------
#include<PLC_PROC.h>
#include<time.h>
#include<string.h>
#include<stdlib.h.>
#include<stdio.h>

#pragma warning (disable:4996)

extern int					pvmCore		(void*	startIns, void** currentIns);
static int procReset(PROC_CONTEXT*	proc)
{
	//---------------------------------------------------
	//	Reset Function when Process Released from INHIBIT
	//---------------------------------------------------
	proc->currentInstruction = proc->instruction;

	return 1;
}

int procInitialize	(PROC_CONTEXT*	proc
					,const PROC_PID	_pid
//					,const size_t		_timeInterval	, used default time interval , Hsien , 2013.03.07
					,const void*		_instruction
					,const size_t		_instructionCounts)
{
	//---------------------------------
	//	_instruction: PFC instruction , have to be build externally
	//----------------------------------
	memset(proc,0,sizeof(PROC_CONTEXT));

	proc->pid		= _pid;
	proc->status	= PROC_STATUS_INHIBIT;
	proc->timeInterval = PROC_DEFAULT_TIME_INTERVAL/*_timeInterval*/;
	proc->instruction  = (void*)_instruction;
	proc->instructionCounts	= _instructionCounts;

#ifdef PROC_ON_MULTI_THREAD
	proc->manageSignal.inhibit	= 1;	
	proc->sharedSignal.inhibit	= 1;
#endif 
	proc->runningSignal.inhibit	= 1;

	return 1;
}



int	procFinalize	(PROC_CONTEXT*	proc)
{
	//------------------------------------
	//	Release Instruction
	//------------------------------------
	if(proc->instruction != NULL){
		free(proc->instruction);
		proc->instruction = NULL;				// Hsien 2013/5/15
	}
	memset(proc,0,sizeof(PROC_CONTEXT));		// reset the header-data
												// Hsien , 2013.01.28
	//----------------------
	//	Made this slot in forbidden state
	// Hsien 2013/12/2
	//----------------------
	proc->status = PROC_STATUS_INHIBIT;
	proc->runningSignal.inhibit = 1;

	return 1;
}

int	procInhibit		(PROC_CONTEXT *proc)
{
	//---------------------------------------------------------------
	//	Made Given Process to be INHIBITED , interlocked with RELEASE
	//		Under Multi-thread environment , it need hand-shake mechanism
	//		Hsien , 2012.10.30
	//---------------------------------------------------------------
#ifdef  PROC_ON_MULTI_THREAD
	PROC_SIGNAL_ACTIVATE	(proc->manageSignal.inhibit);
	PROC_SIGNAL_INACTIVATE	(proc->manageSignal.release);
#else
	PROC_SIGNAL_ACTIVATE	(proc->runningSignal.inhibit);
	PROC_SIGNAL_INACTIVATE	(proc->runningSignal.release);
#endif
	return 1;
}
int	procRelease	(PROC_CONTEXT *proc)
{
	//----------------------------------------------
	//	Made Given Process to be INHIBITED , interlocked with RELEASE
	//----------------------------------------------
#ifdef PROC_ON_MULTI_THREAD
	PROC_SIGNAL_ACTIVATE	(proc->manageSignal.release);
	PROC_SIGNAL_INACTIVATE	(proc->manageSignal.inhibit);
#else
	PROC_SIGNAL_ACTIVATE	(proc->runningSignal.release);
	PROC_SIGNAL_INACTIVATE	(proc->runningSignal.inhibit);
#endif
	return 1;
}
int	procResume	(PROC_CONTEXT	*proc)
{
	//----------------------------------------------
	//	Resume Process from DWELL status , which is made from instruction
	//----------------------------------------------
#ifdef PROC_ON_MULTI_THREAD
	PROC_SIGNAL_ACTIVATE(proc->manageSignal.resume);
#else
	PROC_SIGNAL_ACTIVATE(proc->runningSignal.resume);
#endif
	return 1;
}
#ifdef PROC_ON_MULTI_THREAD
int	procSendSignal	(PROC_CONTEXT	*proc)
{
	//-----------------------
	//	Hand-shake Mechanism
	//-----------------------
	if(proc->sharedSignal.handShakeFlag
		== PROC_SIGNAL_ABLE_TO_WRITE){
			proc->sharedSignal					= proc->manageSignal;
			proc->sharedSignal.handShakeFlag	= PROC_SIGNAL_ABLE_TO_READ;
	}

	return 1;
}
#endif

int	procRunTick		(PROC_CONTEXT	*proc,const size_t	_timeInc)
{
	//--------------------------------
	//	Run Mode: Tick Run (Under Multi-Thread TIMER Environment)
	//---------------------------------
	proc->timeRunningValue += _timeInc;
	
	if(proc->timeRunningValue
		==	proc->timeInterval){
		procRun(proc);
		proc->timeRunningValue  = 0;		// reset timer
	}
	//-------
	//	Else : when RunningValue > timeInc , need to solve
	//		Hsien, 2013.01.11
	//-------

	return 1;
}
int procRun			(PROC_CONTEXT	*proc)
{
	//---------------------------------------------------------------------
	//	Routine to Run given process , Run Mode : Singal Run
	//	Signal Priority:
	//		1. INHIBIT
	//		2. DWELL
	//		3. SLEEP
	//---------------------------------------------------------------------
#ifdef PROC_ON_MULTI_THREAD
	if(proc->sharedSignal.handShakeFlag 
		== PROC_SIGNAL_ABLE_TO_READ){
		proc->runningSignal					= proc->sharedSignal;
		proc->sharedSignal.handShakeFlag	= PROC_SIGNAL_ABLE_TO_WRITE;
	}
#endif

	//------------------------------
	//	First Priority : INHIBIT
	//------------------------------
	if(proc->runningSignal.inhibit){
		proc->status = PROC_STATUS_INHIBIT;	// // Hsien 2013/11/18 , no need to overwrite again
		return proc->status;
	}

	switch(proc->status){
		case PROC_STATUS_INHIBIT:
			if(proc->runningSignal.release){								
				//-----------------------------------
				// if ACTIVATED
				// reset signal , handshake machanism
				//-----------------------------------
				procReset(proc);						// rewind instruction to the very beginning
				proc->status = PROC_STATUS_RUNNING;
			}
			else			
				//-----------------------------------
				//	INACTIVATED : keep waiting
				//-----------------------------------
				return proc->status;							// hold waiting

			break;
		case PROC_STATUS_DWELL:
			if(proc->runningSignal.resume)
				//-----------------------------------
				// if ACTIVATED
				//-----------------------------------
				proc->status = PROC_STATUS_RUNNING;
			else
				//-----------------------------------
				//	INACTIVATED : keep waiting
				//-----------------------------------
				return proc->status;
			break;
		case PROC_STATUS_SLEEP:
			//----------------------------------------
			//	Process Exited From PROG_END , rewind it
			//----------------------------------------
			procReset(proc);						// rewind instruction to the very beginning
			proc->status = PROC_STATUS_RUNNING;
			break;
		default:
			return 0;		// out of expection , fatal error
			break;
	}

	proc->startClock = clock();					// watch dog started.
	//---------------------------------------
	// if proc dwelled  , do further moves
	//	on construction , Hsien , 2012.06.26
	//---------------------------------------
	/*proc->status*/
	proc->returnValue = pvmCore(proc->currentInstruction
		,&proc->currentInstruction);			// resume from the stop-point
												// edited by Hsien , 2012.11.27
	//---------------------------------------------
	//	Decoding Process Should be Done in external
	//	Avoiding to Affect the performance of PVM
	//		Problem found by Hsien , 2013.01.25
	//---------------------------------------------
	proc->status	= ((unsigned short*)&proc->returnValue)[0];	// added by Hsien , 2012.11.27
	proc->debugInfo = ((unsigned short*)&proc->returnValue)[1];	// added by Hsien , 2012.11.27

	proc->endClock		= clock();
	proc->elapsedClocks	= proc->endClock - proc->startClock;
	proc->duringTime= 
		proc->elapsedClocks / CLOCKS_PER_SEC;

	//--------------------
	//	Reset All Signals
	//--------------------
	PROC_SIGNAL_RESET(&proc->runningSignal);

	return proc->status;			
}

#define	PROC_PRINT_FORMAT			"pid:%4d\tstatus:%s\n"
#define	PROC_STATUS_STR_INHIBIT		"INHIBIT"
#define PROC_STATUS_STR_RUNNING		"RUNNING"
#define PROC_STATUS_STR_SLEEP		"SLEEP"
#define	PROC_STATUS_STR_DWELL		"DWELL"

int	procStatusPrint(PROC_CONTEXT*	proc)
{
	static char statusString[16];

	switch(proc->status){
		case PROC_STATUS_INHIBIT:	strcpy(statusString,PROC_STATUS_STR_INHIBIT);	break;
		case PROC_STATUS_RUNNING:	strcpy(statusString,PROC_STATUS_STR_RUNNING);	break;
		case PROC_STATUS_SLEEP:		strcpy(statusString,PROC_STATUS_STR_SLEEP);		break;
		case PROC_STATUS_DWELL:		strcpy(statusString,PROC_STATUS_STR_DWELL);		break;
	}
	printf(PROC_PRINT_FORMAT,proc->pid,statusString);

	return 1;
}

int	procQueryStatus		(PROC_CONTEXT*	proc)
{
	//----------------------------------------------------
	// return proc's status  , added by Hsien , 2012.11.27
	//----------------------------------------------------
	return proc->status;
}
int	procQueryDebugInfo	(PROC_CONTEXT*	proc)
{
	//-----------------------------------------------------
	// return debugging info , added by Hsien , 2012.11.27
	//-----------------------------------------------------
	return proc->debugInfo;
}

int	procSetTimeInteval	(PROC_CONTEXT*	proc,const size_t	_timeInterval)
{
	//-----------------------------------------------------
	// seprate time setting functin from intializing , added by Hsien , 2013.03.07
	//-----------------------------------------------------
	proc->timeInterval = _timeInterval;
	return 1;
}