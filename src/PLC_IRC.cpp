//---------------------------------------------------------
// FILENAME				: PLC_IRC.cpp
// ORIGIN AUTHOR		: Hsien	,	2012.03.08
// EDITOR				: Hsien
// NOTE					: Intermidiate Representaion Code Definition
//							Doesn't follow standard TAC
//							TAC was refered in wiki, may follow this standard in future
//							the Base of Windows Side(Code Generator Side) of LDPLC system
//	how are you hard link
//---------------------------------------------------------
#include<PLC_IRC.h>
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#pragma warning(disable:4996)

#if PFC_VIRTUAL_MACHINE
#include<PLC_PFC.h>
#include<PLC_OPMAG.h>	//	for query operators
#include<PLC_FSMI.h>	//	for query variables

int			irc2pfc				(void*		pfc	,const IRC_UNIT		irc)
{
	/*static*/	void*		tmp;
	/*static*/	PFC_UNIT*	pfc_cast;

	pfc_cast = (PFC_UNIT*)pfc;
	switch(irc.type){
		case _OPERATOR:
			using namespace OPMAG;

			pfc_cast->unitType = OPERATOR;
			tmp = queryOp(irc.operatorId);
			if(!tmp)
				return IRC_ERR_BAD_CODE;
			pfc_cast->operatorData = *((PFC_OPERATOR*)tmp);
			break;
		case _OPERAND:
			using namespace FSMI;			// using query interface

			pfc_cast->unitType = OPERAND;

			switch(irc.operandData.type){
				case CONSTANT	:			// support integer so far , Hsien , 2012.04.23
					*((SGN32*)(pfc_cast->operandData.data)) = irc.operandData.integer;
					pfc_cast->operandData.get = (get_func)BY_VALUE::get;
					pfc_cast->operandData.set = (set_func)BY_VALUE::set;
					break;
				case VARIABLE	:
					tmp = queryAddress(irc.operandData.variable.frmId,
						irc.operandData.variable.elemSize,
						irc.operandData.variable.elemIndex);
					if(!tmp)
						return IRC_ERR_BAD_CODE;
					//-----
					//	MARCOS defined in PFC.h
					//-----
					SET_ADDR(pfc_cast->operandData.data , tmp);
					//-----------------
					//	JUDGE which type this operand is , in BOOL/CHAR/SHORT/INT
					//-----------------
					switch(irc.operandData.variable.elemSize){
						case sizeof(SGN08/*BYTE*/):			// edited by Hsien , to avoiding mis-definite , 2012.04.17
							if(irc.operandData.variable.bitIndex > 7){	// condition mismake , corrected by Hsien , 2012.04.24
								pfc_cast->operandData.get = (get_func)BY_REFERENCE::getByte;
								pfc_cast->operandData.set = (set_func)BY_REFERENCE::setByte;
							}
							else{
								pfc_cast->operandData.get = (get_func)BY_REFERENCE::getBool;
								pfc_cast->operandData.set = (set_func)BY_REFERENCE::setBool;
								SET_BMSK(pfc_cast->operandData.data , irc.operandData.variable.bitIndex);
							}
							break;
						case sizeof(SGN16/*WORD*/):
							pfc_cast->operandData.get = (get_func)BY_REFERENCE::getWord;
							pfc_cast->operandData.set = (set_func)BY_REFERENCE::setWord;
							break;
						case sizeof(SGN32/*DWORD*/):
							pfc_cast->operandData.get = (get_func)BY_REFERENCE::getDword;
							pfc_cast->operandData.set = (set_func)BY_REFERENCE::setDword;
							break;
						default:
							return IRC_ERR_BAD_CODE;
							break;
					}//switch
					break;
				default:
					return IRC_ERR_BAD_CODE;
			}//switch

			break;
		case PROGCTRL:
			pfc_cast->unitType		= PROGCTRL;
			if(		irc.progCtrlData.ctrlCmd != 	_COND_JUMP	
				&&	irc.progCtrlData.ctrlCmd != 	_PROG_WAIT
				&&	irc.progCtrlData.ctrlCmd != 	_EXPR_END
				&&	irc.progCtrlData.ctrlCmd != 	_PROG_END
				&&	irc.progCtrlData.ctrlCmd !=		_JUMP/* Added by Hsien , 2013.01.21*/)
				return IRC_ERR_BAD_CODE;

			pfc_cast->progCtrlData.ctrlCmd	= irc.progCtrlData.ctrlCmd;
			pfc_cast->progCtrlData.insNum	= irc.progCtrlData.insIndex;

			break;

		default:
			return IRC_ERR_BAD_CODE;
			break;
	}//switch()

	return IRC_SUCCESS;
}


