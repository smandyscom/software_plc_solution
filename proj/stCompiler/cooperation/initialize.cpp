#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

#include <PLC_FCLCT.h>
#include <PLC_OPMAG.h>
#include <PLC_OPLIB.h>

#pragma warning (disable:4996)	// unsafe function used warning , quite a plot of M$ , Hsien , 2012.04.06

static int descendSort( const void *elem1, const void *elem2)
{
	return ((*((UNS16*)elem2)) - (*((UNS16*)elem1)));
}
struct frame_attributes_st
{
	UNS16	id		;
	size_t	totalSize	;
};
struct operator_attributes_st
{
	UNS16		id		;
	BOOLEAN		isUnary	;
};

static void stmtGen(char* buf,const char*	fieldName,const char* tableName)
{
	strcpy(buf,"SELECT ");
	strcat(buf,fieldName);
	strcat(buf," FROM ");
	strcat(buf,tableName);
}

int initialize(void)
{
	frame_attributes_st		frmAttr[256];
	operator_attributes_st	opsAttr[256];
	
	char		sqlStmt[256];
	sqlite3			*db;
	sqlite3_stmt	*ppStmt;

	//---
	//	local variable initialize
	//---
	memset(sqlStmt,0,sizeof(sqlStmt));
	memset(frmAttr,0,sizeof(frmAttr));
	memset(opsAttr,0,sizeof(opsAttr));

	if ( sqlite3_open ("D:\\workdir\\vs2005projects\\PLC  framework\\resource\\PLC.DB", &db ) != SQLITE_OK )	//	temporary facility
		return 0;

	//------------------------------
	// 1.drain info into internal memory(opinfo , frameinfo)
	// 2.use these info initialize OPMAG,FCLCT	
	//------------------------------

	//-----------------------------
	//	SELECT framId FROM frame
	//	3rd arg: -1 means all
	//--------------------------
	stmtGen(sqlStmt,"id","frame");
	if ( sqlite3_prepare ( db, sqlStmt, -1, &ppStmt, NULL ) == SQLITE_OK )	
		for(int i=0;sqlite3_step ( ppStmt ) != SQLITE_DONE;i++)
			frmAttr[i].id = sqlite3_column_int( ppStmt,0);

	stmtGen(sqlStmt,"totalSize","frame");	
	if ( sqlite3_prepare ( db, sqlStmt, -1, &ppStmt, NULL ) == SQLITE_OK )
		for(int i=0;sqlite3_step ( ppStmt ) != SQLITE_DONE; i++)
			frmAttr[i].totalSize = sqlite3_column_int(ppStmt,0);
	qsort(frmAttr,256,sizeof(frame_attributes_st),::descendSort);
	FCLCT::Initialize();

	int i=0;
	while(frmAttr[i].id != 0){
		if(!FCLCT::AddFrame(frmAttr[i].id,frmAttr[i].totalSize))
			return FALSE;
		i++;
	}
	//---
	//	After Initializing : self-loop test
	//---
	i=0;
	while(frmAttr[i].id != 0){
		if(!FCLCT::QueryAddress(frmAttr[i].id,frmAttr[i].totalSize,0))
			return FALSE;
		i++;
	}

	stmtGen(sqlStmt,"id","operators");	
	if ( sqlite3_prepare ( db, sqlStmt, -1, &ppStmt, NULL ) == SQLITE_OK )
		for(int i=0;sqlite3_step ( ppStmt ) != SQLITE_DONE; i++)
			opsAttr[i].id = sqlite3_column_int(ppStmt,0);

	stmtGen(sqlStmt,"isUnary","operators");	
	if ( sqlite3_prepare ( db, sqlStmt, -1, &ppStmt, NULL ) == SQLITE_OK )
		for(int i=0;sqlite3_step ( ppStmt ) != SQLITE_DONE; i++)
			opsAttr[i].isUnary = sqlite3_column_int(ppStmt,0);
	qsort(opsAttr,256,sizeof(operator_attributes_st),::descendSort);

	OPMAG::Initialize();
	//--------------------------------------------
	//	to complish this in dynamic linked form in future work
	//	commented by Hsien , 2012.04.09
	//	and grab attribute information from entry funtion rather from data-base
	//	commented by Hsien , 2012.04.24
	//--------------------------------------------
	OPMAG::AddOp(opsAttr[0].id,opsAttr[0].isUnary,Assign);	
	OPMAG::AddOp(opsAttr[1].id,opsAttr[1].isUnary,Not);
	OPMAG::AddOp(opsAttr[2].id,opsAttr[2].isUnary,And);
	OPMAG::AddOp(opsAttr[3].id,opsAttr[3].isUnary,Or);
	OPMAG::AddOp(opsAttr[4].id,opsAttr[4].isUnary,Divide);
	OPMAG::AddOp(opsAttr[5].id,opsAttr[5].isUnary,Multiply);
	OPMAG::AddOp(opsAttr[6].id,opsAttr[6].isUnary,Substract);
	OPMAG::AddOp(opsAttr[7].id,opsAttr[7].isUnary,Add);

	return 1;
} // coldStart()
