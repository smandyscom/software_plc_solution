//---------------------------------------------------------
// FILENAME				: PLC_PFCVM.h
// ORIGIN AYTHOR		: Jay Lin
// EDITOR				: Hsien
// NOTE					: Postfix-instruction Virtual Machine  
//
//-------------------------------------------------------------------------------
#ifndef _PLC_PVM_H
#define _PLC_PVM_H
#include<limits.h>
#include<stdio.h>
#include<PLC_PROC.h>


//-----------------------------------
//	PVM Error Codes Definition
//-----------------------------------
#define PVM_SUCCESS_COMMENT		0x01	// exclusive case used to distinguish it is comment line , Hsien , 2013.03.25
#define	PVM_SUCCESS				0x00
#define	PVM_ERR_NO_MEM			0xf1	// commented by Hsien , 2013.01.25 , occurs when allocating instruction
#define PVM_ERR_FAIL_INIT		0xf2
#define	PVM_ERR_BAD_IRC			0xf3
#define	PVM_ERR_PID_REPEAT		0xf4
#define	PVM_ERR_NULL_PROC		0xf5
#define	PVM_ERR_NO_END			0xf6	// no	prog-end , would cause infinite-loop in core , Hsien , 2013.01.25
#define	PVM_ERR_BAD_TAB			0xf7	// Hsien , 2013.03.25
#define	PVM_ERR_FILE_IO			0xf8	// Hsien , 2013.04.25
#define	PVM_ERR_KPRLY_FILE		0xf9	// Hsien 2013/5/23 , data inconsistent , or FILE-IO of KP error
#define	PVM_ERR_OVER_LEVEL		0xfa	// Hsien 2013/11/18
#define	PVM_ERR_OVER_SLOT		0xfb	// Hsien 2013/11/18
#define	PVM_ERR_REPEATED_SLOT	0xfc	// Hsien 2013/11/18
#define	PVM_ERR_UNKNOWN			0xff	// Hsien , 2013.03.07

//----------------------------
//	PFCVM	attributes
//----------------------------
#define	PVM_MODE_DEBUG			0x01	// debug mode , when prog_wait signal in pvm core is available , wait for external signal to resume
#define	PVM_MODE_CONVENTIAL		0x02	// convential mode , prog_wait signal in pvm core is ineffective , Hsien , 2012.10.24
#define	PVM_MODE_SETUP			0x03

#define	PVM_PRIORITY_LEVEL1		0x00	// indicate the index of process container of PVM , for the level1 priority
#define	PVM_PRIORITY_LEVEL2		0x01	// indicate the index of process container of PVM , for the level2 priority
#define	PVM_PRIORITY_MAX_COUNT	2		// // Hsien 2013/11/18

#define	PVM_PROC_MAX_COUNT		32		// max process number the pvm able to load
#define	PVM_PROC_DESCENT_SORT	1
#define	PVM_PROC_ASCENT_SORT	0

// Hsien 2013/11/18
#define	PVM_PROC_LEVEL2_TIME_MULTI		10	// Level2's time base multiplier , Hsien 2013/11/18
#define PVM_PROC_MAX_COUNT_LEVEL1		10
#define PVM_PROC_MAX_COUNT_LEVEL2		PVM_PROC_LEVEL2_TIME_MULTI

typedef struct pvm_st						PVM;
//typedef struct pvm_procload_command_st		PVM_PROCLOAD_CMD;		// added by Hsien , 2013.02.23

//struct pvm_procload_command_st
//{
//	//------------------------------------------------------
//	//	The Standard Format of Process-Loading Command Block
//	//		Hsien , 2013.02.21
//	//------------------------------------------------------
//	FILE*		_fp;
//	size_t		_timeInterval;		// used in procTimeTick Mode
//	PROC_PID	_pid;				// used when process searching
//};
//
struct pvm_st{
	int				mode;			// current running mode , DEBUG or CONVENTIAL
	size_t			procCounts;		// how many procs	were loaded in PVM
	//--------------------
	//	For implementing Priority-Run Mode
	//		Hsien , 2013.01.25
	//--------------------
	size_t			nextProcIndex;						// to indicate what the next process is going to run , Hsien , 2013.01.25
														// the exclusive process ,will be processed at first
														// Hsien , 2013.01.25

