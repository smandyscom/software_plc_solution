#include<windows.h>
#include<conio.h>		// kbhit
#include<PLC_PVM.h>
#include<PLC_FSMI.h>

#include<stdio.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define PVM_TIME_BASE	20

extern int startViaDqi();

#define FILE_NAME "d:\\workdir\\vs2005projects\\PLC framework\\resource\\irc_files\\stickCircuit_X100.irc"
	
PVM		mainProcessor;
HANDLE	hMutex;
int		__exit = 0;

int  threadPvmRun()
{
	//-----------------------------------------
	//	Data-Collision Problem Encountered
	//		Not good data 
	//	Hsien , 2012.10.25
	//-----------------------------------------
	while(!__exit){
//		WaitForSingleObject(hMutex,INFINITE);
		pvmProcApplyAll(&mainProcessor,procRun);		// run all
//		ReleaseMutex(hMutex);
	}
	ExitThread(1);
	return 1;
}

int	threadMonitor()
{
	while(!__exit){
		//------------------------------------
		//	Status Monitor
		//------------------------------------
		pvmProcApplyAll(&mainProcessor,procStatusPrint);
		system("cls");
	}
	ExitThread(1);
	return 1;
}

int main(int argc, char argv[])
{
	char	inv;
	HANDLE	hPvmRun;
	HANDLE	hMonitor;
	DWORD	tidPvmRun;
	DWORD	tidMonitor;
	DWORD	__exitCodeT1 , __exitCodeT2;

	pvmInitialize(&mainProcessor
		,PVM_TIME_BASE
		,startViaDqi);

	pvmLoadProcFromFile	(&mainProcessor,FILE_NAME,20);
	pvmLoadProcFromFile	(&mainProcessor,FILE_NAME,20);
	pvmLoadProcFromFile	(&mainProcessor,FILE_NAME,20);
	pvmLoadProcFromFile	(&mainProcessor,FILE_NAME,20);
	pvmLoadProcFromFile	(&mainProcessor,FILE_NAME,20);


	pvmProcApplyAll		(&mainProcessor,procRelease);		// unlock all process to run

	//---------------------------
	//	Create Thread to Run Pvm
	//---------------------------
	hPvmRun = CreateThread(NULL
		,0
		,(LPTHREAD_START_ROUTINE)threadPvmRun
		,NULL
		,CREATE_SUSPENDED
		,&tidPvmRun);
	hMonitor = CreateThread(NULL
		,0
		,(LPTHREAD_START_ROUTINE)threadMonitor
		,NULL
		,0
		,&tidMonitor);
	hMutex	= CreateMutex(NULL,0,(LPCWSTR)"mutexForDataShare");		// create mutex

	SetThreadPriority(hPvmRun			,THREAD_PRIORITY_BELOW_NORMAL)	;
	SetThreadPriority(hMonitor			,THREAD_PRIORITY_BELOW_NORMAL)	;
	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_NORMAL)		;

	inv = 0;
	ResumeThread(hPvmRun);
	while(1)				// ctrl+z
	{
		//------------------------------------
		//	Message Loop to Get User's Request
		//------------------------------------
		if(kbhit()){
			inv = (char)getch();
		}
		if(inv == 26)		// ctrl+Z
			break;

		switch(inv){
			case 'p':	
//				WaitForSingleObject(hMutex,INFINITE);
				pvmProcApplyAll(&mainProcessor,procInhibit);
//				ReleaseMutex(hMutex);
				break;
			case 'r':	
//				WaitForSingleObject(hMutex,INFINITE);
				pvmProcApplyAll(&mainProcessor,procRelease);	
//				ReleaseMutex(hMutex);
				break;
			default:
				break;
		}
		pvmProcApplyAll(&mainProcessor,procSendSignal);
	}

	__exit = 1;
	while(1){
		GetExitCodeThread(hPvmRun	,&__exitCodeT1);
		GetExitCodeThread(hMonitor	,&__exitCodeT2);
		if(__exitCodeT1 ==1 && __exitCodeT2 == 1)
			break;
	}

	pvmFinalize(&mainProcessor);
	FSMI::finalize();

	fflush(stdout);
	fflush(stdin);

	_CrtDumpMemoryLeaks();

	return 1;		// process-terminate
}