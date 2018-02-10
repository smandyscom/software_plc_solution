//*****************************************************************************************
// FILENAME: DRV_EPCIO_IOONLY.cpp
// EDITOR  : Hsien
// CDATE   : Hsien, 2013.01.11
// MDATE   : 11st Jan, 2013
// NOTE    : Driver of EPCIO 
//*****************************************************************************************
#pragma warning(disable:4800)

#include <stdio.h>
#include <MEPCIODEV.H>
#include <MEPCIOINI.H>
#include <DRV_EPCIO6000_IOONLY.h>

#include <PLC_RSSTA.h>

//int	resetMachine(void)
//{
//	// Hsien 2013/5/2
//	//----------------------
//	//	Reset The Outputs
//	//----------------------
//	EPCIO_RIO_SetOutputValue(RIO_SET0,RIO_SLAVE0,RIO_PORT0,0,CARD_INDEX);
//	EPCIO_RIO_SetOutputValue(RIO_SET0,RIO_SLAVE0,RIO_PORT1,0,CARD_INDEX);
//	EPCIO_RIO_SetOutputValue(RIO_SET0,RIO_SLAVE0,RIO_PORT2,0,CARD_INDEX);
//	EPCIO_RIO_SetOutputValue(RIO_SET0,RIO_SLAVE0,RIO_PORT3,0,CARD_INDEX);
//
//	EPCIO_RIO_SetOutputValue(RIO_SET1,RIO_SLAVE0,RIO_PORT0,0,CARD_INDEX);
//	EPCIO_RIO_SetOutputValue(RIO_SET1,RIO_SLAVE0,RIO_PORT1,0,CARD_INDEX);
//	EPCIO_RIO_SetOutputValue(RIO_SET1,RIO_SLAVE0,RIO_PORT2,0,CARD_INDEX);
//	EPCIO_RIO_SetOutputValue(RIO_SET1,RIO_SLAVE0,RIO_PORT3,0,CARD_INDEX);
//
//	return 0;
//}
//
int closeMachine(void)
{	
	EPCIO_ResetModule(RESET_ALL, CARD_INDEX);	// Hsien 2013/5/2

	//======================================================================
	// close Rio
	//======================================================================
	EPCIO_RIO_DisableSlaveControl(RIO_SET0, RIO_SLAVE0, CARD_INDEX);
	EPCIO_RIO_DisableSlaveControl(RIO_SET1, RIO_SLAVE0, CARD_INDEX);
	EPCIO_RIO_DisableSetControl(RIO_SET0, CARD_INDEX);
	EPCIO_RIO_DisableSetControl(RIO_SET1, CARD_INDEX);
	//======================================================================
	// close card
	//======================================================================
	if ( !EPCIO_Close(CARD_INDEX) )
		return DRV_ERROR_UNKNOWN;
	return DRV_SUCCESS;
} // closeMachine()

int startMachine(void)
{ 
	//-------------------------
	//	Motion Card Initilaize
	//-------------------------
	if ( !EPCIO6000_Init(
		NULL,	// DDA ISR
		NULL,	// ENC012
		NULL,	// ENC345
		NULL,	// ENC678
		NULL,	// RIO1
		NULL,	// RIO2
		NULL,	// ADC
		NULL,	// LIO
		NULL,	// PCL
		CARD_INDEX) )
		return DRV_ERROR_UNKNOWN;	// Hsien 2013/6/5 , init fail exit
					// abundant to load following driver

	EPCIO_ResetModule(RESET_ALL, CARD_INDEX);
	//======================================================================
	//	RIO Initialize
	//======================================================================
	EPCIO_RIO_SetClockDivider(RIO_SET0, RIO_CLOCK_DIVIDER, CARD_INDEX);	// Enable RIO Control
	EPCIO_RIO_SetClockDivider(RIO_SET1, RIO_CLOCK_DIVIDER, CARD_INDEX);
	EPCIO_RIO_EnableSetControl(RIO_SET0, CARD_INDEX);
	EPCIO_RIO_EnableSetControl(RIO_SET1, CARD_INDEX);
	EPCIO_RIO_EnableSlaveControl(RIO_SET0, RIO_SLAVE0, CARD_INDEX);
	EPCIO_RIO_EnableSlaveControl(RIO_SET1, RIO_SLAVE0, CARD_INDEX);



	//------------
	//	Loading Drivers
	//------------
	//  remarked by Hsien 2013/5/2
	//	unremakred by Hsien , // Hsien 2013/6/5
	driverAssign(closeMachine,	_DEVICE_STOP);
	driverAssign(getDigitalInp,	_PHYSI_SCAN_IN);
	driverAssign(setDigitalOut,	_PHYSI_SCAN_OUT);
	driverAssign(getLio		,	_LIO_SCAN_IN);
	driverAssign(setLio		,	_LIO_SCAN_OUT);	// Hsien 2013/4/29

	return DRV_SUCCESS;
} // startScript()

