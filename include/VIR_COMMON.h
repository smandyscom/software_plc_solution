

#define	VIR_VALUE_SET	0x00
#define	VIR_VALUE_RESET	0x01

using namespace System;

public ref class virtualDeviceCommon:public System::Windows::Forms::UserControl
{
	//--------------------------------------
	//	Attension: Image have to be fully cleared before
	//--------------------------------------
private:
	String^	_deviceName;		
	char*	_deviceAddress;	// link to image
	char	_deviceBitMask;	// the mask used to access each bit
public:
	//------------------------------------------------------
	//	Initializing Method
	//------------------------------------------------------
	int	setDeviceName	(String^	__name	 ,String^	 __textShowsUp);	// set the name and show-up on GUI
	int linkToImage		(char*		__address,const char __bitMask);		// initializing method		: link to taget image
	//---------------------
	//	The Human-Input/Output Event Handlers
	//---------------------
	System::Void	oneShotPressDown(System::Object^	sender,System::EventArgs^ e);
	System::Void	oneShotPressUp	(System::Object^	sender,System::EventArgs^ e);
	System::Void	modalPress		(System::Object^	sender,System::EventArgs^ e);
	//---------------------
	//	The BackGround Accessing Methods:
	//---------------------
	delegate		int	validiateMethod(void);							// the prototype of validiating method
	int	setValue	(char*	__address
		,const char			__command
		,validiateMethod^	__validiateCallBack);		
											// scanning and set method	: throm all the byte from image
											// then compare the address , set/reset it if matched.
};

