#include<PLC_DQI.h>

int main()
{
	if(!dqi_init("D:\\workdir\\vs2005projects\\PLC  framework\\resource\\PLC.DB"));
		return 0;

	return 1;
}