//---------------------------------------------------------
// FILENAME				: LDE_OUTDEV_TIMER.cpp
// ORIGIN AYTHOR		: Hsien , 2013.01.14
// EDITOR				: Hsien
// NOTE					: 
//---------------------------------------------------------
#include<LDE_OUTDEV.h>
#pragma warning(disable:4996)		// warning about sprintf
#include<string.h>
#include<stdlib.h>

#define	RST_TN_G_FORMAT		"TN_G%d	 = 0;\n"
#define RST_TN_EN_FORMAT	"TN_EN%d.%d = 0;\n"
#define	RST_TN_FORMAT		"TN_G%d	 = 0;\nTN_EN%d.%d = 0;\n"	// Hsien 2013/5/23

//#define	SET_TN_G_FORMAT		"TN_G%d = %d;\n"
#define	SET_TN_G_FORMAT		"TN_G%d = %s;\n"		// Hsien 2013/5/21
#define	SET_TN_EN_FORMAT	"TN_EN%d.%d = 1;\n"
#define SET_TN_FORMAT		"TN_G%d = %s;\nTN_EN%d.%d = 1;\n"	// Hsien 2013/5/23

//#define SET_TN_STORE_FORMAT	"%d\n%d\n%c"			// the format used to store/rebuild
												// 1. timer index
												// 2. timer intervale
												// 3. timer type(set/rst)
#define SET_TN_STORE_FORMAT	"%d\n%s\n%c"			// the format used to store/rebuild
												// 1. timer index
												// 2. timer intervale
												// 3. timer type(set/rst)
												// Hsien 2013/5/21 , change timer-interval into string type
#define SET_TN_STORE_FORMAT_MATCHCOUNT	3		// Hsien 2013/5/21 , used to prevent null-string storing

//----------------------
//	String length definition ,  for consisting every length// Hsien 2013/5/23
//----------------------
#define GOAL_STRING_MAX_LENGTH	128
#define	GEN_STRING_MAX_LENGTH	256/*strlen(SET_TN_FORMAT) \
	+ 6 \
	+ GOAL_STRING_MAX_LENGTH*/
#define	STORE_STRING_MAX_LENGTH	GEN_STRING_MAX_LENGTH/*strlen(SET_TN_STORE_FORMAT) \
	+ 6	\
	+ GOAL_STRING_MAX_LENGTH*/

static	char	genString			[GEN_STRING_MAX_LENGTH];
static	char	goalString			[GOAL_STRING_MAX_LENGTH];
static	char	storeStringBuffer	[STORE_STRING_MAX_LENGTH];		//used to store the export string
										// not sure if cause heap error , Hsien , 2013.01.14


string	odNonStopTimer::getGenStr()	
{
	//-----------------
	//	1. Set the Goal Value of Timer
	//	2. Set the Enable bit of Timer
	//-----------------
	div_t	enWordBit;	// word index of enable bit , quotient of timer index divided by 16
						// bit	index of enable bit , reminder of timer index divided by 16
	//char	enString[32];	// the C-string buffer , in order to use Formatted-String
	//char	goalString[32];	//  the C-string buffer , in order to use Formatted-String

	enWordBit = div(this->timerIndex,16);

	switch(this->type){
		case ODSETTN:
			//----------------------
			//	Generate SET string
			//----------------------	
			//sprintf(goalString,	SET_TN_G_FORMAT,	this->timerIndex,	this->timerInterval.c_str());	// Hsien 2013/5/21
			//sprintf(enString,	SET_TN_EN_FORMAT,	enWordBit.quot,		enWordBit.rem);
			sprintf(genString
				,SET_TN_FORMAT
				,this->timerIndex
				,this->timerInterval.c_str()
				,enWordBit.quot
				,enWordBit.rem);	// Hsien 2013/5/23
			break;
		case ODRSTTN:
			//----------------------
			//	Generate RST string
			//----------------------
			//sprintf(goalString,	RST_TN_G_FORMAT,	this->timerIndex);
			//sprintf(enString,	RST_TN_EN_FORMAT,	enWordBit.quot,		enWordBit.rem);
			sprintf(genString
				,RST_TN_FORMAT
				,this->timerIndex
				,enWordBit.quot
				,enWordBit.rem);	// Hsien 2013/5/23
			break;
		default:
			//--------------
			//	Error Occurs
			//--------------
			break;
	}

	this->generatedStr.clear();
	this->generatedStr = genString;	// Hsien 2013/5/23
	//this->generatedStr.append(goalString);
	//this->generatedStr.append(enString);

	return this->generatedStr;
}


void	odNonStopTimer::importData(const char *dataPack)
{
	//-------------
	//	Rebuild the timer index/interval info
	//		input have to be formatted
	//			may implement data-format check in the future
	//			Hsien , 2013.01.14
	//-------------	
	int		matchCount = 0;

	matchCount = sscanf(dataPack
		,SET_TN_STORE_FORMAT
		,&this->timerIndex
		,/*&this->timerInterval*//*stringBuffer*/goalString
		,&this->type);	

	//----------------------
	//	scanf is unable to handle NULL-string
	// Hsien 2013/5/21
	//----------------------
	if(matchCount == SET_TN_STORE_FORMAT_MATCHCOUNT)
		this->timerInterval = goalString;
	else
		this->timerInterval.clear();
}

const char*	odNonStopTimer::exportData()
{
	//-------------
	//	Store the timer index/interval info
	//		Formatted output
	//-------------	
	sprintf(storeStringBuffer
		,SET_TN_STORE_FORMAT
		,this->timerIndex
		,this->timerInterval.c_str()	// Hsien 2013/5/21
		,this->type);			// it's a twin-type class
								// so that type should be stored in case to rebuild
								// Hsien , 2013.01.15
	return storeStringBuffer;
}