//---------------------------------------------------------
// FILENAME				: STG_ERRCODE.h
// ORIGIN AYTHOR		: Hsien , 2013.01.08
// EDITOR				: Hsien
// NOTE					: the ST-Generator Error Code Definition
//----------------------------------------------------------
#ifndef _STG_ERRHDL_H
#define _STG_ERRHDL_H
#include<STC_PCDPM.h>			// for error storage
#include<STC_DQI.h>				// for error querying
#include<STC_ERRHDL.h>

typedef STC_ERRBLK STG_ERRBLK;	// renamed

//-----------------------------------------
//	STG(ST Generator) Error Code Definition
//-----------------------------------------
#define	STG_SUCCESS					0
#define	STG_ERROR					1

#define	STG_ERR_OVERINDEX_ROW		0x01
#define	STG_ERR_OVERINDEX_COLUMN	0x02
#define	STG_ERR_ROUTEOPENED			0x03		// when next element is neither route nor vertice
#define	STG_ERR_UNKNOWN				0x04

#define	STG_MODE_NORMAL				0x00
#define STG_MODE_DEBUG				0x01

#endif