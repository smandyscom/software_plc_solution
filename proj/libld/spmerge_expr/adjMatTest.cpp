#include<stdlib.h>
#include<time.h>
#include<graphGenerate.h>

int main()
{
	edge_st edgeStream[128];
	adjMatrix goingReduce;

	//------------
	//	generate edge stream by random number
	//------------
	int vi,vj;
	srand(time(NULL));
	for(int i=0;i<64;i++){
		vi = rand()%8;
		vj = rand()%8;
		if(vi != vj){	// self-loop is not allowed
			edgeStream[i].from	= vi;
			edgeStream[i].to	= vj;
		}
	}


	goingReduce.build(edgeStream,32);
	cout << goingReduce;

	// random test
	while(!goingReduce.isFullyReduced()){
		switch(goingReduce.stepReduce()){
			case SS_REDUCED: cout << "serie ";			break;
			case SP_REDUCED: cout << "parallel";		break;
			case NO_MORE_REDUCTION: cout << "nothing";	break;
		};
		cout << "Reduced :" << goingReduce.queryVj() << "\n";
		cout << goingReduce;
	}
	// hard to get sample in sort of parallel

	edge_st edgeStream2[32] = {
		{1,2},
		{1,3},
		{3,4},
		{3,8},
		{4,5},
		{5,6},
		{6,7},
		{7,2},
		{8,6},
	};

	goingReduce.build(edgeStream2,32);
	cout << goingReduce;

	// random test
	while(!goingReduce.isFullyReduced()){
		switch(goingReduce.stepReduce()){
			case SS_REDUCED: cout << "serie ";			break;
			case SP_REDUCED: cout << "parallel";		break;
			case NO_MORE_REDUCTION: cout << "nothing";	break;
		};
		cout << "Reduced :" << goingReduce.queryVj() << "\n";
		cout << goingReduce;
	}
	// hard to get sample in sort of parallel

	return 0;
}
