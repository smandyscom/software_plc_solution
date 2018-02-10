#include<LDE_OUTDEV.h>

int main()
{
	odRingCounterCountType2	ctr;
	odRingCounterCountType2 ctr2;
	string	dataPack;
	string	genStr;
	string	genStr2;

	//----------------------
	//	Setup
	//----------------------
	ctr.setCounterIndex(1);
	ctr.setCounterInit("1");
	ctr.setCounterGoal("5");
	ctr.setCounterStep("2");

	//----------------------
	//	Gen 
	//----------------------
	genStr = ctr.getGenStr();

	//----------------------
	//	Export
	//----------------------
	dataPack = ctr.exportData();

	//----------------------
	//	Import
	//----------------------
	ctr2.importData(dataPack.c_str());
	genStr2 = ctr2.getGenStr();

	return 1;
}