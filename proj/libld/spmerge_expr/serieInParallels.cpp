#include<stdlib.h>
#include<time.h>
#include<LD_ADJMAT.h>

int main()
{
	int breakFlag =0;
	adjMatrix goingReduce;

		edge_st edgeStream2[] = {
		{1,2},
		{1,6},
		{1,9},
		{2,3},
		{3,4},
		{4,5},
		{6,7},
		{7,8},
		{8,5},
		{9,10},
		{10,11},
		{11,5},
	};

	goingReduce.build(edgeStream2,sizeof(edgeStream2) / sizeof(edge_st));
	cout << goingReduce;

	// parallels in serie diagram test
	while(1){
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

		switch(goingReduce.queryState()){
			case FULLY_REDUCED:
			case NO_MORE_REDUCTION:
				breakFlag = 1;
				break;
			default:
				break;
		}

		if(breakFlag)
			break;
	}
	// successed! , Hsien ,2012.05.08
	return 0;
}