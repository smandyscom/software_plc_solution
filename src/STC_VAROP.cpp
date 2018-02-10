//---------------------------------------------------------
// FILENAME				: PLC_VAROP.h
// ORIGIN AUTHOR		: Hsien	,	2012.04.18
// EDITOR				: Hsien
// NOTE					: 
//---------------------------------------------------------
#include<STC_VAROP.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
static div_t	hex2Oct_bits(const size_t wordIndex,const size_t bitIndex)
{
	//--------------
	//	turns representation of Hex into Oct , for boolean variable
	//	e.g input wordIndex = 3 , bitIndex = 4 , return {6,4} means byteIndex = 6 , bitIndex = 4
	//	surely , if input wordIndex = 0 , bitIndex = 231 , such kind bit-specific input is allowed
	//	will be useful when device number is counted by its amounts , Hsien , 2012.04.17
	//	index: start from 0
	//--------------
	 size_t numerator;
	 div_t res;


	numerator = wordIndex * 16 + bitIndex ;			//	all units in bit
	return (res = div((int)numerator,8));			//	quot		:	byte	Index			, counst from 0
												//	reminder	:	bit		Index in byte	, counts from 0 , max at 7
}

static size_t	hex2Oct_byte(const size_t wordNum,const size_t byteNum)
{
	return wordNum * 2 + byteNum ;	// all units in bit
}

static int		frmCheck(const size_t totalSize,const size_t index,const size_t elemSize)	
{
	//-----------------------------------------------------
	//	elemSize,totalSize were represented in unit of BYTE
	//	index was represeted in unit of elemSize
	//-----------------------------------------------------
	if(elemSize*(index + 1) > totalSize)	// less or equal is acceptable
		return VAR_ERR_OVERINDEX;
	return VAR_SUCCESS;
}
//-------------------
//	PUBLIC INTERFACEs
//-------------------
int		(*varOpInit)		(sqlite3	**db,const char* dbPath) = dqiInit;
int		(*varOpClose)		(sqlite3	*db)					 = dqiClose;
int		(*op2IrcOperator)	(sqlite3	*db,const char *opStr,int* opId) = dqiQueryOperatorId;
int		var2IrcOperand		(sqlite3	*db,const VAR varAttr/*,const FRMATTR frmAtr*/,IRC_OPERAND*	op)
{
	//-------------------------------------------------------
	//	turns variable representation into IRC representation
	//	arguments : 1. varAttr , variable's attribute , including 
	//					type,word index , bit/byte index(optional).
	//					
	//				2. frmAttr , frame's attribute , used to verify if variable 
	//						input is valid. this information could come from Data Base
	//	output:
	//				IRC_OPERAND , if var data is consistent with setting of frame
	//				output translated IRC_OPERAND block.
	//				otherwise , when error occurs , all things would be zero 
	//	return value :
	//				error code
	//					VAR_SUCCESS
	//					VAR_ERR_OVERINDEX
	//					VAR_ERR_ODD_INDEX
	//-------------------------------------------------------
	div_t	byteIndexBitIndex;
	size_t	byteIndex;
	FRMATTR	frmAtr;
	int		errCode;

	if((errCode = dqiQueryVariable(db,varAttr.name,&frmAtr)) != DQI_SUCCESS)
		return errCode;		// variable not found

	memset(op,0,sizeof(op));

	op->type = VARIABLE;
	switch(varAttr.type){
		//---------
		//	BOOLEAN
		//---------
		case _BOOLEAN :
			if(varAttr.bitByteIndex > 15)
				return VAR_ERR_OVERINDEX;									// added by Hsien , 2012.10.10
			byteIndexBitIndex = 
				hex2Oct_bits(varAttr.wordIndex,varAttr.bitByteIndex);		// defect , wont report error when over-index
																			// Hsien , 2012.10.10
			if((errCode = frmCheck(frmAtr.totalSize
				,byteIndexBitIndex.quot
				,sizeof(char))) != VAR_SUCCESS)
				return errCode;
			op->variable.frmId		=	frmAtr.frameId;
			op->variable.elemIndex	=	byteIndexBitIndex.quot;
			op->variable.elemSize	=	sizeof(char);
			op->variable.bitIndex	=	byteIndexBitIndex.rem;
			break;
		//--------
		//	INT08
		//--------
		case _INT08:
			if(varAttr.bitByteIndex > 1)
				return VAR_ERR_OVERINDEX;								// added by Hsien  ,2012.10.10
			byteIndex = 
				hex2Oct_byte(varAttr.wordIndex,varAttr.bitByteIndex);
			if((errCode = frmCheck(frmAtr.totalSize
				,byteIndex
				,sizeof(char))) != VAR_SUCCESS)
				return errCode;
			op->variable.frmId		=	frmAtr.frameId;
			op->variable.elemIndex	=	byteIndex;
			op->variable.elemSize	=	sizeof(char);
			op->variable.bitIndex	=	NONBIT;
			break;
		//-------
		//	INT16
		//-------
		case _INT16:
			if((errCode = frmCheck(frmAtr.totalSize
				,varAttr.wordIndex
				,sizeof(short))) != VAR_SUCCESS)	// turns into byte representaion
				return errCode;
			op->variable.frmId		=	frmAtr.frameId;
			op->variable.elemIndex	=	varAttr.wordIndex;
			op->variable.elemSize	=	sizeof(short);
			op->variable.bitIndex	=	NONBIT;
			break;
		//------------------
		//	INT32
		//------------------
		case _INT32:
			//----
			//	bug found : misaligned for 32bits integer may occured in user's input. 
			//	Hsien , 2012.04.17
			//	misalign check , wordIndex has to be even
			//------
			if(varAttr.wordIndex % 2)	// if there's reminder , means odd.
				return VAR_ERR_ODD_INDEX;
			if((errCode = frmCheck(frmAtr.totalSize
				,varAttr.wordIndex/2
				,sizeof(int))) != VAR_SUCCESS)	// turns into byte representaion
				return errCode;
			op->variable.frmId		=	frmAtr.frameId;
			op->variable.elemIndex	=	varAttr.wordIndex/2;
			op->variable.elemSize	=	sizeof(int);
			op->variable.bitIndex	=	NONBIT;
			break;
		default:
			return VAR_ERR_NO_VAR;
			break;	// error occurs
	}// switch

	return VAR_SUCCESS;
}

