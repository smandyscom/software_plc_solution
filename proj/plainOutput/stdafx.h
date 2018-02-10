// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once
#include<LDE_OUTDEV.h>
#include<LDE_METADATA.h>
using namespace metaData;
using namespace metaData::device;
#pragma make_public(outputDevice)
#pragma make_public(DEV_METADATA)


#pragma warning(disable:4945)	// warning of imported repeative symbol

//------------------------------
//	Added by Hsien , 2013.01.21
//		Used to Distinguish the Mode when using deviceEdit
//------------------------------
#define OD_ON_EDIT			0x01
#define OD_ON_REBUILD		0x02
#include "plainOutputControl.h"
