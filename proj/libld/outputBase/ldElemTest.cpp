#include<LDE_ELEMENT.h>


int main()
{
	int			code;
	string		s;
	ldElement		le , duplicatedLe;
	normalClose		*nc;

	le.setType(VERTICE);
	le.setVertice(NORMAL_CLOSE);

	nc = (normalClose*)le.getVertice();

	nc->setBitName("X0.01");
	s = nc->getData();

	s = le.exportData(code);
	duplicatedLe.importData(code,s.c_str());	// ok!

	return 0;
}