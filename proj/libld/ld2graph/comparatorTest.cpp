#include<LDE_VERTICE.h>

int main()
{
	comparator	com;
	string	simuBuffer;
	string	genStr;

	com.setLhs("X0:1");
	com.setOp(">=");
	com.setRhs("127");

	//----------------------
	//	Generated String Test
	//----------------------
	genStr.clear();
	genStr = com.getData();

	//----------------------
	//	Export Test
	//----------------------
	simuBuffer.clear();
	simuBuffer = com.exportData();
	com.setLhs("");
	com.setOp("");
	com.setRhs("");
	com.getData();
	//----------------------
	//	Import Test
	//----------------------
	com.importData(simuBuffer.c_str());


	return 1;
}