#include<stdio.h>
#include<stdlib.h>
#include <PLC_FSMI.h>
#include <PLC_OPMAG.h>
#include <PLC_OPLIB.h>

#include <PLC_RSTIMER.h>	// timer		 routine service
#include <PLC_RSED.h>		// edge detector routine service

#include <PLC_PVM.h>		// for return codes

using namespace FSMI;
using namespace FSMI::RSMI;
using namespace	OPMAG;

#pragma warning(disable:4138)
#pragma warning(disable:4996)


#ifndef IN_SAP
//----------------------
//	NC PART
//----------------------
	#include	<PLC_RSCNC.h>		// the routine service of CNC exclusively
	#include	<CNC_CONFIG.h>		// for accessing FRAME initializing information , via VAR system
	#include	<PATH.H>			// Hsien 2013/5/21 , for recognizing keeprelay data file

	//-----------------------------------------------
	// corresponding serial number in variable system
	//-----------------------------------------------
	#define VAR_FRMCOUNTS 3100
	#define VAR_FRMID	  3101
	#define VAR_FRMSIZE	  3201

	//------------------------------------------------------
	// following part are controlled by CP_SWITCH.h so far 
	//	would be removed after File-management system build
	//	, Hsien , 2012.06.20
	//------------------------------------------------------
	#define TIME_INTERVAL	10	//ms
	#define	KEEPRELAY_FILENAME PLC_KEEPRELAY_FILENAME	// Hsien 2013/5/21 , in NC
	extern short*				refSyncCfmImage;		// Hsien 2013/5/22 , link to SYNC_CFM frame
	extern short*				refSyncReqImage;		// Hsien 2013/5/22 , link to SYNC_REQ frame
	extern short*			refKoImage	; // refer to KO_IMAGE in localCopySysOutBlock
	extern short*			refKiImage	; // refer to KI_IMAGE in localCopySysInBlock
#else
	//----------------------
	//	SAP PART
	//----------------------
	#define TIME_INTERVAL 15
	#include	<STC_DQI.h>
	#include	<PLC_RSSTA.h>		// scan-in,out	 routine service , for stand-alone arch
	#include	<string.h>
	#define	KEEPRELAY_FILENAME "keepRelayData.kp"	// Hsien 2013/5/21 , in SAP
#endif

int keepRelayReadout()
{	//----------------------
	//	Keep Relays
	//		1. Readout the contents stored in file
	//----------------------
	FILE*	fp				= fopen(KEEPRELAY_FILENAME,"rb");
	FRAME*	keepRelay		= queryFrame(24);
	size_t	readOutCounts	= 0;

	//----------------------
	//	In case of file does not existed
	//----------------------
	if(fp == NULL){
		fp = fopen(KEEPRELAY_FILENAME,"wb+");	// Hsien 2013/5/15 , re-generating file
		if(fp == NULL)
			return PVM_ERR_KPRLY_FILE;			// path doesn't existed
		fwrite(keepRelay->baseAdr,sizeof(char),keepRelay->frmSize,fp);
		rewind(fp);	// Hsien 2013/5/23
		fflush(fp);	// Hsien 2013/5/23
	}
	//----------------------
	//	Always read-out
	//----------------------
	readOutCounts = fread(keepRelay->baseAdr,sizeof(char),keepRelay->frmSize,fp);


	fclose(fp);
	if(readOutCounts != keepRelay->frmSize)
		return PVM_ERR_KPRLY_FILE;		// Hsien 2013/5/15	, FILE-IO Error
										// Hsien 2013/5/23 file existed but content inconsistent with KP
	return PVM_SUCCESS;
}

int keepRelayWriteIn()
{
	//----------------------
	//	1. Write-Out Keep Relay
	//	2. Frame Finalize
	//----------------------

	//--------------------------------
	//	Store-Back Keep-Relay Contents
	// Hsien 2013/5/15
	//--------------------------------
	FRAME*	keepRelay	= FSMI::queryFrame(24);
	FILE*	fp			= fopen(KEEPRELAY_FILENAME,"wb+");	// Hsien 2013/5/15 , re-generating file
	size_t writeCounts;

	if(fp == NULL)
		return PVM_ERR_KPRLY_FILE;

	writeCounts	= fwrite(keepRelay->baseAdr,sizeof(char),keepRelay->frmSize,fp);
	fclose(fp);
	
	return PVM_SUCCESS;
}



