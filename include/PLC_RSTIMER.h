//---------------------------------------------------------------------------------------------------------------------------------
// FILENAME			: PLC_RSTIMER.cpp
// ORIGIN AUTHOR	: Hsien, 2012.06.14
// EDITOR			: Hsien
// NOTES			: generic timer routine services
//						it's able to be applied on Regular/Non-stop , one-shot/modal timer
//						all numeric are represented in incremental , each incremental represent ms.
//						instruction:	1. link correponding state-control frames with linkTimerEn(),linkTimerRun(),linkTimerGoal()
//													these frames represented as control bits.
//										2. link	output bits frame with tickTimer() , it would tick timer and output according to 
//													settings of enable , running value , goalvalue bits.
//										3. then initialize timer by timerInit()
//										4. running routine services in such order:
//														a. enable frame
//														b. running value frame
//														c. goal value frame
//														d. output value frame
//-----------------------------------------------------------------------------------------------------------------------------------
#ifndef _PLC_RSTIMER_H
#define _PLC_RSTIMER_H
#include<PLC_FRAME.h>

void	timerInit		(const size_t _cycTime);	// 0. initialize
int		linkTimerEn		(FRAME* enableBits);		// 1. enable	frame linkin
int		linkTimerRun	(FRAME*	runningValues);		// 2. runnging	frame linkin
int		linkTimerGoal	(FRAME* goalValues);		// 3. goal		frame linkin
int		tickTimer		(FRAME* outBit);			// 4. tick , and output value


#endif