// pvmTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "d:\workdir\vs2005projects\PLC framework\src\PLC_PFCVM.cpp"

int _tmain(int argc, _TCHAR* argv[])
{
	PVM			pvm;
	PROC_TABLE	tmpTable;
	PROC_CONTEXT*	ptr;

	pvm.procCounts = 5;
//	pvm.procTable  = (PROC_CONTEXT**)malloc(sizeof(PROC_CONTEXT*) * 5);
//	tmpTable	   = (PROC_CONTEXT**)malloc(sizeof(PROC_CONTEXT*) * 5);

	for(int i=0;i<32;i++){
//		pvm.procTable[i] = (PROC_CONTEXT*)malloc(sizeof(PROC_CONTEXT));
		pvm.procTable[i].pid = 0;
	}

	//for(int i=0;i<5;i++)
	//	tmpTable[i] = pvm.procTable[i];

	for(int i=0;i<5;i++){
		pidMark(&pvm,/*pvm.procTable*/&pvm.procTable[i]);	// can't input pvm.procTable directly , would raise dynamic behavior
													// bug found by Hsien , 2012.10.17
		printf("%i\n",pvm.procTable[i].pid);
	}
			// mark done , Hsien , 2012.10.17

	for(int i=0;i<5;i++){
		if(!(ptr = pidSearch(&pvm,pvm.procTable[i].pid)))
			return 0;
	}
			// search done , Hsien , 2012.10.17

	return 0;
}

