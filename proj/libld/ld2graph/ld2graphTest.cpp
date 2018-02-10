#include<LDE_LIB0.h>
#include<vector>
using namespace std;
typedef vector<VERTEX_SN>	SN_LIST;

//LD_ARRAY ld_diagram(1,LD_ROW(6));
//EDGE_STREAM	es;
extern int	nextVerticeScan	(LD_ARRAY	&sourceArray
							 ,size_t	row
							 ,size_t	column
							 ,SN_LIST	&ls);


void correctLD(LD_ARRAY &ld_diagram);
void badLD_1(LD_ARRAY &ld_diagram);

int main()
{
	LD_DEVICE_LIST	ldl;
	SN_LIST		sl;
	LD_ARRAY ld_diagram(1,LD_ROW(6));
	EDGE_STREAM	es;

	size_t	i = 0;	
	int		errorCode;
	correctLD(ld_diagram);
	//badLD_1();
//-----------------------------------
//	end of texture LD diagram editing
//-----------------------------------

//-----------------------------------
// compile start
//-----------------------------------
	nextVerticeScan	(ld_diagram
							 ,0
							 ,0
							 ,sl);
	errorCode = ld2EdgeStream(ld_diagram,es);		// heap currption comes , Hsien , 2012.07.13
	collectVertices(ld_diagram,ldl);

	return 0;
}

void correctLD(LD_ARRAY &ld_diagram)
{
		LD_ROW::iterator	itElem = ld_diagram[0].begin();
	itElem->setType(VERTICE);				itElem->setVertice(NORMAL_OPEN);	dispatchSn(*itElem->getVertice());	itElem++;	//v1
	itElem->setType(BRANCH_UP_FORWARD);	itElem++;	//buf
	itElem->setType(BRANCH_FORWARD);	itElem++;	//bf
	itElem->setType(VERTICE);	itElem->setVertice(NORMAL_OPEN);	dispatchSn(*itElem->getVertice());	itElem++;		//v2
	itElem->setType(BRANCH_FORWARD);	itElem++;	//bf
	itElem->setType(VERTICE);	itElem->setVertice(NORMAL_OPEN);	dispatchSn(*itElem->getVertice());	itElem++;		//v3

	ld_diagram.push_back(LD_ROW(6));			// append a new row
	itElem = ld_diagram[1].begin();				
	itElem->setType(EMPTY);						itElem++;
	itElem->setType(BRANCH_UP_FORWARD);			itElem++;	//buf
	itElem->setType(VERTICE);		itElem->setVertice(NORMAL_OPEN);	dispatchSn(*itElem->getVertice());	itElem++;	//v4
	itElem->setType(BRANCH_FORWARD);			itElem++;	//bf
	itElem->setType(BRANCH_DOWN);				itElem++;	//bd
	itElem->setType(EMPTY);						itElem++;

	ld_diagram.push_back(LD_ROW(6));
	itElem = ld_diagram[2].begin();
	itElem->setType(EMPTY);						itElem++;
	itElem->setType(BRANCH_UP_FORWARD);		itElem++;		//buf
	itElem->setType(VERTICE);	itElem->setVertice(NORMAL_OPEN);	dispatchSn(*itElem->getVertice());	itElem++;		//v5
	itElem->setType(VERTICE);	itElem->setVertice(NORMAL_OPEN);	dispatchSn(*itElem->getVertice());	itElem++;		//v6
	itElem->setType(BRANCH_DOWN);							//bd

	ld_diagram.push_back(LD_ROW(6));
	itElem = ld_diagram[3].begin();
	itElem++;			
	itElem->setType(BRANCH_FORWARD);		itElem++;		//buf
	itElem->setType(VERTICE);		itElem->setVertice(NORMAL_OPEN);	dispatchSn(*itElem->getVertice());	itElem++;	//v7
	itElem->setType(BRANCH_FORWARD);		itElem++;		
	itElem->setType(VERTICE);	itElem->setVertice(NORMAL_OPEN);	dispatchSn(*itElem->getVertice());	itElem++;		//v8
}

void badLD_1(LD_ARRAY &ld_diagram)
{
		LD_ROW::iterator	itElem = ld_diagram[0].begin();
	itElem->setType(VERTICE);				itElem->setVertice(NORMAL_OPEN);	dispatchSn(*itElem->getVertice());	itElem++;	//v1
	itElem->setType(BRANCH_DOWN/*BRANCH_UP_FORWARD*/);	itElem++;	//buf
	itElem->setType(BRANCH_FORWARD);	itElem++;	//bf
	itElem->setType(VERTICE);	itElem->setVertice(NORMAL_OPEN);	dispatchSn(*itElem->getVertice());	itElem++;		//v2
	itElem->setType(BRANCH_FORWARD);	itElem++;	//bf
	itElem->setType(VERTICE);	itElem->setVertice(NORMAL_OPEN);	dispatchSn(*itElem->getVertice());	itElem++;		//v3

	ld_diagram.push_back(LD_ROW(6));			// append a new row
	itElem = ld_diagram[1].begin();
	itElem++;
	itElem->setType(BRANCH_UP_FORWARD);			itElem++;	//buf
	itElem->setType(VERTICE);		itElem->setVertice(NORMAL_OPEN);	dispatchSn(*itElem->getVertice());	itElem++;	//v4
	itElem->setType(BRANCH_FORWARD);			itElem++;	//bf
	itElem->setType(BRANCH_DOWN);				itElem++;	//bd

	ld_diagram.push_back(LD_ROW(6));
	itElem = ld_diagram[2].begin();
	itElem++;
	itElem->setType(BRANCH_UP_FORWARD);		itElem++;		//buf
	itElem->setType(VERTICE);	itElem->setVertice(NORMAL_OPEN);	dispatchSn(*itElem->getVertice());	itElem++;		//v5
	itElem->setType(VERTICE);	itElem->setVertice(NORMAL_OPEN);	dispatchSn(*itElem->getVertice());	itElem++;		//v6
	itElem->setType(BRANCH_DOWN);							//bd

	ld_diagram.push_back(LD_ROW(6));
	itElem = ld_diagram[3].begin();
	itElem++;			
	itElem->setType(BRANCH_FORWARD);		itElem++;		//buf
	itElem->setType(VERTICE);		itElem->setVertice(NORMAL_OPEN);	dispatchSn(*itElem->getVertice());	itElem++;	//v7
	itElem->setType(BRANCH_FORWARD);		itElem++;		
	itElem->setType(VERTICE);	itElem->setVertice(NORMAL_OPEN);	dispatchSn(*itElem->getVertice());	itElem++;		//v8

}
