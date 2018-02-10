//---------------------------------------------------------
// FILENAME				: LDE_ELEMENT.h
// ORIGIN AYTHOR		: Hsien , 2012.05.08
// EDITOR				: Hsien
// NOTE					: Definition of Input Element of Ladder-Diagram
//								content including:	1. neithor	route descriptor
//													2. nor		vertice
//---------------------------------------------------------
#ifndef _LDE_ELEMENTS_H
#define _LDE_ELEMENTS_H
#include<LDE_VERTICE.h>
#include<vector>
using namespace std;

enum ldElem_type_enum
{
	BRANCH_FORWARD,
	BRANCH_FORWARD_TERMINAL,			//¢|extension code
	BRANCH_FORWARD_TSECTION,			//¢sextension code

	BRANCH_UP_FORWARD,
	BRANCH_UP_FORWARD_TSECTION_TYPE1,	//¢sextension code
	BRANCH_UP_FORWARD_TSECTION_TYPE2,	//¢uextension code

	BRANCH_DOWN,
	BRANCH_DOWN_TERMINAL,				//¢}extension code
	BRANCH_DOWN_TSECTION,				//¢textension code

	EMPTY,
	VERTICE,							// the subset of devices
	MAX_NUM_LD_ELEMENT,
};

class ldElement
{
protected:
	unsigned char	type;
	vertice*			v;			// to store vertice if any
public:
	ldElement(const ldElement& source);			// copy constructor for deep duplicating  [8/27/2012 smandyscom]
	ldElement();
	~ldElement();

	int							setType(unsigned char type);
	unsigned char				getType()	{	return this->type	;	}
	vertice*					getVertice(){	return this->v		;	}
	int							setVertice(const int verticeType);
	unsigned char				getVerticeType()
	{
		if(this->type == VERTICE)
			return this->v->getType();
		return MAX_NUM_VERTICE;				// as invalid vertice code
	}

	const char* exportVerticeData(/*int			&code*/);							//, export vertice data , if any	
	int			exportData();
	int			importData			(const int	_Code/*,const char*	data*/);
	int			importVerticeData	(const char*	data);
};



#endif