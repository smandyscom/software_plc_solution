#include<LDE_VERTICE.h>

int main()
{
	int index = 0;
	nonStopTimerNo	testTimer;

	testTimer.setTimerIndex(3);
	index = testTimer.getTimerIndex();

	testTimer.setTimerIndex(27);
	index = testTimer.getTimerIndex();
	//-------
	//	done , no problem , Hsien , 2013.01.16
	//-------

	return 1;
}