//---------------------------------------------------------
// FILENAME				: PLC_PFCVM.h
// ORIGIN AYTHOR		: Jay Lin
// EDITOR				: Hsien
// NOTE					: Postfix-instruction Virtual Machine  
//	Notes:	time-driven virtual machine , it has to be put in constant-time calling frame
//				controlled many divided thread(fiber) , they are also in time driven style.
//------------------------------------------------------------------------------------------
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<time.h>

#include<PLC_FRAME.h>
#include<PLC_IRC.h>
#include<PLC_PFC.h>
#include<PLC_PVM.h>

extern int					pvmCore		(void*	startIns, void** currentIns);
											// prototype changed by Hsien , 2012.10.16


static int		pidCompar				(const void*	elem1,const void*	elem2)
{
	//-----------------------
	//	Acend Sorting/Bsearch
	//-----------------------
#if	PVM_PROC_DESCENT_SORT	
	return (((PROC_CONTEXT*)elem2)->pid - ((PROC_CONTEXT*)elem1)->pid);
#else 	
	return (((PROC_CONTEXT*)elem1)->pid - ((PROC_CONTEXT*)elem2)->pid);
#endif
}

static int		pidSort					(PVM*	target)
{
	//--------------------------
	//	organizing before search
	//--------------------------
	//----------------------
	//	would raise serious problem of priority choas
	//----------------------
	// Hsien 2013/5/8
	qsort(target->procTable
		,target->procCounts
		,sizeof(PROC_CONTEXT)
		,pidCompar);

	return PVM_SUCCESS;
}
PROC_PID		pidRandomize			(PVM*	target/*,PROC_CONTEXT*	proc*/)
{
	//--------------------------------------------
	// Hsien 2013/12/2
	// Preparation , do srand() externaly
	//	1. sorting proc-table
	//	2. generate tmpPid
	//	3. bsearch if repeated
	//	4. if not , return ok
	//	5. if yes , re-do step 2 until 4 satisfied
	//--------------------------------------------
	/*unsigned char*/PROC_PID	tmpPid;
	//PROC_PID*	tmpPidPtr = &tmpPid;
	/*void**/int			result;

	//--------------------------
	//	organizing before search
	//--------------------------
	// Hsien 2013/5/7
//	pidSort(target);

#define PID_REPEATED	1
#define	PID_UNIQUE		0

//	srand((unsigned int)time(NULL));	// Hsien 2013/12/2  ,remarked , no need to reset srand frequently
	do{
		//----------------------
		//	Linear Search for repeatitive pid
		// Hsien 2013/12/2
		//----------------------
		tmpPid = rand()%PROC_PID_LESS_THAN/* + 100*/;
		result = PID_UNIQUE;
		for(int i =0;i < (int)target->procCounts;i++)
			if(tmpPid == target->procTable[i].pid)
				result = PID_REPEATED;
		// Hsien 2013/5/7
		//result = bsearch(&tmpPid/*&tmpPidPtr*//*tmpProc*/
		//	,target->procTable
		//	,target->procCounts
		//	,sizeof(PROC_CONTEXT)
		//	,pidCompar);
	}while(result == PID_REPEATED		// NULL as FOUNDED , Hsien 2013/5/7
		|| tmpPid == 0);		// if pid repeated or Zero pid , re-mark

	//proc->pid = tmpPid;

	return tmpPid;
}


static PROC_CONTEXT*	pidSearch	(PVM*	target,/*unsigned char*/PROC_PID	pid)
{
	//------------------------------
	//	do binary-search after qsort
	//------------------------------
	PROC_CONTEXT*	result = NULL;
	PROC_PID*		pidPtr = &pid;

	pidSort(target);

	//--------------------------
	//	get reference of pointer
	//--------------------------
	result = (PROC_CONTEXT*)bsearch(&pid/*&pidPtr*/
		,target->procTable
		,target->procCounts
		,sizeof(PROC_CONTEXT)
		,pidCompar);

	return result;/*NULL means no corresponding process searched*/
}



