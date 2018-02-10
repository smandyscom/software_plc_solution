#include<sqlite3.h>
#include<PLC_PVM.h>
#include<STC_DQI.h>

//----------------------
//	Going to test
//	1. Form2 Tab Loading/Format test
//	2. Priority Running Mode form2 test
// Hsien 2013/11/20
//----------------------
#define TEST_PATH "d:\workdir\vs2005projects\PLC framework\branches\SAP_TAB\proj\pfcvm\pvmTest\tabForm2Template.txt"

extern int	startWithRs(void*	arg);
extern int	startViaDqiWithoutRs(sqlite3*	_db);

int main()
{
	FILE*	fp = NULL;
	PVM	pvmMain;
	sqlite3*	db = NULL;
	int			errorCode;

	dqiInit(&db,"D:\\workdir\\vs2005projects\\PLC framework\\branches\\SAP_TAB\\resource\\PLC.DB");


	fp = fopen("tabForm2Template.txt","rb");
	pvmProcDirPreset("D:\\workdir\\vs2005projects\\PLC framework\\branches\\SAP_TAB\\resource\\irc_files\\");



	pvmInitialize(&pvmMain,10,(int(*)(void*))startViaDqiWithoutRs,db);

	errorCode = pvmLoadFromTab2(&pvmMain,fp);
	//----------------------
	//	Running mode test
	//----------------------
	while(1)
		pvmRunPriority2(&pvmMain,0);


	return 0;
}