//---------------------------------------------------------------------------------------------------
// FILENAME			: PLC_OPLIB_LOGIC.cpp
// ORIGIN AUTHOR	: Hsien, 2013.01.16
// EDITOR			: Hsien
// NOTES			: PFC OPERATOR LIBRARY , LOGIC RELATED
//							== , != , > , < , >= , <=
//-------------------------------------------------------------------------------
//	PRIORITY: PERFORMANCE IN RUN TIME
//-------------------------------------------------------------------------------
#include<PLC_PFC.h>

//---------------------------
//	Output Block Initializing
//---------------------------
static PFC_OPERAND result = {
	(get_func)BY_VALUE::get,	// method-get
	(set_func)BY_VALUE::set,	// method-set
	{0,0,0,0,0x01}				// always indicate first bit
};			

PFC_OPERAND	EqualTo		(PFC_OPERAND*	op1		,PFC_OPERAND*	op2)
{
	result.set(result.data
		,op1->get(op1->data) == op2->get(op2->data));

	return result;
}

PFC_OPERAND	NotEqualTo		(PFC_OPERAND*	op1		,PFC_OPERAND*	op2)
{
	result.set(result.data
		,op1->get(op1->data) != op2->get(op2->data));

	return result;
}

PFC_OPERAND	GreaterThan		(PFC_OPERAND*	rhs/*lhs*/		,PFC_OPERAND*	lhs/*rhs*/)
{
	//---------------------
	//	result = lhs > rhs
	//		Watch out the PVM-CORE pop-up sequence(register sequence)
	//			register[0] -> rhs
	//			register[1] -> lhs
	//				noticed by Hsien  , 2013.01.21
	//---------------------
	result.set(result.data
		,lhs->get(lhs->data) > rhs->get(rhs->data));

	return result;
}

PFC_OPERAND	SmallerThan		(PFC_OPERAND*	rhs/*lhs*/		,PFC_OPERAND*	lhs/*rhs*/)
{
	//---------------------
	//	result = lhs < rhs
	//---------------------
	result.set(result.data
		,lhs->get(lhs->data) < rhs->get(rhs->data));

	return result;
}

PFC_OPERAND	GreaterEqual		(PFC_OPERAND*	rhs/*lhs*/		,PFC_OPERAND*	lhs/*rhs*/)
{
	//---------------------
	//	result = lhs >= rhs
	//---------------------
	result.set(result.data
		,lhs->get(lhs->data) >= rhs->get(rhs->data));

	return result;
}

PFC_OPERAND	SmallerEqual		(PFC_OPERAND*	rhs/*lhs*/		,PFC_OPERAND*	lhs/*rhs*/)
{
	//---------------------
	//	result = lhs <= rhs
	//---------------------
	result.set(result.data
		,lhs->get(lhs->data) <= rhs->get(rhs->data));

	return result;
}
