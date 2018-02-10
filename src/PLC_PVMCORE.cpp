//---------------------------------------------------------
// FILENAME				: PLC_PVMCORE.cpp
// ORIGIN AYTHOR		: Jay Lin
// EDITOR				: Hsien
// NOTE					: Core component of PostFix Codes Virtual Machine 
//			Catagory : 0-operand ,stack-machine , with very small instructions set
//			for more info about virtual-machine and machine architecture , refer to:
//				MIPS_architecture , instruction-set-architecture , 
//				Popek and Goldberg virtualization requirements (many trend architecture followed that requirements)
//				Comparison_of_CPU_architectures (hope that someday we could fully list our VM's properties)
//				Data sruveryed by Hsien , 2012.4.19
//---------------------------------------------------------
#include <stdio.h>
#include <PLC_PFC.h>		// only need PFC information
							// phsical address info was embedded in instructions
#define PFCCORE_SUCCESS				0x00
#define	PFCCORE_ERR_STACKOVERFLOW	0xa0
#define	PFCCORE_ERR_STACKUNDERFLOW	0xa1

#define REGISTER_SIZE	2
#define STACK_SIZE		64

struct					pfcexe_stack_st
{
	SGN16			topIndex;			// last-write-in, going to read-in index
	PFC_OPERAND		block[STACK_SIZE];
};	// store operand only , Hsien , 2012.02.25

static	struct	pfcexe_stack_st	_stack			;			
										//	Postfix Codes Register in execution	,	
										//	would be flush out after every execution
										//	scope : this function

static /*inline*/ void		fStackInit()
{
	//-----------------------------------------------------------------------------
	// fStackInit
	// Initialize stack. Make sure you have called this function before you use it.
	//-----------------------------------------------------------------------------
	_stack.topIndex = -1;
	return;
}
static /*inline*/ int	fStackPop	(PFC_OPERAND* popout)
{
	//-----------------------------------------------------------------------------
	// fStackPop
	// Delete(pop) next element(the newest element).
	//-----------------------------------------------------------------------------
	if(_stack.topIndex == -1)
		return PFCCORE_ERR_STACKUNDERFLOW;			// stack empty

	*popout = _stack.block[_stack.topIndex];
	--_stack.topIndex;
	return	PFCCORE_SUCCESS;		
}


static /*inline*/ int	fStackPush	(const PFC_OPERAND pushin)
{
	//-----------------------------------------------------------------------------
	// fStackPush
	// Insert an element
	//-----------------------------------------------------------------------------
	if(_stack.topIndex + 1 > STACK_SIZE)	
		return PFCCORE_ERR_STACKOVERFLOW;	// stack overflow

	++_stack.topIndex;
	_stack.block[_stack.topIndex] = pushin;
	return PFCCORE_SUCCESS;
}




