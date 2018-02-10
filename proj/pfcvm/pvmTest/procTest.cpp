#include<windows.h>		// for multi-threading

#include<PLC_PROC.h>
#include<PLC_IRC.h>
#include<PLC_PFC.h>
#include<stdio.h>

extern int startScript();

int main()
{
	IRC_UNIT*		ircIns		= NULL;
	PFC_UNIT*			pfcIns		= NULL;

	size_t			ircCounts	= 0;
	PROC_CONTEXT	procTest;

	startScript();
	ircCounts
		= irc_fopen("..\\..\\..\\resource\\irc_files\\stickCircuit_X100.irc",&ircIns);
	if(ircCounts == -1)
		return 0;
	pfcIns = (PFC_UNIT*)malloc(ircCounts * sizeof(PFC_UNIT));
	for(size_t	i=0;i<ircCounts;i++)
		if(!irc2pfc(&pfcIns[i],ircIns[i]))
			return 0;

	procInitialize(&procTest
		,2683
		,100
		,pfcIns
		,ircCounts);

	procInhibit		(&procTest);
	procRun			(&procTest);		// won't run pvmCore

	procOffInhibit	(&procTest);
	procRun			(&procTest);		// will	 run pvmCore

	procOffDwell	(&procTest);
	procRun			(&procTest);		// will  run 

	for(size_t i=0;i<10000;i++)
		procRunTick(&procTest,25);

			// seems fine , Hsien , 2012.10.22
			//	test done , no violation occurs , Hsien , 2012.10.22
	return 1;
}