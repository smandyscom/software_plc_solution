#include<string.h>
#include<DRV_VIRDEV.h>
#include<PLC_RSSTA.h>		// for getting the HAL-like interface

char	virInpImage[VIR_INP_IMG_SIZE];
char	virOutImage[VIR_OUT_IMG_SIZE];

static long long			buffer;

int		virImageScanIn	(void* data,const size_t	dataSize)
{
	//--------------------------------
	//	General I part 
	//--------------------------------
	memcpy(data,virInpImage,dataSize);

	return 1;
}

int		virImageScanOut	(void* data,const size_t	dataSize)
{
	//--------------------------------
	//	General O part
	//--------------------------------
	memcpy(virOutImage,data,dataSize);
											// then call refresh function

	return 1;
}

int		virImageClean()
{
	memset(virInpImage,0,VIR_INP_IMG_SIZE);
	memset(virOutImage,0,VIR_OUT_IMG_SIZE);

	return 1;
}

int		virStartScript()
{
	driverAssign(virImageClean	,_DEVICE_STOP)		;
	driverAssign(virImageScanIn	,_PHYSI_SCAN_IN)	;
	driverAssign(virImageScanOut,_PHYSI_SCAN_OUT)	;
	driverAssign(NULL			,_LIO_SCAN_IN)		;

	return 1;
}