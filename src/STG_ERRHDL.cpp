#include<STG_ERRHDL.h>
#include<string.h>

#pragma warning(disable:4996)

//---------------------------------------------------
// generic error handler? inherited from STC ERR BLK?
//--------------------------------------------------

#define	STG_ERR_MSG_OVERINDEX_ROW		"Row over-indexed"
#define	STG_ERR_MSG_OVERINDEX_COLUMN	"Column over-indexed"
#define	STG_ERR_MSG_ROUTEOPENED			"Route opened"		// when next element is neither route nor vertice
#define	STG_ERR_MSG_UNKNOWN				"Unknown error"

#define STG_FATAL	STC_FATAL

int	stgErrorHandler(PCDPM_FAMILY*	errStack
					,const int		errCode
					,const char*	errContent
					,const int		rungSn)
{
	//--------------
	//	multi-thread version: all variable are referenced
	//--------------
	STG_ERRBLK	errorBlock;

	if(errCode == STG_SUCCESS)
		return STG_SUCCESS;

	//------------------------------------------------
	//	Query DB-Base to get explainition string
	//		have to modify dqiQueryError into Multi-thread version
	//		before apply it on this error handler
	//------------------------------------------------
	//dqiQueryError(modName,localErrCode,&errorAttr);

	//---------------------------------------
	//	STC ERRORCODE = modSn + localErrorCode
	//----------------------------------------
	errorBlock.errCode = 0;			//reset
	((char*)&errorBlock.errCode)[1]	= MOD_STG;
	((char*)&errorBlock.errCode)[0]	= errCode;	

	errorBlock.errLevel		= STG_FATAL	;
	errorBlock.errLineNo	= rungSn	;
	strcpy(errorBlock.errContent,errContent);
	//-----------------------------------------------------------
	//	instead , hard-coding applied so far , Hsien , 2012.10.12
	//-----------------------------------------------------------
	switch(errCode){
		case STG_ERR_OVERINDEX_ROW:		strcpy(errorBlock.errExplain,STG_ERR_MSG_OVERINDEX_ROW);		break;
		case STG_ERR_OVERINDEX_COLUMN:	strcpy(errorBlock.errExplain,STG_ERR_MSG_OVERINDEX_COLUMN);	break;
		case STG_ERR_ROUTEOPENED:		strcpy(errorBlock.errExplain,STG_ERR_MSG_ROUTEOPENED);		break;
		case STG_ERR_UNKNOWN:			strcpy(errorBlock.errExplain,STG_ERR_MSG_UNKNOWN);			break;
	}

	pcdpmWriteData(errStack,&errorBlock,0);		// write into stack

	return STG_FATAL;
}