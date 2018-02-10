// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

//------------------
//	For PVM-Simulator
//------------------
#include<PLC_PVM.h>
//------------------
//	For VAR-WATCH Interface
//------------------
#include<STC_DQI.h>
#include<STC_VAROP.h>
#include"ldeSimuData.h"
#include<PLC_FSMI.h>

#include<sqlite3.h>

#pragma make_public(FILE)
#pragma make_public(sqlite3)

extern int	startViaDqiWithoutRs(sqlite3*/*void**/);
#include<string>
extern void MarshalString ( System::String ^ s, std::string& os );