static long long		rioBuffer;

int getDigitalInp		(void* data	,const size_t size)
{
	EPCIO_RIO_GetInputValue(RIO_SET0,RIO_SLAVE0,RIO_PORT0,&((unsigned short*)&rioBuffer)[0],CARD_INDEX);
	EPCIO_RIO_GetInputValue(RIO_SET0,RIO_SLAVE0,RIO_PORT1,&((unsigned short*)&rioBuffer)[1],CARD_INDEX);
	EPCIO_RIO_GetInputValue(RIO_SET0,RIO_SLAVE0,RIO_PORT2,&((unsigned short*)&rioBuffer)[2],CARD_INDEX);
	EPCIO_RIO_GetInputValue(RIO_SET0,RIO_SLAVE0,RIO_PORT3,&((unsigned short*)&rioBuffer)[3],CARD_INDEX);
	((long long*)data)[0] = ~rioBuffer;

	EPCIO_RIO_GetInputValue(RIO_SET1,RIO_SLAVE0,RIO_PORT0,&((unsigned short*)&rioBuffer)[0],CARD_INDEX);
	EPCIO_RIO_GetInputValue(RIO_SET1,RIO_SLAVE0,RIO_PORT1,&((unsigned short*)&rioBuffer)[1],CARD_INDEX);
	EPCIO_RIO_GetInputValue(RIO_SET1,RIO_SLAVE0,RIO_PORT2,&((unsigned short*)&rioBuffer)[2],CARD_INDEX);
	EPCIO_RIO_GetInputValue(RIO_SET1,RIO_SLAVE0,RIO_PORT3,&((unsigned short*)&rioBuffer)[3],CARD_INDEX);
	((long long*)data)[1] = ~rioBuffer;

	return 1;
}

int setDigitalOut		(void* data	,const size_t size)
{
	rioBuffer = ((long long*)data)[0];
	EPCIO_RIO_SetOutputValue(RIO_SET0,RIO_SLAVE0,RIO_PORT0,((unsigned short*)&rioBuffer)[0],CARD_INDEX);
	EPCIO_RIO_SetOutputValue(RIO_SET0,RIO_SLAVE0,RIO_PORT1,((unsigned short*)&rioBuffer)[1],CARD_INDEX);
	EPCIO_RIO_SetOutputValue(RIO_SET0,RIO_SLAVE0,RIO_PORT2,((unsigned short*)&rioBuffer)[2],CARD_INDEX);
	EPCIO_RIO_SetOutputValue(RIO_SET0,RIO_SLAVE0,RIO_PORT3,((unsigned short*)&rioBuffer)[3],CARD_INDEX);

	rioBuffer = ((long long*)data)[1];
	EPCIO_RIO_SetOutputValue(RIO_SET1,RIO_SLAVE0,RIO_PORT0,((unsigned short*)&rioBuffer)[0],CARD_INDEX);
	EPCIO_RIO_SetOutputValue(RIO_SET1,RIO_SLAVE0,RIO_PORT1,((unsigned short*)&rioBuffer)[1],CARD_INDEX);
	EPCIO_RIO_SetOutputValue(RIO_SET1,RIO_SLAVE0,RIO_PORT2,((unsigned short*)&rioBuffer)[2],CARD_INDEX);
	EPCIO_RIO_SetOutputValue(RIO_SET1,RIO_SLAVE0,RIO_PORT3,((unsigned short*)&rioBuffer)[3],CARD_INDEX);
	// SET1 , SLAVE0  , Hsien , 2013.04.26
	return DRV_SUCCESS;
}

