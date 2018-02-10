//----------------------------------------------------------------------------------------------------------------
// FILENAME			: PLC_PFC.h
// ORIGIN AUTHOR	: Jay Lin
// EDITOR			: Hsien, 2012.02.14
// NOTES			: Post-Fix Codes definition
//			properties:
//				1. instruction encoding : fixed , bounding size
//				2. type : register-memory
//				3. branch evalution : condition code
//				4. max operands : 2 (surpport nither binary operation nor unary)
//						commented after data survey from WIKI:Comparison_of_CPU_architectures , Hsien , 2012.04.19
//-----------------------------------------------------------------------------------------------------------------
#ifndef _PLC_PFCUNIT_H
#define _PLC_PFCUNIT_H
#include "basic_def.h"


#define _64BITS 0
#define	_32BITS	1

#if	_32BITS
	#define	DATA_SIZE	5
	#define BIT_MASK	4		//	which content(index) represent bitMask
	//--------------------------------------------------------------
	// turns content of target pointer into address
	//	32bits data to 64bits pointer warning
	//	won't happended if knows target machines type at beginning
	//	controlled by precompiler switch _32BITS/_64BITS
	//--------------------------------------------------------------
	#define GET_ADDR( data )			(*(SGN32 *) (data) )
	#define	SET_ADDR( data , target_p)	*(SGN32*)(data) = (SGN32)target_p	
			//	data	:	base of	 length-4 integer array
			//	target	:	point to target data location
	#pragma warning (disable:4312)

#else if	_64BITS
	#define	DATA_SIZE	9
	#define BIT_MASK	8
	#define GET_ADDR( data )			(*(SGN64 *) (data) )
	#define	SET_ADDR( data , target)	*(SGN64*)(data) = (SGN64)&target
#endif

const UNS08	bitMasks[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};	//	constant reference
#define	GET_BMSK( data )		(((SGN08*)data)[BIT_MASK])			//
#define	SET_BMSK( data , bitIndex)	data[BIT_MASK] = bitMasks[bitIndex]		//	

//-------------------------------
//	alias define
//-------------------------------
typedef struct	pfc_unit_st		PFC_UNIT			;							
typedef struct	pfc_operator_st	PFC_OPERATOR		;
typedef struct	pfc_operand_st	PFC_OPERAND			;
typedef struct	pfc_progctrl_st	PFC_PROGCTRL		;

typedef PFC_OPERAND	(*PFC_OPERATION)(PFC_OPERAND* ,PFC_OPERAND*);
			//	parameter 1 : point to first	operand
			//	parameter 2 : point to second	operand
			//	return value: result after operation

enum	pfc_unit_type_enum
{
	OPERATOR	,
	OPERAND		,
	PROGCTRL	,
};	
enum	pfc_progctrl_cmd_enum
{
	COND_JUMP	=	0x01,			//	jump when data in stack is not true , step is counted in relative path
	/*NEG_COND_JUMP*/				//	jump when data in stack is true		, step is counted in relative path
	JUMP		=	0x02,			//	unconditional jump , added by Hsien 2012.04.19 , step is counted in relative path
	PROG_WAIT	=	0x04,			//	
	EXPR_END	=	0x08,			//	clear stack
	PROG_END	=	0x10,			//	exit execution	
};

struct	pfc_operator_st
{
	BOOLEAN			isUnary;			//	for deciding how many operand should pop out
	void*			operation;			//	function pointer
};

typedef SGN32	(*get_func)(void* pfcOperandData);
typedef void	(*set_func)(void* pfcOperandData,SGN32 value);
struct	pfc_operand_st
{
	//------------------------------------------------------------
	//	function pointer , for handling this operand
	//	used to decide whether get/set by reference either value
	//------------------------------------------------------------
	get_func				get;				
	set_func				set;
	SGN08				data[DATA_SIZE];			//	abstract data , may be in reference or value
											//	1-4 : data address/dword bit string
											//	last container[5th] is bitMask when in BOOLEAN
};
struct	pfc_progctrl_st
{
	UNS08				ctrlCmd	;
	SGN32				insNum	;	//	for conditional jump
									//	ver1 : program position refer to first instruction , i.e refer to instruction[0]
									//	UNS16 , implict max surported instrction length is 65535(16bits)
									//	ver2 : relative program position ,refer to current instruction postition.
									//	so that , it's possiblem to jump forward or backward. Hsien , 2012.06.11

									//	"offset" , is much more offical calling to such thing
									//	commented after data survey , Hsien , 2012.04.19

									//	if prog_wait occured , this variable would represent sub-program's identifier
									//	commented by Hsien ,2012.04.19
};

struct	pfc_unit_st
{
	UNS08				unitType;
	union
	{
		PFC_OPERATOR	operatorData;
		PFC_OPERAND		operandData	;
		PFC_PROGCTRL	progCtrlData;
	};
};

namespace BY_VALUE
{
	//--------------------------------------------------
	//	type regardless , bool/char/short/int are represented in 32 bits
	//	by 2's complement rule , adding for all of them could complished by binary adding
	//--------------------------------------------------
	SGN32	get	(SGN32*	data);
	void	set	(SGN32* data,SGN32 value);
}

namespace BY_REFERENCE
{
	//---------------------------------------------------
	//	Drain data along address
	//---------------------------------------------------
	SGN32	getBool		(void*	data);
	SGN32	getByte		(void*	data);
	SGN32	getWord		(void*	data);
	SGN32	getDword	(void*	data);
	REAL32	getFloat	(void*	data);		
	//---------------------------------------------------
	//	Drain data along address
	//---------------------------------------------------
	void	setBool		(void*	data,BOOLEAN value);
	void	setByte		(void*	data,SGN08	value);
	void	setWord		(void*	data,SGN16	value);
	void	setDword	(void*	data,SGN32	value);
	void	setFloat	(void*	data,REAL32	value);
}

#endif