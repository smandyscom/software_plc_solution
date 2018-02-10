//---------------------------------------------------------
// FILENAME				: PLC_VAROP.h
// ORIGIN AUTHOR		: Hsien	,	2012.04.18
// EDITOR				: Hsien
// NOTE					: VAR/OP Query/conversion Interface  
//---------------------------------------------------------
#ifndef _PLC_VAR2IRC_H
#define _PLC_VAR2IRC_H

#include<PLC_PFC.h>
#include<PLC_IRC.h>
#include<STC_DQI.h>

#define VAR_NAME_LENGTH		16

//---------------------------
//	ERROR CODE DEFINITION , use 2nd BYTE to identify
//---------------------------
#define	VAR_SUCCESS			0x00
#define	VAR_ERR_OVERINDEX	0x07						//	index is out of scope
#define	VAR_ERR_ODD_INDEX	0x08						//	word index couldn't be odd
//--------------------
//	INHERITED FROM DQI
//--------------------
#define	VAR_ERR_NO_VAR			DQI_ERR_NO_VAR			// no corresponding variable for given string
#define	VAR_ERR_NO_OP			DQI_ERR_NO_OP			// no corresponding operator for given string
#define	VAR_ERR_NO_DB			DQI_ERR_NO_DB			// data-base file is missing
#define VAR_ERR_NO_MEM			DQI_ERR_NO_MEM			// memory exhausted when accessing data-base
#define	VAR_ERR_NO_FRM_TABLE	DQI_ERR_NO_FRM_TABLE	// no "frame"	table in data-base
#define VAR_ERR_NO_OP_TABLE		DQI_ERR_NO_OP_TABLE		// no "op"		table in data-base

//enum _var_type
//{
//--------------------------
//	VARIABLE TYPE DEFINITION
//--------------------------
#define	_BOOLEAN	0/*	=0,*/
#define	_INT08		1/*= 1,*/
#define	_INT16		2/*= 2,	*/
#define	_INT32		4/*= 4,*/
#define	_FLOAT32	4/*= 4,*/
#define	_UNKNOWN	9
//};

typedef struct var_st VAR;
struct var_st
{
	char	name[VAR_NAME_LENGTH];
	char	type;			// boolean,int08,int16,int32,float
	size_t	wordIndex;
	size_t	bitByteIndex;	// bit or byte Index
};

extern int		(*varOpInit)		(sqlite3	**db		,const char* dbPath)			;
extern int		(*varOpClose)		(sqlite3	*db)									;
int				var2IrcOperand		(sqlite3	*db	,const VAR varAttr	,IRC_OPERAND	*outOp)			;	// frmAttr_st is offered by DQI
extern int		(*op2IrcOperator)	(sqlite3	*db	,const char *opStr	,int			*opId)	;	// renamed function

int				varString2Var		(sqlite3	*db	,const char*	string	,VAR			*varAtr);		// convert raw-string into internal var-format
int				var2PfcOperand		(sqlite3	*db	,const VAR varAttr		,PFC_OPERAND	*pfcOperand);	// pfcOperand have to be initialized before feeding in , Hsine , 2012.11.26
																						// implicit PFC_OPERAND


#endif