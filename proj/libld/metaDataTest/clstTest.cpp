#include<LDE_METADATA.h>
#include<stdlib.h>
#include<time.h>

using namespace metaData;
int main()
{
	srand(time(NULL));
	//-------------------
	//	going to test clst::fWrite,fRead
	//		and if memory leakage happens
	//-------------------
	CLST_METADATA*	clst	= NULL;
	DEV_METADATA*	tmpDev	= NULL;
	FILE*			fp = tmpfile();

	clst = cluster::initialize(
		(DUPLICATOR)device::duplicate
		,(DESTRUCTOR)device::finalize
		,2);
	clst->leafFileWriter = (FILEWRITER)device::fWrite;
	clst->leafFileReader = (FILEREADER)device::fRead;

	tmpDev = device::initialize();

	device::writeIn(tmpDev,rand(),"DYUIKNBG");
	cluster::writeIn(clst,0,device::duplicate(tmpDev));
	device::writeIn(tmpDev,rand(),"GHJIKBYUHIO");
	cluster::writeIn(clst,1,device::duplicate(tmpDev));

	cluster::fWrite(clst,fp);
	cluster::finalize(clst);
	clst = NULL;
	rewind(fp);
	cluster::fRead(&clst,fp);

	return 0;
}