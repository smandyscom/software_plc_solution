//---------------------------------------------------------
// FILENAME				: PLC_IRC.h
// ORIGIN AUTHOR		: Hsien	,	2012.03.08
// EDITOR				: Hsien
// NOTE					: Intermidiate Representaion Code definition
//							Doesn't follow standard TAC
//							TAC was refered in wiki, may follow this standard in future
//							the Base of Windows Side(Code Generator Side) of LDPLC system
//---------------------------------------------------------
#ifndef _PLC_IRC_H
#define _PLC_IRC_H
#ifndef _BASIC_DEF_H
#include<BASIC_DEF.H>
#endif

#define ST_COMPILER				1
#define PFC_VIRTUAL_MACHINE		1


#define NONBIT	8

//-------------------
//	IRC-OPERAND DEFINATION
//-------------------
enum	_def_enum
{
	//-------
	//	UNIT TYPES DEFINITION
	//-------
	_OPERATOR,
	_OPERAND,
	_PROGCTRL,
	//-------
	//	OPERAND TYPES DEFINITION
	//-------
	CONSTANT,
	VARIABLE,
};

struct variable_st
{
	//--------------------------------
	// use to query memory manager
	// id , element size , index in indicated element size 
	// , bit index
	//--------------------------------
	UNS16	frmId;
	size_t	elemSize;		// defect : there's noly size information , 
							//	no any type info , i.e whether integer or float
							//	found by Hsien , 2012.04.17
	size_t	elemIndex;
		//----------------------------
		//	stand for "bitIndex" , counts from 0
		//	e.g bitIndex = 6 , indicate 7th bit of target Byte
		//----------------------------
	UNS08	bitIndex;		// edited by Hsien , 2012.04.17
};

typedef	struct irc_operand_st	IRC_OPERAND			;
struct	irc_operand_st
{
	UNS08				type	;
	union{
		struct variable_st			variable;
		SGN32				integer;
		REAL32				_float;
	};
};

enum	irc_progctrl_cmd_enum
{
	_COND_JUMP	=	0x01,
	/*NEG_COND_JUMP*/				//	jump when data in stack is true		, step is counted in relative path
	_JUMP		=	0x02,			//	unconditional jump , added by Hsien 2012.04.19
	_PROG_WAIT	=	0x04,			//	
	_EXPR_END	=	0x08,			//	clear stack
	_PROG_END	=	0x10,			//	exit execution	
};
typedef struct irc_progctrl_st IRC_PROGCTRL;
struct irc_progctrl_st
{
	UNS08	ctrlCmd	;
	union{
		UNS16	insIndex		;		// instruction index when COND_JUMP encounterd, edited by Hsien ,2012.04.19
		UNS16	sectionIndex	;		// section index , returned when PROG_WAIT encounterd. Hsien , 2012.10.31
										// if compling at ST stage , this represented as line-number
										// if at LD stage , this represented as rung-SN
		UNS16	progIdentifier	;
	};
};

//-------------------
//	IRC-UNIT DEFINATION
//-------------------
typedef	struct irc_unit_st		IRC_UNIT		;	
struct	irc_unit_st
{
	UNS08				type;
	union{	
		IRC_OPERAND		operandData;
		UNS08			operatorId;
		IRC_PROGCTRL	progCtrlData;
	};
};

//---------------------------------------------
//	IRC utilities
//---------------------------------------------

#include<stdio.h>
#include<string.h>

#define IRC_SUCCESS			0x00
#define IRC_ERR_UNKNOWN		0xf1
#define	IRC_ERR_OVERSIZE	0xf2
#define IRC_ERR_NO_MEM		0xf3
#define IRC_ERR_BAD_CODE	0xf4
#define	IRC_ERR_FILE_IO		0xf5		// Hsien , 2013.04.26

#define FORMAT_CODENO		"code:%d\t"
#define FORMAT_OPERATOR		"op:%d\t\n"
#define FORMAT_PROGCTRL		"command:%s\tinstruction:%d\n"
#define FORMAT_OPERAND		"type:%d\t"
#define FORMAT_VARIABLE		"frame id:%d\telementSize:%d\telemIndex:%d\tbitIndex:%d\n"
#define FORMAT_CONSTANT		"integer:%d\n"

void		ircFprintfInit();
void		ircFprintf(FILE*	fp,const IRC_UNIT irc);
int			ircFopen(const char* filename,IRC_UNIT** ircInstruction);	// open .irc file and allocating buffer for loading irc instructions
int			irc2pfc	(void*	pfc	,const IRC_UNIT	irc);
																		// finally return size of buffer , if opend successfully , Hsien , 2012.06.12
int			ircFread (FILE*		fp,IRC_UNIT** ircInstruction);

#endif

