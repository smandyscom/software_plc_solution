//---------------------------------------------------------
// FILENAME				: LDE_OUTDEV.h
// ORIGIN AYTHOR		: Hsien , 2012.05.20
// EDITOR				: Hsien
// NOTE					: Defination of OutDevices(output devices in Ladder-Diagram)
//							including its manipulators:
//								1. editing/sentence generating
//								2. meta-data storing
//---------------------------------------------------------
#ifndef _LD_OUTDEV_H
#define _LD_OUTDEV_H
#include<string>
#include<vector>	

using namespace std;

enum outD_enum
{
	//------------------------------------------------------------------------
	//	priority : IF_STMT > SINGLE , means if there's any if_stmt kind device
	//		showed in rung , then this rung has to apply if-statement skeleton 
	//		to depict.
	//------------------------------------------------------------------------
	SINGLE,		// e.g stmt1 = expr , no any multi-word statement need to generate
	IF_STMT,	// e.g if(expr){ stmt1; stmt2;} , stmt1 may be like X= Y ,
	EMPTY_STMT,	// added by Hsien , to depict empty statement,";" , 2012.06.25
	//---------------------
	//	Used to Indentify Which Type the Output Device is
	//		commented by Hsien , 2013.01.14
	//---------------------
	NONE,		// added by Hsien , to depict empty device , 2012.06.25
				//	stand for ST-block as well  , commented by Hsien , 2013.01.21
	ODCOIL,
	ODSET,
	ODSETTN,				// Non-Stop Timer Set , Hsien , 2013.01.14
	ODRSTTN,				// Non-Stop Timer Rst , Hsien , 2013.01.14
	ODSTBLOCK,				// ST-Block Output Device , Hsien , 2013.01.21
	//--------------------------------------------------
	//	Ring Counter Part  , Added by Hsien , 2013.01.21
	//--------------------------------------------------
	ODRINGCTR_SET,			// Ring-Counter Device SET-UP,  Hsien , 2013.01.21
	ODRINGCTR_COUNTUP	,	// Ring-Counter Device Count-UP,	Hsien , 2013.01.21
	ODRINGCTR_COUNTDOWN	,	// Ring-Counter Device Count-Down,  Hsien , 2013.01.21
	//----------------------
	//	NC Sync-IO confirm send , implemented on indexer-coil // Hsien 2013/5/22 
	//----------------------
	ODNC_CONFIRM,
	//--------------------------------------------------
	//	Ring Counter Type 2 , much more concised  // Hsien 2013/5/23
	//--------------------------------------------------
	ODRINGCTR_TYPE2_COUNTUP		,	// Ring-Counter Device Count-UP,	Hsien , 2013.01.21
	ODRINGCTR_TYPE2_COUNTDOWN	,	// Ring-Counter Device Count-Down,  Hsien , 2013.01.21
};

class outputDevice
{
	//----------------------------------------------------------------------------
	//	each child class stand for a single statement
	//	e.g countUp class , it would generate
	//			%s = %s + 1
	//	this base class is used to store generated string and basic attributes only.
	//-----------------------------------------------------------------------------
protected:
	unsigned char kind		;	// used to indetify  which statement-skeleton to use
	unsigned char type		;	// used to identify child's type , when down-cast comes.
	string generatedStr		;	// the generated string compound by input strings within specific format
public:
	outputDevice(const outputDevice& source);	// copy constructor , nessarary to build for loading on vector
	outputDevice(){
		this->kind = EMPTY_STMT;
		this->type = NONE;						// added by Hsien , to depict empty statement , 2012.06.25
	}
	~outputDevice(){};
	unsigned char	getKind()	{ return this->kind;}	// query the kind of this device
	unsigned char	getType()	{ return this->type;}	// query the type of this device
	//-----------------
	// able to override
	//-----------------
	virtual string		getGenStr	()	{ return this->generatedStr;};	// query the generated string ,would trigger generating function
	virtual void		importData	(const	char*	dataPack);			// data-rebuild function
	virtual const char*	exportData	(void);								// data-store	function

};

