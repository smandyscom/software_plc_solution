//---------------------------------------------------------
// FILENAME				: LDE_OUTDEV_COUNTER.cpp
// ORIGIN AYTHOR		: Hsien , 2013.01.21
// EDITOR				: Hsien
// NOTE					: 
//---------------------------------------------------------
#include<LDE_OUTDEV.h>
#pragma warning(disable:4996)		// warning about sprintf
#include<string.h>
#include<stdlib.h>

//#define RING_COUNTER_SETUP_FORMAT	"RC_R%d = %d;\nRC_G%d = %d;\n"
#define RING_COUNTER_SETUP_FORMAT	"RC_R%d = %s;\nRC_G%d = %s;\n"	// Hsien 2013/5/21 , change type

//#define RING_COUNTER_SETUP_STORE	"%d\n%d\n%d"
												// the format used to store/rebuild
												// 1. counter index
												// 2. counter goal
												// 3. counter initial
#define RING_COUNTER_SETUP_STORE	"%d\n%s\n%s"
												// the format used to store/rebuild
												// 1. counter index
												// 2. counter goal
												// 3. counter initial
												// Hsien 2013/5/21
#define RING_COUNTER_SETUP_STORE_MATCHCOUNT	3


#define	RING_COUNTER_COUNTUP_FORMAT		"if(RC_R%d >= RC_G%d)\nRC_R%d = 0;\nelse\nRC_R%d = RC_R%d + 1;\nend\n"
									// 5- arguments
#define	RING_COUNTER_COUNTDOWN_FORMAT	"if(RC_R%d <= 0)\nRC_R%d = RC_G%d;\nelse\nRC_R%d = RC_R%d - 1;\nend\n"
									// 5- arguments
#define RING_COUNTER_OUTPUT_FORMAT		"if(RC_R%d == RC_G%d)\nRC_O%d.%d = 1;\nelse\nRC_O%d.%d = 0;\nend\n"
									// the output part after counted
									// 6- arguments
#define RING_COUNTER_COUNT_STORE		"%d\n%d"
									// the format used to store/rebuild
									// 1. type
									// 2. index

//----------------------
//	String length definition ,  for consisting every length// Hsien 2013/5/23
//----------------------
#define	INDEX_STRING_MAX_LENGTH	2
#define	INIT_STRING_MAX_LENGTH	128
#define GOAL_STRING_MAX_LENGTH	INIT_STRING_MAX_LENGTH
#define	STEP_STRING_MAX_LENGTH	INIT_STRING_MAX_LENGTH
#define	GEN_STRING_MAX_LENGTH	320/*strlen(RING_COUNTER_SETUP_FORMAT) \
	+ 2*INDEX_STRING_MAX_LENGTH \
	+ INIT_STRING_MAX_LENGTH \
	+ GOAL_STRING_MAX_LENGTH*/
#define	STORE_STRING_MAX_LENGTH	GEN_STRING_MAX_LENGTH/*strlen(RING_COUNTER_SETUP_STORE) \
	+ INIT_STRING_MAX_LENGTH\
	+ GOAL_STRING_MAX_LENGTH*/

static	char	setupString			[GEN_STRING_MAX_LENGTH];	// Hsien 2013/5/23 , should be enough for worst case , prevent overrun
static	char	goalString			[GOAL_STRING_MAX_LENGTH];
static	char	initString			[INIT_STRING_MAX_LENGTH];			// Hsien 2013/5/21
static	char	stepString			[STEP_STRING_MAX_LENGTH];			// Hsien 2013/5/24
static	char	storeStringBuffer	[STORE_STRING_MAX_LENGTH];	//used to store the export string
										// not sure if cause heap error , Hsien , 2013.01.21

string odRingCounterSet::getGenStr()
{
	//----------------------
	//	Set-Up the Parameter
	//		1. Running	Value
	//		2. Goal		Value
	//----------------------

	sprintf(setupString,
		RING_COUNTER_SETUP_FORMAT,
		this->counterIndex,
		this->counterInit.c_str(),
		this->counterIndex,
		this->counterGoal.c_str());				// print the infomation as ST
												// Hsien 2013/5/21
	this->generatedStr.clear();
	this->generatedStr = setupString;	// ST assignment

	return this->generatedStr;
}


void		odRingCounterSet::importData(const char *dataPack)
{
	//-------------
	//	Rebuild the counter index/goal/initial info
	//		input have to be formatted
	//			may implement data-format check in the future
	//			Hsien , 2013.01.21
	//-------------	
	int		matchCount = 0;

	matchCount = sscanf(dataPack,
		RING_COUNTER_SETUP_STORE,
		&this->counterIndex,
		/*&this->counterGoal*/goalString,
		/*&this->counterInit*/initString);

	//----------------------
	//	scanf is unable to handle NULL-string
	// Hsien 2013/5/21
	//----------------------
	if(matchCount == RING_COUNTER_SETUP_STORE_MATCHCOUNT){
		this->counterGoal	= goalString;
		this->counterInit	= initString;
	}
	else{
		this->counterGoal.clear();
		this->counterInit.clear();
	}
}

const char* odRingCounterSet::exportData()
{
	//-------------
	//	Store the Counter index/goal/initial info
	//		Formatted output
	//-------------	
	sprintf(storeStringBuffer,
		RING_COUNTER_SETUP_STORE,
		this->counterIndex,
		this->counterGoal.c_str(),
		this->counterInit.c_str());
// Hsien 2013/5/21

	return storeStringBuffer;
}

