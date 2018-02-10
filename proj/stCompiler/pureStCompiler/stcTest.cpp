#include<STC_ASM.h>

extern PCDPM_FAMILY	errorQueue;

int main()
{
	FILE*	infp	= fopen(".\\testFile.st","r");	
	FILE*	outfp   = tmpfile();
	size_t	_errorCount;
	STC_ERRBLK	_errblk;
	int errCode;

	if(stcInit("..\\..\\..\\resource\\PLC.DB") == STC_FATAL)
		return 0;

	if(stcCompile(infp,outfp,STC_MODE_NORMAL) == STC_ERROR){
		_errorCount = *errorCount;
		while(*errorCount != 0){
			pcdpmReadData(&errorQueue,&_errblk,0);
			stcErrorFwrite(&_errblk,stdout);
//			_errorCount--;
		}
	}

	stcClose();

	return 1;
}