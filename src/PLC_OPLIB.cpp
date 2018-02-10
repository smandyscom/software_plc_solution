//---------------------------------------------------------------------------------------------------
// FILENAME			: PLC_LIBOP.cpp
// ORIGIN AUTHOR	: Hsien, 2012.03.29
// EDITOR			: Hsien
// NOTES			: PFC OPERATOR LIBRARY:
//				may be packed as Dynamic Linked Library in the future
//-------------------------------------------------------------------------------
//	PRIORITY: PERFORMANCE IN RUN TIME
//-------------------------------------------------------------------------------
#include<PLC_PFC.h>

PFC_OPERAND	Add			(PFC_OPERAND*	op1		,PFC_OPERAND*	op2)
{
	static PFC_OPERAND result = {(get_func)BY_VALUE::get,(set_func)BY_VALUE::set,{0,0,0,0,0x01}};

	result.set(result.data, 
	op1->get(op1->data) + op2->get(op2->data));

	return result;
}

PFC_OPERAND	Substract	(PFC_OPERAND*		/*minuend*/subtrahend,PFC_OPERAND*	/*subtrahend*/minuend)
{
	//----------------------------------------
	//	all variable name was refered in WIKI: Substract
	//		difference = minuend - subtrahend.
	//			operand sequence corrected by Hsien , 2013.01.22
	//----------------------------------------
	static PFC_OPERAND difference = {(get_func)BY_VALUE::get,(set_func)BY_VALUE::set,{0,0,0,0,0x01}};

	difference.set(difference.data, 
	minuend->get(minuend->data) - subtrahend->get(subtrahend->data));

	return difference;
}

PFC_OPERAND	Multiply	(PFC_OPERAND*	op1		,PFC_OPERAND*	op2)
{
	static PFC_OPERAND result = {(get_func)BY_VALUE::get,(set_func)BY_VALUE::set,{0,0,0,0,0x01}};

	result.set(result.data, 
	op1->get(op1->data) * op2->get(op2->data));

	return result;
}

PFC_OPERAND	Divide		(PFC_OPERAND*	divisor,PFC_OPERAND*	dividend/*PFC_OPERAND*	dividend,PFC_OPERAND*	divisor*/)
{
	//----------------------
	//	swaped arguments order to fit definition of divide
	// Hsien 2013/11/15
	//----------------------
	static PFC_OPERAND quotient = {(get_func)BY_VALUE::get,(set_func)BY_VALUE::set,{0,0,0,0,0x01}};

	quotient.set(quotient.data, 
		dividend->get(dividend->data) / divisor->get(divisor->data));

	return quotient;
}

PFC_OPERAND	And			(PFC_OPERAND*	op1		,PFC_OPERAND*	op2)
{
	//-----------------------------------------------------
	// bitwise And(full-bits And) indeed , rather logic and
	//	so did for "Or'
	//	commented by Hsien , 2012.04.01
	//-----------------------------------------------------
	static PFC_OPERAND result = {(get_func)BY_VALUE::get,(set_func)BY_VALUE::set,{0,0,0,0,0}};

	result.set(result.data,
		op1->get(op1->data) & op2->get(op2->data));

	return result;
}

PFC_OPERAND	Or			(PFC_OPERAND*	op1		,PFC_OPERAND*	op2)
{
	static PFC_OPERAND result = {(get_func)BY_VALUE::get,(set_func)BY_VALUE::set,{0,0,0,0,0}};

	result.set(result.data,
		op1->get(op1->data) | op2->get(op2->data));

	return result;
}

PFC_OPERAND	Not			(PFC_OPERAND*	op1		,PFC_OPERAND*	/*op2*/)
{
	//-------------------------------------------------------------
	//	logic not , true<->false , commented by Hsien , 2012.04.09
	//-------------------------------------------------------------
	static PFC_OPERAND result = {(get_func)BY_VALUE::get,(set_func)BY_VALUE::set,{0,0,0,0,0}};

	if(op1->get(op1->data))
		result.set(result.data,false);
	else
		result.set(result.data,true);
	return result;
}
PFC_OPERAND	Assign		(PFC_OPERAND*	target	,PFC_OPERAND*	value)
{
	//--------------------------------------------------------------------------------------------------------
	//	no more type-problem , extened from bool-only to 4types(bool,char,short,int)
	//--------------------------------------------------------------------------------------------------------
	// target(first one poped , lhs) = value(second one poped , rhs) , no problem when type of target is the same as value
	//	if they'r different , truncation or expantion will happen automatically.
	//	commented by Hsien  , 2012.04.09
	//--------------------------------------------------------------------------------------------------------
	static PFC_OPERAND result = {(get_func)BY_VALUE::get,(set_func)BY_VALUE::set,{0,0,0,0,0}};

	target->set(target->data,value->get(value->data));
	result.set(result.data,value->get(value->data));

	return result;
}

