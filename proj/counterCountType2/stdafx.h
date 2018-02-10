// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#include<LDE_OUTDEV.h>
#pragma make_public(outputDevice)	// worked  , to change default setting of ' private 'for native objects
#pragma warning(disable:4996)

//---------------------------
//	THE TABINDEX oF TExt BOX of SETUP FORM
//---------------------------
#define TABINDEX_GOAL		0
#define	TABINDEX_INITIAL	1
#define TABINDEX_STEP		2
#define DATA_COUNTS			3

using namespace System;
extern void MarshalString ( String ^ s, std::string& os ); 

#include"counterCountType2Setup.h"
#include"counterCountType2Control.h"
