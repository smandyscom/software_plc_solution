#include<LD_OUTDEV.h>

int main()
{
	odCoil			oc;
	outputDevice	od;
	odSet			os;

	oc.strGen("X0.00");
	oc.kindChange(IF_STMT);
	oc.kindChange(SINGLE);

	od = (outputDevice)oc;					// memberwise copy
	
	((odCoil*)&od)->kindChange(IF_STMT);	// down-casting
	((odCoil*)&od)->kindChange(SINGLE);

	os.strGen("D0","100");

	oc.strGen("X0.00\n");

	return 0;
}