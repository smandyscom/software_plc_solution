#include<STC_VAR2IRC.h>
#include<STC_DQI.h>
#include<STC_PCDPM.h>		// 

int varProcess(	const char*			varName,
				const char			type,
				const size_t		wordIndex,
				const size_t		bitByteIndex,
				IRC_OPERAND/*&*/		*output)
{
	//--------------------------------------------------------
	//	closed form function , no any external variable needed
	//--------------------------------------------------------
	/*static*/ VAR		varAttributes;
	/*static*/ FRMATTR	frmAttributes;
	int		errCode;

	if((errCode = dqiQueryVariable(varName
		,&frmAttributes)) != DQI_SUCCESS)
		return 1;		// return STC_ERR_?
	//if(!frmAttributes.frameId)
	//	return 0;					// failed variable name
		
	varAttributes.type			=	type;
	varAttributes.wordIndex		=	wordIndex;
	varAttributes.bitByteIndex	=	bitByteIndex;
	
	//output
	//	=	var2IrcOperand(varAttributes,frmAttributes);
	if((errCode = var2IrcOperand(varAttributes
		,frmAttributes
		,output)) != VAR_SUCCESS)
		return 1;		// return stcErrorHandler(moduleName,errorCode) , STC_ERR
		
	//if(!output.variable.frmId)
	//	return 0;					//	something wrong
	
	output->type = VARIABLE;

	return 1;	// return STC_SUCCESS
}

int	opProcess(const char *opName,IRC_UNIT& outputElement)
{
	//--------------------------------------------------------
	//	closed form function , no any external variable needed
	//--------------------------------------------------------
	int errCode,opId;
	if((errCode = dqiQueryOperatorId(opName,&opId)) != DQI_SUCCESS)
		return 1;	// return stcErrHandler
	//if(!opId)
	//	return 0;
		
	outputElement.operatorId = opId;
	outputElement.type		 = _OPERATOR;
	
	return 1;
}

/*void*/int branchProcess()
{
	int		 errCode;
	IRC_UNIT outputElement;
	//--------------------------------------------------------
	//	closed form function , no any external variable needed
	//--------------------------------------------------------
	outputElement.type					= _PROGCTRL;
	outputElement.progCtrlData.ctrlCmd	= _COND_JUMP;
	outputElement.progCtrlData.insIndex	= (unsigned short)
		/*+*/( 1							// after condition jump output , expected	
		+ pcdpmQueryCounts(CHILD))	// compound statement size					
		;							// value in incremental meaning , Hsien , 2012.10.04
		
	if((errCode = pcdpmWriteData(&outputElement
		,sizeof(IRC_UNIT)
		,PARENT)) != PCDPM_SUCCESS)
		return errCode;
	
	if((errCode = pcdpmCloseChild()) != PCDPM_SUCCESS)
		return errCode;			 // merge child into parent and close child

	return 1;
}