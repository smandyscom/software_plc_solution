#include<windows.h>
#include<conio.h>		// kbhit
#include<HS_MECH.h>
#include<stdio.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define TRUE_VALUE	0x00132832
#define THREAD_SAFE	1

int		__exit = 0;

typedef struct
{
	HS_SIGNAL	sg;
	short data1;
	short data2;
}TEST_DATA;

TEST_DATA testData;

int  threadWrite()
{
	//-----------------------------------------
	//	Data-Collision Problem Encountered
	//		Not good data 
	//	Hsien , 2012.10.25
	//-----------------------------------------
	int localData;

	while(!__exit){
		localData = TRUE_VALUE;
#if THREAD_SAFE == 0
		testData.data1 = ((short*)&localData)[0];
		testData.data2 = ((short*)&localData)[1];
#else
		hsWrite((HS_SIGNAL*)&testData,&localData);
#endif
	}
	ExitThread(1);
	return 1;
}

int	threadRead()
{
	int localData;

	while(!__exit){
		localData = 0;
#if THREAD_SAFE == 0
		((short*)&localData)[0] = testData.data1;
		((short*)&localData)[1] = testData.data2;
		if(localData != TRUE_VALUE)
			printf("Error\n");
#else
		if(hsRead((HS_SIGNAL*)&testData,&localData) == HS_SUCCESS)
			if(localData != TRUE_VALUE){
			printf("Error\n");
		}
#endif
	}
	ExitThread(1);
	return 1;
}

int main(int argc, char argv[])
{
	char	inv;

	HANDLE	hWrite;
	HANDLE	hRead;
	DWORD	tidWrite;
	DWORD	tidRead;
	DWORD	__exitCodeT1 , __exitCodeT2;

	hsInitial((HS_SIGNAL*)&testData,sizeof(int));			// initialize
//	testData.data = TRUE_VALUE;

	//---------------------------
	//	Create Thread to Run Pvm
	//---------------------------
	hWrite = CreateThread(NULL
		,0
		,(LPTHREAD_START_ROUTINE)threadWrite
		,NULL
		,CREATE_SUSPENDED
		,&tidWrite);
	hRead = CreateThread(NULL
		,0
		,(LPTHREAD_START_ROUTINE)threadRead
		,NULL
		,0
		,&tidRead);

	SetThreadPriority(hWrite			,THREAD_PRIORITY_BELOW_NORMAL)	;
	SetThreadPriority(hRead				,THREAD_PRIORITY_BELOW_NORMAL)	;
	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_NORMAL)		;

	inv = 0;
	ResumeThread(hWrite);
	ResumeThread(hRead);

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
	}

	__exit = 1;
	while(1){
		GetExitCodeThread(hWrite	,&__exitCodeT1);
		GetExitCodeThread(hRead		,&__exitCodeT2);
		if(__exitCodeT1 ==1 && __exitCodeT2 == 1)
			break;
	}

	_CrtDumpMemoryLeaks();

	return 1;		// process-terminate
}