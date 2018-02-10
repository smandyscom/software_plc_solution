
#include<LD_OUTDEV.h>

int main()
{
	odCoil c;
	odSet  s;
	char	str[32];

	//---------------------------------------------
	// black box test , using public functions only
	//---------------------------------------------
	c.strGen("D0.00");
	s.strGen("D0","10");

	c.setKind(IF_STMT);
	c.setKind(SINGLE);
	c.setKind(IF_STMT);

	strcpy(str,((outputDevice*)&c)->getGenStr());
	strcpy(str,((outputDevice*)&s)->getGenStr());

	return 0;
}