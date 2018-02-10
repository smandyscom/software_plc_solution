// virtualDeviceCommon.h

#pragma once

using namespace System;
using namespace System::Windows;

namespace _virtualDevice {

//	//--------------------------------
//	//	The Command to Set/Reset Value
//	//--------------------------------
//#define	VIR_VALUE_SET	0x00
//#define	VIR_VALUE_RESET	0x01
//
//#define	VIR_ONE_SHOT_PRESS_DOWN	VIR_VALUE_SET
//#define	VIR_ONE_SHOT_PRESS_UP	VIR_VALUE_RESET

public ref class virtualDeviceCommon:public System::Windows::Forms::UserControl
{
	//--------------------------------------
	//	Attension: Image have to be fully cleared before
	//		Bit-Based UI device
	// Hsien 2013/5/29
	//--------------------------------------
protected:
	String^	_deviceName;	
	String^	_deviceSymbol;	// Hsien , 2013.01.30
	char*	_deviceAddress;	// link to image
	char	_deviceBitMask;	// the mask used to access each bit
	char	isAct();		// used to return if this device in act/inact
public:
	virtualDeviceCommon(){};
	~virtualDeviceCommon(){};
	//------------------------------------------------------
	//	Initializing Method
	//------------------------------------------------------
	int	setDeviceName	(String^	__name/*,Object^	showsUp*/);								// set the name and show-up on GUI
	int linkToImage		(char*		__address,const char __bitIndex);		// initializing method		: link to taget image
																			// _bitIndex count from zero
	String^	getDeviceSymbol	(){return this->_deviceSymbol;}
	void	setDeviceSymbol	(String^ str){this->_deviceSymbol = str;}
	//---------------------
	//	The Human-Input/Output Event Handlers
	//---------------------
	int	oneShotPress	(const char downOrUp);
	int	modalPress		();
	//---------------------
	//	The BackGround Accessing Methods:
	//---------------------
	delegate		int	validiateMethod(const char status);							// the prototype of validiating method
	int	setUIEffect	(char*	__address
		,validiateMethod^	__validiateCallBack);		
											// scanning and set method	: throm all the byte from image
											// then compare the address , set UI it if matched.
	int	setUIEffect (validiateMethod^	__validiateCallBack);
};

public ref class virtualDeviceCommonWord:public System::Windows::Forms::UserControl
{
	//--------------------------------------
	//	Attension: Image have to be fully cleared before
	//		Word-Based UI device
	//	Offered Data structure and Function to Access image
	// Hsien 2013/5/29
	//--------------------------------------
#define	INDEX_ADR_NAME	0
#define INDEX_SYMBOL	1
#define	INDEX_COMMENT	2
#define	ATR_MAX_COUNT	3

#define	INDEX_MONITOR_IMAGE	0
#define	INDEX_OVRRIDE_IMAGE	1

protected:
	//String^	addressName;	// e.g X0	
	//String^	symbol;			// e.g operationMode
	//String^ comment;
	array<String^>^	attributes;
	short*	monitorAddress;		// link to image , the address for monitering
	short*	ovrrideAddress;		// link to image , the address to override
public:
	virtualDeviceCommonWord(){
		attributes = gcnew array<String^>(ATR_MAX_COUNT);
	};
	~virtualDeviceCommonWord(){};
	//------------------------------------------------------
	//	Initializing Method
	//------------------------------------------------------
	int	setAttributes	(String^	str/*in*/	,const int atrIndex){
		//--------------------------------
		// set the name and show-up on GUI
		//--------------------------------
		if(atrIndex >= ATR_MAX_COUNT)
			return 0;
		this->attributes[atrIndex] = str;
		return 1;
	}
	int linkToImage		(short*		address		,const int imgIndex){
		//-----------------------------------------------
		// initializing method		: link to taget image
		// _bitIndex count from zero
		//-----------------------------------------------
		switch(imgIndex){
			case INDEX_MONITOR_IMAGE:	this->monitorAddress = address; break;
			case INDEX_OVRRIDE_IMAGE:	this->ovrrideAddress = address; break;
			default:
				return 0;
				break;
		}//switch

		return 1;
	}
	String^	getAttributes	(const int atrIndex){return this->attributes[atrIndex];}

	int		ovrrideImage(const short ovrValue)	{ *this->ovrrideAddress = ovrValue; return 1;}		// set value
	short	monitorImage(void)					{ return *this->monitorAddress;}			// get value
	//---------------------
	//	The Human-Input/Output Event Handlers
	//---------------------
	//int	oneShotPress	(const char downOrUp);
	//int	modalPress		();
	//---------------------
	//	The BackGround Accessing Methods:
	//		GUI to Image : automatic handled by Event Handler
	//		Image to GUI : must via delegate call
	//---------------------
	delegate				int	delegatePrototype/*monitorRefreshMethod*/(const int arg);	// prototype annoncement, Hsien 2013/5/31						// the prototype of validiating method
	delegatePrototype^	monitorRefresh;								// the common UI refresh method
	delegatePrototype^	actOvrride;							// used to loading event handler Hsien 2013/6/4
	delegatePrototype^	inactOvrride;						// used to unload event handler Hsien 2013/6/4
	//int	setUIEffect	(char*	__address
	//	,validiateMethod^	__validiateCallBack);		
	//										// scanning and set method	: throm all the byte from image
	//										// then compare the address , set UI it if matched.
	//int	setUIEffect (validiateMethod^	__validiateCallBack);
};


}// namespace
