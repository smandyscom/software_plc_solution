#include<vector>
#include<bitset>
#include<input_vertice.h>
using namespace std;

int func(void* base,size_t size){
	edge_st tmp;
	tmp = ((vector<edge_st>*)base)->front();

	return 0;
}



int func2()
{
	int i = 0;
	size_t j=0;

	bitset<32> rows[32];
	bitset<32> columns[32];

	rows[1] = 0x01010101;
	i = rows[1].count();	// use this to count '1' s
	rows[1].set(0);			// set specific position

	//----------------------------------
	//	dispatch graph information to em
	//----------------------------------
	edge_st e1 = {2,3};			
	rows[e1.from].set(e1.to);
	columns[e1.to].set(e1.from);		// which columns is transport matrix of rows

	//size_t j=0;

	for(int i=0;i<32;i++)
		if(rows[i].count() == 1){

			//------------------
			//	check if simple series 
			//------------------
			// find out which entry is 1 on row Vi
			bitset<32> *tmp;
			tmp = &rows[i];
			long l = rows[i].to_ulong();
			for(j=0;j<32;j++)
				if(rows[i].test(j))
					break;				// j is that one
				else					
					;					// if no 'else' branch , would cause debugger error?

			if(columns[j].count() ==1)
				printf("!\n");	// simple series handling
					// end of simple series handle
			else	// no any pattern matched
				printf("X\n");	// end of simple series handle

			//-------------------
			//	check if simple parallel
			//-------------------
			for(j=i;j<32;j++)
				if(rows[i].to_ulong() == rows[j].to_ulong()){
					;	// do further check for their columns
					if(columns[i].to_ulong() == rows[j].to_ulong())
						;	// simple parallel handling
							// end of parallel handle
				}
				//	no any pattern matched , 
				//	end of simple parallel handle
		};


	return 0;
}

int main()
{
	edge_st tmp;
	vector<edge_st> edgeList;

	tmp.from = 2;
	tmp.to   = 3;
	edgeList.push_back(tmp);

	func((void*)&edgeList,sizeof(edgeList));

	vector<int> d3Mat(3,1);
	vector<vector<int>> d2Mat;

	func2();
	return 0;
}