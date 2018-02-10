//---------------------------------------------------------------------------------------------------
// FILENAME			: PLC_LIBOP.h
// ORIGIN AUTHOR	: Hsien, 2012.03.29
// EDITOR			: Hsien
// NOTES			: PFC OPERATOR LIBRARY:
//				may be packed as Dynamic Linked Library in the future
//-------------------------------------------------------------------------------
//	PRIORITY: PERFORMANCE IN RUN TIME
//	how are you
//-------------------------------------------------------------------------------
#include<PLC_PFC.h>

PFC_OPERAND	Add			(PFC_OPERAND*	op1		,PFC_OPERAND*	op2);
PFC_OPERAND	Substract	(PFC_OPERAND*	minuend	,PFC_OPERAND*	subtrahend);
PFC_OPERAND	Multiply	(PFC_OPERAND*	op1		,PFC_OPERAND*	op2);
PFC_OPERAND	Divide		(PFC_OPERAND*	dividend,PFC_OPERAND*	divisor);
PFC_OPERAND	And			(PFC_OPERAND*	op1		,PFC_OPERAND*	op2);
PFC_OPERAND	Or			(PFC_OPERAND*	op1		,PFC_OPERAND*	op2);
PFC_OPERAND	Not			(PFC_OPERAND*	op1		,PFC_OPERAND*	/*op2*/);
PFC_OPERAND	Assign		(PFC_OPERAND*	target	,PFC_OPERAND*	value);
//--------
//	Logic Operation , Added by Hsien , 2013.01.16
//--------
 PFC_OPERAND	EqualTo		( PFC_OPERAND*	op1		, PFC_OPERAND*	op2);
 PFC_OPERAND	NotEqualTo		( PFC_OPERAND*	op1		, PFC_OPERAND*	op2);
 PFC_OPERAND	GreaterThan		( PFC_OPERAND*	lhs		, PFC_OPERAND*	rhs);
 PFC_OPERAND	SmallerThan		( PFC_OPERAND*	lhs		, PFC_OPERAND*	rhs);
 PFC_OPERAND	GreaterEqual		( PFC_OPERAND*	lhs		, PFC_OPERAND*	rhs);
 PFC_OPERAND	SmallerEqual		( PFC_OPERAND*	lhs		, PFC_OPERAND*	rhs);
