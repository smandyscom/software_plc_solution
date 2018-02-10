//---------------------------------------------------------
// FILENAME				: PLC_DQI.cpp
// ORIGIN AUTHOR		: Hsien	,	2012.04.18
// EDITOR				: Hsien
// NOTE					: Database Query Interface 
//							1. query frame attributes	by name-string
//							2. query operator id		by name-string
//		Advice : possible to use sprintf to generate formatted string
//---------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<STC_DQI.h>

#pragma warning(disable:4996)		// i like STANDARD functions without reason
//--------
//
//	some SQL example : 
//		1. select id from frame where name in ('SYNC_REQ')
//				# search for id from 'frame' table ,and its name matched 'SYNC_REQ'
//		2. select id from operators where string in ('+')
//				# search for id from 'operators' table , and its name mathced '+'
//-----------

//static sqlite3					*db = NULL;

#define	SQL_QUERY_BASIC			"SELECT %s FROM %s	"
#define	SQL_QUERY_WHEREIN		" WHERE %s IN ('%s')"

//-------------------
//	PUBLIC INTERFACEs
//-------------------
int			dqiInit(sqlite3	**db,const char* dbPath)
{	
	FILE*	fp;
	char				sqlStmt[256];	// data localized , Hsien , 2013.01.31		
	sqlite3_stmt		*ppStmt		;	// data localized , Hsien , 2013.01.31
	//----------------
	//	open data base
	//----------------
	memset(sqlStmt,0,sizeof(sqlStmt));

	if(!(fp = fopen(dbPath,"r")))
		return DQI_ERR_NO_DB;		// error , data base missing
	fclose(fp);						// used to check if file existed. Hsien , 2012.06.11

	if (*db != NULL)
		return DQI_ERR_REPEAT_INIT;	// in order to reject repeated initialization , Hsien , 2013.01.03

	if ( sqlite3_open (dbPath, db ) == SQLITE_ERROR )
		return DQI_ERR_NO_MEM;			// memory allocating error , no any db-not-found error will occur
	//-------------------------------------------------------------------------
	//	ought to do further checking , i.e check if data-base content's format 
	//	is in expection
	//	implement em in the future
	//	Hsien , 2012.04.18
	//-------------------------------------------------------------------------
	sprintf(sqlStmt,SQL_QUERY_BASIC,"*",TABLE_NAME_FRAME);
	if( sqlite3_prepare(*db,sqlStmt,-1,&ppStmt,NULL) != SQLITE_OK){
		//----------------
		// frame table not found
		//----------------
		sqlite3_finalize(ppStmt);
		sqlite3_close(*db);
		return DQI_ERR_NO_FRM_TABLE;
	}
	sqlite3_finalize(ppStmt);					// added by Hsien , 2012.10.11
												// sqlite3_prepare/finalize should existed in pair for preventing memory-leakage

	sprintf(sqlStmt,SQL_QUERY_BASIC,"*",TABLE_NAME_OPERATORS);
	if( sqlite3_prepare(*db,sqlStmt,-1,&ppStmt,NULL) != SQLITE_OK){
		//--------------------------
		// operation table not found
		//--------------------------
		sqlite3_finalize(ppStmt);
		sqlite3_close(*db);
		return DQI_ERR_NO_OP_TABLE;
	}

	sqlite3_finalize(ppStmt);
	return DQI_SUCCESS;
}

int			dqiClose(sqlite3	*db)
{
	sqlite3_close		( db );
	db = NULL;

	return DQI_SUCCESS;
}

int				dqiQueryOperatorId(sqlite3	*db,const char *opStr,int* opId)
{
	char				sqlStmt[256]	;	// data localized , Hsien , 2013.01.31
	char				appendStmt[32]	;
	sqlite3_stmt		*ppStmt			;	// data localized , Hsien , 2013.01.31

	sprintf	(sqlStmt		,SQL_QUERY_BASIC	,FIELD_NAME_ID		,TABLE_NAME_OPERATORS);
	sprintf	(appendStmt		,SQL_QUERY_WHEREIN	,FIELD_NAME_STR		,opStr);
	strcat	(sqlStmt		,appendStmt);
	
	if(sqlite3_prepare(db,sqlStmt,-1,&ppStmt,NULL) != SQLITE_OK){
		sqlite3_finalize ( ppStmt );					// critical move , otherwise moemory leakage would happend , Hsien 2012.04.17
		return DQI_ERR_NO_OP;
	}
	if(sqlite3_step ( ppStmt ) == SQLITE_DONE){			//	single step(result) assumed , not generic
														//	if result is multiple , choosed the first one
		sqlite3_finalize ( ppStmt );
		return DQI_ERR_NO_OP;
	}

	*opId = sqlite3_column_int	(ppStmt,0);		// id-column

	sqlite3_finalize			( ppStmt );
	return DQI_SUCCESS;
}

