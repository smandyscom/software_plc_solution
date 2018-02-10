// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#include<procInterfaceDefinition.h>

using namespace System;

void insertControl	(Windows::Forms::Control^ ref,Windows::Forms::Control^ insert);
void addControl		(Windows::Forms::Control^ ref,Windows::Forms::Control^ newControl);
void deleteControl	(Windows::Forms::Control^ ref);


#define ITEM_INDEX_15MS		0
#define	ITEM_INDEX_30MS		1
#define ITEM_INDEX_60MS		2
#define	ITEM_INDEX_120MS	3