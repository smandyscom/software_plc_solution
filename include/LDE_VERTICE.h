//---------------------------------------------------------
// FILENAME				: LDE_VERTICE.h
// ORIGIN AYTHOR		: Hsien , 2012.05.08
// EDITOR				: Hsien
// NOTE					: Defination of Vertices(input devices in Ladder-Diagram)
//							including its manipulators:
//								1. editing/senetence generating
//								2. graph analyzing
//								3. meta-data storing
//---------------------------------------------------------
#ifndef _LD_VERTICE_H
#define _LD_VERTICE_H
#include<string>
using namespace std;

typedef short  VERTEX_SN;

enum vertice_enum
{
	__SHORT,
	NORMAL_OPEN,
	NORMAL_CLOSE,
	RISING_EDGE,
	FALLING_EDGE,

	INTERMEDIATE,				// intermediate vertce type , when vertices in merging process
								// Hsien , 2012.06.25
	//-----------------------
	//	INDEX-DEPICTED DEvice , Hsien , 2013.01.24
	//-----------------------
	NONSTOP_TIMER_OUTPUT_NO,	// added by Hsien , 2013.01.16 , to do string replacement , in normal-open type
	RING_COUNTER_OUTPUT_NO,		// added by Hsien , 2013.01.24 , to do string replacement , in normal-open type

	//----------------------
	//	Comparator // Hsien 2013/5/15
	//----------------------
	COMPARATOR,
	//----------------------
	//	NC Sync-IO request receive , shared the implementation with indexer // Hsien 2013/5/22 
	//----------------------
	NC_REQUEST,	

	MAX_NUM_VERTICE,
};

class vertice
{
protected:
	VERTEX_SN			snNumber	;	// vertex serial number , unique
	bool				isSnMarked	;	// lock mechanism to avoid overmark
	string				data		;	// the compound statement , used in logic evaluation , e.g data of NC: !X0.00 , data of indexer: TNO0.0 , data of comparator: (X0:0 == 8)
	unsigned char		type		;	// used to identify child type
public:
	//-----------------------------------------------------------------------------------------
	//	following manipulation of this class are setted for mapping to manipulation on UI layer
	//-----------------------------------------------------------------------------------------
	vertice(const vertice& source);		//	copy-constructor , Hsien ,2012.09.25
	vertice(void);
	~vertice(){};
	//-------------------------------------------
	// interface when editing
	//	virtual : possible to overriding by child
	//-------------------------------------------
	unsigned char		getType	()		{return this->type;}	
	VERTEX_SN			getSn	()		{return this->snNumber;}
	virtual string		getData	()		{return (this->data);}
	virtual void		importData(const char* dataPack);	// found , invent a new base abstract class to be inherented by vertice and output device? Hsien , 2012.07.05
	virtual const char*	exportData();

	//-------------------------------------------------
	//	interface when graph analyzing , string merging
	//-------------------------------------------------
	friend void			dispatchSn	(vertice& v);	// shall be global function , build reset mechanism
	friend void			releaseSn	(vertice& v);

	//---------------------------------------------------------------------------------------------------------
	//	interface for doing semetic checking/variable monitoring, via DQI and FCLCT , added by Hsien 2012.09.02 
	//	still considering
	//---------------------------------------------------------------------------------------------------------
	//virtual	bool semeticChecking((struct frmAttr_st)(const *varName)	dqiInterface);	// in call-back func form
	//virtual	int	 variableMonitoring();

friend vertice		operator& (vertice& lhs,vertice& rhs);		// overloaded operator
friend vertice		operator| (vertice& lhs,vertice& rhs);
void				operator= (vertice rhs);

};	// vertice

class _short:public vertice
{
public:
	_short(){this->type = __SHORT;this->data = "1";}

	virtual void importData(const char*){/*do nothing*/}
};



class normalOpen: public vertice
{
public:
	normalOpen()							{this->type = NORMAL_OPEN;}
	virtual void	setBitName(const string bitName){this->data = bitName;}
						// Hsien 2013/5/22 , added "virtual" depictor , to let derived class able to override
};

class normalClose: public vertice
{
private:
	string bitName;		// orginal input value assign from user
public:
	normalClose(){this->type = NORMAL_CLOSE;}
	void			setBitName(const string _bitName)	{this->bitName = _bitName;}
	string			getBitName()						{return this->bitName;}
	//-----------
	// overriding
	//-----------
	virtual string		getData();	
	virtual void		importData(const char* dataPack);	// found , invent a new base abstract class to be inherented by vertice and output device? Hsien , 2012.07.05
	virtual const char*	exportData();
};


class risingEdge: public vertice{
	//-------------------------------------------------------------------
	//	using shadow technique , e.g X0.0 -> build SX0.0 behind the scene
	//-------------------------------------------------------------------
};

class fallingEdge: public vertice{
};

class indexerNo:public normalOpen
{
	//----------------------------------------------------------------------------
	//	Indexer normal-open output , all method inheritant from No
	//		Could be applied on ring-counter , timer , ...and other indexed device
	//		Hsien , 2013.01.24
	//----------------------------------------------------------------------------
public:
	indexerNo(unsigned char _type){
		this->type = _type;
		this->setIndex(0);
	}
	~indexerNo(){};
	int		setIndex(const int _index);
	int		getIndex(void);
	void	setType (const unsigned char _type){this->type = _type;}
};

class comparator:public vertice
{
	//----------------------
	//	Used to output the value after compared , e.g X0:1 > 3
	// Hsien 2013/5/15
	//----------------------
private:
	string lhs	;		// left-hand side , raw info
	string rhs	;		// right-hand side , raw info
	string op	;		// operator
public:
	comparator()	{
		this->type = COMPARATOR;
		//----------------------
		//	Default Value used as example
		//----------------------
		this->lhs	= "X0"	;
		this->op	= "<"	;
		this->rhs	= "X1"	;
	}
	~comparator(){};

	int	setLhs	(const string _lhs)	{this->lhs	= _lhs	; return 1;}
	int	setRhs	(const string _rhs)	{this->rhs	= _rhs	; return 1;}
	int	setOp	(const string _op)	{this->op	= _op	; return 1;}

	string getLhs	(){return this->lhs;}
	string getRhs	(){return this->rhs;}
	string getOp	(){return this->op;}

	virtual string		getData	();								// used to generating (lhp op rhp) statement
	virtual void		importData(const char* dataPack);		// found , invent a new base abstract class to be inherented by vertice and output device? Hsien , 2012.07.05
	virtual const char*	exportData();
};

//------------------------------------------------------
//	Global function , to mark serial numbers on vertices
//	when doing graph analyzing
//------------------------------------------------------
void resetSn	(void);
void dispatchSn	(vertice& v);
void releaseSn	(vertice& v);

vertice* createVertice(const int typeId);	// given id , create vertice/derived class instance

#endif