#define VAR_REGXP_FORMAT_1	"%[A-Za-z_]%u%[A-Za-z_.:]%u"			// regular-expression of VARIABLE
#define	VAR_REGXP_FORMAT_2	"%[A-Za-z_]%u"							// exclusive format for WORD
#define	VAR_REGXP_MATCH_COUNT_FORMAT1	4
#define	VAR_REGXP_MATCH_COUNT_FORMAT2	2

int	varString2Var(sqlite3	*db	,/*input*/const char*	string,/*output*/VAR	*tempVarAtr)
{
	//--------------------------------------------------------------
	//	Turns var-string to var attribute	, if the format is legal
	//--------------------------------------------------------------
	char	secondaryString[8];
	int		matchCount;

	memset(secondaryString,0,sizeof(secondaryString));

	//---------------------
	//	Scan Out format
	//---------------------
	matchCount = sscanf(string
		,VAR_REGXP_FORMAT_1
		,&tempVarAtr->name
		,&tempVarAtr->wordIndex
		,&secondaryString
		,&tempVarAtr->bitByteIndex);

	if(matchCount != VAR_REGXP_MATCH_COUNT_FORMAT1){
		//----------------------------
		//	in case of WORD expression
		//----------------------------
		matchCount = sscanf(string
			,VAR_REGXP_FORMAT_2
			,&tempVarAtr->name
			,&tempVarAtr->wordIndex);
		if(matchCount != VAR_REGXP_MATCH_COUNT_FORMAT2){
			tempVarAtr->type = _UNKNOWN;
			return VAR_ERR_NO_VAR;
		}
	}
	//----------------------
	//	identify Which TYPE
	//----------------------
	if(!strcmp(secondaryString,"."/*BOOL*/)){
		tempVarAtr->type = _BOOLEAN;
	}			
	else if(!strcmp(secondaryString,":"/*BYTE*/)){
		tempVarAtr->type = _INT08;
	}
	else if(!strcmp(secondaryString,tempVarAtr->name/*DWORD*/)){
		tempVarAtr->type = _INT32;
	}
	else if(strlen(secondaryString) == 0/*WORD*/){
		tempVarAtr->type = _INT16;
	}
	else{
		//-----------------------------------------------
		//	Left Item can't be recognized , unknown ERROR
		//-----------------------------------------------
		tempVarAtr->type = _UNKNOWN;
		return VAR_ERR_NO_VAR;
	}

	return VAR_SUCCESS;
}

int	var2PfcOperand(sqlite3	*db	,const VAR varAttr,PFC_OPERAND	*op)
{
	//-------------------------------------------------
	//	Turns Var-representation int PFC-operand , 
	//		which is containing phisical-memory-address
	//			and access by reference
	//	Presumption : FSMI had been initialized
	//-------------------------------------------------
	IRC_UNIT		ircUnit;
	PFC_UNIT		pfcUnit;
	int				errCode;

	ircUnit.type = _OPERAND;
	if((errCode = var2IrcOperand(db,varAttr,&ircUnit.operandData)) != VAR_SUCCESS)
		return errCode;

	if(irc2pfc(&pfcUnit,ircUnit) != IRC_SUCCESS)
		return VAR_ERR_NO_VAR;

	*op = pfcUnit.operandData;	// cast-back , and write-back , Hsien , 2012.11.26

	return VAR_SUCCESS;
}