#define DATA_HOME		0
#define DATA_TRAVELUP	1
#define	DATA_TRAVELDOWN	2

static unsigned short	lioBuffer;
//----------------------
//	Bit Masks Difinition
//----------------------
static unsigned char dataAxis[] = {
	0x01,
	0x02,
	0x04,
	0x08,
	0x10,
	0x20,
	0x40,
	0x80,
};

typedef int  (_stdcall *LIO_FUNC)(unsigned short
								  ,unsigned short*
								  ,unsigned short);

static LIO_FUNC	__lioFunc[] = {
	EPCIO_LIO_GetHomeSensor,
	EPCIO_LIO_GetOverTravelUp,
	EPCIO_LIO_GetOverTravelDown,
};

int	getLio			(void* data , const size_t size)
{
#define	WORD_HOME_SENSOR	0
#define	WORD_OTPLUS			1
#define	WORD_OTNEG			2
#define	WORD_EMGSTP			3

	//-----------------------
	//	Get LIO  
	//-----------------------
	for(int func=0;func<3;func++){
		for(int axis=0;axis<6;axis++){
			lioBuffer = 0;
			__lioFunc[func](axis, &lioBuffer,	CARD_INDEX);	// in sequence of HOME , TUP, TDOWN

			if(lioBuffer)
				((short*)data)[func] |= dataAxis[axis];			// swap to next word when funct changes
			else
				((short*)data)[func] &= ~dataAxis[axis];

		}// internal for
	}// externnal for

	//----------------------
	//	Word 3 , bit one , inidicate Emg Status , Hsien 2013/4/28
	//----------------------
	lioBuffer = 0;
	EPCIO_LIO_GetEmgcStopStatus(&lioBuffer,CARD_INDEX);
	if(lioBuffer)
		((short*)data)[WORD_EMGSTP] |= dataAxis[0];			// swap to next word when funct changes
	else
		((short*)data)[WORD_EMGSTP] &= ~dataAxis[0];

	return DRV_SUCCESS;
}

int setLio			(void* data , size_t size)
{
#define WORD_SERVO_ON_OFF	0
#define	WORD_PRDY			1
#define	WORD_OUTPUT_ENABLE	2
	//----------------------
	// Set Lio Part, Implemented According to MY_20130329.xlsx
	//		Word 0 : ServoOn/Off
	//		Word 1 : PRDY
	//		Word 2 : Output Enable ( DAC/DDA)
	//----------------------
	for(int axis=0;axis<6;axis++){
		if(((short*)data)[WORD_SERVO_ON_OFF] & dataAxis[axis])
			EPCIO_LIO_ServoOn	(axis, CARD_INDEX);
		else
			EPCIO_LIO_ServoOff	(axis, CARD_INDEX);
	}

	if(((short*)data)[WORD_PRDY])
		EPCIO_LIO_EnablePrdy	(CARD_INDEX);
	else
		EPCIO_LIO_DisablePrdy	(CARD_INDEX);

	if(((short*)data)[WORD_OUTPUT_ENABLE])
		EPCIO_LIO_EnablePulseDAC	(CARD_INDEX);
	else
		EPCIO_LIO_DisablePulseDAC	(CARD_INDEX);


	return DRV_SUCCESS;
}