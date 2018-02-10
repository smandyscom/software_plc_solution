#include<PLC_PFC.h>


extern PFC_OPERAND	EqualTo		( PFC_OPERAND*	op1		, PFC_OPERAND*	op2);
extern PFC_OPERAND	NotEqualTo		( PFC_OPERAND*	op1		, PFC_OPERAND*	op2);
extern PFC_OPERAND	GreaterThan		( PFC_OPERAND*	lhs		, PFC_OPERAND*	rhs);
extern PFC_OPERAND	SmallerThan		( PFC_OPERAND*	lhs		, PFC_OPERAND*	rhs);
extern PFC_OPERAND	GreaterEqual		( PFC_OPERAND*	lhs		, PFC_OPERAND*	rhs);
extern PFC_OPERAND	SmallerEqual		( PFC_OPERAND*	lhs		, PFC_OPERAND*	rhs);

int main()
{
	short i = 30000,j = 30000;

	PFC_OPERAND op1,op2,result;

	//-------------------------------
	//	Set-Up
	//-------------------------------
	op1.get = (get_func)BY_REFERENCE::getWord;
	op1.set = (set_func)BY_REFERENCE::setWord;
	SET_ADDR(op1.data,&i); 

	op2.get = (get_func)BY_REFERENCE::getWord;
	op2.set = (set_func)BY_REFERENCE::setWord;
	SET_ADDR(op2.data,&j); 

	result.get = (get_func)BY_VALUE::get;
	result.set = (set_func)BY_VALUE::set;	

result = 	EqualTo		( &op1		, &op2);
result = 	NotEqualTo		( &op1		, &op2);
result = 	GreaterThan		( &op1		, &op2);
result = 	SmallerThan		( &op1		, &op2);
result = 	GreaterEqual		( &op1		, &op2);
result = 	SmallerEqual		( &op1		, &op2);
	// Test Done, Hsien , 2013.01.16


	return 1;
}