class odSet:public outputDevice
{
	//-----------------------------
	//	string generate : lhs = rhs
	//-----------------------------
private:
	string lhs;				// raw info 
	string rhs;				// raw info 
public:
	odSet(){
		this->kind = IF_STMT;					// fixed property
		this->type = ODSET;
	}
	~odSet(){
	};
	void			setLhs		(const string	lhs){this->lhs = lhs;}
	void			setRhs		(const string	rhs){this->rhs = rhs;}

	string			getLhs		(){ return this->lhs;}
	string			getRhs		(){	return this->rhs;}

	virtual string	getGenStr	();	
	virtual void		importData(const char* dataPack);
	virtual const char*	exportData(void);
};

class odCoil:public outputDevice
{
	//------------------------------------------------------------------
	//	two mode to output Coil's statement
	//		1. Y0.00 = expr , so that output Y0.00 only
	//		2. Y0.00 = 1,	  so that output entire statement "Y0.00 = 1" 
	//------------------------------------------------------------------
/*private*/protected:		// Hsien 2013/5/23 for able to inheritant by child
	string	bitName;	// raw info of coil
public:
	odCoil(){
		this->kind = SINGLE;
		this->type = ODCOIL;
	}
	~odCoil(){};

	string			getBitName	()									{ return this->bitName;}
	void			setBitName	(const string			bitName)	{ this->bitName = bitName;}
	int				setKind		(const unsigned char	newKind)	{ this->kind = newKind;}		//	change stored generatedStr when Kind changes.

	virtual string	getGenStr	();
	virtual void		importData	(const char* dataPack);
	virtual const char*	exportData	(void);
};


class odNonStopTimer:public outputDevice
{
	//------------------------------------------------------------
	//	Added by Hsien , 2013.01.04 , for function block extending
	//		Both For Timer Set/Reset
	//	Related Frame:
	//		TN_EN
	//		TN_G
	//------------------------------------------------------------
private:
	int		timerIndex;			// the timer index		, count from zero
	/*int*/string		timerInterval;		// the timer interval
											// Hsien 2013/5/21 , change into pure string
public:
	odNonStopTimer(const char _type){
		//----------------------------------------------------------
		//	Constructor , decide which type this timer is set or rst
		//----------------------------------------------------------
		this->kind = IF_STMT;			// run under the bracket of if(){}
		this->type = _type;	
		//this->type = ODSETTN;
		this->timerIndex		= 0;
		this->timerInterval		= "15";	// Hsien 2013/5/21
	}
	~odNonStopTimer(){};
	//---------------
	//	Set/Get Functions:
	//---------------
	void	setTimerIndex	(const int _index)		{ this->timerIndex		= _index;}
	void	setTimerInterval(const /*int*/string _interval)	{ this->timerInterval	= _interval;}	// Hsien 2013/5/21
	void	setTimerType	(const char _type)		{ this->type			= _type;}	// type is able to set after constructed
																						// for data-rebuilding/storing
																						// Hsien , 2013.01.15
	int		getTimerIndex	()	{return this->timerIndex;}
	/*int*/string		getTimerInterval()	{return this->timerInterval;}
	char	getTimerType	()	{return this->type;}


	virtual string	getGenStr	();
	virtual void		importData	(const char* dataPack);
	virtual const char*	exportData	(void);
};

class odRingCounterSet:public outputDevice
{
	//------------------------------------------------------------
	//	Added by Hsien , 2013.01.04 , for function block extending
	//		would accompany with rountine service
	//------------------------------------------------------------
private:
	int		counterIndex;				// index of counter , 
	/*int*/string		counterGoal;				// the goal		value of counter
	/*int*/string		counterInit;				// the initial	value of cournter running value
													// Hsien 2013/5/21 , change into pure string
public:
	odRingCounterSet(){
		//----------------------------------------------------------
		//	Constructor , 
		//----------------------------------------------------------
		this->kind = IF_STMT;
		this->type = ODRINGCTR_SET;

		counterIndex	= 0;
		counterGoal		= "0";
		counterInit		= "0";
	};
	~odRingCounterSet(){};
	//---------------
	//	Set/Get Functions:
	//---------------
	void setCounterIndex	(const int _counterIndex)	{ this->counterIndex	= _counterIndex;}
	void setCounterGoal		(const /*int*/string _counterGoal)	{ this->counterGoal		= _counterGoal;}	
	void setCounterInit		(const /*int*/string _counterInit)	{ this->counterInit		= _counterInit;}

