#include<LD_OUTDEV.h>

int main()
{
	setD	s;

	//s.strGen("Y0.01","X0.00"); old version of strGen , short of this struct is  , due to build-in template string
								// and they'r totally repeative ,  would cost unnessary memory waste
								// so that , use friend function to implement
	//s.strGen("Y0.02","D0.00");

	strGen(s,"Y0.01","X0.00");
	strGen(s,"Y0.02","D0.00");
	return 0;
}