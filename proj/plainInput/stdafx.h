// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once
#include<LDE_ELEMENT.h>
#include<LDE_METADATA.h>

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma warning(disable:4945)	// warning of imported repeative symbol
using namespace metaData;
using namespace metaData::device;
#pragma make_public(DEV_METADATA)
#pragma make_public(ldElement)
//#include "plainInputControl.h"

//------------------------------
//	Added by Hsien , 2013.01.22
//		Used to Distinguish the Mode when using deviceEdit
//------------------------------
#define ID_ON_EDIT			0x01
#define ID_ON_REBUILD		0x02
#include "plainInputControl.h"
