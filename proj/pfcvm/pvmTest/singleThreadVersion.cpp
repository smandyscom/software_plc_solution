#include<conio.h>
#include<PLC_PVM.h>
#include<PLC_FSMI.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define PVM_TIME_BASE	20
extern int startViaDqi();

#define FILE_NAME "d:\\workdir\\vs2005projects\\PLC framework\\resource\\irc_files\\stickCircuit_X100.irc"


int main()
{
	PVM		mainProcessor;

	char	inv;

	pvmInitialize(&mainProcessor
		,PVM_TIME_BASE
		,startViaDqi);

	pvmLoadProcFromFile	(&mainProcessor,FILE_NAME,20);
	pvmLoadProcFromFile	(&mainProcessor,FILE_NAME,20);
	pvmLoadProcFromFile	(&mainProcessor,FILE_NAME,20);
	pvmLoadProcFromFile	(&mainProcessor,FILE_NAME,20);
	pvmLoadProcFromFile	(&mainProcessor,FILE_NAME,20);

	pvmProcApplyAll		(&mainProcessor,procRelease);		// unlock all process to run

	inv = 0;
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
			case 'p':	pvmProcApplyAll(&mainProcessor,procInhibit);break;
			case 'r':	pvmProcApplyAll(&mainProcessor,procRelease);break;
			default:
				break;
		}
//		pvmProcApplyAll(&mainProcessor,procSendSignal);		// send signal all
		pvmProcApplyAll(&mainProcessor,procRun);			// run all

		pvmProcApplyAll(&mainProcessor,procStatusPrint);	// print all
		system("cls");
	}


	pvmFinalize(&mainProcessor);
	FSMI::finalize();

	_CrtDumpMemoryLeaks();	// leakage mainly happened in startScript()	
							// leakage-cleared , Hsien  ,2012.10.29
							// hand-shaking on single-thread is no-problem
							// Hsien , 2012.10.29

	return 1;
}