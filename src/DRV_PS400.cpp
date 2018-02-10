//---------------------------------------------------------
// FILENAME			: DRV_PS400.cpp
// ORIGIN AUTHOR	: Hsien 2013/6/20
// EDITOR			: Hsien
// NOTES			: PLC_RSSTA suited interface from ICPDAP's motion controll board PS400
//					
//----------------------
#include<DRV_PS400.h>
#include<windows.h>
#include<ps400.h>

#include <PLC_RSSTA.h>

static BYTE  bCardID;			// shared to every function belong to this file  , Hsien 2013/6/20
static WORD	 diModuleStatus;	// indicate if each DI module onlined for each bit
static BYTE	 bitMask[] = {
	0x01,
	0x02,
	0x04,
	0x08,
	0x10,
	0x20,
	0x40,
	0x80
};								// Hsien 2013/6/20 used to filter out which diModule is onlined
static short errorCode;
//static WORD	 diBuffer[8];


int ps400Start(void)
{
	//----------------------
	//	Configured for one card only
	// Hsien 2013/6/20
	//	1. open card
	//	2. config DI module
	//----------------------

	errorCode = ps400_get_cardinfo	(0,&bCardID)	;	// Hsien 2013/6/20 , get card id
	if(errorCode != SUCCESS_NO_ERROR)
		return errorCode;

	errorCode = ps400_open			(bCardID)		;	// Hsien 2013/6/20
	if(errorCode != SUCCESS_NO_ERROR)
		return errorCode;
	errorCode = ps400_reset_FRnet	(bCardID)		;

	errorCode = ps400_scan_FRnet_DI	(bCardID,&diModuleStatus);
					// Hsien 2013/6/25 , somehow it detected nothing sometimes , why?
	if(errorCode != SUCCESS_NO_ERROR)
		return errorCode;
	errorCode = ps400_config_FRnet	(bCardID
		,FRNET_PERIODIC_READING_ENABLE_FEATURE
		,0);		// Hsien 2013/6/20 , enable periodic T reading DI
					// third parameter indicate period factor , T = period factor * 2.88ms
	if(errorCode != SUCCESS_NO_ERROR)
		return errorCode;

	//----------------------
	//	Loading Drivers
	// Hsien 2013/6/20
	//----------------------
	driverAssign(ps400Close			,	_DEVICE_STOP);
	driverAssign(ps400GetDigitalInp	,	_PHYSI_SCAN_IN);
	driverAssign(ps400SetDigitalOut	,	_PHYSI_SCAN_OUT);
	driverAssign(NULL				,	_LIO_SCAN_IN);
	driverAssign(NULL				,	_LIO_SCAN_OUT);	

	return SUCCESS_NO_ERROR;
}

#define DI_MODULE_INDEX_OFFSET FRNET_SA8
#define DO_MODULE_INDEX_OFFSET FRNET_RA0

int ps400GetDigitalInp		(void* data	,const size_t size)
{
	//----------------------
	//	Scanning and readout
	//		Preassumption : data size have to be over than 128bits
	//----------------------
	static BYTE diIndex = 0;

	errorCode = SUCCESS_NO_ERROR;
	for(diIndex = 0; diIndex < 8 ; diIndex++){
		//if( diModuleStatus & bitMask[diIndex]){
			//----------------------
			//	DI module enabled
			//----------------------
			errorCode |= ps400_get_FRnet_DI(bCardID
				,diIndex + DI_MODULE_INDEX_OFFSET	// Hsien 2013/6/20
				,/*&diBuffer[diIndex]*//*)*/&((WORD*)data)[diIndex]			// word by word write-in		Hsien 2013/6/20
				,FRNET_DISABLE_DIRECT_ACCESS);		// due to periodic reading mode Hsien 2013/6/20
		//}//if
	}//for
	if(errorCode != SUCCESS_NO_ERROR)
		return errorCode;
	//memcpy(data,diBuffer,sizeof(diBuffer));

	return SUCCESS_NO_ERROR;
}

int ps400SetDigitalOut		(void* data	,const size_t size)
{
	//----------------------
	//
	//----------------------
	static BYTE doIndex = 0;

	errorCode = SUCCESS_NO_ERROR;
	for(doIndex = 0;doIndex < 8; doIndex++){
		//----------------------
		//	DO module sendout
		//----------------------
		errorCode |= ps400_set_FRnet_DO(bCardID
			,doIndex + DO_MODULE_INDEX_OFFSET
			,((WORD*)data)[doIndex]);
	}
	if(errorCode != SUCCESS_NO_ERROR)
		return errorCode;

	return SUCCESS_NO_ERROR;
}

int	ps400Close(void)
{
	errorCode = ps400_close(bCardID);
	if(errorCode != SUCCESS_NO_ERROR)
		return errorCode;

	return SUCCESS_NO_ERROR;
}