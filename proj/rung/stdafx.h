// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

//---------------------------
//	precopiled header of Rung
//---------------------------

#pragma once
#include <LDE_ELEMENT.h>
#include <LDE_OUTDEV.h>
#include <LDE_METADATA.h>
#include <LDE_LIB2.h>		//	statement generator

using namespace plain;
using namespace metaData;
using namespace System;
//-------------------
//	from LDE_LIB1.cpp
//-------------------
extern void insertControl(Windows::Forms::Control^ ref
				   ,Windows::Forms::Control^ insert);
extern void deleteControl	(Windows::Forms::Control^ ref);
extern void addControl		(Windows::Forms::Control^ ref,Windows::Forms::Control^ newControl);

#pragma make_public(CLST_METADATA)
#define INITIAL		1
#define FOLLOWER	0
#include "rungControl.h"
#include <STG_ERRHDL.h>		// ST-generator's error-handler
#pragma make_public(PCDPM_FAMILY)
#pragma make_public(STC_ERRHDL)

extern void MarshalString ( String ^ s, std::string& os );
using namespace _ldEditor;



#define RUNG_INDEPENDENT_DEVELOP	0
