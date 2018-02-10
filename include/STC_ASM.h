//--------------------------------------------------------------------------------------------------------------
//// FILENAME			: PLC_STCASM.h
// ORIGIN AUTHOR		: Hsien	,	2012.04.23
// EDITOR				: Hsien
// NOTE					: 
//						the ST compiler assembly , components including :
//				1. DQI , Database Query Interface
//				2. VAR2IRC , conversion and verification function for variable representation in ST 
//				3. IRC , Intermeidate Representation Codes definition, which is the target code of ST compiler
//				4. lex and yacc parser , engine of this compiler ,generated from specification file st.l  , st.y
//---------------------------------------------------------------------------------------------------------------
#ifndef _PLC_STCASM_H
#define _PLC_STCASM_H
#include<stdio.h>
#include<STC_ERRHDL.h>
#include<STC_PROC.h>

//---------------------------
//	compiling mode definition
//---------------------------
#define	STC_MODE_NORMAL	0		// would generate IRC in outputFp  , stop compiling when syntax/semetic error occurs
#define	STC_MODE_CHECK	1		// wouldn't generate IRC in outputFp , stop compiling when syntax error occurs

//#define STC_SUCCESS		0
//#define STC_ERROR		1
//#define STC_FATAL		2

//#define STC_NO_MORE_ERROR	STC_ERROR


int		stcInit		(/*const char* dbPath*/sqlite3* _db
					 ,STC_ERRHDL*	_errorHdl);
int		stcCompile	(FILE*	inputFp		,FILE* outputFp,int mode);
void	stcClose	();
//int		stcReadError(STC_ERRBLK	*outBlk);

#endif
