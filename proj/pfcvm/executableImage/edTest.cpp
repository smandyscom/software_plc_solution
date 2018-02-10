#include<PLC_RSED.h>
//#include<PLC_RSLIB.h>
#include<stdio.h>
#include<string.h>

#define MAIN	0
#define RISING	1
#define FALLEN	2
#define EDGE	3

extern int memoryRefresh(FRAME*	frm);


int main()
{
	FRAME*	edges[4];

	edges[MAIN]		= frmAllocate(1,sizeof(char));
	edges[RISING]	= frmAllocate(2,sizeof(char));
	edges[FALLEN]	= frmAllocate(3,sizeof(char));
	edges[EDGE]		= frmAllocate(4,sizeof(char));

	frmAddRs(edges[MAIN],imageBackup);
	//frmAddRs(edges[MAIN],memoryRefresh);
	frmAddRs(edges[MAIN],detectRisingEdge);
	frmAddRs(edges[MAIN],detectFallenEdge);
	frmAddRs(edges[MAIN],detectEdge);

	frmAddRs(edges[RISING],linkRisingEdge);
	frmAddRs(edges[FALLEN],linkFallenEdge);
	frmAddRs(edges[EDGE],linkEdge);

	edgeDetectorInit();

	while(1){
		frmRunRs(edges[RISING]);		// attention the running sequence
		frmRunRs(edges[FALLEN]);
		frmRunRs(edges[EDGE]);
		frmRunRs(edges[MAIN]);
	}

		// works fine , the testing break point should be setted at the end of imageBackup() , 
		// a very important move to trigger edge detector
		// Hsien , 2012.06.15
}