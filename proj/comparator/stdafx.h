// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once


#include<LDE_ELEMENT.h>
#pragma warning(disable:4996)
#pragma make_public(vertice)


//---------------------------
//	THE TABINDEX oF TExt BOX of SETUP FORM
// Hsien 2013/5/16
//---------------------------
#define TABINDEX_LHS		0
#define	TABINDEX_OP			1
#define TABINDEX_RHS		2

#define TABINDEX_OK			3
#define TABINDEX_CANCEL		4

using namespace System;
extern void MarshalString ( String ^ s, std::string& os ) ;

#include"comparatorSetup.h"