//------------
//	Start
//------------
int	pvmInitialize(PVM*	pvm
				  ,const size_t	_timeBase
				  ,int (*startScript)(void*)/* return 0: SUCCESS*/
				  ,void*	arg)		
							// the prototype of startscipt was changed by 
							//	Hsine , 2013.01.11
{
	//--------------------------
	//	1. Reset All Value
	//	2. Set time-base value
	//	3. 
	//--------------------------
	int	errorCode = 0;

	memset(pvm,0,sizeof(PVM));
	pvm->timeBase		= _timeBase;
	pvm->nextProcIndex			= PVM_PRIORITY_LEVEL2;	// Hsien 2013/7/22 , should be initialized

	pvm->nextProcIndexLevel2	= 0;					// Hsien 2013/11/18 , start from first entry
	pvm->procTableMain[0] = pvm->procTableLevel1;		// Hsien 2013/11/18
	pvm->procTableMain[1] = pvm->procTableLevel2;		// Hsien 2013/11/18

//-----------------------------------
//	Set The Default Value of Pid
//-----------------------------------
#if		PVM_PROC_DESCENT_SORT	
	for(int i=0;i<PVM_PROC_MAX_COUNT;i++)
		pvm->procTable[i].pid = 0;
#else
	//----------------------
	//	PVM_PROC_ASCENT_SORT
	//----------------------
	for (int i=0;<PVM_PROC_MAX_COUNT;i++)
		pvm->procTable[i].pid = USHRT_MAX;
#endif


	if(startScript != NULL)				
		errorCode = startScript(arg);			// call-back startScript function

	if(errorCode != PVM_SUCCESS)		// Hsien 2013/5/21 , in heritant PVM return code
		return PVM_ERR_FAIL_INIT;

	return PVM_SUCCESS;
}

int	pvmFinalize(PVM* pvm 
				,int (*stopScript)(void*)
				,void*	arg)	// Hsien 2013/5/15
{
	int	errorCode = 0;

	//----------------------
	//	1. Unloading All Process
	//	2. Do Finalizer Script
	//	3. memset PVM
	//----------------------
	// Hsien 2013/5/15
	pvmProcApplyAll(pvm,procFinalize);

	if(stopScript != NULL)				
		errorCode = stopScript(arg);			// call-back startScript function
	
	if(errorCode != PVM_SUCCESS)		// Hsien 2013/5/21 , in heritant PVM return code
		return PVM_ERR_FAIL_INIT;


	memset(pvm,0,sizeof(PVM));

	return PVM_SUCCESS;
}
//---------------------
//	Management (Thread)
//---------------------
int	pvmLoadProcFromFile			(PVM*	pvm
								 ,const char*	filename
								 ,const PROC_PID	_pid)
{
	//------------------------------------------
	//	source : irc-file name
	//	return : positive pid , if successfully
	//			 negtive error-code , if failed
	// given file , output procHandle if success
	//------------------------------------------
	size_t		instructionLength	= 0;
	IRC_UNIT*	ircInstruction		= NULL;
	int			errorCode;
	FILE*	fp;

	//----------------------
	//	First Try with rb mode , // Hsien 2013/4/29
	//----------------------
	fp = fopen(filename,"rb");		// Mode Setted , Hsien , 2013.04.25
	if(!fp)
		return PVM_ERR_FILE_IO;		// error occured
	instructionLength = ircFread(fp,&ircInstruction);
	if(instructionLength < 0/*== IRC_ERR_UNKNOWN*/)
		return PVM_ERR_FILE_IO;					// irc instruction failed
	errorCode = pvmLoadProcFromBuffer(pvm
		,ircInstruction
		,instructionLength
		,_pid);

	free(ircInstruction);						// whether success or not , free the indimediate buffer
	fclose(fp);

	return errorCode;
}
int	pvmLoadProcFromFile2			(PVM*	pvm
								 ,const char*	filename
								 ,const PROC_PID	_pid
								 ,const PROC_LEVEL	_level
								 ,const PROC_SLOT	_slotIndex)
{
	//----------------------
	//	Second Form
	// Hsien 2013/11/20
	//----------------------
	size_t		instructionLength	= 0;
	IRC_UNIT*	ircInstruction		= NULL;
	int			errorCode;
	FILE*	fp;

	//----------------------
	//	First Try with rb mode , // Hsien 2013/4/29
	//----------------------
	fp = fopen(filename,"rb");		// Mode Setted , Hsien , 2013.04.25
	if(!fp)
		return PVM_ERR_FILE_IO;		// error occured
	instructionLength = ircFread(fp,&ircInstruction);
	if(instructionLength < 0/*== IRC_ERR_UNKNOWN*/)
		return PVM_ERR_FILE_IO;					// irc instruction failed
	errorCode = pvmLoadProcFromBuffer2(pvm
		,ircInstruction
		,instructionLength
		,_pid
		,_level
		,_slotIndex);

	free(ircInstruction);						// whether success or not , free the indimediate buffer
	fclose(fp);

	return errorCode;
}
int	pvmLoadProcFromFp			(PVM*	pvm
								 ,FILE*	fp
//								 ,const size_t		_timeInterval
								 ,const	PROC_PID	_pid/* added by Hsien , 2013.01.08*/)
{
	//-------------------------------------
	//	Added by Hsien  ,2013.01.07
	//		fileName call FILE* call Buffer
	//-------------------------------------
	IRC_UNIT*		ircInstruction	= NULL;
	int				ircSize			= 0;
	int				returnCode;

	if( (ircSize = ircFread(fp,&ircInstruction)) < 0){
		free(ircInstruction);

		return PVM_ERR_BAD_IRC;
	}

	returnCode =  pvmLoadProcFromBuffer(pvm
		,ircInstruction
		,ircSize
//		,_timeInterval
		,_pid);

	free(ircInstruction);

	return returnCode;
}

