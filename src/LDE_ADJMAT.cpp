//---------------------------------------------------------
// FILENAME				: LDE_ADJMAT.cpp
// ORIGIN AYTHOR		: Hsien , 2012.05.08
// EDITOR				: Hsien
// NOTE					: Adjecent Matrix Class , used to describe connection map of vertices
//							and operating merge alogorithm depends on Simple-Series connection or Simple-Parallel connection
//---------------------------------------------------------
#include<LDE_ADJMAT.h>

adjMatrix::adjMatrix()
{
	this->empty();
}

int		adjMatrix::build(edge_st *edgeStream, size_t counts)
{
	//--------------
	//	matrix reset
	//--------------
	this->empty();

	for(size_t i=0;i<counts;i++){
		//--------------------------------------------------
		// approched the bound of adjMatrix , building fail
		//--------------------------------------------------
		if(edgeStream[i].from > this->matrixSize ||
			edgeStream[i].to  > this->matrixSize){
				this->empty();
				return 0;
		}
		//----------------------------------
		//	dispatch graph information to em
		//----------------------------------
		this->rows[edgeStream[i].from].set(edgeStream[i].to);
		this->columns[edgeStream[i].to].set(edgeStream[i].from);
	}

	this->calDin();		// calculating sum of indegree
	this->calDout();	// calculating sum of outdegree
	//-------------------------------------------
	// thinking: is that ture this matrix failed 
	// when indegree or outdegrees was in odd
	// other meanings , this graph is not euler graph.
	//	think about it.
	//  Hsien , 2012.05.08
	//-------------------------------------------
	this->state = BUILDED;
	return 1;
}

int		adjMatrix::build(EDGE_STREAM es)
{
	edge_st tmp;

	this->empty();

	for(;es.size() > 0;){				
		tmp = es.back();
		es.pop_back();
		this->rows[tmp.from].set(tmp.to);
		this->columns[tmp.to].set(tmp.from);
	}

	this->calDin();
	this->calDout();
	this->state = BUILDED;
	return 0;
}
bool	adjMatrix::serieReduce(size_t vi)
{
	//----------------------
	//	vi->vj->vk
	// Hsien 2013/6/18
	//----------------------
	size_t vj;	// the vertice which have series relationship with vi
	size_t vk;	// the vertice which connected with vj

	for(vj=0;vj<this->matrixSize;vj++)		// find-out which one is vi linked to
		if(this->rows[vi].test(vj))
			break;

	if(	/*this->columns[vj].test(vi)			// would be much more robust?
		&&*/ this->columns[vj].count() ==1){
		//-------------------------
		//	in-serie relation found , handle it
		//-------------------------
		this->rows[vi] = this->rows[vj];	// redirect vi to these verices vj point to	
											
		for(vk=0;vk<this->matrixSize;vk++)	// then update "from" table
			if(this->rows[vi].test(vk)){
				this->columns[vk].set(vi,true);
				this->columns[vk].reset(vj);
			}

		this->rows[vj].reset();			// isolate vj
		this->columns[vj].reset();
		//-----------
		//	outputs
		//-----------
		this->vi = vi;
		this->vj = vj;

		return REDUCED;	
	}

	return NOTHING_REDUCED;	
}

bool	adjMatrix::parallelReduce(size_t vi)
{
	//----------------------
	// vm---vi---vn
	//	  |-vj-|
	// Hsien 2013/6/18
	//----------------------
	size_t vj;
	size_t vm,vn;

	//---------
	//	comparision method modified , using string instead of u_long , range extended
	//		Hsien , 2012.07.16
	//---------
	for(vj =0 ;vj < this->matrixSize ; vj++){
		//----------------------
		//	Condition: the both end of vi,vj are the same , and vi.vj are not identical vertice
		// Hsien 2013/6/18
		//----------------------
		if((this->rows[vi].to_string()/*.to_ulong()*/ == this->rows[vj].to_string()/*.to_ulong()*/) &&
			(this->columns[vi].to_string()/*.to_ulong()*/ == this->columns[vj].to_string()/*.to_ulong()*/) &&
			(vj != vi )){
				//-------------------------
				//	in-parallel realtion found , handle it
				//-------------------------
				for(vn=0;vn<this->matrixSize;vn++)	// find vn , that one both vi,vj point to
					if(rows[vi].test(vn))			// 'to' table
						break;
				for(vm=0;vm<this->matrixSize;vm++)  // find vm , that one both vi,vj be pointed
					if(columns[vi].test(vm))		// 'from' table
						break;

				this->rows[vj].reset();				// isolate vj
				this->columns[vj].reset();			// isolate vj
				this->rows[vm].reset(vj);			// cut link of vj to	vm
				this->columns[vn].reset(vj);		// cut link of vj from	vn

				this->vi = vi;
				this->vj = vj;

				return REDUCED;
		}//if()
	}//for()
	
	return NOTHING_REDUCED;
}

int		adjMatrix::stepReduce()
{
	//----------------------
	//	Sequential reducing method , vertice by vertice reduce
	// Hsien 2013/6/18
	//----------------------
	/*static*/ size_t vi;

	for(vi=0;vi<this->matrixSize;vi++){
		if(rows[vi].count() == 1){
			//-------------------------
			//	simple series check out 
			//-------------------------
			if(this->serieReduce(vi) == REDUCED){
				this->calDin();
				this->calDout();
				this->state = SS_REDUCED;
				return this->state;
			}
			//--------------------------
			//	check if simple parallel
			//--------------------------
			if(this->parallelReduce(vi) == REDUCED){
				this->calDin();
				this->calDout();
				this->state = SP_REDUCED;
				return this->state;	
			}
		}// endif : nothing to reduce
	}//for()

	this->calDin();
	this->calDout();	
	//------------------------------------
	// check condition of adjecent matrix 
	//	minimun reduced condition:
	//  zero matrix(all vertices isolated)
	//------------------------------------
	if((this->din == 0) && (this->dout == 0 ))
		this->state = FULLY_REDUCED;
	else
		this->state = NO_MORE_REDUCTION;

	return this->state;
}

ostream& operator<<(ostream& out , const adjMatrix& adjMat)
{
	//-------------------------------------------------------------------
	//	standard-out operator for printing out reduction result of adjMat
	//-------------------------------------------------------------------
	for(int i=0;i<adjMat.matrixSize;i++){
		out << i << "\t" ;
		for(int j=0;j<adjMat.matrixSize;j++){
			if(adjMat.rows[i].test(j))
				out << 1 << " ";
			else
				out << 0 << " ";
		}
		out << "\n";
	}
	out<< "\n";

	return out;
}

void	adjMatrix::empty()
{
	for(size_t i=0;i<this->matrixSize;i++){
		rows[i].reset();
		columns[i].reset();
	}
	this->vi = 0;
	this->vj = 0;
	this->din = 0;
	this->dout = 0;

	this->state = _EMPTY;
}