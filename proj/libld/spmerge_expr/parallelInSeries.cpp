#include<stdlib.h>
#include<time.h>
#include<graphGenerate.h>

int main()
{
	adjMatrix goingReduce;

		edge_st edgeStream2[] = {
		{1,2},
		{1,6},
		{1,8},
		{2,3},
		{3,4},
		{3,7},
		{3,9},
		{4,5},
		{6,3},
		{7,5},
		{8,3},
		{9,5}
	};

	goingReduce.build(edgeStream2,sizeof(edgeStream2) / sizeof(edge_st));
	cout << goingReduce;

	// parallels in serie diagram test
	while(!goingReduce.isFullyReduced()){
		switch(goingReduce.stepReduce()){
			case SS_REDUCED: cout << "serie ";			break;
			case SP_REDUCED: cout << "parallel";		break;
			case NO_MORE_REDUCTION: cout << "nothing";	break;
		};
		cout << "Reduced :" << 
			goingReduce.queryVi()	<<
			" <- "					<<
			goingReduce.queryVj()	<< "\n";
		cout << goingReduce;
	}
	// successed! , Hsien ,2012.05.08
	return 0;
}