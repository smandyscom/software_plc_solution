// virtualDevice.cpp : main project file.

#include "stdafx.h"
#include "virtualDeviceControl.h"

using namespace _virtualDevice;

int virtualDeviceControl::autoSave()
{
	System::IO::FileStream^		fs = gcnew System::IO::FileStream("virsymbols.vd",IO::FileMode::OpenOrCreate,IO::FileAccess::Write);
	System::IO::StreamWriter^	sw = gcnew System::IO::StreamWriter(fs);
	
	//----------------------------------------------------------------------
	//	Write-In 
	//----------------------------------------------------------------------
	for each (virtualButtonControl^ e in this->inpForm->tlpButtons->Controls)
		sw->WriteLine(e->getDeviceSymbol());
	for each (virtualLightControl^ e in this->outForm->tlpLights->Controls)
		sw->WriteLine(e->getDeviceSymbol());
	
	sw->Close();
	fs->Close();

	return 1;
}


int	virtualDeviceControl::autoLoad()
{
	System::IO::FileStream^		fs = gcnew System::IO::FileStream("virsymbols.vd",IO::FileMode::OpenOrCreate,IO::FileAccess::Read);
	System::IO::StreamReader^	sr = gcnew System::IO::StreamReader(fs);
	
	//----------------------------------------------------------------------
	//	Read-Out 
	//----------------------------------------------------------------------
	for each (virtualButtonControl^ e in this->inpForm->tlpButtons->Controls){
		e->setDeviceSymbol(sr->ReadLine());
		e->postSymbol();	
	}
	for each (virtualLightControl^ e in this->outForm->tlpLights->Controls){
		e->setDeviceSymbol(sr->ReadLine());
		e->postSymbol();
	}
	
	sr->Close();
	fs->Close();

	return 1;
}
