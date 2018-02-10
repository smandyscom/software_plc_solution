#include<STC_PCDPM.h>
#include<stdio.h>
#include<string.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define SOP if(1){printf("Hi\n");}

int main()
{
	int			data;
	size_t		counts;
	FILE*			fp = tmpfile();
	PCDPM_FAMILY	family1;

	pcdpmInit(&family1,fp,sizeof(data));
	pcdpmCreateChild(&family1);
	
	data = 1;pcdpmWriteData(&family1,&data,0);
	data = 2;pcdpmWriteData(&family1,&data,1);

	pcdpmReadData(&family1,&data,0);
	pcdpmReadData(&family1,&data,1);

	counts = pcdpmQueryCounts(&family1,0);

	pcdpmResetData(&family1,0);

	pcdpmReadIndexOfData(&family1,&data,0,0);
			// seems fine. Hsien , 2012.10.5

	pcdpmClose(&family1);
	fclose(fp);

	_CrtDumpMemoryLeaks();
			// no leakage , memory-safe , Hsien , 2012.10.5

	return 1;
}