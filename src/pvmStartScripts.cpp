#include<stdlib.h>
#include<PLC_FSMI.h>

#include<PLC_OPMAG.h>
#include<PLC_OPLIB.h>

#include<STC_DQI.h>

using namespace FSMI;
using namespace	OPMAG;

#define SAP_TIME_BASE 15

#ifdef ON_LDE	
int	startViaDqiWithoutRs(sqlite3*	_db/*void**/	/*arg*/)
{
	//-------------------------------------
	//	The Start Script to Initializing PVM , via DQI interface , without any RS
	//-------------------------------------
	FRMATTR*	ref;
	size_t		counts;
	int			errorCode = 0;

	//--------------------------------------
	//	FRAME INITIALIZING
	//		not really good for such path-senced loading method
	//			need to improve it , Hsien, 2013.01.09
	//--------------------------------------
	//errorCode = dqiInit("PLC.DB") ;					// presumption , the working path had been setted.
	if(errorCode != DQI_SUCCESS
		&& errorCode != DQI_ERR_REPEAT_INIT)		// repeative initializing is count
													// Hsien , 2013.01.17
		return 0;
	errorCode = dqiLoadFrameAttr(_db,&ref,&counts);
	if(errorCode != DQI_SUCCESS)
		return 0;

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
	//------------
	//	Logical Operation , Hsien , 2013.01.16
	//------------
	addOp(9,false	,EqualTo);
	addOp(10,false	,NotEqualTo);
	addOp(11,false	,GreaterThan);
	addOp(12,false	,SmallerThan);
	addOp(13,false	,GreaterEqual);
	addOp(14,false	,SmallerEqual);

	//---------------------------------------
	//	RS	INITIALIZING	(NO ANY RS SO FAR)
	//---------------------------------------

	free(ref);		// release buffer
//	dqiClose();
	return 1;
}


#else

#include<sqlite3.h>
#include <PLC_RSTIMER.h>	// timer		 routine service
#include <PLC_RSSTA.h>		// scan-in,out	 routine service , for stand-alone arch
#include <PLC_RSED.h>		// edge detector routine service
using namespace FSMI;
using namespace FSMI::RSMI;


int	startViaDqiWithRs(/*const char*	filename*/sqlite3*	_db/*db: externallyinitialized datb-base*/)
{
	//-------------------------------------
	//	The Start Script to Initializing PVM , via DQI interface , with RS
	//-------------------------------------
	FRMATTR*	ref;
	size_t		counts;
	int			errorCode;

	//--------------------------------------
	//	FRAME INITIALIZING
	//		not really good for such path-senced loading method
	//			need to improve it , Hsien, 2013.01.09
	//--------------------------------------
	//errorCode = dqiInit(filename) ;
	//if(errorCode != DQI_SUCCESS)
	//	return 0;

	errorCode = dqiLoadFrameAttr(_db,&ref,&counts);		// Hsien , 2013.04.25
	if(errorCode != DQI_SUCCESS)
		return 0;

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

	//------------
	//	Logical Operation , Hsien , 2013.01.16
	//------------
	addOp(9,false	,EqualTo);
	addOp(10,false	,NotEqualTo);
	addOp(11,false	,GreaterThan);
	addOp(12,false	,SmallerThan);
	addOp(13,false	,GreaterEqual);
	addOp(14,false	,SmallerEqual);

	//---------------------------------------
	//	RS	INITIALIZING
	//---------------------------------------
	edgeDetectorInit();
	timerInit(SAP_TIME_BASE);				// 

	//--------------------------------------
	//	edge detector part , X frames series
	//--------------------------------------
	addRoutineService(15,linkRisingEdge);
	addRoutineService(16,linkFallenEdge);
	addRoutineService(17,linkEdge);
	addRoutineService(18,imageBackup);
	addRoutineService(18,physicalScanIn);
	addRoutineService(18,detectRisingEdge);
	addRoutineService(18,detectFallenEdge);
	addRoutineService(18,detectEdge);
	//--------------------------------------
	//	scan out , Y frame
	//--------------------------------------
	addRoutineService(2,physicalScanOut);
	//--------------------------------------
	//	timer part , TN frames series
	//--------------------------------------
	addRoutineService(6,linkTimerEn);		// TN_EN, enable
	addRoutineService(7,linkTimerRun);		// TN_R	, running
	addRoutineService(8,linkTimerGoal);		// TN_G	, goal
	addRoutineService(9,tickTimer);			// TN_O , output

	//-------------------------------------
	//	Machine Inputs
	//-------------------------------------
	addRoutineService(22,machineScanIn);

	free(ref);		// release buffer
	//dqiClose();

	return 1;
}
#endif 