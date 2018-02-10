#include<LD_VERTICE.h>
#include<LD_ADJMAT.h>
#include<iostream>
#include <algorithm>
#include <vector>

#include<time.h>
#include<stdlib.h>
#include<string>
using namespace std;

int ascendSort( const void *elem1, const void *elem2) {
	return ((*(vertice**)elem1)->getSn() - (*(vertice**)elem2)->getSn());
}
int compare( const void *elem1, const void *elem2) {
	return ((*(int*)elem1)- (*(vertice**)elem2)->getSn());
}

//bool ascendSort (vertice* v1,vertice* v2) { return (v1->getSn() > v2->getSn()); }

int main()
{
	_short v0;
	_short v1,v5;
	normalOpen v2,v3,v4,v6,v7,v8,v9;
	vertice *vertices[9] = {
		((vertice*)&v1),
		((vertice*)&v2),
		((vertice*)&v3),
		((vertice*)&v4),
		((vertice*)&v5),
		((vertice*)&v6),
		((vertice*)&v7),
		((vertice*)&v8),
		((vertice*)&v9),
	};
	vector<vertice*> vV(vertices,vertices+8);
	vector<vertice*>::iterator it;
	edge_st edgeStream[] = {
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
	adjMatrix goingReduce;


	srand(time(NULL));
	string tmp;
	v0.setSn();
	char sn[2];
	for(int i=0;i<9;i++){
		vertices[i]->setSn();
		//tmp = 65 + rand()%26;				// ascii code
		itoa(vertices[i]->getSn(),sn,10);
		tmp = sn;
		vertices[i]->setData(tmp);
	}

	int key = 3;
	vertice **v = NULL;
	qsort(vertices,9,sizeof(void*),ascendSort);
	v = (vertice**)bsearch(&key,vertices,9,sizeof(void*),compare); // return address of target container

//---------------------------------
//	end of preparation of test
//---------------------------------

	goingReduce.build(edgeStream,sizeof(edgeStream) / sizeof(edge_st));

	int Vi,Vj;
	vertice *vVi,*vVj;
	while(1){
		goingReduce.stepReduce();
		Vi	= goingReduce.queryVi();
		Vj	= goingReduce.queryVj();
		vVi = *((vertice**)bsearch(&Vi,vertices,9,sizeof(void*),compare));
		vVj = *((vertice**)bsearch(&Vj,vertices,9,sizeof(void*),compare));
		if(goingReduce.queryRelation() == SS_REDUCED){
			*vVi = *vVi & *vVj ;
		}
		else if(goingReduce.queryRelation() == SP_REDUCED){
			*vVi = *vVi	| *vVj;
		}
		else{
			switch(goingReduce.queryState()){
			case FULLY_REDUCED:
			case NO_MORE_REDUCTION:
				break;
			default:
				break;
			}
			break;
		}
	}
	// successed! , Hsien ,2012.05.08
	// done  , string merged in expection

	return 0;
}
