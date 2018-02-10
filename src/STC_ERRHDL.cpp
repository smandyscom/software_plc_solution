#include<STC_ERRHDL.h>
#include<string.h>
#include<stdlib.h>
#pragma warning(disable:4996)

//extern PCDPM_FAMILY		errorQueue			;			// for standard error output of STC
//														// actually it is error stack , Hsien , 2013.01.07
//
//extern sqlite3				*db				;			// FOR FUNCTION OF STC(except the errorHandler) , it's acceptable to use global data
//														// Hsien , 2013.01.31
///*static*/ PCDPM_FAMILY*	errorStack /*= NULL*/;		// make this error handler possible to redirect output
//														// Hsien , 2012.10.12
///*static*/ size_t*			errorCount;



int	stcErrorHandlerInit	(STC_ERRHDL	*stcErrHdl
						 //,PCDPM_FAMILY*	_errorStack/*const char* dbName*/
						 ,sqlite3*		_db)
{
	//--------------------------
	//	try to accessing data base 
	//		and check out if field exists
	//	Presumption: errorStack had been initialized
	//		Hsien , 2012.10.12
	//--------------------------
	memset(stcErrHdl,0,sizeof(STC_ERRHDL));

	stcErrHdl->errorStack	= (PCDPM_FAMILY*)malloc(sizeof(PCDPM_FAMILY));/*_errorStack;*/
	pcdpmInit(stcErrHdl->errorStack,tmpfile(),sizeof(STC_ERRBLK));

	stcErrHdl->dbRef		= _db;
	stcErrHdl->errorCount	= &(stcErrHdl->errorStack->root.counts);
	//errorCount = &(errorStack->root.counts);

	return 1/*STC_SUCCESS*/;
}
int	stcErrorHandlerFinalize	(STC_ERRHDL	*stcErrHdl)
{
	fclose		(stcErrHdl->errorStack->root.output);
	pcdpmClose	(stcErrHdl->errorStack);
	free		(stcErrHdl->errorStack);
	return 1;
}


int stcErrorHandler		(STC_ERRHDL	*stcErrHdl
						 ,const char		modSn				// module serial number
						 ,int				localErrCode
						 ,const char*		localErrContent
						 ,const int			lineNo)
{
	//------------------------------------------------------------
	//	Presumption: DATA_BASE have to be alived
	//	1. reject SUCCESS code
	//	2. querying data base by input info
	//	2. prepare ERROR_BLOCK according to error code and content
	//	3. write into errorQueue
	//------------------------------------------------------------
	STC_ERRBLK errorBlock;
	ERRATTR	   errorAttr;
	char	   modName[32];

	//-------------
	//	Socket connected to external data pack
	//-------------
	PCDPM_FAMILY	*errorStack	= stcErrHdl->errorStack;
	sqlite3			*db			= stcErrHdl->dbRef;

	if(localErrCode == STC_SUCCESS)		// reject SUCCESS codes (all SUCCESS codes are mapping to zero)
		return STC_SUCCESS;


	//------------------------------------------
	//	turns numerical mod number into mod name , for query data-base
	//------------------------------------------
	switch(modSn){
		case MOD_DQI:	strcpy(modName,TABLE_NAME_DQI_ERRCODE);		break;
		case MOD_VAR:	strcpy(modName,TABLE_NAME_VAR_ERRCODE);		break;
		case MOD_PCDPM: strcpy(modName,TABLE_NAME_PCDPM_ERRCODE);	break;
		case MOD_LY:	strcpy(modName,TABLE_NAME_LY_ERRCODE);		break;
		case MOD_STG:	strcpy(modName,TABLE_NAME_STG_ERRCODE);		break;
		default:
			break;
	}

	//------------------------------------------------
	//	Query DB-Base to get explainition string
	//------------------------------------------------
	dqiQueryError(db,modName,localErrCode,&errorAttr);

	//---------------------------------------
	//	STC ERRORCODE = modSn + localErrorCode
	//----------------------------------------
	errorBlock.errCode = 0;			//reset
	((char*)&errorBlock.errCode)[1]	= modSn;
	((char*)&errorBlock.errCode)[0]	= localErrCode;			
	
	errorBlock.errLineNo	= lineNo;
	errorBlock.errLevel		= errorAttr.errLevel;
	strcpy(errorBlock.errContent,localErrContent);
	strcpy(errorBlock.errExplain,errorAttr.errExplain);
	
	pcdpmWriteData(errorStack/*&errorQueue*/,&errorBlock,0);
				// redirect by Hsien  , 2012.10.12

	//--------------------
	//	error level returning
	//--------------------
	if(errorBlock.errLevel == STC_FATAL)
		return STC_FATAL;

	return STC_ERROR;
}


int stcErrorFwrite		(STC_ERRBLK	*errBlk
						 ,FILE*	fp)
{
	//--------------------------------------------------------------
	//	e.g
	//	error : 4671 , line 23 , "Z"		variable is not valid.
	//	error : 3452 , line 47 , "X0.17"	variable is over-indexed
	//	fatal : 3535 , line 35 , "PLC.DB"	data-base is missing
	//--------------------------------------------------------------
	char	errLevelString[8];

	switch(errBlk->errLevel){
		case STC_FATAL:	strcpy(errLevelString,"FATAL"); break;
		case STC_ERROR: strcpy(errLevelString,"ERROR");	break;
		default:
			break;
	}

	fprintf(fp,STC_ERR_FWRITE_FORMAT
		,errLevelString
		,errBlk->errCode
		,errBlk->errLineNo
		,errBlk->errContent
		,errBlk->errExplain);

	return STC_SUCCESS;
}

int stcErrorRead(STC_ERRHDL	*stcErrHdl,
				 STC_ERRBLK	*outBlk)
{
	//------------------------------------
	//	public interface for reading error
	//------------------------------------
	if(*stcErrHdl->errorCount == 0)
		return STC_NO_MORE_ERROR;				// no more error
	
	pcdpmReadData(stcErrHdl->errorStack,outBlk,0);		// read out blk

	return STC_SUCCESS;
}

int	stcErrorReset(STC_ERRHDL	*stcErrHdl)
{
	//----------------------------------
	//
	//----------------------------------
	pcdpmResetData(stcErrHdl->errorStack,0);

	return STC_SUCCESS;
}

size_t	stcErrorCount(STC_ERRHDL*	stcErrHdl)
{
	//---------------------------------------
	//	Usend to Query How many Errors the Handler Holds
	//---------------------------------------
	return *stcErrHdl->errorCount;
}