	PROC_CONTEXT	procTable[PVM_PROC_MAX_COUNT];		// process container , LEVEL2 process as well
														// the index stand-for SLOT-index , count from zero
	//----------------------
	//	New Schedualing system of PVM
	//		1. each process from level1 and level2 will be called in constant time interval
	//		2. the time interval of each process of level2 would be the amount of level2 slots * level1 time interval
	// Hsien 2013/11/18
	//----------------------
	unsigned char	nextProcIndexLevel2;
	PROC_CONTEXT*	procTableMain	[PVM_PRIORITY_MAX_COUNT];		// the main reference for procTable , [0] point to procTableLevel1 , so on , Hsien 2013/11/18
	PROC_CONTEXT	procTableLevel1	[PVM_PROC_MAX_COUNT_LEVEL1];
	PROC_CONTEXT	procTableLevel2	[PVM_PROC_MAX_COUNT_LEVEL2];

	size_t			timeBase;			// Hsien 2013/11/18 , level1 time base
};

int	pvmInitialize(PVM*	pvm
				  ,const size_t	_timeBase
				  ,int (*startScript)(void*)
				  ,void*	arg);		
							// the prototype of startscipt was changed by 
							//	Hsine , 2013.01.11
//int	pvmFinalize(PVM* pvm);	// Hsien 2013/5/15
int	pvmFinalize(PVM* pvm 
				,int (*stopScript)(void*)
				,void*	arg);	// Hsien 2013/5/15
//---------------------
//	Management (Thread)
//---------------------
int	pvmLoadProcFromFile			(PVM*	pvm
								 ,const char*	filename
								 ,const PROC_PID		_pid);
int	pvmLoadProcFromFp			(PVM*	pvm
								 ,FILE*	fp
								 ,const	PROC_PID	_pid/* added by Hsien , 2013.01.08*/);
int	pvmLoadProcFromBuffer		(PVM*	pvm
								 ,void*			ircSource
								 ,const size_t	insLength
								 ,const PROC_PID		_pid);		// return pid or error-code																	
																	// would duplicate a copy of ircSource
int	pvmLoadFromTab				(PVM* pvm,FILE*	tabfp);				// Hsien , 2013.03.25 , given formatted tab , loading all process described in tab

int	pvmLoadProcFromBuffer2		(PVM*				pvm
								 ,void*				ircSource
								 ,const size_t		insLength
								 ,const PROC_PID	_pid
								 ,const PROC_LEVEL	_level
								 ,const PROC_SLOT	_slotIndex);	// Hsien 2013/11/20
																	// the second form 
int	pvmLoadProcFromFile2			(PVM*	pvm
								 ,const char*	filename
								 ,const PROC_PID	_pid
								 ,const PROC_LEVEL	_level
								 ,const PROC_SLOT	_slotIndex);		// Hsien 2013/11/20
																	// the second form
int	pvmLoadFromTab2			(PVM* pvm,FILE*	tabfp);				// Hsien , 2013.03.25
																	// the second form of tab-file
																	// [filename] [level] [seqence]
int	pvmProcApplyAll2				(PVM*	pvm,int (*_procMethod)(PROC_CONTEXT*));	// Hsien 2013/12/2
																					// Object to 
int	pvmProcDirPreset	(const char*	dirPath);						// Hsien 2013/11/20
int	pvmProcDirUnset		();												// Hsien 2013/11/20

int	pvmUnloadProc				(PVM*	pvm,const PROC_PID	pid);
int	pvmProcApplyAll				(PVM*	pvm,int (*_procMethod)(PROC_CONTEXT*));
int	pvmProcApplySingle			(PVM*	pvm,const PROC_PID	pid,int (*_procMethod)(PROC_CONTEXT*));
PROC_PID		pidRandomize	(PVM*	target/*,PROC_CONTEXT*	proc*/);	// used to genrate pid in random mode , Hsien , 2013.01.08

//int	pvmReset					(PVM*	pvm,const int _cmd);
int	pvmProcSetTimeInterval		(PVM*	pvm,const PROC_PID	pid,const size_t	timerInterval);
//----------------
//	Running Thread
//----------------
int	pvmRunTick		(PVM*	pvm,const int	mode);
int	pvmRunPriority	(PVM*	pvm,const int	_arg);		// running by LEVEL-SLOT
														// in the environment of Timer-Tick
														// Hsien , 2013.01.25
int	pvmRunPriority2	(PVM*	pvm,const int	_arg);		// the second form for running pfc
														// running by LEVEL-SLOT , but amount of slot level2 is fixed
														// time base of level 2 is also fixed
														// Hsien 2013/11/18

#endif