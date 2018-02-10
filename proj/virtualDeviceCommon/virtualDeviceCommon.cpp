// This is the main DLL file.

#include "stdafx.h"

#include "virtualDeviceCommon.h"

#pragma warning (disable:4138)

using namespace _virtualDevice;

char	virtualDeviceCommon::isAct		()
{
	//---------------------------------------
	//	To Report The Status of Device
	//---------------------------------------
	return (*_deviceAddress & _deviceBitMask);
}

int virtualDeviceCommon::linkToImage(char*		__address,const char __bitIndex)
{
	unsigned char	bitMask[]={
		0x01,
		0x02,
		0x04,
		0x08,
		0x10,
		0x20,
		0x40,
		0x80,
	};

	this->_deviceAddress = __address;
	this->_deviceBitMask = bitMask[__bitIndex];

	return 1;
}

int virtualDeviceCommon::setDeviceName(System::String ^__name/*, Object^	showsUp*/)
{
	this->_deviceName			= __name;
	return 1;
}


int virtualDeviceCommon::oneShotPress(const char downOrUp)
{
	//--------------------------------------------
	//	The Standard Handler for One-Shot Device
	//--------------------------------------------

	switch(downOrUp){
		case VIR_ONE_SHOT_PRESS_DOWN:	*_deviceAddress |= _deviceBitMask;		break;		// set the bit
		case VIR_ONE_SHOT_PRESS_UP:		*_deviceAddress &= ~(_deviceBitMask);	break;		// reset the bit
		default:break;
	}//switch

	return 1;
}

int virtualDeviceCommon::modalPress()
{
	//--------------------------------------------
	//	The Standard Handler for Modal Device
	//--------------------------------------------

	if(isAct()/**_deviceAddress & _deviceBitMask*/)
		//---------------
		//		If has been Pressed
		// reset the bit
		//---------------
		*_deviceAddress &= ~(_deviceBitMask);		
	else
		//---------------
		//		If has not Pressed
		// set the bit
		//---------------
		*_deviceAddress |= _deviceBitMask;		

	return 1;
}

int	virtualDeviceCommon::setUIEffect(char *__address, virtualDeviceCommon::validiateMethod ^__validiateCallBack)
{
	//-----------------------------------
	//	Reject The Address not belonging to this device
	//-----------------------------------
	if(__address != this->_deviceAddress)
		return 1;

	//--------
	//	Raising GUI validiate method
	//--------
	__validiateCallBack(isAct());

	return 1;
}

int	virtualDeviceCommon::setUIEffect(virtualDeviceCommon::validiateMethod ^__validiateCallBack)
{
	//--------
	//	Raising GUI validiate method
	//--------
	return __validiateCallBack(isAct());;
}