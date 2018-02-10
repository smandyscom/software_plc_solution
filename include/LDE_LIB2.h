//---------------------------------------------------------
// FILENAME				: LDE_LIB2.h
// ORIGIN AYTHOR		: Hsien , 2012.09.26
// EDITOR				: Hsien
// NOTE					: 
//----------------------------------------------------------
#ifndef	_LDE_LIB2_H
#define _LDE_LIB2_H
#include<STG_ERRHDL.h>
#include<LDE_ADJMAT.h>
#include<LDE_OUTDEV.h>

int	exprGenerator	(LD_ARRAY	&sourceArray,string	&expr);							// given ld-array generate expression
int stmtGenerator	(OD_ARRAY	&allOds		,string &expr,string &statement);		// given od-array and expression , generate statment

#endif