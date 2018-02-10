#include<VIR_COMMON.h>

#pragma warning (disable:4138)

int virtualDeviceCommon::linkToImage(char*		__address,const char __bitMask)
{
	this->_deviceAddress = __address;
	this->_deviceBitMask = __bitMask;

	return 1;
}

int virtualDeviceCommon::setDeviceName(System::String ^__name, System::String ^__textShowsUp)
{
	this->_deviceName	= __name;
	__textShowsUp		= __name;			// the GUI control going to show-up

	return 1;
}


System::Void virtualDeviceCommon::oneShotPressDown(System::Object ^sender, System::EventArgs ^e)
{
	//--------------------------------------------
	//	The Standard Handler for One-Shot Device
	//--------------------------------------------
	//virtualDeviceCommon^	hdl
	//	= safe_cast<virtualDeviceCommon^>(sender);

	*/*hdl->*/_deviceAddress |= /*hdl->*/_deviceBitMask;			// set the bit
}

System::Void virtualDeviceCommon::oneShotPressUp(System::Object ^sender, System::EventArgs ^e)
{
	//--------------------------------------------
	//	The Standard Handler for One-Shot Device
	//--------------------------------------------
	//virtualDeviceCommon^	hdl
	//	= safe_cast<virtualDeviceCommon^>(sender);

	*/*hdl->*/_deviceAddress &= ~(/*hdl->*/_deviceBitMask);		// reset the bit
}

System::Void virtualDeviceCommon::modalPress(System::Object ^sender, System::EventArgs ^e)
{
	//--------------------------------------------
	//	The Standard Handler for Modal Device
	//--------------------------------------------
	//virtualDeviceCommon^	hdl
	//	= safe_cast<virtualDeviceCommon^>(sender);

	if(*/*hdl->*/_deviceAddress & /*hdl->*/_deviceBitMask)
		//---------------
		//		If has been Pressed
		// reset the bit
		//---------------
		*/*hdl->*/_deviceAddress &= ~(/*hdl->*/_deviceBitMask);		
	else
		//---------------
		//		If has not Pressed
		// set the bit
		//---------------
		*/*hdl->*/_deviceAddress |= /*hdl->*/_deviceBitMask;		
}

int	virtualDeviceCommon::setValue(char *__address, const char __command, virtualDeviceCommon::validiateMethod ^__validiateCallBack)
{
	//-----------------------------------
	//	Reject The Address not belonging to this device
	//-----------------------------------
	if(__address != this->_deviceAddress)
		return 1;

	//------------------
	//	Set/Reset Value
	//------------------
	switch(__command){
		case VIR_VALUE_SET:		*_deviceAddress |= _deviceBitMask;break;
		case VIR_VALUE_RESET:	*_deviceAddress |= _deviceBitMask;break;
		default:
			break;
	}
	//--------
	//	Raising GUI validiate method
	//--------
	__validiateCallBack();

	return 1;
}