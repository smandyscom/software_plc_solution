//--------------------------------------------------------------------
// FILENAME				: PLC_DQI.h
// ORIGIN AUTHOR		: Hsien	,	2012.04.18
// EDITOR				: Hsien
// NOTE					: Database Query Interface 
//							1. query frame attributes	by name-string
//							2. query operator id		by name-string
//--------------------------------------------------------------------
#ifndef _PLC_DQI_H
#define _PLC_DQI_H
#include<stddef.h>					// for size_t definition . Hsien , 2012.04.19
#include<sqlite3.h>

//-----------------------
//	error code definition , use 1st BYTE to identify
//-----------------------
#define	DQI_SUCCESS				0x00
#define	DQI_ERR_NO_VAR			0x01
#define	DQI_ERR_NO_OP			0x02
#define	DQI_ERR_NO_DB			0x03
#define DQI_ERR_NO_MEM			0x04
#define	DQI_ERR_NO_FRM_TABLE	0x05
#define DQI_ERR_NO_OP_TABLE		0x06
#define DQI_ERR_REPEAT_INIT		0x08

#define DQI_ERR_UNKNOWN_ERROR	0x07


#ifndef DB_NAME
	#define DB_NAME	"PLC.DB"		//	default path
#endif

#define	TABLE_NAME_FRAME		"frame"
#define TABLE_NAME_OPERATORS	"operators"

#define TABLE_NAME_DQI_ERRCODE		"dqiErrorCodes"
#define TABLE_NAME_VAR_ERRCODE		"varErrorCodes"
#define TABLE_NAME_PCDPM_ERRCODE	"pcdpmErrorCodes"
#define TABLE_NAME_LY_ERRCODE		"lyErrorCodes"		// yyerror
#define TABLE_NAME_STG_ERRCODE		"stgErrorCodes"		// st-generator error codes in LDE

#define	FIELD_NAME_ID			"id"
#define FIELD_NAME_STR			"string"									
#define FIELD_NAME_ERRCODE		"localErrorCode"							// added for error attributes  querying key , Hsien , 2012.10.08

//#define	FIELDS_VAR				"id,totalSize,elementCounts,elementSize"
#define	FIELDS_VAR				"id,totalSize,elementCounts,elementSize,string,isOverridable,overrideFrameOffset,monitorFrameOffset"	// Hsien 2013/6/4 , extended
#define FIELDS_ERR				"localErrorLevel,localExplaination"			// added for error explaination querying	, Hsien , 2012.10.08

//----------------------------
//	For DB-ERR-Query definition
//----------------------------
#define ERR_LEVEL_FATAL	2	// cannot parse anymore
#define ERR_LEVEL_ERROR	1	// error , but able to continue
#define ERR_LEVEL_SUCCESS 0	// the success code should set to be zero , for every module
#define ERR_UNKNOWN_STRING	"Unknown error"

typedef struct frmAttr_st FRMATTR;
typedef struct errAttr_st ERRATTR;

struct frmAttr_st
{
	int		frameId;
	size_t	totalSize;			//	unit in byte , totalSize = elementMaxNum * elementSize/8
	size_t	elementMaxNum;		//	how many elements are;
	size_t	elementSize;		//	unit in bits
	char	frameString[32];	// Hsien 2013/6/4 
	//----------------------
	//	Attribute for IO-maintain(monitor and override)
	// Hsien 2013/6/4
	//----------------------
	size_t	isOverridable;
	size_t	overrideFrameOffset;
	size_t	monitorFrameOffset;
};

#define	ERROR_EXPLAIN_LENGTH	64
#define	ERROR_CONTENT_LENGTH	32		// compound content

struct errAttr_st
{
	//---------------
	//	Added by Hsien , 2012.10.08 , for query error attributes
	//---------------
	char	errLevel;
	char	errExplain[ERROR_EXPLAIN_LENGTH];
};

int					dqiInit				(sqlite3	**db,const char* dbPath);			// avoid to using C++ namespace , which the parser not surport , Hsien , 2012.04.18
int					dqiClose			(sqlite3	*db);
int					dqiQueryOperatorId	(sqlite3	*db,const char *opStr,int*	opId);	// return errorCode if query successfully
int					dqiQueryVariable	(sqlite3	*db,const char *varName,FRMATTR*	varAttr);
int					dqiQueryError		(sqlite3	*db,const char* modName
										 ,const int localErrorCode
										 ,ERRATTR*	 errAttributes);

int					dqiLoadFrameAttr	(sqlite3	*db,FRMATTR**	ref,size_t*	counts);		// loading all frame's attributes in internal buffer


#endif