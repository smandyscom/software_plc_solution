// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

//#define SHADOW_FRAME_OFFSET_ORIGIN	200		// X'	, from original input
#define	SHADOW_FRAME_OFFSET_OVRRIDE	100		// X''	, overrided input from GUI


// TODO: reference additional headers your program requires here
#include<PLC_PVM.h>
#include<PLC_FSMI.h>	// in order to call runAllRoutineService
#include<PLC_RSSTA.h>	// in order to call driver management function
#include<string>
#include<procInterfaceDefinition.h>	// in order to define RUn LEVEL

#include<STC_DQI.h>					// in order to have sqlite3 and DQI
									// Hsien , 2013.04.25

#define SAP_TIME_BASE				15		// unit in ms
#define	SAP_MONITOR_TIME_BASE		100		// Hsien 2013/6/4
											// monitor valid when freq of monitor lower than PVM

extern void MarshalString		(System::String ^ s, std::string& os ) ;
extern int	startWithRs		(void*);	// Hsien 2013/4/29
extern int	stopWithFileOut	(char*);	// Hsien 2013/5/15
extern int	overrideEnable	(void);		// Hsien 2013/6/4
extern int	overrideDisable	(void);		// Hsien 2013/6/4
extern int keepRelayReadout	();			// Hsien 2013/6/5	, for exclusive frame-K
extern int keepRelayWriteIn	();			// Hsien 2013/6/5	, for exclusive frame-K

//----------------------------
//	Function to Start EPCIO
//----------------------------
extern int startMachine(void);
extern int closeMachine(void);

extern int ldePathSetup(array<System::String ^> ^args);

//---------------------------
//	Indicate the tag-index of controls
//---------------------------
#define	SAP_TAGINDEX_START	0
#define SAP_TAGINDEX_STOP	1
//#define SAP_TAGINDEX_SETUP	2			// added by Hsien , 2013.01.29
//-----------------------------------------
//	Indicate which is current output device
//-----------------------------------------
//#include<DRV_VIRDEV.h>					// driver of virtual device
#include<DRV_EPCIO6000_IOONLY.h>		// driver of EPCIO
#include<DRV_PS400.h>					// Hsien 2013/6/20
//#include<DRV_BOTH.h>					// Hsien 2013/5/2
//#define SAP_DEVCODE_NULL		0x00
//#define	SAP_DEVCODE_EPCIO6000	0x01
//#define	SAP_DEVCODE_VIRDEV		0x02
//#define	SAP_DEV_MAX_NUM			3
//#include"sapSetup.h"

#include"standAlonePvm.h"

