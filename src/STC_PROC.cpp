#include<STC_VAROP.h>
#include<STC_PCDPM.h>		// 
#include<STC_PROC.h>
#include<string.h>

#include<STC_ASM_COMMONS.h>

#pragma warning(disable:4996)

int varProcess(	const char*			varName,
				const char			type,
				const size_t		wordIndex,
				const size_t		bitByteIndex,
				IRC_OPERAND			*output)
{
	//--------------------------------------------------------
	//	closed form function , no any external variable needed
	//--------------------------------------------------------
	VAR		varAttributes;
	int		errCode;

	strcpy(varAttributes.name,varName);				// added by Hsien , 2012.10.05
	varAttributes.type			=	type;
	varAttributes.wordIndex		=	wordIndex;
	varAttributes.bitByteIndex	=	bitByteIndex;
	

	if((errCode = var2IrcOperand(refDb,varAttributes
		,output)) != VAR_SUCCESS)
		return errCode;		// return stcErrorHandler(moduleName,errorCode) , STC_ERR
		
	output->type = VARIABLE;

	return errCode;	// return STC_SUCCESS
}

int	opProcess(const char *opName,IRC_UNIT* outputElement)
{
	//--------------------------------------------------------
	//	closed form function , no any external variable needed
	//--------------------------------------------------------
	int errCode,opId;

	if((errCode = op2IrcOperator(refDb,opName,&opId)) != VAR_SUCCESS)
		return errCode;	// return stcErrHandler

	//----------------------
	//	Set-up Element's type
	// Hsien 2013/12/6
	//----------------------
	outputElement->operatorId = (unsigned char)opId;
	outputElement->type		 = _OPERATOR;
	
	return errCode;
}

int branchProcess		(PCDPM_FAMILY*	family , const int _type , const int _jumpOffset/* to adjest the additional jump steps , Hsien , 2013.01/21*/)
{
	//------------------------------------------
	//	Branch Code Generating: ( This Part Handle "if(condition) statement")
	//	Calculating Child Branch length , 
	//	write all info-blocks from Child Branch into Parent Branch
	//		than Close Child Branch
	//------------------------------------------
	int		 errCode;
	IRC_UNIT outputElement;
	//--------------------------------------------------------
	//	closed form function , no any external variable needed
	//--------------------------------------------------------
	outputElement.type					= _PROGCTRL;
	outputElement.progCtrlData.ctrlCmd	= _type/*_COND_JUMP*/;		// swithcing _type according to input argument
																	// for reusing this function
																	// Hsien , 2013.01.21
	outputElement.progCtrlData.insIndex	= (unsigned short)
		/*+*/( _jumpOffset					// after condition jump output , expected	
											// when singal-if statement , the offset = 1
											// when if-else statement , the offset = 2
		+ pcdpmQueryCounts(family,CHILD))	// compound statement size					
		;									// value in incremental meaning , Hsien , 2012.10.04
		
	if((errCode = pcdpmWriteData(family		
		,&outputElement
		,PARENT)) != PCDPM_SUCCESS)
		return errCode;						// 1. write-in COND_JUMP block in parent first
	
	if((errCode = pcdpmCloseChild(family)) != PCDPM_SUCCESS)
		return errCode;						// 2. then merge 'all' child into parent and close child

	return errCode;
}

int negationProcess(void)
{
	//----------------------
	//	1. Prepare outputElem->Operand , which is CONSTANT-type with value -1
	//	2. Write into PCDPM queue
	//	3. Prepare Op block - multiplication
	//	4. Write into PCDPM quque
	//	5. formed postfix expession
	// Hsien 2013/12/6
	//----------------------
	IRC_UNIT	tmpBlock;
	int			opId = 0;
	const char*	op	 = "*";
	int		 errCode;
	extern int				stlineno;


	tmpBlock.type = _OPERAND;
	tmpBlock.operandData.type		= CONSTANT;
	tmpBlock.operandData.integer	= -1;			// key point , Hsien 2013/12/6
	
	stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&tmpBlock,generation),popText(),stlineno);

	//----------------------
	//	Setup multiplication block
	//----------------------
	tmpBlock.type = _OPERATOR;
	if((errCode = op2IrcOperator(refDb,op,&opId)) != VAR_SUCCESS)
		return errCode;	// return stcErrHandler
	tmpBlock.operatorId = (unsigned char)opId;

	stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&tmpBlock,generation),popText(),stlineno);


	return errCode;
}

//------------------------------
//	Raw Stream Storage Mechanism
//------------------------------
char	textBuffer	[32];
char	outBuffer	[32];
void pushText(const char* str)
{
	strcat(textBuffer,str);				// append new string into buffer
}

const char* popText(void)
{
	strncpy(outBuffer,textBuffer,32);	// copy string to output buffer
	strset(textBuffer,0);				// reset string
	return outBuffer;
}

void resetText(void)
{
	strset(textBuffer,0);
}