static int pvmLoadProcBasic(void* ircSource
							,const size_t insLength
							,void**	_pfcInstruction/* output */)
{
	//----------------------
	//	THe common procedure for loading instruction from buffer , both for FromBuffer and FromBuffer2
	// Hsien 2013/11/18
	//----------------------
	//---------------------------------------------
	//	copy source(IRC-format) into internal space(PFC-format)
	//---------------------------------------------
	IRC_UNIT*	ircInstruction = (IRC_UNIT*)ircSource;
	PFC_UNIT*	pfcInstruction = NULL;
	PFC_UNIT*	endInstruction;

	//--------------------------------------------------------------
	//	Security Check  And Allocating memory and do conversion directly
	//		conversion move would implicit induce deep-copy behavior
	//		Hsien , 2012.10.24
	//--------------------------------------------------------------
	pfcInstruction = (PFC_UNIT*)malloc(sizeof(PFC_UNIT) * insLength);
	if(pfcInstruction == NULL)
		return PVM_ERR_NO_MEM;

	for(size_t	i=0;i<insLength;i++){
		if(irc2pfc(&pfcInstruction[i],ircInstruction[i]) != IRC_SUCCESS){
			free(pfcInstruction);
			return PVM_ERR_BAD_IRC;
		}//if
	}//for

	endInstruction = &(pfcInstruction[insLength-1]);
	if(endInstruction->unitType != PROGCTRL
		|| endInstruction->progCtrlData.ctrlCmd != PROG_END){
			free(pfcInstruction);
			return PVM_ERR_NO_END;
	}

	*_pfcInstruction = pfcInstruction;		// passout reference
	return PVM_SUCCESS;
}

