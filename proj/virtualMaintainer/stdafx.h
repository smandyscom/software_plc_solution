// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once


#include<virtualDeviceDefinition.h>
//----------------------
//	This assembly contains following thress components
//----------------------
#include"virtualBitAccessor.h"
#include"virtualByteInteger.h"
#include"virtualWordInteger.h"

#define INDEX_MONITOR_BIT	0
#define INDEX_MONITOR_BYTE	1
#define INDEX_MONITOR_WORD	2
#define MAX_MONITOR_COUNT	3