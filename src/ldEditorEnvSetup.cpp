#include"stdafx.h"

#undef GetEnvironmentVariable		// otherwise , can't use System namespace , Hsien , 2013.01.17
#undef GetCurrentDirectory

using namespace System;

#ifdef _DEBUG
#define LDE_ENV_USER_VAR_RESPATH	"PLCRES"	
#endif

int ldePathSetup(array<System::String ^> ^args){
	// to get module's path , Hsien , 2013.11.14
	//-------------------------------
	//	Target Machine Executing Path Setup ( the User-Variable have to be setup on develop time
	//		Hsien , 2013.01.17
	//		1.	Get the Target Path from pre-defined User-Variable
	//		2.	Change current Path to Target Path
	//-------------------------------
#ifdef ON_SETUPPACK
	//----------------------
	//	Setup executable modules path , to locate .resx files
	//----------------------
	// Hsien 2013/11/15
	System::Diagnostics::Process^	p = System::Diagnostics::Process::GetCurrentProcess();
	System::IO::Directory::SetCurrentDirectory(p->MainModule->FileName->Replace(p->MainModule->ModuleName,nullptr));

#else
#ifdef IN_SAP
	//----------------------
	//	SAP directory structure
	// Hsien 2013/6/20
	//----------------------
	System::IO::Directory::SetCurrentDirectory("..\\..\\..\\resource");
#else
	//----------------------
	//	LDE directory structure
	// Hsien 2013/6/20
	//----------------------
	//System::IO::Directory::SetCurrentDirectory(args[0]);	// Hsien , 2013.11.14
	System::IO::Directory::SetCurrentDirectory("..\\..\\resource");
#endif

#endif

	//String^	resPath = 
	//	Environment::GetEnvironmentVariable(
	//	LDE_ENV_USER_VAR_RESPATH						// var name		:
	//	,System::EnvironmentVariableTarget::User);		// target		: user-level
	//
	////System::String^ currentDir = System::IO::Directory::GetCurrentDirectory();
	//System::IO::Directory::SetCurrentDirectory(resPath);	// change current path

	return 1;
}