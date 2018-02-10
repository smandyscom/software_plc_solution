#include<LDE_METADATA.h>
#include<stdlib.h>
#include<time.h>

using namespace metaData;
int main()
{
	srand(time(NULL));
	//-------------------
	//	going to test clst::fWrite,fRead for Depth-2 Cluster
	//-------------------
	CLST_METADATA*	clst	= NULL , *clst2 = NULL;
	DEV_METADATA*	tmpDev	= NULL;
	FILE*			fp = tmpfile();

	clst = cluster::initialize(
		(DUPLICATOR)device::duplicate
		,(DESTRUCTOR)device::finalize
		,2);
	clst->leafFileWriter = (FILEWRITER)device::fWrite;
	clst->leafFileReader = (FILEREADER)device::fRead;

	clst2 = cluster::initialize(
		(DUPLICATOR)cluster::duplicate
		,(DESTRUCTOR)cluster::finalize
		,2);
	clst2->leafFileWriter = (FILEWRITER)cluster::fWrite;
	clst2->leafFileReader = (FILEREADER)cluster::fRead;


	tmpDev = device::initialize();

	device::writeIn(tmpDev,rand(),"DYUIKNBG");
	cluster::writeIn(clst,0,device::duplicate(tmpDev));
	device::writeIn(tmpDev,rand(),"GHJIKBYUHIO");
	cluster::writeIn(clst,1,device::duplicate(tmpDev));

	cluster::writeIn(clst2,0,cluster::duplicate(clst));
	cluster::writeIn(clst2,1,cluster::duplicate(clst));

	cluster::fWrite(clst2,fp);
	cluster::finalize(clst2);
	clst2 = NULL;
	rewind(fp);
	cluster::fRead(&clst2,fp);
		// depth-2 tested OK, data write-in, read-out successfully
		// Hsien ,2012.10.1

	return 0;
}