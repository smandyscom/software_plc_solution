#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

#include <PLC_FSMI.h>
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

int pvm_init(void)
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
	using namespace FSMI;
	initialize();

	int i=0;
	while(frmAttr[i].id != 0){
		if(!addFrame(frmAttr[i].id,frmAttr[i].totalSize))
			return FALSE;
		i++;
	}
	//---
	//	After Initializing : self-loop test
	//---
	i=0;
	while(frmAttr[i].id != 0){
		if(!queryAddress(frmAttr[i].id,frmAttr[i].totalSize,0))
			return FALSE;
		i++;
	}


	//--------------------------------------------
	//	to complish this in dynamic linked form in future work
	//	commented by Hsien , 2012.04.0
	//	and grab attribute information from entry funtion rather from data-base
	//	commented by Hsien , 2012.04.24
	//	following part could be implemented by Code Generator + Data base
	//		Hsien , 2012.06.12
	//--------------------------------------------
	stmtGen(sqlStmt,"id","operators");	
	if ( sqlite3_prepare ( db, sqlStmt, -1, &ppStmt, NULL ) == SQLITE_OK )
		for(int i=0;sqlite3_step ( ppStmt ) != SQLITE_DONE; i++)
			opsAttr[i].id = sqlite3_column_int(ppStmt,0);

	stmtGen(sqlStmt,"isUnary","operators");	
	if ( sqlite3_prepare ( db, sqlStmt, -1, &ppStmt, NULL ) == SQLITE_OK )
		for(int i=0;sqlite3_step ( ppStmt ) != SQLITE_DONE; i++)
			opsAttr[i].isUnary = sqlite3_column_int(ppStmt,0);
	qsort(opsAttr,256,sizeof(operator_attributes_st),::descendSort);

	OPMAG::initialize();

	OPMAG::addOp(opsAttr[0].id,opsAttr[0].isUnary,Assign);	
	OPMAG::addOp(opsAttr[1].id,opsAttr[1].isUnary,Not);
	OPMAG::addOp(opsAttr[2].id,opsAttr[2].isUnary,And);
	OPMAG::addOp(opsAttr[3].id,opsAttr[3].isUnary,Or);
	OPMAG::addOp(opsAttr[4].id,opsAttr[4].isUnary,Divide);
	OPMAG::addOp(opsAttr[5].id,opsAttr[5].isUnary,Multiply);
	OPMAG::addOp(opsAttr[6].id,opsAttr[6].isUnary,Substract);
	OPMAG::addOp(opsAttr[7].id,opsAttr[7].isUnary,Add);
//-----------------------------------------------------------
//	acheivied by Code Generator , due to static function properties
//	impossible to dynamic building function in run time.
//	Hsien , 2012.06.12
//-----------------------------------------------------------
	return 1;
} // coldStart()
