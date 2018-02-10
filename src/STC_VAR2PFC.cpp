//---------------------------------------------------------
// FILENAME				: PLC_VAROP.h
// ORIGIN AUTHOR		: Hsien	,	2012.04.18
// EDITOR				: Hsien
// NOTE					: 
//---------------------------------------------------------
#include<STC_VAROP.h>

int	var2PfcOperand(const VAR varAttr,PFC_OPERAND	*op)
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
	if((errCode = var2IrcOperand(varAttr,&ircUnit.operandData)) != VAR_SUCCESS)
		return errCode;

	if(!irc2pfc(&pfcUnit,ircUnit))
		return VAR_ERR_NO_VAR;

	*op = pfcUnit.operandData;	// cast-back , and write-back , Hsien , 2012.11.26

	return VAR_SUCCESS;
}