int	pvmLoadProcFromBuffer		(PVM*				pvm
								 ,void*				ircSource
								 ,const size_t		insLength
//								 ,const size_t		_timeInterval
								 ,const	PROC_PID	_pid			/* added by Hsien , 2013.01.08*/)
{
	//---------------------------------------------
	//	copy source(IRC-format) into internal space(PFC-format)
	//---------------------------------------------
	//IRC_UNIT*	ircInstruction = (IRC_UNIT*)ircSource;
	PFC_UNIT*	pfcInstruction = NULL;
	int			errorCode;
	//PFC_UNIT*	endInstruction;

	////--------------------------------------------------------------
	////	Security Check  And Allocating memory and do conversion directly
	////		conversion move would implicit induce deep-copy behavior
	////		Hsien , 2012.10.24
	////--------------------------------------------------------------
	//pfcInstruction = (PFC_UNIT*)malloc(sizeof(PFC_UNIT) * insLength);
	//if(pfcInstruction == NULL)
	//	return PVM_ERR_NO_MEM;

	//for(size_t	i=0;i<insLength;i++){
	//	if(irc2pfc(&pfcInstruction[i],ircInstruction[i]) != IRC_SUCCESS){
	//		free(pfcInstruction);
	//		return PVM_ERR_BAD_IRC;
	//	}//if
	//}//for

	//endInstruction = &(pfcInstruction[insLength-1]);
	//if(endInstruction->unitType != PROGCTRL
	//	|| endInstruction->progCtrlData.ctrlCmd != PROG_END){
	//		return PVM_ERR_NO_END;
	//}
	errorCode = pvmLoadProcBasic(ircSource,insLength,(void**)&pfcInstruction);
	if(errorCode != PVM_SUCCESS)
		return errorCode;


	//procFinalize(&pvm->procTable[pvm->procCounts];	// to prevent repeative initilizing
	if(!procInitialize(&pvm->procTable[pvm->procCounts]
	,_pid											// mark pid simaltanously
//		,_timeInterval
		,pfcInstruction
		,insLength)){
			free(pfcInstruction);
			return PVM_ERR_FAIL_INIT;	// intializing failed somehow
	}
	pvm->procCounts++;				// count-up number of process'

	// Hsien 2013/5/7	, bug! would mess-up priority sorting
//	pidSort(pvm);					// sort process by Pid

	return PVM_SUCCESS;	
}
int	pvmLoadProcFromBuffer2		(PVM*				pvm
								 ,void*				ircSource
								 ,const size_t		insLength
								 ,const PROC_PID	_pid
								 ,const PROC_LEVEL	_level
								 ,const PROC_SLOT	_slotIndex)
{
	//----------------------
	//	The Second Form for Loading irc into buffer
	//	Loading instruction and convert into internal format
	//		and assign each process into corresponding Level and Slot , according to the input argument
	// Hsien 2013/11/18
	//----------------------
	PFC_UNIT*		pfcInstruction = NULL;
	PROC_CONTEXT*	proc = NULL;
	int			errorCode;

	//----------------------
	//	Secure check , Over index
	//----------------------
	if(_level > PVM_PRIORITY_LEVEL2)
		return PVM_ERR_OVER_LEVEL;
	if(_level == PVM_PRIORITY_LEVEL1 && _slotIndex >= PVM_PROC_MAX_COUNT_LEVEL1)
		return PVM_ERR_OVER_SLOT;
	if(_level == PVM_PRIORITY_LEVEL2 && _slotIndex >= PVM_PROC_MAX_COUNT_LEVEL2)
		return PVM_ERR_OVER_SLOT;
	//----------------------
	//	Check repetive slot
	//----------------------
	if(((PROC_CONTEXT*)(pvm->procTableMain[_level]))[_slotIndex].pid != 0)
		return PVM_ERR_REPEATED_SLOT;

	errorCode = pvmLoadProcBasic(ircSource,insLength,(void**)&pfcInstruction);
	if(errorCode != PVM_SUCCESS)
		return errorCode;

	proc = &((PROC_CONTEXT*)(pvm->procTableMain[_level]))[_slotIndex];
	if(!procInitialize(proc
		,_pid											// mark pid simaltanously
		,pfcInstruction
		,insLength)){
			free(pfcInstruction);
			return PVM_ERR_FAIL_INIT;	// intializing failed somehow
	}
	procRelease(proc);					// Hsien 2013/12/2 , releasing this slot , ready to lanuch

	return PVM_SUCCESS;
}
int	pvmUnloadProc				(PVM*	pvm,const PROC_PID	pid)
{
	//--------------------------
	//	Remove Process From List:
	//		1. Search the Address of corresponding PID
	//		2. Do process Finalize
	//		3. un-count the number of Processes
	//--------------------------
	PROC_CONTEXT*	targetProc = pidSearch(pvm,pid);
	if(targetProc == NULL)
		return PVM_ERR_NULL_PROC;			// no correspoding process

	procFinalize(targetProc);

	//memset(targetProc,0,sizeof(PROC_CONTEXT));	// Hsien 2013/5/15 , redundant
	pvm->procCounts--;

	return PVM_SUCCESS;
}
int	pvmProcApplyAll				(PVM*	pvm,int (*_procMethod)(PROC_CONTEXT*))
{
	//----------------------------
	//	Generic Iteration Methods , input call-back function to determine
	//		what action will applied
	//		Hsien , 2012.10.24
	//----------------------------

//	pidSort(pvm);	// cause priority problem , Hsien 2013/5/8

	for(size_t i=0;i<pvm->procCounts;i++)
		_procMethod(&pvm->procTable[i]);

	return PVM_SUCCESS;
}

int	pvmProcApplyAll2				(PVM*	pvm,int (*_procMethod)(PROC_CONTEXT*))
{
	//----------------------------
	//	Generic Iteration Methods , input call-back function to determine
	//		what action will applied
	// Hsien 2013/12/2
	//----------------------------

	for(size_t i=0;i<PVM_PROC_MAX_COUNT_LEVEL1;i++)
		_procMethod(&pvm->procTableLevel1[i]);
	for(size_t i=0;i<PVM_PROC_MAX_COUNT_LEVEL2;i++)
		_procMethod(&pvm->procTableLevel2[i]);

	return PVM_SUCCESS;
}


int	pvmProcApplySingle			(PVM*	pvm,const PROC_PID	pid,int (*_procMethod)(PROC_CONTEXT*))
{
	//----------------------------
	//	Generic Call-back Method , input call-back function to determine
	//		what action will applied
	//		Hsien , 2012.10.24
	//----------------------------
	//----------------------
	//	Following Statement would raise priority problem
	//		// Hsien 2013/5/8
	//----------------------
	pidSort(pvm);
	PROC_CONTEXT*	targetProc = pidSearch(pvm,pid);

	if(targetProc == NULL)
		return PVM_ERR_NULL_PROC;			// no correspoding process

	return _procMethod(targetProc);
}

