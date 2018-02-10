#include<PLC_DQI.h>

int main()
{
	if(!dqi_init("_PLC.db"))
		return 0;

	return 1;
}