//---------------------------------------------------------
// FILENAME				: PLC_OPMAG.h
// ORIGIN AUTHOR		: Hsien	,	2012.02.29
// EDITOR				: Hsien
// NOTE					: PostFix Code Operation Magzine 
//	hardlink
//---------------------------------------------------------
#ifndef _PLC_PFCOPMAG_H
#define	_PLC_PFCOPMAG_H

#include<limits.h>

#define OPMAG_SIZE	UCHAR_MAX

namespace OPMAG
{
	void			initialize(void);
		//----------------------------------------------------------------------------------------------
		//	AddOp:
		//	whetherUnary , true: Unary operator , false: Binary operator
		//	this attribue controlled by BOOLEAN variable , means it constrained variety type of operator
		//	Unary or Binary , Trinary or more Operand is impossible
		//----------------------------------------------------------------------------------------------
	bool			addOp	(const unsigned char	opId,const bool whetherUnary,void*	funcAdr);
	bool			removeOp(const unsigned char	opId);
		//----------------------------------------------------------------------------------
		//	QueryOp:
		//	return value
		//		address of corresponding function , in void* representation of PFC_OPERATOR*
		//		in order to decoupling the rely of specific type , in header
		//		Hsien , 2012,04,01
		//----------------------------------------------------------------------------------
	void*			queryOp	(const unsigned char	opId);
}

#endif