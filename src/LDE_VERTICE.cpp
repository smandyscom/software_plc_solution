//---------------------------------------------------------
// FILENAME				: LDE_VERTICE.cpp
// ORIGIN AYTHOR		: Hsien , 2012.05.08
// EDITOR				: Hsien
// NOTE					: Defination of Vertices(input devices in Ladder-Diagram)
//							including its manipulators:
//								1. editing
//								2. graph analyzing(ST generating)
//---------------------------------------------------------
#include<LDE_VERTICE.h>
#include<limits.h>

vertice::vertice(const vertice& source)
{
	//-------------------
	//	copy-constructor , Hsien ,2012.09.25
	//-------------------
	this->isSnMarked = source.isSnMarked;
	this->snNumber	 = source.snNumber;
	this->type = source.type;
	this->data = source.data;
}

vertice::vertice	(void)
{
	//--------------------------------------------------
	//	Modify : mark serial number when needed
	//	for covering usage of deleting , adding randomly
	//--------------------------------------------------
	this->snNumber		= -1;		// as invalid serial number
	this->isSnMarked	= false;
	this->data.clear();
	this->type = INTERMEDIATE;
}
void		vertice::operator =	(vertice rhs)
{
	//--------------------------------------------
	//	duplicate type and data only(shallow copy)
	//--------------------------------------------
	this->data = rhs.getData();					
	this->type = rhs.getType();					
}

void		vertice::importData(const char* dataPack)
{
	this->data.assign(dataPack);
}
const char*	vertice::exportData()
{
	return this->data.c_str();
}

//-------------------------------
//	Serial Number Marking functions
//	(GLOBAL scope for vertices)
//-------------------------------
static VERTEX_SN contigousSn = 0;				// serial number record
												// WARNING : may raise multi-thread problem
												// when use static variable
void	resetSn		(void){
	contigousSn =0;
}
void	dispatchSn	(vertice& v)
{
	if(!v.isSnMarked){
		v.snNumber = ::contigousSn;
		::contigousSn++;
		v.isSnMarked = true;
	}
}
void	releaseSn	(vertice& v)
{
	v.snNumber		= -1;
	v.isSnMarked	= false;
}


vertice	operator& (vertice& lhs,vertice& rhs)
{
	vertice tmp;
	//----------
	//	global funtion beyond every instance of class vertice
	//----------
	tmp.type = INTERMEDIATE;
	if(rhs.getType() == __SHORT)			//	when rhs is "short" vertex
		tmp = lhs;						//	'=' operator overloaded
	else if(lhs.getType() == __SHORT)		//	when lhs is "short" vertex
		tmp = rhs;
	else
		tmp.data = lhs.getData() + "&" + rhs.getData();
	return tmp;
}

vertice	operator| (vertice& lhs,vertice& rhs)
{
	vertice tmp;

	tmp.type = INTERMEDIATE;
	if(rhs.getType() == __SHORT|| lhs.getType() == __SHORT)				//	when rhs is "short" vertex
		//tmp.data.clear();				//	'=' operator overloaded
		tmp.data = "1";
	else
		tmp.data = "(" + lhs.getData() + "|" + rhs.getData() + ")";
	return tmp;
}


vertice* createVertice(int typeId)
{
	//-------------------------------
	//	runtime rebuilding by type-id
	//	inevitable hardcoding
	//-------------------------------
	vertice*	tmp = NULL;

	switch(typeId){
		case __SHORT:		tmp = (vertice*)new _short();		break;
		case NORMAL_OPEN:	tmp = (vertice*)new normalOpen();	break;
		case NORMAL_CLOSE:	tmp = (vertice*)new normalClose();	break;
		case INTERMEDIATE:	tmp = (vertice*)new vertice();		break;
			//--------------
			//	Timer Device , added by Hsien , 2013.01.16
			//--------------	
			//--------------
			//	Indexer Device , added by Hsien , 2013.01.24
			//--------------		
		case NONSTOP_TIMER_OUTPUT_NO:	tmp = (vertice*)new indexerNo(NONSTOP_TIMER_OUTPUT_NO)/*nonStopTimerNo()*/;					break;
		case RING_COUNTER_OUTPUT_NO:	tmp = (vertice*)new indexerNo(RING_COUNTER_OUTPUT_NO);	break;
			//----------------------
			//	Comparator Device // Hsien 2013/5/16
			//----------------------
		case COMPARATOR:				tmp = (vertice*)new comparator();			break;
			//----------------------
			//	NC Sync-IO request receive , shared the implementation with indexer // Hsien 2013/5/22 
			//----------------------
		case NC_REQUEST:				tmp = (vertice*)new indexerNo(NC_REQUEST);	break;
			break;
		default:
			tmp = NULL;
			break;
	}

	return tmp;
}

string		comparator::getData	()
{
#define COMPARATOR_FORMAT	"\(%s %s %s\)"	// 1. lhs
											// 2. op
											// 3. rhs
											// Hsien 2013/5/16
	char	comparatorString[32];

	sprintf(comparatorString
		,COMPARATOR_FORMAT
		,this->lhs.c_str()
		,this->op.c_str()
		,this->rhs.c_str());
	
	this->data = comparatorString;

	return this->data;
}

#define COMPARATOR_STORE	"%s\n%s\n%s"	// 1. lhs
											// 2. op
											// 3. rhs
											// Hsien 2013/5/16
#define	COMPARATOR_STORE_MATCH_COUNT	3	// Hsien 2013/5/17 , to sort out null-string case

static	char storeStringBuffer[32];			// Hsien 2013/5/16, used as temporary buffer

void		comparator::importData(const char* dataPack)
{	
	int		count = 0;
	char	_lhs[16];
	char	_rhs[16];
	char	_op[16];

	//memset(storeStringBuffer,0,sizeof(storeStringBuffer));
	//----------------------
	//	Meta-data works well(I/O normally), but sscanf is unable to handle NULL-string
	// Hsien 2013/5/17
	//----------------------
	count = sscanf(dataPack
		,COMPARATOR_STORE
		,_lhs
		,_op
		,_rhs);
	if(count == COMPARATOR_STORE_MATCH_COUNT){
		this->lhs	= _lhs;
		this->rhs	= _rhs;
		this->op	= _op;
	}
	else{
		//-----------------------------------
		//	Avoid to grab incorrect statement
		//-----------------------------------
		this->lhs.clear();
		this->rhs.clear();
		this->op.clear();
	}
}
const char*	comparator::exportData()
{
	int	count = 0;	// Hsien 2013/5/17 , used to monitor sprintf

	memset(storeStringBuffer,0,sizeof(storeStringBuffer));
	count = sprintf(storeStringBuffer
		,COMPARATOR_STORE
		,this->lhs.c_str()
		,this->op.c_str()
		,this->rhs.c_str());

	return storeStringBuffer;
}
