#include<STC_DQI.h>

extern int	dqiQeuryError(const char* modName
				  ,const int localErrorCode
				  ,ERRATTR*	 errAttributes);

int main()
{
	ERRATTR	errBlk;

	dqiInit("d:\\workdir\\vs2005projects\\PLC framework\\resource\\PLC.DB");
	dqiQeuryError("varErrorCodes"
		,1
		,&errBlk);		// done , worked , Hsien , 2012.10.08
	

	return 1;
}