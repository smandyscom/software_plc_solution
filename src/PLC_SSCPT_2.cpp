#include<stdlib.h>
#include<PLC_FSMI.h>

#include<PLC_OPMAG.h>
#include<PLC_OPLIB.h>

using namespace FSMI;
using namespace	OPMAG;

int	startSponWithoutRs(void*)
{
	for(size_t i=0;i<32;i++)
		addFrame(i,64);

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

	//------------
	//	Logical Operation , Hsien , 2013.01.16
	//------------
	addOp(9,false	,EqualTo);
	addOp(10,false	,NotEqualTo);
	addOp(11,false	,GreaterThan);
	addOp(12,false	,SmallerThan);
	addOp(13,false	,GreaterEqual);
	addOp(14,false	,SmallerEqual);


	return 1;
}
