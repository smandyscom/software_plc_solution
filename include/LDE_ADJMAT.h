//---------------------------------------------------------
// FILENAME				: LDE_ADJMAT.h
// ORIGIN AYTHOR		: Hsien , 2012.05.08
// EDITOR				: Hsien
// NOTE					: Adjecent Matrix Class , used to describe connection map of vertices
//							and operating merge alogorithm depends on Simple-Series connection or Simple-Parallel connection
//---------------------------------------------------------
#ifndef _LD_ADJMAT_H
#define _LD_ADJMAT_H
#include<bitset>
#include<iostream>
#include<LDE_LIB0.h>		// for edgeStream_t
using namespace std;

#define ADJMAT_SIZE 256/*32*/	// due to limitation of to_ulong() of bitset class
								// Hsien 2013/5/27 , extend size of adjecent matrix
								// able to load 256/19 = 13 rows
enum adjMat_return_enum
{
	REDUCED,
	NOTHING_REDUCED,
	//--------------
	// transit state
	//--------------
	SS_REDUCED,
	SP_REDUCED,
	//-------------
	// matrix state
	//-------------
	_EMPTY,
	BUILDED,				// add by Hsien 2012.08.31
	NO_MORE_REDUCTION,
	FULLY_REDUCED,
	REDUCING,
};

class adjMatrix
{
private:
	static const size_t matrixSize = ADJMAT_SIZE;
	bitset<ADJMAT_SIZE/*32*/> rows[ADJMAT_SIZE],columns[ADJMAT_SIZE];		
											// they'r stand for the identical adjecent matrix , 'rows' is transpose of 'columns'
											//	'rows' stores outlet relations for every vetex , called 'to-table'.
											//	then 'columns' stored inlet relations for every vertex , called 'from-table'
	int state;						// internal state

	size_t vi;
	size_t vj;						// when reducing occurs , vi = vi (&|) vj;
	size_t din;						// sum of indegree
	size_t dout;					// sum of outderee
	void		calDin(){
		//-----------------------------------------
		//	calculting degree of inputs
		//		how many of one's from columns view
		//-----------------------------------------
		this->din = 0;
		for(int i=0;i<this->matrixSize;i++)
			this->din += this->columns[i].count();
	}
	void		calDout(){
		//-----------------------------------------
		//	calculting degree of inputs
		//		how many of one's from columns view
		//-----------------------------------------
		this->dout = 0;
		for(int i=0;i<this->matrixSize;i++)
			this->dout += this->rows[i].count();
	}


	bool serieReduce	(size_t vi);	// given vi , find-out if any vj could do simple serie		Reduce with vi
	bool parallelReduce	(size_t vi);	// given vi , find-out if any vj could do simple parallel	Reduce with vi
public:
	adjMatrix();
	int		build(edge_st* edgeStream,size_t counts);	// build adjecent matrix by edges stream , C-array-type form
	int		build(EDGE_STREAM es);						// build adjecent matrix by edges stream , vector-type form

	int		stepReduce();								// do once reduce , if s-redcude fail then p-reduce
	//int		scanReduce();								// Hsien 2013/6/18
														// 

	friend ostream& operator<<(ostream& out , const adjMatrix& adjMat);			// overloading print operator

	void	empty();									// reset entirm adjmat to state of initialization

	inline size_t queryVi()		{ return this->vi;}
	inline size_t queryVj()		{ return this->vj;}
	inline int	  queryState()	{ return this->state;}
};

#endif
