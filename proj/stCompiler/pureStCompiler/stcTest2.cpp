#include<STC_ASM.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main()
{
	FILE*	infp	= fopen(".\\testFile_IFELSE.st","r");	
	FILE*	outfp   = fopen("..\\..\\..\\resource\\irc_files\\test.irc","w");

	sqlite3*		db = NULL;
	STC_ERRHDL		errorHandler;

	size_t	_errorCount;
	STC_ERRBLK	_errblk;
	int errCode;
	int	count = 0;

	//------------------------------------------
	//	DB and Error Handler Init
	//------------------------------------------
	dqiInit(&db,"..\\..\\..\\resource\\PLC.DB");
	stcErrorHandlerInit(&errorHandler,db);
	//------------------------------------------
	//	STC init
	//------------------------------------------
	if(stcInit(db,&errorHandler) == STC_FATAL)
		return 0;

	if(stcCompile(infp,outfp,STC_MODE_NORMAL) == STC_ERROR){
		while(stcErrorRead(&errorHandler,&_errblk) != STC_NO_MORE_ERROR)
			stcErrorFwrite(&_errblk,stdout);
	}
	else
		printf("Compile OK\n");

	stcClose();
	stcErrorHandlerFinalize(&errorHandler);
	dqiClose(db);

	fclose(infp);
	fclose(outfp);

_CrtDumpMemoryLeaks();	// memory leakage happened in stparse() , malloc in yy_flex_alloc
						// ok! memory leak duduced , by yy_delete_buffer calling in stparseClose();

	return 1;		// file output ok , test done,  Hsien , 2012.10.11
}