#endif 
//---------------------------------------------
//	IRC print function in console mode
//---------------------------------------------
#if ST_COMPILER
static unsigned int codeIndex = 0;

void ircFprintfInit()
{
	codeIndex = 0;
}
void ircFprintf(FILE*	fp,const IRC_UNIT irc)
{
	static char format[/*32*/256];
	static char buffer[/*32*/64];

	memset(&format,0,sizeof(format));

	strcat(format,FORMAT_CODENO);

	switch(irc.type){
		case _OPERATOR	:
			strcat(format,FORMAT_OPERATOR);
			fprintf(fp,format,codeIndex,irc.operatorId);
			break;
		case _OPERAND	:
			strcat(format,FORMAT_OPERAND);
			if(irc.operandData.type == VARIABLE){
				strcat(format,FORMAT_VARIABLE);
				fprintf(fp,format,
					codeIndex,
					irc.operandData.type,
					irc.operandData.variable.frmId,
					irc.operandData.variable.elemSize,
					irc.operandData.variable.elemIndex,
					irc.operandData.variable.bitIndex);
			}
			else if(irc.operandData.type == CONSTANT){
				strcat(format,FORMAT_CONSTANT);
				fprintf(fp,format,
					codeIndex,
					irc.operandData.type,
					irc.operandData.integer);
			}
			else
				;
			break;
		case _PROGCTRL	:
			strcat(format,FORMAT_PROGCTRL);
			switch(irc.progCtrlData.ctrlCmd){
					case _COND_JUMP	:
						strcpy(buffer,"COND_JUMP");
						break;
					case _PROG_WAIT	:
						strcpy(buffer,"PROG_WAIT");
						break;
					case _EXPR_END	:
						strcpy(buffer,"EXPR_END");
						break;
					case _PROG_END	:
						strcpy(buffer,"PROG_END");
						break;
						//---------
						//
						//---------
					case _JUMP	:
						strcpy(buffer,"JUMP");
						break;
					default:
						strcpy(buffer,"UNKNOWN");		// added by Hsien , 2013.01.21
						break;
			}
			fprintf(fp,format,
				codeIndex,
				buffer,
				irc.progCtrlData.insIndex);
			break;
	}
	codeIndex++;
}
#endif 

int			ircFread (FILE*		fp,IRC_UNIT** ircInstruction)
{
	size_t	instructionSize;

	//----------------------
	//	evaluating file size
	//----------------------
	int size		= sizeof(IRC_UNIT);						// 24 bytes , Hsien , 2013.04.25
	int	err;

	if((err = fseek(fp,0L,SEEK_END)) != 0)
		return IRC_ERR_FILE_IO;							//	reposition to end-of-file
														//	cplus cplus , Hsine , 2013.04.25
														//	For streams open in binary mode, the new position is defined by adding offset to a reference position specified by origin.

	instructionSize = (ftell(fp)/sizeof(IRC_UNIT));		// counting file size
	if(instructionSize > INT_MAX){
		fclose(fp);
		return IRC_ERR_OVERSIZE;		// can't represent size
	}
	//rewind(fp);												// set position indicator to begining of file.
	if((err = fseek(fp,0L,SEEK_SET)) != 0)
		return IRC_ERR_FILE_IO;									// Hsien , 2013.04.25

	if(*ircInstruction != NULL)
		free(*ircInstruction);			// if ircInstruction has been allocated , free it
	*ircInstruction = (IRC_UNIT*)malloc(instructionSize * sizeof(IRC_UNIT));

	if(!(*ircInstruction)){
		fclose(fp);
		return IRC_ERR_NO_MEM;		// memory exhausted
	}

	if(fread(*ircInstruction,sizeof(IRC_UNIT),instructionSize,fp)
		!= instructionSize){
			//------------------
			// read out contents
			//------------------
			free(*ircInstruction);
			fclose(fp);
			return IRC_ERR_FILE_IO;
	}
	fclose(fp);

	return (int)instructionSize;
}

int			ircFopen(const char* filename,IRC_UNIT** ircInstruction)
{
	//---------------------------------------------------------------------------
	// open .irc file and allocating buffer for loading irc instructions
	// finally return size of buffer , if opened successfully , Hsien , 2012.06.12
	//----------------------------------------------------------------------------
	FILE*	fp;

//	fp = fopen(filename,"r");
	fp = fopen(filename,"rb");		// Mode Setted , Hsien , 2013.04.25

	if(!fp)
		return IRC_ERR_UNKNOWN;		// error occured

	return ircFread(fp,ircInstruction);
}