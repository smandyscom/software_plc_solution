//---------------------------------------------------------
// FILENAME				: PLC_OPMAG.cpp
// ORIGIN AUTHOR		: Hsien	,	2012.02.29
// EDITOR				: Hsien
// NOTE					: PostFix Code Operation Magzine 
//---------------------------------------------------------
#include<PLC_PFC.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

#ifndef	OPMAG_SIZE
#define OPMAG_SIZE	UCHAR_MAX
#endif

struct opmag_st
{
	UNS08			opId;
	PFC_OPERATOR	opData;
};

static unsigned char			topIndex;
static struct opmag_st	opMagazine[OPMAG_SIZE];

static int CompareID( const void *elem1, const void *elem2)
{
	return	(*((unsigned char*)elem2) - *((unsigned char*)elem1));		//	descand sorting/searching
}

static inline opmag_st* SearchOp(unsigned char opId)
{
	return (opmag_st*)bsearch(&opId,opMagazine,topIndex,sizeof(opmag_st),CompareID);
}

namespace OPMAG
{
	void initialize(void)
	{
		memset(::opMagazine,0,sizeof(opmag_st) * OPMAG_SIZE);
		topIndex = 0;
	}

	bool	addOp(const unsigned char opId,const bool whetherUnary,void*	funcAdr)
	{
		opMagazine[::topIndex].opId				=	opId;
		opMagazine[::topIndex].opData.isUnary	=	whetherUnary;
		opMagazine[::topIndex].opData.operation	=	funcAdr;

		++::topIndex;
		qsort(::opMagazine,::topIndex,sizeof(::opmag_st),::CompareID);

		return TRUE;
	}

	bool removeOp(const unsigned char opId)
	{
		opmag_st		*tmp;

		tmp = SearchOp(opId);
		memset((void*)tmp,0,sizeof(opmag_st));

		qsort(opMagazine,topIndex,sizeof(opmag_st),CompareID);
		--topIndex;
		return TRUE;
	}

	void* queryOp(const unsigned char opId)
	{
		//------------
		//	return :
		//		address of corresponding function , in void* form
		//		in order to decoupling the rely of specific type , in header
		//		Hsien , 2012,04,01
		//------------
		opmag_st *tmp;

		tmp = SearchOp(opId);

		if(!tmp)
			return NULL;
		return (void*)(&tmp->opData);
	}
}//namespace()