int				dqiQueryVariable(sqlite3	*db,const char *varName
								 ,FRMATTR*	varAttributes)
{
	//------------------
	//	given:	1.	variable name
	//			2.	type
	//			3.	word index
	//			4.	bit/byte index(optional)
	//	output:	1.	frame id
	//			2.	elemSize(byte/word/dword)	
	//			3.	elemIndex
	//			4.	bit index(optional)
	//	return code
	//------------------
	//	SQL : 
	//	select id,totalSize,elementMaxNum,elementSize from frame where name in ('X')
	//-----------------
	char				sqlStmt[256]	;	// data localized , Hsien , 2013.01.31		
	char				appendStmt[32]	;
	sqlite3_stmt		*ppStmt			;	// data localized , Hsien , 2013.01.31

	memset(/*&*/varAttributes,0,sizeof(varAttributes));

	sprintf	(sqlStmt		,SQL_QUERY_BASIC	,FIELDS_VAR			,TABLE_NAME_FRAME);
	sprintf	(appendStmt		,SQL_QUERY_WHEREIN	,FIELD_NAME_STR		,varName);
	strcat	(sqlStmt		,appendStmt);

	if(sqlite3_prepare(db,sqlStmt,-1,&ppStmt,NULL) != SQLITE_OK){
		//----------------------------------------------
		//	turns charateristic statement into prepared statement 
		//	divided choosed scope from raw table
		//, Hsien , 2012.10.4															
		//----------------------------------------------
		sqlite3_finalize ( ppStmt );	//	critical move , otherwise memory leakage would happend , Hsien 2012.04.17
		return DQI_ERR_NO_VAR/*varAttributes*/;			//	return with frameId = 0 , means error
	}

	if(sqlite3_step ( ppStmt ) == SQLITE_DONE){
		sqlite3_finalize ( ppStmt );
		return DQI_ERR_NO_VAR/*varAttributes*/;			//	return with frameId = 0 , means error
	}

	varAttributes->frameId			=	sqlite3_column_int(ppStmt,0);	//id	-column
	varAttributes->totalSize		=	sqlite3_column_int(ppStmt,1);	//size	-column
	varAttributes->elementMaxNum	=	sqlite3_column_int(ppStmt,2);	//count	-column
	varAttributes->elementSize		=	sqlite3_column_int(ppStmt,3);	//size	-column

	sqlite3_finalize ( ppStmt );
	return DQI_SUCCESS;
}

int	dqiQueryError(sqlite3	*db,const char* modName
				  ,const int localErrorCode
				  ,ERRATTR*	 errAttributes)
{
	//------------------
	//	given:	1.	mod name
	//			2.	local error code
	//	output:	1.	errAttributes
	//	return code
	//	sql statement:
	//		select localErrorLevel,localExplaination FROM dqiErrorCodes WHERE localErrorCode IN ('1')
	//------------------
	char				sqlStmt	[256]	;	// data localized , Hsien , 2013.01.31		
	char				appendStmt[32]	;
	sqlite3_stmt		*ppStmt			;	// data localized , Hsien , 2013.01.31

	char	_string[4];					// string buffer for itoa
	char	*errLevelString;

	sprintf	(sqlStmt		,SQL_QUERY_BASIC	,FIELDS_ERR			,modName);
	sprintf	(appendStmt		,SQL_QUERY_WHEREIN	,FIELD_NAME_ERRCODE	,itoa(localErrorCode,_string,10));
	strcat	(sqlStmt		,appendStmt);

	if(sqlite3_prepare(db,sqlStmt,-1,&ppStmt,NULL) != SQLITE_OK){
		sqlite3_finalize ( ppStmt );
		errAttributes->errLevel			=ERR_LEVEL_FATAL;
		strcpy(errAttributes->errExplain,ERR_UNKNOWN_STRING);
		return DQI_ERR_UNKNOWN_ERROR;
	}

	if(sqlite3_step ( ppStmt ) == SQLITE_DONE){
		sqlite3_finalize ( ppStmt );
		errAttributes->errLevel			=ERR_LEVEL_FATAL;
		strcpy(errAttributes->errExplain,ERR_UNKNOWN_STRING);
		return DQI_ERR_UNKNOWN_ERROR;
	}

	//-----------------------------
	//	string-numerical conversion
	//-----------------------------
	errLevelString = (char*)sqlite3_column_text(ppStmt,0);
	if(strcmp(errLevelString,"FATAL") == 0)				// Zero means equal
		errAttributes->errLevel = ERR_LEVEL_FATAL;
	else if(strcmp(errLevelString,"ERROR") == 0)
		errAttributes->errLevel = ERR_LEVEL_ERROR;

	strcpy(errAttributes->errExplain,(const char*)sqlite3_column_text(ppStmt,1));

	sqlite3_finalize(ppStmt);

	return DQI_SUCCESS;
}



