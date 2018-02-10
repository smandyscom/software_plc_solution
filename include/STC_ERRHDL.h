//---------------------------------
//
//---------------------------------
#ifndef _STC_ERRHDL_H
#define _STC_ERRHDL_H
#include<STC_DQI.h>
#include<STC_PCDPM.h>

typedef struct stc_err_blk_st	STC_ERRBLK;
typedef struct stc_err_hdl_st	STC_ERRHDL;
//--------------------
//	INHERITED FROM DQI
//--------------------
#define STC_FATAL	ERR_LEVEL_FATAL		// cannot excutes anymore , have to return immediatly
#define STC_ERROR	ERR_LEVEL_ERROR		// error , but able to continue
#define STC_SUCCESS ERR_LEVEL_SUCCESS	// 

#define	MOD_DQI		'D'					// module code
#define	MOD_VAR		'V'		
#define	MOD_PCDPM	'P'
#define MOD_LY		'L'
#define	MOD_STG		'S'					// added by Hsien , 2013.01.07

#define STC_NO_MORE_ERROR	STC_ERROR

struct stc_err_blk_st
{
	//--------------------------------------
	//	quering information of data base
	//		and used to do formatting output
	//	e.g
	//	error : 4671 , line 23 , "Z"		variable is not valid.
	//	error : 3452 , line 47 , "X0.17"	variable is over-indexed
	//	fatal : 3535 , line 35 , "PLC.DB"	data-base is missing
	//--------------------------------------
	int		errCode										;
	int		errLineNo									;
	char	errLevel									;
	char	errContent[ERROR_CONTENT_LENGTH]		;
	char	errExplain[ERROR_EXPLAIN_LENGTH]		;
};


struct stc_err_hdl_st
{
	//-----------------
	//	The STC ErrorHandler Data Struct(CLASS)
	//		Hsien , 2013.01.31
	//-----------------
	sqlite3*			dbRef		;		// all error message , codes are refer to this data-base
	PCDPM_FAMILY*		errorStack	;
	size_t*				errorCount	;
};
#define STC_ERR_FWRITE_FORMAT	"%s : %u , line %u , \"%s\" %s\n"


int		stcErrorHandlerInit		(STC_ERRHDL*	stcErrHdl
									,sqlite3*		_dbRef);		// handler need db to support
																// Hsien , 2013.01.31
int		stcErrorHandlerFinalize	(STC_ERRHDL	*stcErrHdl);
int		stcErrorHandler		(STC_ERRHDL*	stcErrHdl
						 ,const char		modSn				// module serial number
						 ,int			localErrCode
						 ,const char*	localErrContent
						 ,const int		lineNo);
int		stcErrorFwrite		(STC_ERRBLK*	errBlk/*source*/,FILE*	fp/*output*/);
int		stcErrorRead		(STC_ERRHDL*	stcErrHdl,STC_ERRBLK*	outBlk);
int		stcErrorReset		(STC_ERRHDL*	stcErrHdl);
size_t	stcErrorCount		(STC_ERRHDL*	stcErrHdl);


#endif