//----------------
//	Running Thread
//----------------
int	pvmRunTick		(PVM*	pvm,const int	mode)
{
	//------------------------------------
	//	Going to Define MODE-Behavior in PVM_CORE
	//		Hsien , 2012.10.24
	//------------------------------------
	switch(mode){
		case PVM_MODE_DEBUG:
			// pvmCore(PVM_DEBUG);
		case PVM_MODE_CONVENTIAL:
			break;
	}

	for(size_t i=0;i<pvm->procCounts;i++)
		procRunTick(&pvm->procTable[i],pvm->timeBase);

	return PVM_SUCCESS;
}

int	pvmRunPriority(PVM*	pvm,const int _arg)
{
	//-----------------------------------------------
	// running all process in sequence of LEVEL-SLOT
	//		i.e LEVEL1 , LEVEL2-SLOT1 , -SLOT2 , .... 
	// in the environment of Timer-Tick
	// Hsien , 2013.01.25
	//		1. Always Run Level1 Process
	//		2. Ring-Running The Process Stored in procTable(LEVEL2)
	//-----------------------------------------------

	procRun(&pvm->procTable[PVM_PRIORITY_LEVEL1]/*&(pvm->procLevel1)*/);						// always run level1
	procRun(&(pvm->procTable[pvm->nextProcIndex]));		// run the slot in level2 according to index
	pvm->nextProcIndex++;								// counting the index , start form 1

	//----------------------
	//	Fixing LV2-Rotation unexpecting problem
	// Hsien 2013/7/22
	//----------------------
	// when pvm->proCounts = 0 , pvm->procCounts -1 will be overflowed 
	//, Hsien 2013/11/18
	if(pvm->nextProcIndex > (pvm->procCounts/* -1*/))		// set to zero if over-counted
		pvm->nextProcIndex = PVM_PRIORITY_LEVEL2/*0*/;

	return PVM_SUCCESS;
}

int pvmRunPriority2(PVM* pvm,const int _arg)
{
	//----------------------
	// running all process in sequence of SLOT for each LEVEL
	//	but Level1 in highest priority , every process in Level1 will be executed for every base time
	//	for Level2 , run only one process for every base time , every process will be recalled in every 10 times of base time(default)
	//		i.e | LEVEL1-1 LEVEL1-2...LEVEL1-n LEVEL2-1 | LEVEL1-1...LEVEL1-n LEVEL2-2 | ...so on
	//	
	// Hsien 2013/11/18
	//----------------------

	//----------------------
	//	Level1 execution
	// Hsien 2013/11/18 , stack allocating cost down performance
	//----------------------
	for (unsigned char index =0;index < PVM_PROC_MAX_COUNT_LEVEL1;index++)
		procRun(&pvm->procTableLevel1[index]);

	//----------------------
	//	Level2 execution
	//----------------------
	procRun(&pvm->procTableLevel2[pvm->nextProcIndexLevel2]);
	pvm->nextProcIndexLevel2++;
	if(pvm->nextProcIndexLevel2 > (PVM_PROC_MAX_COUNT_LEVEL2 -1))
		pvm->nextProcIndexLevel2 = 0;	// Hsien 2013/11/18 , reset


	return PVM_SUCCESS;
}
//------------------
//	Debug Interface
//------------------
int	pvmGetDebugInfo	(PVM*	pvm,const PROC_PID pid);		// get insNum information of PFC_PROGCTRL
int	pvmGetValue		(PVM*	pvm);							// 
int	pvmSetValue		(PVM*	pvm,const int	value);

//-----------------------------------------------------
//	going to implemented in G3 PVM , Hsien , 2012.10.24
//-----------------------------------------------------
int	(*pvmCreateLocalMemory)		(PVM* pvm,const PROC_PID pid,const FID frmId,const size_t frmSize)	= NULL ; // renamed function for FSMI
int	(*pvmDeleteLocalMemory)		(PVM* pvm,const PROC_PID pid,const FID frmId)						= NULL ; // renamed function for FSMI

int	(*pvmStartRoutineService)	(PVM* pvm)		= NULL;	// renamed function from RSMI , for pausing PVM into IO-maintaining mode
int	(*pvmStopRoutineService)	(PVM* pvm)		= NULL;	// renamed function from RSMI , for pausing PVM into IO-maintaining mode	

//-----------------------------------------------------
//	PVM	Loading Via Table File
//-----------------------------------------------------

#define	PROCLOAD_FORMAT_COMMENT		"#%s['\n']"
#define	PROCLOAD_FORMAT_DIR			"DIR=%s"			// Hsien , 2013.04.01

#define PROCLOAD_FORMAT_PROC		"[%d]%s%s"
//#define PROCLOAD_FORMAT_PROC		"\[%d\]%s%s"
#define	PROCLOAD_FORMAT_PROC2		"%s%u%u"			// [filename][level][slot] Hsien 2013/11/20
#define	PROCLOAD_MATCH_COUNT_PROC		3	
#define	PROCLOAD_MATCH_COUNT_PROC2		3

