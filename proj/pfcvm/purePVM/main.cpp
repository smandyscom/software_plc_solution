#include<PLC_IRC.h>
#include<PLC_PFC.h>
#include<PLC_PFCVM.h>
#include<PLC_FSMI.h>

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

extern int pvm_init(void);

#define FILE_NAME	"..\\..\\..\\resource\\irc_files\\test.irc"

int main()
{
	//------------------------------------------
	//	0. init pvm
	//	0. readin .irc file
	//	1. allocating irc instruction
	//	1. translate irc->pfc , buildin pfc instrcution
	//	2. running PFC VM until stop signal send
	//	3. memory moniting
	//------------------------------------------
	PFC_UNIT*	pfcInstruction = 0;
	IRC_UNIT*	ircInstruction = 0;
	size_t		instructionSize;
	clock_t		c1,c2;

	if(!pvm_init())
		return 0;
	pvmInitialize(3,10);

	instructionSize = 
		irc_fopen(FILE_NAME,&ircInstruction);
	pfcInstruction = (PFC_UNIT*)malloc(instructionSize*sizeof(PFC_UNIT));

	for(size_t i=0;i<instructionSize;i++)
		irc2pfc((void*)&pfcInstruction[i],ircInstruction[i]);

	pvmProcMount(0,50,pfcInstruction,instructionSize);
//	pvmProcMount(1,30,pfcInstruction,instructionSize);
//	pvmProcMount(2,50,pfcInstruction,instructionSize);


	c1 = clock();
	while(1){
		// running vm every 10ms
		c2 = clock();
		if(((c2-c1)/CLOCKS_PER_SEC) >= 0.01){			
			c1 = clock();
			// scan in
			pvmTickRun();					// tested , worked fine , Hsien , 2012.06.27
			//pvmCore((void*)pfcInstruction);
			// scan out
		}
	}

	return 0;
}