//---------------------------------------------------------
// FILENAME				: PLC_PROC.h
// ORIGIN AYTHOR		: Hsien , 2012.10.29
// EDITOR				: Hsien
// NOTE					: PLC-Process Context-Definition and Manipulator
//							with build-in thread-safe facility : hand-shake mechanism
//-------------------------------------------------------------------------------
#ifndef _PLC_PROC_H
#define _PLC_PROC_H
#include<time.h>
#include<limits.h>

#undef PROC_ON_MULTI_THREAD						// switch on when under multi-thread environment 
//#define PROC_ON_MULTI_THREAD


typedef struct proc_context_st	PROC_CONTEXT;
typedef struct proc_signal_st	PROC_SIGNAL;
//--------------------------------
//	Error Code Definition
//--------------------------------

//---------------------------------------------------------------------------------------
//	Process Context , used to control process's behavior under time-driven infrastructure
//---------------------------------------------------------------------------------------
#define	PROC_STATUS_INHIBIT		0x00		//	process inhibited , wait for external confirm signal to resume
#define PROC_STATUS_RUNNING		0x01		//	on executing
#define PROC_STATUS_SLEEP		0x10		//	sleeping , wait for next calling when tick comes , coincidence with PROG_END of pfc code
#define	PROC_STATUS_DWELL		0x04		//	process suspended , wait for external signal to trigger

//----------------------
//	For defining new schedualing system
// Hsien 2013/11/18
//----------------------
#define	PROC_LEVEL				unsigned char
#define	PROC_SLOT				unsigned char

#define	PROC_PID				unsigned short		// type of pid
#define	PROC_PID_LESS_THAN		USHRT_MAX			// limitation of pid
#define	PROC_PID_MAIN			0x01				// pid of main program , running @ first order cyc-time

#define	PROC_SIGNAL_ACTIVATE(signal)	signal = (char)1					// indicate signal
#define PROC_SIGNAL_INACTIVATE(signal)	signal = (char)0					// 

#define	PROC_SIGNAL_ABLE_TO_READ	1								// definition of hand-shake-flag
#define PROC_SIGNAL_ABLE_TO_WRITE	0								// definition of hand-shake-flag
#define	PROC_SIGNAL_RESET(_signalPtr)		memset(_signalPtr,0,sizeof(PROC_SIGNAL))

#define PROC_DEFAULT_TIME_INTERVAL	15

struct proc_signal_st
{
	char handShakeFlag	;	// hand-shake flag to ensure thread-safe , Hsien , 2012.10.29

	char inhibit:1		;	// make instruction fall into INHIBIT state
	char release:1		;	// one-shot signal to make process release from INHIBIT	state , and reset current insturction to initial state
	char resume:1		;	// one-shot signal to make process resume  from DWELL	state , would keep current instruction
	char dummy0:1		;
	char dummy1:1		;
	char dummy2:1		;
	char dummy3:1		;
};


struct proc_context_st
{
	PROC_PID		pid;				// process's id , Hsien , 2012.10.16 , the priority
	unsigned short	status;				// process's status , used to control
	unsigned short	debugInfo	;	// the MSB of return value of PVM CORE , indicate the info drain from PROC_CTRL block
	int				returnValue	;	// added by Hsien , internal use

	//------------------------------------------------------
	//	Need to Decouple Signals on Multi-Thread Environment
	//------------------------------------------------------
#ifdef	PROC_ON_MULTI_THREAD
	PROC_SIGNAL	manageSignal	;		// local  signal for only MANAGE THREAD
	PROC_SIGNAL	sharedSignal	;		// shared signal for both MANAGE THREAD and RUNNING THREAD
#endif
	PROC_SIGNAL	runningSignal	;		// local  signal for only RUNNING THREAD

	size_t	timeRunningValue;		// accumulating time , program will be trigger when running value = time interval
	size_t	timeInterval;			// in mini-second(ms)

	size_t	instructionCounts;		// how many instructions this context hold.
	void*	instruction;			// base address of instruction
	void*	currentInstruction;		// recoded current instruction just executed
									// for record next instruction , when program dwelled
									// this reqest is came from programmer's willing
									// then calling dwellHandler(call back function)? wondering if this facility is neccesary

	//----------------
	//	For Watch Dog
	//----------------
	clock_t	startClock;				// for building watch-dog facility
									// and monitoring work loading
	clock_t endClock;
	clock_t	elapsedClocks;			
	double	duringTime;				// in second
};

int procInitialize	(PROC_CONTEXT*	proc
					,const PROC_PID	_pid
//					,const size_t		_timeInterval  , Hsien , 2013.03.07
					,const void*		_instruction
					,const size_t		_instructionCounts);
int	procFinalize	(PROC_CONTEXT*	proc);
//-----------------------------------------
//	Signal Transmission , when Multi-Thread 
//-----------------------------------------
#ifdef PROC_ON_MULTI_THREAD
int	procSendSignal	(PROC_CONTEXT	*proc);		// send signal from MANAGE-COPY to SHARED
#endif
//-----------------------------------------
//	Manage Thread , Write Data into management-copy
//-----------------------------------------
int	procInhibit			(PROC_CONTEXT	*proc);
int	procRelease			(PROC_CONTEXT	*proc);
int	procResume			(PROC_CONTEXT	*proc);
int	procSetTimeInterval	(PROC_CONTEXT	*proc);	//added by Hsien ,2013,03.07
//-----------------------------------------
//	Running Thread
//-----------------------------------------
int	procRunTick		(PROC_CONTEXT	*proc,const size_t	_timeInc);
int procRun			(PROC_CONTEXT	*proc);
//-----------------------------------------
//	Monitor Thread
//-----------------------------------------
int	procStatusPrint		(PROC_CONTEXT*	proc);

int	procQueryStatus		(PROC_CONTEXT*	proc);		// return proc's status  , added by Hsien , 2012.11.27
int	procQueryDebugInfo	(PROC_CONTEXT*	proc);		// return debugging info , added by Hsien , 2012.11.27

#endif