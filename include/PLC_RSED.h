//---------------------------------------------------
// FILENAME			: PLC_RSED.h
// ORIGIN AUTHOR	: Hsien, 2012.06.14
// EDITOR			: Hsien
// NOTES			: generic edge trigger services
//---------------------------------------------------
#ifndef _PLC_RSED_H
#define	_PLC_RSED_H
#include<PLC_FRAME.h>

#define BUFFER_SIZE	64 // Hsien 2013/6/5 count in byte

int edgeDetectorInit();
int imageBackup		(FRAME* frm);
int detectRisingEdge(FRAME* state);
int detectFallenEdge(FRAME*	state);
int detectEdge		(FRAME*	state);
int linkRisingEdge	(FRAME*	re);
int linkFallenEdge	(FRAME*	fe);
int linkEdge		(FRAME*	e);

#endif