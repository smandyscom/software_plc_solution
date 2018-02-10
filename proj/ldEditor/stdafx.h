// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once
#include<windows.h>
#include<wincon.h>

#pragma warning(disable:4996)
// TODO: reference additional headers your program requires here

#include<string>
#include<stdio.h>		// for C-style file io accessing , (ST-compiler)
#include<LDE_METADATA.h>
#include<STC_ASM.h>
#include<STC_PCDPM.h>	// for error-stack
#include<STG_ERRHDL.h>	// for error-handling
#include<STC_ERRHDL.h>	// for error-reset
#include<PLC_IRC.h>		// for inserting PROG_WAIT between every rung , Hsien , 2012.10.31s

#include<PLC_FSMI.h>	// fro reseting frame after debugging , Hsien , 2013.01.07

extern void MarshalString ( System::String ^ s, std::string& os );
extern FILE*	fopenString(System::String^ filename,const char*	mode);

#include"ldEditorSetup.h"
#include"ldEditorOutput.h"

#include"d:\workdir\vs2005projects\PLC framework\trunk\proj\ldeVarWatch\ldeVarWatch\ldeSimuData.h"
		// for knowing the simu data type
using namespace metaData;

#define		LDE_STATUS_DEBUGGING		0x01
#define		LDE_STATUS_EDITTING			0x02
#define		LDE_STATUS_DEBUGGING_STOP	0x03	// Hsien 2013/5/24

//----------------------
//	Message Codes , 2nd byte = group
//----------------------
#define	LDE_MSG_NONE					0x0000
#define	LDE_MSG_DEBUGGING				0x0100
#define	LDE_MSG_DEBUG_START				0x0101
#define	LDE_MSG_DEBUG_FAIL				0x0102
#define	LDE_MSG_DEBUG_STOP				0x0103
#define	LDE_MSG_COMPILE_SUCCESS			0x0200
#define	LDE_MSG_COMPILE_FAIL			0x0201
#define	LDE_MSG_DIALOG_CANCEL			0x0202
//----------------------
//	Error Codes
//----------------------
//#define	LDE_SUCCESS_DEBUGGING				0x03
//#define LDE_SUCCESS_DEBUG_START				0x02					// Hsien 2013/5/24
//#define LDE_SUCCESS_CANCELED				0x01					// Hsien 2013/5/24
#define LDE_SUCCESS							0x00
#define LDE_ERROR							0xf0

#define LDE_COMPILE_MODE_DEBUG	0x00
#define LDE_COMPILE_MODE_NORMAL	0x01
//
//#define LDE_OUTPUT_MODE_SIMULATOR	0x00
//#define	LDE_OUTPUT_MODE_IRCFILE		0x01

#define	LDE_FILE_INDEX_LMDS			1
#define	LDE_FILE_INDEX_ST			2
#define LDE_FILE_INDEX_IRC			3
#define LDE_FILE_TYPES_COUNT		4	// Hsien 2013/5/24


#define	LDE_DBSTATUS_MISS			0
#define LDE_DBSTATUS_LINK			1

extern int ldePathSetup(array<System::String ^> ^args);


#pragma make_public(sqlite3)
//#include "ldeMessage.h"

#include "ldEditor.h"

//#include "ldeItems.h"
