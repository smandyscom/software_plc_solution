#include<LDE_VERTICE.h>
#include<stdlib.h>

#define TN_O_FORMAT	"TN_O%d.%d"	
#pragma warning(disable:4996)

int nonStopTimerNo::setTimerIndex(const int _index)
{
	//-------------------------------------
	//	coding timer index into bit name form
	//		Hsien , 2013.01.16
	//-------------------------------------
	div_t	outWordBit		= div(_index,16);
	char	outString[32]; 
	
	sprintf(
		outString,
		TN_O_FORMAT,
		outWordBit.quot,
		outWordBit.rem);

	this->setBitName(outString);

	return 1;
}

int	nonStopTimerNo::getTimerIndex()
{
	//-------------------------------
	//	decode bit name form to timer index
	//		Hsien , 2013.01.16
	//-------------------------------
	int	wordIndex = 0;
	int	bitIndex  = 0;

	sscanf(
		this->data.c_str(),
		TN_O_FORMAT,
		&wordIndex,
		&bitIndex);

	return wordIndex*16+bitIndex;
}

