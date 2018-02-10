#include<stdlib.h>
#include<PLC_FSMI.h>

#include<PLC_OPMAG.h>
#include<PLC_OPLIB.h>

#include<STC_DQI.h>

using namespace FSMI;
using namespace	OPMAG;

//-------------------------------------------------
//	The Start Script with no any	Routine Service
//-------------------------------------------------

int startViaDqi()
//int main()
{
	//-------------------------------------
	//	The Start Script to Initializing PVM , via DQI interface
	//-------------------------------------
	FRMATTR*	ref;
	size_t		counts;

	//--------------------------------------
	//	FRAME INITIALIZING
	//--------------------------------------
	dqiInit("..\\..\\..\\resource\\PLC.DB");
	dqiLoadFrameAttr(&ref,&counts);
	
	FSMI::initialize();
	for(size_t i=0;i<counts;i++)
		addFrame(ref[i].frameId,ref[i].totalSize);

	//---------------------------------------
	//	OP INITIALIZING
	//---------------------------------------
	OPMAG::initialize();
		OPMAG::initialize();
	addOp(1,false	,Add);
	addOp(2,false	,Substract);
	addOp(3,false	,Multiply);
	addOp(4,false	,Divide);
	addOp(5,false	,Or);
	addOp(6,false	,And);
	addOp(7,true	,Not);
	addOp(8,false	,Assign);

	//---------------------------------------
	//	RS	INITIALIZING	(NO ANY RS SO FAR)
	//---------------------------------------

	free(ref);		// release buffer
	dqiClose();
	return 1;
}