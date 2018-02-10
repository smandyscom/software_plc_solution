#include<LDE_VERTICE.h>

int main()
{
	string s;
	char *str;

	vertice			v;
	normalOpen		no;
	normalClose		nc;
	_short			srt;

	normalOpen		*noPtr;

	no.setBitName("X0.01");
	nc.setBitName("Y0.02");
	s = nc.getData();

	str = (char*)no.exportData();

//	v = (*(vertice*)&nc & *(vertice*)&no);
	v = ((vertice)nc & (vertice)no);
	v = ((vertice)nc | (vertice)no);
	s = ((vertice)nc | (vertice)no).getData();
	s = ((vertice)nc | (vertice)srt).getData();
	s = ((vertice)nc & (vertice)srt).getData();

	no.~normalOpen();
	no.getData();

	noPtr = new normalOpen();
	delete noPtr;

	return 0;
}