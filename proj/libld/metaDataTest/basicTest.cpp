#include<LDE_METADATA.h>
#include<stdlib.h>
#include<time.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


int main()
{
	int code;
	void*	fp;
	FILE*	_fp = tmpfile();
	metaData::DEV_METADATA	*dev;
	
	code = metaData::metaFunc2Code(metaData::cluster::duplicate);
	fp	 = metaData::metaCode2Func(code);
		// okay worked.

	dev = metaData::device::initialize();

	srand(time(NULL));
	using namespace metaData;

	device::writeIn(dev,rand(),"ABCDEFGggdfgsdgdsgrtgrhgrthrhrehrhrhreasfejkrjhgfe");
	device::fWrite(dev,_fp);
	device::finalize(dev);
	dev = NULL;
	rewind(_fp);
	device::fRead(&dev,_fp);
		// done , device file write/reader worked

	fclose(_fp);
	device::finalize(dev);
	_CrtDumpMemoryLeaks();

	return 0;
}