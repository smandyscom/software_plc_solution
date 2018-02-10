#include<LDE_OUTDEV.h>

void func1(char* buf)
{
	//buf = (char*)malloc(10);

	//strcpy(buf,"gjkl");
	free(buf);
	buf = 0;
}

//int main(void)
//{
//	string s;
//
//	char*	pack		= NULL;
//	size_t	packSize	=0;
//	odSet	test;
//	odCoil	coil;
//	outputDevice	base;
//	outputDevice*	od;
//	odSet *os;
//
//	test.setLhs("X0.00");
//	test.setRhs("X0.01");
//
//	s = test.getGenStr();
//	s = ((outputDevice*)&test)->getGenStr();
//	od = ((outputDevice*)&test);
//	od->getGenStr();			// access to derive's function
//
//	os = (odSet*)&base;
//	//os->getGenStr();			// access to base's function
//
//	packSize = test.exportData(&pack);
//	test.setLhs("0");
//	test.setRhs("90");
//	test.importData(pack,packSize);
//
//	//func1(pack);
//	coil.setBitName("Y0.02");
//	pack = (char*)malloc(10);
//	func1(pack);
////	free(pack);
//	pack = 0;
//	//free(pack);
//	packSize = coil.exportData(&pack);
//	coil.setBitName("0");
//	coil.importData(pack,packSize);
//
//	return 0;
//}


int main(void)
{
	size_t		 length;
	char		 *cStr;
	outputDevice od;
	odSet		 os;
	odCoil		 oc;

	os.setLhs("X0.00");
	os.setRhs("Y0.01");
	os.getGenStr();
	cStr = (char*)os.exportData();
	length = strlen(cStr);

	oc.setBitName("M1.11");
	oc.getGenStr();
	cStr = (char*)oc.exportData();

	cStr = (char*)malloc(14);
	strcpy(cStr,"hello.");
	free(cStr);

	//--------------------------------
	//	error occurs when free cStr
	//--------------------------------
	cStr
		= (char*)malloc(strlen(os.exportData())+1);	
	//	critical move , 
	//	need to allocating one more byte to place NULL when strcpy
	//	Hsien , 2012.07.05

	strcpy(cStr,os.exportData());					// after strcpy , it rise error.
	free(cStr);

	return 0;
}