	int	 getCounterIndex	(){ return this->counterIndex;}
	/*int*/string	 getCounterGoal		(){ return this->counterGoal;}
	/*int*/string	 getCounterInit		(){ return this->counterInit;}

	virtual string	getGenStr	();
	virtual void		importData	(const char* dataPack);
	virtual const char*	exportData	(void);
};

class odRingCounterCount:public outputDevice
{
private:
	int	counterIndex;
public:
	odRingCounterCount(const char _type){
		this->kind = IF_STMT;
		this->type = _type;

		counterIndex = 0;
	}
	~odRingCounterCount(){};
	//---------------
	//	Set/Get Functions:
	//---------------
	void setCounterIndex	(const int _counterIndex)	{ this->counterIndex	= _counterIndex;}
	void setCounterType		(const char _type)			{ this->type = _type;}			// type is able to set after constructed
																						// for data-rebuilding/storing
																						// Hsien , 2013.01.21
	int		getCounterIndex	()							{	return this->counterIndex;}

	virtual string	getGenStr	();
	virtual void		importData	(const char* dataPack);
	virtual const char*	exportData	(void);
};

class odStBlock:public outputDevice
{
	//------------------------------------------------------------
	//	Added by Hsien , 2013.01.21 , for function block extending , to promoting the device developing
	//		really a easy, opened device
	//------------------------------------------------------------
private:
public:
	odStBlock(){
		this->kind = IF_STMT;
		this->type = ODSTBLOCK;
	}
	~odStBlock(){};
	//---------------
	//	Set/Get Functions:
	//---------------
	void setStStatement(const string _stStmt){ this->generatedStr = _stStmt;}
	//--------------
	//	Import/Export Functions:
	//--------------
	virtual void		importData	(const char* dataPack);
	virtual const char*	exportData	(void);
};

class odIndexerCoil : public odCoil
{
	//----------------------
	//	Turns index input into word.bit sytle
	//		import() , export() were inheritant from class odCoil
	// Hsien 2013/5/23
	//----------------------
public:
	odIndexerCoil(unsigned char _type){
		this->type = _type;
		this->setIndex(0);
	}
	~odIndexerCoil(){};
	int		setIndex(const int _index);
	int		getIndex(void);
	void	setType (const unsigned char _type){this->type = _type;}
};

class odRingCounterCountType2 : public outputDevice
{
	//------------------------------------------------------------
	// Hsien 2013/5/24 
	//		different to type1 , do Goal Set and Compare at once
	//	merge from counterSet and counterCount
	//		and extend counterStep 
	//------------------------------------------------------------
private:
	int		counterIndex;				// index of counter , 
	string	counterGoal;				// the goal		value of counter
	string	counterInit;				// the initial	value of cournter running value
										// Hsien 2013/5/21 , change into pure string
	string	counterStep;
public:
	odRingCounterCountType2(unsigned char _type){
		//----------------------------------------------------------
		//	Constructor , 
		//----------------------------------------------------------
		this->kind = IF_STMT;
		this->type = _type;

		counterIndex	= 0;
		counterGoal		= "0";
		counterInit		= "0";
		counterStep		= "1";
	};
	~odRingCounterCountType2(){};
	//---------------
	//	Set/Get Functions:
	//---------------
	void setCounterIndex	(const int _counterIndex)	{ this->counterIndex	= _counterIndex;}
	void setCounterGoal		(const string _counterGoal)	{ this->counterGoal		= _counterGoal;}	
	void setCounterInit		(const string _counterInit)	{ this->counterInit		= _counterInit;}
	void setCounterStep		(const string _counterStep)	{ this->counterStep		= _counterStep;}
	void setCounterType		(const char _type)			{ this->type = _type;}			// type is able to set after constructed

	int		getCounterIndex	(){ return this->counterIndex;}
	string	getCounterGoal	(){ return this->counterGoal;}
	string	getCounterInit	(){ return this->counterInit;}
	string	getCounterStep	(){ return this->counterStep;}

	virtual string	getGenStr	();
	virtual void		importData	(const char* dataPack);
	virtual const char*	exportData	(void);
};

typedef vector<outputDevice>	OD_ARRAY/*odArray_t*/;	// Hsien 2013/5/24 , used when ST generating
outputDevice* createOd(const int typeId);

#endif