int	dqiLoadFrameAttr(sqlite3	*db,FRMATTR**	ref,size_t*	counts)
{
	int					index = 0;
	char				sqlStmt[256];	// data localized , Hsien , 2013.01.31		
	sqlite3_stmt		*ppStmt		;	// data localized , Hsien , 2013.01.31

	sprintf(sqlStmt,SQL_QUERY_BASIC,FIELDS_VAR,TABLE_NAME_FRAME);

	size_t		frmAttrCounts = 0;
	FRMATTR*	frmAttrBuffer = NULL;
	
	if(sqlite3_prepare(db,sqlStmt,-1,&ppStmt,NULL) != SQLITE_OK){
		//----------------------------------------------
		//	turns charateristic statement into prepared statement 
		//	divided choosed scope from raw table
		//, Hsien , 2012.10.4															
		//----------------------------------------------
		sqlite3_finalize ( ppStmt )	;	//	critical move , otherwise memory leakage would happend , Hsien 2012.04.17
		return DQI_ERR_NO_VAR		;	//	return with frameId = 0 , means error
	}
	
	//-----------------------------------------------
	//	Counting How Many Frames this data-base holds
	//-----------------------------------------------
	frmAttrCounts = 0;
	while(sqlite3_step ( ppStmt ) != SQLITE_DONE)
		frmAttrCounts++;
	sqlite3_finalize(ppStmt);			// Avoiding Memory Leakage , Hsien , 2012.10.29

	//------------------------------------------------
	//	Reallocating Buffer according to frame-counts
	//------------------------------------------------
	frmAttrBuffer = 
		(FRMATTR*)realloc(frmAttrBuffer,sizeof(FRMATTR) * frmAttrCounts);

	//------------------------------------------------
	//	Loading Buffer Iterativly;
	//------------------------------------------------
	sqlite3_prepare(db,sqlStmt,-1,&ppStmt,NULL);
	index = 0;
	while(sqlite3_step ( ppStmt ) != SQLITE_DONE){
		frmAttrBuffer[index].frameId		=	sqlite3_column_int(ppStmt,0);	//id	-column
		frmAttrBuffer[index].totalSize		=	sqlite3_column_int(ppStmt,1);	//size	-column
		frmAttrBuffer[index].elementMaxNum	=	sqlite3_column_int(ppStmt,2);	//count	-column
		frmAttrBuffer[index].elementSize	=	sqlite3_column_int(ppStmt,3);	//size	-column
		
		strcpy(frmAttrBuffer[index].frameString,(const char*)sqlite3_column_text(ppStmt,4));	// Hsien 2013/6/4 //string	-column
		//----------------------
		//	Attribute for IO-maintain(monitor and override)
		// Hsien 2013/6/4
		//----------------------
		frmAttrBuffer[index].isOverridable			=	sqlite3_column_int(ppStmt,5);
		frmAttrBuffer[index].overrideFrameOffset	=	sqlite3_column_int(ppStmt,6);
		frmAttrBuffer[index].monitorFrameOffset		=	sqlite3_column_int(ppStmt,7);


		index++;
	}
	sqlite3_finalize(ppStmt);

	*ref	= frmAttrBuffer;
	*counts	= frmAttrCounts;


	return DQI_SUCCESS;
}