#define PROCLOAD_FORMAT_EMPTY		""

#define	PROCLOAD_MATCH_COUNT_COMMENT	1
#define	PROCLOAD_MATCH_COUNT_DIR		1


#define	PROCLOAD_STR_PID_RANDOM			"random"

#pragma	warning (disable:4996) 

#define PVM_SUCCESS_COMMENT 0x01

//----------------------
//	DIR Name presetting
// Hsien 2013/11/20
//----------------------
static char*	pvmProcLoadDir = NULL;
int	pvmProcDirPreset(const char*	dirPath)
{
	size_t ln = strlen(dirPath);
	pvmProcLoadDir = (char*)malloc(ln+1);
	strcpy(pvmProcLoadDir,dirPath);
	
	return PVM_SUCCESS;
}
int	pvmProcDirUnset	()
{
	free(pvmProcLoadDir);
	pvmProcLoadDir = NULL;

	return PVM_SUCCESS;
}


/*static*/ int procLoadDir(const char*	_sourceLineStr/*FILE* tabFp*/
				   ,char*		_dirNameStr)
{
	int matchCount;
	//-------------------------------------
	//	Turns Raw String into Sperated Formatted String
	//-------------------------------------
	matchCount = sscanf(
		_sourceLineStr
		,PROCLOAD_FORMAT_DIR
		,_dirNameStr);

	if(matchCount != PROCLOAD_MATCH_COUNT_DIR){
		//--------------------------------------------------
		//	If this line is not represented as Command Line
		//		Check if comment
		//--------------------------------------------------
		matchCount = sscanf(_sourceLineStr
			,PROCLOAD_FORMAT_COMMENT
			,_dirNameStr);
		if(matchCount != PROCLOAD_MATCH_COUNT_COMMENT)
			//------------
			//	Error
			//------------
					//------------------------------------------------------------
					//	Ignore Empty Line , Hsien , 2013.04.01 , not implement yet
					//------------------------------------------------------------
			return PVM_ERR_BAD_TAB;
		else{
			//--------------
			//	Clear Buffer
			//--------------
			return PVM_SUCCESS_COMMENT;	// return as comment
		}
	}

	return PVM_SUCCESS;
}

static int procLoadByLine(const char*	_sourceLineStr/*FILE* tabFp*/
				   ,int*		_level
				   ,char*		_filenameStr
				   ,char*		_pidStr)
{
	int matchCount;
	//-------------------------------------
	//	Turns Raw String into Sperated Formatted String
	//-------------------------------------
	matchCount = sscanf(
		_sourceLineStr
		,PROCLOAD_FORMAT_PROC
		,_level
		,_filenameStr
		,_pidStr);
	if(matchCount != PROCLOAD_MATCH_COUNT_PROC){
		//--------------------------------------------------
		//	If this line is not represented as Command Line
		//		Check if comment
		//--------------------------------------------------
		matchCount = sscanf(_sourceLineStr
			,PROCLOAD_FORMAT_COMMENT
			,_filenameStr);
		if(matchCount != PROCLOAD_MATCH_COUNT_COMMENT)
			//------------
			//	Error
			//------------
					//------------------------------------------------------------
					//	Ignore Empty Line , Hsien , 2013.04.01 , not implement yet
					//------------------------------------------------------------
			return PVM_ERR_BAD_TAB;
		else{
			//--------------
			//	Clear Buffer
			//--------------
			return PVM_SUCCESS_COMMENT;	// return as comment
		}
	}

	return PVM_SUCCESS;
}

static int procLoadByLine2(const char*		_sourceLineStr
						   ,char*			_filenameStr
						   ,PROC_LEVEL*		_level
						   ,PROC_SLOT*		_slot)
{
	int		matchCount;
	char	anyChar = 0;
	char	nonWhiteCharCount = 0;

	//------------------------------------------------------------
	//	Ignore Empty Line , Hsien , 2013.04.01 , not implement yet
	//------------------------------------------------------------
	for(int i=0;;i++){
		//----------------------
		//	Scanning until carrige return reached
		//----------------------
		if(_sourceLineStr[i] == 13
			|| _sourceLineStr[i] == 10)
			break;
		//----------------------
		//
		//----------------------
		if(
			 _sourceLineStr[i] != '\t'
			&& _sourceLineStr[i] != 9
			&& _sourceLineStr[i] != 32)
			nonWhiteCharCount++;
	}
	if(nonWhiteCharCount == 0)
		return PVM_SUCCESS_COMMENT;

	//----------------------
	//	Ignore Comment
	//----------------------
	unsigned int	level32;
	unsigned int	slot32;

	matchCount = sscanf(_sourceLineStr
			,PROCLOAD_FORMAT_COMMENT
			,_filenameStr);
	if(matchCount == PROCLOAD_MATCH_COUNT_COMMENT)
			return PVM_SUCCESS_COMMENT;	// return as comment

	//-------------------------------------
	//	Turns Raw String into Sperated Formatted String
	//-------------------------------------
	matchCount = sscanf(
		_sourceLineStr
		,PROCLOAD_FORMAT_PROC2
		,_filenameStr
		,&level32
		,&slot32);
	*_level =	(PROC_LEVEL)level32;
	*_slot	=	(PROC_SLOT)slot32;

	if(matchCount != PROCLOAD_MATCH_COUNT_PROC2)
			return PVM_ERR_BAD_TAB;

	return PVM_SUCCESS;
}

