#include<LDE_OUTDEV.h>

int main()
{
	odIndexerCoil od(ODNC_CONFIRM);
	odIndexerCoil od2(ODNC_CONFIRM);
	int	index;
	string genStr;
	string dataPack;

	od.setIndex(17);
	//------------
	//	Gen string test
	//------------
	genStr = od.getGenStr();

	//------------
	// Index test
	//------------
	index = od.getIndex();

	//-----------
	//	export test
	//-----------
	dataPack = od.exportData();

	//-----------
	//	import test
	//-----------
	od2.importData(dataPack.c_str());

	return 1;
}