int	startWithRs(void*	arg)
{
#ifndef	IN_SAP
	//-------------------------------------
	//	The Start Script to Initializing PVM , via VAR interface , with RS
	//-------------------------------------
	size_t		frmCounts;
	size_t		frmId;
	size_t		frmSize;

	int			errorCode;

	//--------------------------------------
	//	FRAME INITIALIZING
	//		not really good for such path-senced loading method
	//			need to improve it , Hsien, 2013.01.09
	//--------------------------------------
	FSMI::initialize();

	frmCounts = (SGN32)vsGetInteger( VAR_FRMCOUNTS, 0 );	// modified by tan, 2012.9.1


	for(size_t i=0; i< frmCounts ; i++){
		frmId	=	(SGN32)vsGetInteger( VAR_FRMID+i	, 0 );		// modified by tan, 2012.9.1
		frmSize =	(SGN32)vsGetInteger( VAR_FRMSIZE+i	, 0 );
		errorCode = addFrame(frmId,frmSize);
		if(errorCode == 0)
			return 0;		// need to build error report mechaism
	}
#else
	//-------------------------------------
	//	The Start Script to Initializing PVM , via DQI interface , with RS
	//-------------------------------------
	FRMATTR*	ref;
	size_t		counts;
	int			errorCode;
	sqlite3*	_db = (sqlite3*)arg;

	//--------------------------------------
	//	FRAME INITIALIZING
	//		not really good for such path-senced loading method
	//			need to improve it , Hsien, 2013.01.09
	//--------------------------------------
	errorCode = dqiLoadFrameAttr(_db,&ref,&counts);		// Hsien , 2013.04.25
	if(errorCode != DQI_SUCCESS)
		return 0;

	FSMI::initialize();
	for(size_t i=0;i<counts;i++)
		addFrame(ref[i].frameId,ref[i].totalSize);
	free(ref);		// release buffer
#endif
		
	//---------------------------------------
	//	OP INITIALIZING
	//---------------------------------------
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
	timerInit(TIME_INTERVAL);				// 

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
	addRoutineService(22,machineInputScanIn);
	addRoutineService(23,machineInputScanOut);	// Hsien 2013/4/29

#ifndef IN_SAP
	//----------------------
	//	IO Monitoring , for all frame
	// Hsien 2013/7/15
	//----------------------
	for(int i=2;i<25;i++)
		addRoutineService(i,imageToVar);
	//-------------------------------------
	//	NC communcating
	//-------------------------------------
	//addRoutineService(10,internalScanIn);		// KI frame
	//addRoutineService(11,internalScanOut);		// KO frame
	refKoImage = (short*)FSMI::queryFrame(11)->baseAdr	; // refer to KO_IMAGE in localCopySysOutBlock
	refKiImage = (short*)FSMI::queryFrame(10)->baseAdr	; // refer to KI_IMAGE in localCopySysInBlock

	//addRoutineService(12,syncRequestReceive);	// SYNC_REQ frame
	//addRoutineService(13,syncConfirmSend);		// SYNC_CFM frame	// Hsien 2013/5/22
	//----------------------
	//	linkin , for following NC communicating mechansim and data consistent
	//----------------------
	// Hsien 2013/5/22
	refSyncCfmImage = (short*)FSMI::queryFrame(13)->baseAdr;
	refSyncReqImage	= (short*)FSMI::queryFrame(12)->baseAdr;

	errorCode = keepRelayReadout();
	if(errorCode != PVM_SUCCESS)
		return errorCode;
#endif

	return PVM_SUCCESS;
}

int	stopWithFileOut(char*	/*filename*/)
{
	//----------------------
	//	1. Write-Out Keep Relay
	//	2. Frame Finalize
	//----------------------
	int	errorCode;

	errorCode = keepRelayWriteIn();

	FSMI::finalize();

	if(errorCode != PVM_SUCCESS)
		return errorCode;

	return PVM_SUCCESS;
}

int overrideEnable(void)
{
	//----------------------
	//
	//----------------------
#ifdef IN_SAP
	//----------------------
	//	Able to Override Frame , X, Y , KI , KO , SYNC_REQ , SYNC_CFM
	//----------------------
	addRoutineService(2,memoryOverride);	//Y
	addRoutineService(11,memoryOverride);	//KO
	addRoutineService(13,memoryOverride);	// SYNC_CFM
	addRoutineService(24,memoryOverride);	// K
	addRoutineService(23,memoryOverride);	// MY


#endif
	return 1;
}

int overrideDisable(void)
{
	//----------------------
	//
	//----------------------
#ifdef IN_SAP
	//----------------------
	//	Able to Override Frame , X, Y , KI , KO , SYNC_REQ , SYNC_CFM
	//----------------------
	removeRoutineService(2,memoryOverride)	;		//Y
	removeRoutineService(11,memoryOverride)	;	//KO
	removeRoutineService(13,memoryOverride)	;	// SYNC_CFM
	removeRoutineService(24,memoryOverride)	;	// K
	removeRoutineService(23,memoryOverride)	;	// MY

#endif
	return 1;
}
