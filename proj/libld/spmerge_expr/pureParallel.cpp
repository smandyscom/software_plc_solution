#include<stdlib.h>
#include<time.h>
#include<graphGenerate.h>

int main()
{
	adjMatrix goingReduce;

		edge_st edgeStream2[] = {
		{0,1},
		{0,2},
		{0,3},
		{0,4},
		{1,5},
		{2,5},
		{3,5},
		{4,5},
	};

	goingReduce.build(edgeStream2,sizeof(edgeStream2) / sizeof(edge_st));
	cout << goingReduce;

	// parallel diagram test
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