string odRingCounterCount::getGenStr()
{
	//----------------------
	//	Generate Count Up/Down Statement Section According to this->type
	//			Statement Section could be divided into two parts:
	//		1. Count	Part
	//		2. Output	Part
	//----------------------
//	char	countString[64];
	div_t	wordBit;

	//----------------
	//	Part1 : Count Section
	//----------------
	switch(this->type){
		case ODRINGCTR_COUNTUP:
			sprintf(/*countString*/setupString,
				RING_COUNTER_COUNTUP_FORMAT,
				this->counterIndex,
				this->counterIndex,
				this->counterIndex,
				this->counterIndex,
				this->counterIndex);
			break;
		case ODRINGCTR_COUNTDOWN:
			sprintf(/*countString*/setupString,
				RING_COUNTER_COUNTDOWN_FORMAT,
				this->counterIndex,
				this->counterIndex,
				this->counterIndex,
				this->counterIndex,
				this->counterIndex);
			break;
		default:
			break;
	}

	this->generatedStr.clear();
	this->generatedStr = setupString;

	//----------------
	//	Part2 : Output
	//----------------
	wordBit = div(this->counterIndex,16);
	sprintf(/*countString*/setupString,
		RING_COUNTER_OUTPUT_FORMAT,
		this->counterIndex,
		this->counterIndex,
		wordBit.quot,
		wordBit.rem,
		wordBit.quot,
		wordBit.rem);

	this->generatedStr.append(setupString);		// append the output string

	return this->generatedStr;
}

void odRingCounterCount::importData(const char *dataPack)
{
	sscanf(dataPack
		,RING_COUNTER_COUNT_STORE
		,&this->type
		,&this->counterIndex);
}

const char* odRingCounterCount::exportData()
{
	sprintf(storeStringBuffer
		,RING_COUNTER_COUNT_STORE
		,this->type
		,this->counterIndex);

	return storeStringBuffer;
}


#define	RING_COUNTER_TYPE2_COUNTUP_FORMAT		"\
		if(RC_R%d >= %s)\
			RC_R%d = %s;\
		else \
			RC_R%d = RC_R%d + %s; \
		end"
											// 7- arguments
#define	RING_COUNTER_TYPE2_COUNTDOWN_FORMAT		"\
		if(RC_R%d <= %s)\
			RC_R%d = %s;\
		else \
			RC_R%d = RC_R%d - %s; \
		end"
											// 7- arguments
#define RING_COUNTER_TYPE2_STORE	"%d\n%s\n%s\n%s"
												// the format used to store/rebuild
												// 1. counter index
												// 2. counter goal
												// 3. counter initial
												// 4. counter step
#define RING_COUNTER_TYPE2_STORE_MATCHCOUNT	4	// Hsien 2013/5/24

string odRingCounterCountType2::getGenStr()
{
	//----------------------
	//	Generate Count Up/Down Statement Section According to this->type
	//	Count Only
	// Hsien 2013/5/24 , maybe no more output? use comparator to instead of it
	//----------------------

	//----------------
	//	Part1 : Count Section
	//----------------
	switch(this->type){
		case ODRINGCTR_TYPE2_COUNTUP:
			sprintf(setupString,
				RING_COUNTER_TYPE2_COUNTUP_FORMAT,
				this->counterIndex,
				this->counterGoal.c_str(),
				this->counterIndex,
				this->counterInit.c_str(),
				this->counterIndex,
				this->counterIndex,
				this->counterStep.c_str());
			break;
		case ODRINGCTR_TYPE2_COUNTDOWN:
			sprintf(setupString,
				RING_COUNTER_TYPE2_COUNTDOWN_FORMAT,
				this->counterIndex,
				this->counterInit.c_str(),
				this->counterIndex,
				this->counterGoal.c_str(),
				this->counterIndex,
				this->counterIndex,
				this->counterStep.c_str());
			break;
		default:
			break;
	}

	this->generatedStr.clear();
	this->generatedStr = setupString;

	return this->generatedStr;
}



const char*	odRingCounterCountType2::exportData()
{
	//----------------------
	//
	//----------------------
	// Hsien 2013/5/24
	sprintf(storeStringBuffer
		,RING_COUNTER_TYPE2_STORE
		,this->counterIndex
		,this->counterGoal.c_str()
		,this->counterInit.c_str()
		,this->counterStep.c_str());

	return storeStringBuffer;
}

void odRingCounterCountType2::importData(const char *dataPack)
{
	//----------------------
	//
	// Hsien 2013/5/24
	//----------------------
	int		matchCount = 0;

	matchCount = sscanf(dataPack
		,RING_COUNTER_TYPE2_STORE
		,&this->counterIndex
		,goalString
		,initString
		,stepString);

	//----------------------
	//	scanf is unable to handle NULL-string
	// Hsien 2013/5/21
	//----------------------
	if(matchCount == RING_COUNTER_TYPE2_STORE_MATCHCOUNT){
		this->counterGoal = goalString;
		this->counterInit = initString;
		this->counterStep = stepString;
	}
	else{
		this->counterGoal.clear();
		this->counterInit.clear();
		this->counterStep.clear();
	}
}