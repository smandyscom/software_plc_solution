// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#include<LDE_OUTDEV.h>
#pragma make_public(outputDevice)			// worked  , to change default setting of ' private 'fornative objects
#pragma warning(disable:4996)

//---------------------------
//	THE TABINDEX oF TExt BOX of SETUP FORM
//---------------------------
#define TABINDEX_INDEX		0
#define	TABINDEX_GOAL		1
#define TABINDEX_INITIAL	2

#define TABINDEX_OK			3
#define TABINDEX_CANCEL		4

using namespace System;
extern void MarshalString ( String ^ s, std::string& os ); 


#include"counterSetup.h"

