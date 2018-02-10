#include<stdlib.h>
#include<time.h>
#include<graphGenerate.h>

int main()
{
	adjMatrix goingReduce;

		edge_st edgeStream2[] = {
		{0,1},
		{1,2},
		{2,3},
		{3,4},
		{4,5},
		{5,6},
		{6,7},
		{7,8},
		{8,9},
	};

	goingReduce.build(edgeStream2,sizeof(edgeStream2) / sizeof(edge_st));
	cout << goingReduce;

	// pure series test
	while(!goingReduce.isFullyReduced()){
		switch(goingReduce.stepReduce()){
			case SS_REDUCED: cout << "serie ";			break;
			case SP_REDUCED: cout << "parallel";		break;
			case NO_MORE_REDUCTION: cout << "nothing";	break;
		};
		cout << "Reduced :" << goingReduce.queryVj() << "\n";
		cout << goingReduce;
	}
	// successed , Hsien ,2012.05.08

	return 0;
}