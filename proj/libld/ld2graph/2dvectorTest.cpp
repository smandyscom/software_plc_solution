#include<LD_ELEMENTS.h>
#include<vector>
#include<algorithm>
using namespace std;

void func2(ldElement_st& e)			// important! , must be in call-by-reference 
									// Hsien , 2012.05.09
{
	e.type = EMPTY;
}

void func1(vector<ldElement_st>& v) // unary function
{
	for_each(v.begin(),v.end(),func2);
}

vector<ldElement_st> list;
int funcPrototype(vector<vector<ldElement_st>>::iterator	row,size_t column);


int main()
{
	vector<vector<int>> d2vector(5,vector<int>(5));
	vector<vector<int>> asymetricVector;

	asymetricVector.push_back(vector<int>(3));
	asymetricVector.push_back(vector<int>(4));

	vector<vector<ldElement_st>> ldVector(2,vector<ldElement_st>(10));

	for_each(ldVector.begin(),ldVector.end(),func1);

	for_each(ldVector[0].begin(),ldVector[0].end(),func2);

	ldVector[0][0].type = EMPTY;

	vector<vector<ldElement_st>>::iterator		itrow	;
	vector<ldElement_st>::iterator				itcolumn;

	ldVector[0][0].type = BRANCH_FORWARD;

	itrow	 = ldVector.begin();
	itcolumn = (itrow)->begin();
	itrow++	;
	
	;

//-----------------------------------------
//	algorithm test
//-----------------------------------------
	ldVector.clear();
	ldVector.resize(2,vector<ldElement_st>(10));
	ldVector.push_back(vector<ldElement_st>(10));
	list.clear();
	//------------
	//	branch-forward only test
	//------------
	ldVector[0][0].type = VERTICE;
	ldVector[0][0].v.setSn();
	ldVector[0][1].type = BRANCH_FORWARD;
	ldVector[0][2].type = BRANCH_FORWARD;
	ldVector[0][3].type = VERTICE;
	ldVector[0][3].v.setSn();

	funcPrototype(ldVector.begin(),1);

	// fine , get one further vertice

	//----------
	// including branch-up-forward test
	//----------
	ldVector[0][1].type = BRANCH_UP_FORWARD;
	ldVector[1][1].type = BRANCH_FORWARD;
	ldVector[1][2].type = VERTICE;
	ldVector[1][2].v.setSn();

	funcPrototype(ldVector.begin(),1);

	return 0;
}


int funcPrototype(vector<vector<ldElement_st>>::iterator	row,size_t column)
{
	//--------------------------------------------------------------
	// why use vector rather C-array :
	//  easy to allocate dynamically , build-in search, sort methods
	//--------------------------------------------------------------
	//	secure problem , doesn't prevent access violation
	//		e.g, when next row either next column doesn't existed
	//--------------------------------------------------------------
	switch((row->at(column)).type){		// right way to access , Hsien , 2012.05.09
		case BRANCH_UP_FORWARD:	
			funcPrototype(row,column+1);	// scan forward
			funcPrototype(row+1,column);	// scan upward
			return 0;
			break;
		case BRANCH_FORWARD:
			funcPrototype(row,column+1);	// scan forward
			return 0;
			break;
		case BRANCH_DOWN:
			funcPrototype(row-1,column);	// scan downward
			return 0;
			break;
		case VERTICE:
			list.push_back(row->at(column));
			return 0;
		case EMPTY:
			return 0;
		default:
			break;
	}

	return 0;
}