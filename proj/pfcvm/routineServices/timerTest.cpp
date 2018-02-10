#include<PLC_RSTIMER.h>
#include<stdio.h>
#include<string.h>
#define EN		0
#define RUN		1
#define GOAL	2
#define OUT		3

#define DEV_NUM	8

int main(void)
{
	FRAME*	timer[4];

	timer[EN]	= frmAllocate(1,DEV_NUM * sizeof(char)/8);
	timer[RUN]	= frmAllocate(2,DEV_NUM * sizeof(short));
	timer[GOAL]	= frmAllocate(3,DEV_NUM * sizeof(short));
	timer[OUT]	= frmAllocate(4,DEV_NUM * sizeof(char)/8);

	frmAddRs(timer[EN],linkTimerEn);
	frmAddRs(timer[RUN],linkTimerRun);
	frmAddRs(timer[GOAL],linkTimerGoal);
	frmAddRs(timer[OUT],tickTimer);

	timerInit(5);

	while(1){
		memset(timer[OUT]->baseAdr,0,timer[OUT]->frmSize);
		for(int i=0;i<4;i++)
			frmRunRs(timer[i]);
	}	// works fine , Hsien , 2012.6.14

	return 0;
}