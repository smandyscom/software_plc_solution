//---------------------------------------------------------
// FILENAME			: PLC_RSSTA.cpp
// ORIGIN AUTHOR	: Hsien, 2013.01.11
// EDITOR			: Hsien
// NOTES			: Routine Service library When Stand-Alone
//---------------------------------------------------------
#include<PLC_RSSTA.h>
#include<string.h>

static	/*_devIoFunc*/void*	driver[_DRIVER_MAX_NUM];		// the container to store fucntion pointer

//int memoryRefresh(FRAME*	frm)
//{
//	//-----------------------------
//	//	reset all content in memory
//	//-----------------------------
//	memset(frm->baseAdr,0,frm->frmSize);
//
//	return 1;
//}
//
//
#include<PLC_FSMI.h>
int physicalScanIn(FRAME* frm)
{
	//--------------
	//	X-Frame
	//--------------

	//-----------------------------------------------
	//	1. read-in data from driver interface
	//	2. load them onto frame memory
	//		warning : considering about size problem.
	//-----------------------------------------------
	memcpy(&frm->baseAdr[32],&(FSMI::queryFrame(frm->frmId+100)->baseAdr[32]),32);		// virtual memory , last 32-bytes
																						// Hsien 2013/6/5
	if(driver[_PHYSI_SCAN_IN] == NULL){
		//----------------------
		//	EPCIO doesn't existed , insteadly, using override memory
		//----------------------
		memcpy(frm->baseAdr,FSMI::queryFrame(frm->frmId+100)->baseAdr,32);		// first 32-bytes
																				// Hsien 2013/6/5 , works
		return 0;
	}
	((_devIoFunc)driver[_PHYSI_SCAN_IN])(frm->baseAdr,frm->frmSize);
	//getDigitalInp(frm->baseAdr,frm->frmSize);

	return 1;
}

int physicalScanOut(FRAME* frm)
{
	//---------------
	//	Y-Frame
	// Hsien 2013/6/4
	//---------------

	if(driver[_PHYSI_SCAN_OUT] == NULL)
		return 0;	
	((_devIoFunc)driver[_PHYSI_SCAN_OUT])(frm->baseAdr,frm->frmSize);

	//setDigitalOut(frm->baseAdr,frm->frmSize);
	return 1;
}

int machineInputScanIn(FRAME* frm)
{
	//--------------
	//	MX-Frame
	//--------------

	//--------------------------------
	//	Added by Hsien , 2013.01.23
	//--------------------------------
	if(driver[_LIO_SCAN_IN] == NULL)
		return 0;
	((_devIoFunc)driver[_LIO_SCAN_IN])(frm->baseAdr,frm->frmSize);

	//getLio(frm->baseAdr,frm->frmSize);

	return 1;
}

int	machineInputScanOut(FRAME* frm)
{
	//----------------------
	//	MY-Frame
	//----------------------

	//----------------------
	//	// Hsien 2013/4/29
	//----------------------
	if(driver[_LIO_SCAN_OUT] == NULL)
		return 0;
	((_devIoFunc)driver[_LIO_SCAN_OUT])(frm->baseAdr,frm->frmSize);

	return 1;
}

int driverInit()
{
	//----------------------
	//	Initialize the Space
	//----------------------
	for(int i=0;i<_DRIVER_MAX_NUM;i++)
		driver[i] = NULL;
	return 1;
}

int	driverAssign(/*_devIoFunc*/void*	newDriver,const char target)
{
	//----------------------
	//	Replace the target driver
	//----------------------
	driver[target] = newDriver;

	return 1;
}

int	startDevice()
{
	int	errorCode;

	if(driver[_DEVICE_START] == NULL)
		return DRV_ERROR_UNKNOWN;

	//----------------------
	//	Report Error if any , error code definition is followed by taget driver
	// Hsien 2013/6/25
	//----------------------
	errorCode = ((_devStartStopFunc)driver[_DEVICE_START])();

	if(errorCode != DRV_SUCCESS)
		return errorCode;

	return DRV_SUCCESS;
}
int	stopDevice()
{
	int	errorCode;

	if(driver[_DEVICE_STOP] == NULL)
		return DRV_ERROR_UNKNOWN;

	//----------------------
	//	Report Error if any , error code definition is followed by taget driver
	// Hsien 2013/6/25
	//----------------------
	errorCode = ((_devStartStopFunc)driver[_DEVICE_STOP])();
	if(errorCode != DRV_SUCCESS)
		return errorCode;

	return DRV_SUCCESS;
}

//----------------------
//	
//----------------------
#include<PLC_FSMI.h>
int	memoryOverride(FRAME* frm)
{
	//----------------------
	//	Override rather than |= , &=
	//	Used when Forced Output Mode activated
	// Hsien 2013/6/5
	//----------------------
	for(int i=0;i<frm->frmSize;i++)
		frm->baseAdr[i] = (FSMI::queryFrame(frm->frmId + 100)->baseAdr[i]);

	return 1;
}
