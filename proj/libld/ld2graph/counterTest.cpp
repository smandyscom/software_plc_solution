#include<LDE_OUTDEV.h>

int main()
{
	string					str;
	odRingCounterSet		set;
	odRingCounterCount		count(ODRINGCTR_COUNTUP);

	set.setCounterIndex(1);			// counter index
	set.setCounterGoal(16);			// count to
	set.setCounterInit(3);			// count from
	str = set.getGenStr();
	//----------
	//	Set OK , Hsien , 2013.01.22
	//----------

	count.setCounterIndex(21);		// count-up
	str = count.getGenStr();

	count.setCounterType(ODRINGCTR_COUNTDOWN);
	count.setCounterIndex(43);
	str = count.getGenStr();		// count-down
	//----------
	//	Test Done , Hsien , 2013.01.22
	//----------

	return 1;
}