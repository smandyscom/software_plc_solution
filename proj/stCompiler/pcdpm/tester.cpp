//#include"d:\workdir\vs2005projects\PLC  framework\src\STC_PCDPM.cpp"
#include<STC_PCDPM.h>
#include<stdio.h>
#include<string.h>

#define SOP if(1){printf("Hi\n");}

int main()
{
	FILE*	fp = tmpfile();
	int arrayInt[128];
	size_t fpSize = 0;
	int data;

	pcdpm_init(fp);

/*	pcdpm_creatChild();

	for(int i=0;i<10;i++){
		pcdpm_writeInChild((void*)&i,sizeof(int));
	}
	data = 32767 ; pcdpm_writeInParent((void*)&data,sizeof(int));
	pcdpm_closeChild();
	data =3 ;	pcdpm_writeInChild((void*)&data,sizeof(int));*/

	for(int i=0;i<10;i++)		// reverse sequence
		pcdpm_creatChild();
	for(int i=0;i<10;i++){
		//pcdpm_writeInParent((void*)&i,sizeof(int));		// old style interface
		pcdpm_writeData((void*)&i,sizeof(int),1);			// new style interface
															// 1: first parent
															// 2. grand parent
															// .. so on.
		pcdpm_closeChild();
	}

	pcdpm_close();

	fpSize = ftell(fp);
	rewind(fp);
	fread(arrayInt,sizeof(char),fpSize,fp);

	SOP;
	return 0;
}