/*void**/int					pvmCore		(void*	startIns, void** currentIns)
{
	//-----------------------------------------------------------------------------
	//	INITIALIZE
	//-----------------------------------------------------------------------------
	//	for "static" modification , varibale would not reallocating stack memory
	//		in every function calling.
	//	this move will enhance perfomance in run time.
	//	Hsien , 2012,03.05
	//-----------------------------------------------------------------------------
	//	Parameters:
	//			startIns	, pvmCore execute this instruction for beginning
	//			currentIns	, next	  instruction when pvmCore returned	 // edited by Hsien , 2012.11.26
	//	Return Value:
	//			which code raise pvm returning , and its context
	//
	//	no any error preventing in this core procedure , due to performance reason
	//		so that , every instructions comes into this core area have to be security-ensured.
	//		Hsien , 2012.10.16
	//----------------------------------------------------------------------------
	static	PFC_UNIT*		_instrc						;
	static	PFC_OPERAND		_register[REGISTER_SIZE]	;	//	static SGN32 _register[2] , 
															//	such definiton will be much simply , enhance performance when running as well		
															//	fixed it in next version , Hsien ,2012.04.19
	static  int				returnValue					;	//  coded return value
															//	first	word indicate any command of PROG_CTRL
															//	second	word indicate context of PROC_CTRL
															//	Hsien , 2012.11.26
	_instrc					=	(PFC_UNIT*)startIns;

	fStackInit();
	while(1){

		switch(_instrc->unitType){
			case OPERATOR:
			//---------------------------------------------------------------------------
			// 1.	pop out n operands , n is according to operator attributes
			// 2.	input arguments into given function pointer , and catch return value
			// 3.	push back consequence , if needed
			//----------------------------------------------------------------------------
																				// depends on binary/unary operator
				if(_instrc->operatorData.isUnary)		//unary operator
					fStackPop(&_register[0]);
				else {									//otherwise , binary operator
					fStackPop(&_register[0]);
					fStackPop(&_register[1]);
				}										//	wont do stack underflow sencing
														//	every instruction comes here has to be presumed they're safe
														//	to execute.
				fStackPush(
					((PFC_OPERATION)_instrc->operatorData.operation)(&_register[0],&_register[1])
					);		
							//	cast-back in compiling time   
							//	this move cost constant run-time performance. since everything is determined in compiling-time
							//	always pushback
		
				++_instrc		;												//	forword
				break;

			case OPERAND:

				fStackPush(_instrc->operandData);

				++_instrc		;		// forword
				break;

			case PROGCTRL:
				switch(_instrc->progCtrlData.ctrlCmd){
				//--------------------------------------------------------------
				//	0.	conditional jump to specific program position
				//			could implement 'if' 'for-loop'	'while-loop'
				//--------------------------------------------------------------
						case COND_JUMP:
							fStackPop(&_register[0]);

							if(_register[0].get(_register[0].data))								// condition held ,
								++_instrc			;											// do further moves : single step
							else
								_instrc += _instrc->progCtrlData.insNum;						// condition not held,
																								// jump several steps , it's direction decided by sign of insNum
																								// Hsien , 2012.06.11

							fStackInit();			// reset stack ,  Hsien ,  2012.04.16
							break;
				//---------------------------------------------------------------
				//	1.	pause, to implement sub-routine, 
				//			exit and execute another program(instruction array) , 
				//			after memorize current instruction position
				//	2.	sub-routine may implemented by means of JP or CJ ,
				//		this code may used to implement watch-dog facility.
				//		commented by Hsien , 2012.06.26
				//---------------------------------------------------------------
						case PROG_WAIT:
							((unsigned short*)&returnValue)[0] = PROG_WAIT;
							((unsigned short*)&returnValue)[1] = _instrc->progCtrlData.insNum;	// represent debugging information
																								// edited by Hsien , 2012.11.27 

							*currentIns = (void*)(++_instrc);						// switch to next instruction
							return returnValue/*PROG_WAIT*//*(endAdr = (void*)(++_instrc))*/	;	// 
							break;
				//---------------------------------------------------------------
				//	2.	expression end,	clear stack , and forword instruc	
				//---------------------------------------------------------------
						case EXPR_END:
							fStackInit()	;
							++_instrc			;
							break;
				//---------------------------------------------------------------
				//	3.	program end,	exit this execution process 
				//---------------------------------------------------------------
						case PROG_END:
							((unsigned short*)&returnValue)[0] = PROG_END;
							((unsigned short*)&returnValue)[1] = _instrc->progCtrlData.insNum;	// edited by Hsien , 2012.11.27 

							*currentIns = (void*)(_instrc);
							return	returnValue/*0*//*(endAdr = (void*)(_instrc))*/;	// edited by Hsien , 2012.06.26 , return a invalid ptr
																		//	used to indicate this instruction has been done.
							break;
				//---------------------------------------------------------------
				//	4.	unconditional jump , to the certain instruction
				//			Hsien, 2013.01.21
				//---------------------------------------------------------------
						case JUMP:
							_instrc += _instrc->progCtrlData.insNum;	// jump several steps , Hsien , 2013.01.21
							fStackInit();								// reset the stack
							break;
				}
				break;
		}// switch()
	}//while()
}
