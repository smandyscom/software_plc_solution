//---------------------------------------------------------------------------------------------------------------------------------
// FILENAME			: PLC_RSED.h
// ORIGIN AUTHOR	: Hsien, 2012.06.14
// EDITOR			: Hsien
// NOTES			: generic edge trigger services
//---------------------------------------------------
#include<PLC_RSED.h>
#include<string.h>

static char		lastState	[BUFFER_SIZE];
static FRAME	*risingEdge,*fallenEdge,*edge;

int edgeDetectorInit()
{
	memset(lastState,0,sizeof(lastState));

	return 0;
}

int imageBackup		(FRAME* frm)
{
	if(frm->frmSize > BUFFER_SIZE)
		return 0;	// unable to backup

	memset(lastState,0,sizeof(lastState));
	memcpy(lastState,frm->baseAdr,frm->frmSize);

	return 1;
}

int detectRisingEdge(FRAME* state)
{
	static size_t byteIndex;

	if(!risingEdge)
		return 0;
	if(risingEdge->frmSize != state->frmSize)
		return 0;

	for(byteIndex = 0 ;byteIndex<state->frmSize;byteIndex++)
		risingEdge->baseAdr[byteIndex] = ((char)(~lastState[byteIndex])	& state->baseAdr[byteIndex]	)	;	//	detect rising edge , bitwise not , bitwise and

	return 1;
}
int detectFallenEdge(FRAME*	state)
{	
	static size_t byteIndex;
	
	if(!fallenEdge)
		return 0;
	if(fallenEdge->frmSize != state->frmSize)
		return 0;

	for(byteIndex = 0 ;byteIndex<state->frmSize;byteIndex++)
		fallenEdge->baseAdr[byteIndex] = (	lastState[byteIndex]	&(~state->baseAdr[byteIndex]))	;	//	detect fallen edge , 

	return 1;
}
int detectEdge		(FRAME*	state)
{
	static size_t byteIndex;

	if(!edge)
		return 0;
	if(edge->frmSize != state->frmSize)
		return 0;

	for(byteIndex = 0 ;byteIndex<state->frmSize;byteIndex++)
		edge->baseAdr[byteIndex] = (	lastState[byteIndex]	^ state->baseAdr[byteIndex])	;	//	detect edge  ,XOR operation

	return 1;
}

int linkRisingEdge	(FRAME*	re)
{
	risingEdge = re;

	return 1;
}

int linkFallenEdge	(FRAME*	fe)
{
	fallenEdge = fe;
	
	return 1;
}

int linkEdge		(FRAME*	e)
{
	edge = e;

	return 1;
}