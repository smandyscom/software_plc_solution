//---------------------------------------------------------
// FILENAME			: PLC_RSSTA.h
// ORIGIN AUTHOR	: Hsien, 2012.06.14
// EDITOR			: Hsien
// NOTES			: Routine Service library When Stand-Alone
//						equivalent to Driver Common Interface , masked varient different driver API
//---------------------------------------------------------
#ifndef _PLC_RSLIB_H
#define _PLC_RSLIB_H
#include<PLC_FRAME.h>

//-----------------------------------
//	The Calls
//-----------------------------------
int	startDevice();
int	stopDevice();

int memoryRefresh		(FRAME*	frm);
int physicalScanIn		(FRAME* frm);
int physicalScanOut		(FRAME* frm);
int internalScanIn		(FRAME*	frm);
int internalScanOut		(FRAME* frm);
int syncRequestReceive	(FRAME* frm);
int syncConfirmSend		(FRAME*	frm);

int machineInputScanIn		(FRAME* frm);
int	machineInputScanOut		(FRAME*	frm);	// Hsien 2013/4/29

int	memoryOverride	(FRAME* frm);			// Hsien 2013/6/4
int	mergeMemoryX	(FRAME* frm);			// Hsien 2013/6/4


//----------------
//	The Driver Management Interface , Hsien , 2013.01.29
//----------------
typedef int (*_devIoFunc)			(void*,const size_t);
typedef int	(*_devStartStopFunc)	();						// the prototype of start/stop device
															// Hsine , 2013.01.30
//---------------
//	The index 
//---------------
#define	_DEVICE_START	0
#define	_DEVICE_STOP	1
#define	_PHYSI_SCAN_IN	2
#define	_PHYSI_SCAN_OUT	3
#define	_LIO_SCAN_IN	4
#define	_LIO_SCAN_OUT	5	// Hsien 2013/4/29
#define	_DRIVER_MAX_NUM 6
//----------------------
//	DRIVER return codes
// Hsien 2013/6/25
//----------------------
#define	DRV_SUCCESS			0
#define	DRV_ERROR_UNKNOWN	-1	
//---------------
//	The Management Interface
//---------------
int	driverInit();												// clear the memory
int	driverAssign(/*_devIoFunc*/void*	newDriver,const char target);		// replace the target driver with the new one


#endif