#include"d:\workdir\vs2005projects\PLC framework\src\STC_ERRHDL.cpp"
#include<stdio.h>
extern PCDPM_FAMILY		errorQueue		;			// for standard error output of STC

int main()
{
	FILE*		fp = tmpfile();
	STC_ERRBLK	errBlk,errBlk2;
	dqiInit("d:\\workdir\\vs2005projects\\PLC framework\\resource\\PLC.DB");
	pcdpmInit(&errorQueue,fp,sizeof(STC_ERRBLK));

	stcErrorHandler(MOD_VAR,1,"X0.01",44);
	pcdpmReadData(&errorQueue,&errBlk,0);

	stcErrBlkFwrite(&errBlk,stdout);

	pcdpmWriteData(&errorQueue,&errBlk,0);
	pcdpmWriteData(&errorQueue,&errBlk,0);
	pcdpmWriteData(&errorQueue,&errBlk,0);
	pcdpmWriteData(&errorQueue,&errBlk,0);
	pcdpmReadData(&errorQueue,&errBlk2,0);
	pcdpmReadData(&errorQueue,&errBlk2,0);
	pcdpmReadData(&errorQueue,&errBlk2,0);
	pcdpmReadData(&errorQueue,&errBlk2,0);
	pcdpmReadData(&errorQueue,&errBlk2,0);	// error will occur
	pcdpmReadData(&errorQueue,&errBlk2,0);	// error will occur
	pcdpmWriteData(&errorQueue,&errBlk,0);
	pcdpmWriteData(&errorQueue,&errBlk,0);
	pcdpmReadData(&errorQueue,&errBlk2,0);
	pcdpmReadData(&errorQueue,&errBlk2,0);
				// done , PCDPM works as a queue , Hsien , 2012.10.09


	return 1;
}