#define	PROCLOAD_BUFFER_SIZE_RAW		128
#define	PROCLOAD_BUFFER_SIZE_FILENAME	128
#define	PROCLOAD_BUFFER_SIZE_DIRNAME	128
#define	PROCLOAD_BUFFER_SIZE_FULLNAME	PROCLOAD_BUFFER_SIZE_DIRNAME + PROCLOAD_BUFFER_SIZE_FILENAME
#define	PROCLOAD_BUFFER_SIZE_PID		32


#define PROCLOAD_STATE_BEGIN	0
#define PROCLOAD_STATE_DIR		1		// loading dir name and ignore comment
#define PROCLOAD_STATE_PROC		2		// loading proc and ignore comment
#define PROCLOAD_STATE_ERROR	3		// error state to finalize pvm
#define PROCLOAD_STATE_END		4		// end state to return value and set state back

int	pvmLoadFromTab					(PVM* pvm,FILE*	tabfp)
{
	//-----------------------------------------------------------
	//	Line By Line Loading Process , Eject If any Error Occurs
	//		Hsien , 2013.03.07
	//		1. get raw line string
	//		2. scan formatted string
	//		3. do pid command
	//		4. load into pvm
	//-----------------------------------------------------------
	char	BufRaw		[PROCLOAD_BUFFER_SIZE_RAW];
	char	BufFilename	[PROCLOAD_BUFFER_SIZE_FILENAME];
	char	BufDirname	[PROCLOAD_BUFFER_SIZE_DIRNAME];
	char	BufFullname	[PROCLOAD_BUFFER_SIZE_FULLNAME];
	char	BufPid		[PROCLOAD_BUFFER_SIZE_PID];
	int		BufLevel;

	int		state = PROCLOAD_STATE_BEGIN;

	int			errorCode;
	PROC_PID	convertedPid;

	//-------------------------------------------
	//	Accumulating when the EOF doesn't matched
	//-------------------------------------------
	while(1){
		
		switch(state){
			case PROCLOAD_STATE_BEGIN:
				//--------
				//	BEGIN
				//--------
				state = PROCLOAD_STATE_DIR;
				break;
			case PROCLOAD_STATE_DIR:
				//--------
				//	DIR
				//--------
				if(fgets(BufRaw,PROCLOAD_BUFFER_SIZE_RAW,tabfp) == NULL){
					//---------
					//	EOF approched , error
					//---------
					state = PROCLOAD_STATE_ERROR;
				}
				else{
					errorCode = procLoadDir(BufRaw,BufDirname);
					switch(errorCode){
						case PVM_SUCCESS:
							//-----------
							//	LOADING PROC
							//-----------
							state = PROCLOAD_STATE_PROC;
							break;
						case PVM_SUCCESS_COMMENT:
							//-----------
							//	STAY HERE
							//-----------
							break;
						case PVM_ERR_BAD_TAB:
							//-----------
							//
							//-----------
							state = PROCLOAD_STATE_ERROR;
							break;
						default:
							break;
					}//switch
				}//else
				break;
			case PROCLOAD_STATE_PROC:
				if(fgets(BufRaw,PROCLOAD_BUFFER_SIZE_RAW,tabfp) == NULL){
					//----------------------
					//	EOF approched , DONE
					//----------------------
					state = PROCLOAD_STATE_END;
				}
				else{
					//----------------------
					//	Attribute for each Process
					//		1. Level
					//		2. IRC filename
					//		3. Pid
					//----------------------
					errorCode = procLoadByLine(BufRaw
						,&BufLevel
						,BufFilename
						,BufPid);
					switch(errorCode){
						case PVM_SUCCESS:
							//-----------
							//	LOADING PROC
							//-----------
							if(strcmp(BufPid,PROCLOAD_STR_PID_RANDOM) == 0){
								//----------------------------------
								//	Randomize Requset , Do Randomize
								//----------------------------------
								convertedPid = pidRandomize(pvm);
							}
							else{
								//---------------
								//	Direct Request , Do Conversion
								//---------------
								convertedPid = (PROC_PID)strtol(BufPid,NULL,10);
								if(convertedPid == 0){
									state = PROCLOAD_STATE_ERROR;		// invalid string
								}
							}

							//--------
							//	MERGE PATH and Filename
							//--------
							memset(BufFullname,0,PROCLOAD_BUFFER_SIZE_FULLNAME);
							strcpy(BufFullname,BufDirname);
							strcat(BufFullname,BufFilename);

							if((errorCode = pvmLoadProcFromFile(pvm,BufFullname/*BufFilename*/,convertedPid)) != PVM_SUCCESS){
								state = PROCLOAD_STATE_ERROR;		// invalid string
							}
							break;
						case PVM_SUCCESS_COMMENT:
							//-----------
							//	STAY HERE
							//-----------
							break;
						case PVM_ERR_BAD_TAB:
							//-----------
							//
							//-----------
							state = PROCLOAD_STATE_ERROR;
							break;
						default:
							break;
					}
				}
				break;
			case PROCLOAD_STATE_ERROR:
				//------------------
				//	Clear PVM
				//------------------
//				pvmFinalize(pvm);		// clear PVM , // Hsien 2013/5/15
										// Finalizing by External
				return errorCode;
				break;
			case PROCLOAD_STATE_END:
				return PVM_SUCCESS;
				break;
			default:
				break;
				}//switch
		}//while
}//function

