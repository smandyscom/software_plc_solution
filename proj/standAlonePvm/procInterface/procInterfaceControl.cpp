// procInterface.cpp : main project file.

#include "stdafx.h"
#include "procInterfaceControl.h"

using namespace procInterface;

int procInterfaceControl::markPid(const int _pid)
{
	//---------------
	//
	//---------------
	this->pid			= _pid;
	this->tbPid->Text	= _pid.ToString();

	//-----------------------------------------
	//	Show-Up The LEVEL-SLOT accroding to PID
	//-----------------------------------------


	return 1;
}

