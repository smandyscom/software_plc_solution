//----------------------
//	
//----------------------
#include <PLC_RSSTA.h>	// for recognizing HAL-like driver interface , Hsien 2013/5/2
#include <DRV_BOTH.h>

#include <DRV_VIRDEV.h>				// for connecting virtual device	
#include <DRV_EPCIO6000_IOONLY.h>	// for connecting physical device


//----------------------
//	FOLLOWING SPECIFICATION IMPLEMENTED FROM XY_20130429.xlsx
//----------------------
#define WORDS_GPIO_EDIO_PART			0	// word 0-3
#define WORDS_GPIO_VDEVICE_PART			16	// word 16-19
#define WORDS_DEIO_VDEVICE_PART			4	// word 16-19

#define	WORD_SIZE_GPIO_EDIO				8			
#define	WORD_SIZE_DEIO_EDIO				4		
#define	WORD_SIZE_GPIO_EDIO_INBYTE		WORD_SIZE_GPIO_EDIO*2	// Hsien 2013/5/9	
#define	WORD_SIZE_DEIO_EDIO_INBYTE		WORD_SIZE_DEIO_EDIO*2	// Hsien 2013/5/9		


#define	WORD_SIZE_GPIO_VDEVICE			8	// Hsien 2013/5/9
#define	WORD_SIZE_GPIO_VDEVICE_INBYTE	WORD_SIZE_GPIO_VDEVICE*2	// Hsien 2013/5/9
																	// in memcpy

int	startIODevice()
{
//	startMachine	()	;		// EPCIO
	virImageClean	()	;		// virtual Device

	//------------
	//	Loading Drivers // Hsien 2013/5/2
	//------------
	driverAssign(closeIODevice	,	_DEVICE_STOP);
	driverAssign(getGenericInput,	_PHYSI_SCAN_IN);
	driverAssign(setGenericOut	,	_PHYSI_SCAN_OUT);
	driverAssign(getDedicatedInput		,	_LIO_SCAN_IN);
	driverAssign(setDedicatedOutput		,	_LIO_SCAN_OUT);	// Hsien 2013/4/29

	return 1;
}

int	closeIODevice()
{
//	closeMachine	()	;		// EPCIO
	resetMachine	()	;
	virImageClean	()	;		// virtual Device

	return 1;
}

int getGenericInput(void* data, const size_t fullSize)
{
	//----------------------
	//	X part
	//----------------------
	short*	addressPhysical = (short*)data;							// words 0
//	short*	addressVirtual	= &((short*)data)[WORDS_GPIO_VDEVICE_PART];	// words 16-19
												
	// Hsien 2013/6/4  ,due to new virtualMaintainer activated

		//----------------------
		//	Phisical I from EDIO-S003
		//		Word 0-7
		//----------------------
	getDigitalInp((void*)addressPhysical 
		, WORD_SIZE_GPIO_EDIO_INBYTE);

		//----------------------
		//	Virtual I from Virtual Panel
		//		Word  16-19
		//----------------------
	//virImageScanIn((void*)addressVirtual
	//	,WORD_SIZE_GPIO_VDEVICE_INBYTE);	// Hsien 2013/6/4 ,due to new virtualMaintainer activated

	return 1;
}

int setGenericOut	(void* data, const size_t)
{
	//----------------------
	//	Y part
	//----------------------
	short*	addressPhysical = (short*)data;							// words 0
	//short*	addressVirtual	= &((short*)data)[WORDS_GPIO_VDEVICE_PART];	// words 16-19

		//----------------------
		//	Phisical I from EDIO-S003
		//		Word 0-7
		//----------------------
	setDigitalOut((void*)addressPhysical 
		, WORD_SIZE_GPIO_EDIO_INBYTE);

		//----------------------
		//	Virtual I from Virtual Panel
		//		Word  16-19
		//----------------------
	//virImageScanOut((void*)addressVirtual
	//	,WORD_SIZE_GPIO_VDEVICE_INBYTE);

	return 1;
}

int getDedicatedInput(void* data, const size_t fullSize)
{
	//----------------------
	//	MX part
	//----------------------
	short*	addressPhysical = (short*)data;							// words 0
	short*	addressVirtual	= &((short*)data)[WORDS_DEIO_VDEVICE_PART];	// words 16-19

		//----------------------
		//	Phisical I from EDIO-S003
		//		Word 0-3
		//----------------------
	getLio(addressPhysical,WORD_SIZE_DEIO_EDIO_INBYTE);

	return 1;
}

int setDedicatedOutput(void* data, const size_t fullSize)
{
	//----------------------
	//	MX part
	//----------------------
	short*	addressPhysical = (short*)data;							// words 0
	short*	addressVirtual	= &((short*)data)[WORDS_DEIO_VDEVICE_PART];	// words 16-19

		//----------------------
		//	Phisical I from EDIO-S003
		//		Word 0-3
		//----------------------
	setLio(addressPhysical,WORD_SIZE_DEIO_EDIO_INBYTE);

	return 1;
}