int	pvmLoadFromTab2					(PVM* pvm,FILE*	tabfp)
{
	//-----------------------------------------------------------
	//	Line By Line Loading Process , Eject If any Error Occurs
	//		Hsien , 2013.11.18
	//		1. get raw line string
	//		2. scan formatted string
	//		3. attach randomized pid 
	//		4. load into pvm
	//-----------------------------------------------------------
	char	BufRaw		[PROCLOAD_BUFFER_SIZE_RAW];
	char	BufFilename	[PROCLOAD_BUFFER_SIZE_FILENAME];
	char	BufFullname	[PROCLOAD_BUFFER_SIZE_FULLNAME];
	PROC_LEVEL	BufLevel;
	PROC_SLOT	BufSlot	;

	int		state = PROCLOAD_STATE_BEGIN;

	int			errorCode;
	PROC_PID	convertedPid;
	srand((unsigned int)time(NULL));

	//-------------------------------------------
	//	Accumulating when the EOF doesn't matched
	//-------------------------------------------
	while(1){
		
		switch(state){
			case PROCLOAD_STATE_BEGIN:
				//--------
				//	BEGIN
				//--------
				state = PROCLOAD_STATE_PROC;
				break;
			case PROCLOAD_STATE_PROC:
				if(fgets(BufRaw,PROCLOAD_BUFFER_SIZE_RAW,tabfp) == NULL){
					//----------------------
					//	EOF approched , DONE
					//----------------------
					state = PROCLOAD_STATE_END;
				}
				else{
					//----------------------
					//	Attribute for each Process
					//		1. IRC filename
					//		2. Level
					//		3. Slot
					//----------------------
					errorCode = procLoadByLine2(BufRaw
						,BufFilename
						,&BufLevel
						,&BufSlot);
					switch(errorCode){
						case PVM_SUCCESS:
							//-----------
							//	LOADING PROC , Pid assignment
							//-----------
							convertedPid = pidRandomize(pvm);
							//--------
							//	MERGE PATH and Filename
							//--------
							memset(BufFullname,0,PROCLOAD_BUFFER_SIZE_FULLNAME);
							if(pvmProcLoadDir != NULL)
								strcpy(BufFullname,pvmProcLoadDir);				// Hsien 2013/11/20
							strcat(BufFullname,BufFilename);

							if((errorCode = pvmLoadProcFromFile2(pvm
								,BufFullname
								,convertedPid
								,BufLevel
								,BufSlot)) != PVM_SUCCESS){
								state = PROCLOAD_STATE_ERROR;		// invalid string
							}
							break;
						case PVM_SUCCESS_COMMENT:
							//-----------
							//	STAY HERE
							//-----------
							break;
						case PVM_ERR_BAD_TAB:
							//-----------
							//
							//-----------
							state = PROCLOAD_STATE_ERROR;
							break;
						default:
							break;
					}
				}
				break;
			case PROCLOAD_STATE_ERROR:
				//------------------
				//	Clear PVM
				//------------------
//				pvmFinalize(pvm);		// clear PVM , // Hsien 2013/5/15
										// Finalizing by External
				return errorCode;
				break;
			case PROCLOAD_STATE_END:
				return PVM_SUCCESS;
				break;
			default:
				break;
				}//switch
		}//while
}//function
