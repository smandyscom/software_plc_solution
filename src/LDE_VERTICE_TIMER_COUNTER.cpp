#include<LDE_VERTICE.h>
#include<stdlib.h>

#define TN_O_FORMAT		"TN_O%d.%d"
#define RC_O_FORMAT		"RC_O%d.%d"		// aded by Hsien ,2013.01.24
#define	SYNC_REQ_FORMAT	"SYNC_REQ%d.%d"	// Hsien 2013/5/22
#pragma warning(disable:4996)

//int nonStopTimerNo::setTimerIndex(const int _index)
//{
//	//-------------------------------------
//	//	coding timer index into bit name form
//	//		Hsien , 2013.01.16
//	//-------------------------------------
//	div_t	outWordBit		= div(_index,16);
//	char	outString[32]; 
//	
//	sprintf(
//		outString,
//		TN_O_FORMAT,
//		outWordBit.quot,
//		outWordBit.rem);
//
//	this->setBitName(outString);
//
//	return 1;
//}
//
//int	nonStopTimerNo::getTimerIndex()
//{
//	//-------------------------------
//	//	decode bit name form to timer index
//	//		Hsien , 2013.01.16
//	//-------------------------------
//	int	wordIndex = 0;
//	int	bitIndex  = 0;
//
//	sscanf(
//		this->data.c_str(),
//		TN_O_FORMAT,
//		&wordIndex,
//		&bitIndex);
//
//	return wordIndex*16+bitIndex;
//}
//
//int ringCounterNo::setRcIndex(const int _index)
//{
//	return 1;
//}
//
//int	ringCounterNo::getRcIndex()
//{
//	return 1;
//}
//
int	indexerNo::setIndex(const int _index)
{
	//-------------------------------------
	//	coding index form into bit name form
	//		switch the format according to type
	//		Hsien , 2013.01.24
	//-------------------------------------
	div_t	outWordBit		= div(_index,16);
	char	outString[32]; 
	
	switch(this->type){
		case NONSTOP_TIMER_OUTPUT_NO:
			sprintf(outString,TN_O_FORMAT,outWordBit.quot,outWordBit.rem);
			break;
		case RING_COUNTER_OUTPUT_NO:
			sprintf(outString,RC_O_FORMAT,outWordBit.quot,outWordBit.rem);
			break;
			//----------------------
			//	NC Sync-IO request receive , shared the implementation with indexer // Hsien 2013/5/22 
			//----------------------
		case NC_REQUEST:
			sprintf(outString,SYNC_REQ_FORMAT,outWordBit.quot,outWordBit.rem);
			break;
		default:
			break;
	}

	this->setBitName(outString);

	return 1;
}

int	indexerNo::getIndex()
{
	//-------------------------------
	//	decode bit name form to index form
	//		Hsien , 2013.01.24
	//-------------------------------
	int	wordIndex = 0;
	int	bitIndex  = 0;

	switch(this->type){
		case NONSTOP_TIMER_OUTPUT_NO:
			sscanf(this->data.c_str(),TN_O_FORMAT,&wordIndex,&bitIndex);
			break;
		case RING_COUNTER_OUTPUT_NO:
			sscanf(this->data.c_str(),RC_O_FORMAT,&wordIndex,&bitIndex);
			break;
			//----------------------
			//	NC Sync-IO request receive , shared the implementation with indexer // Hsien 2013/5/22 
			//----------------------
		case NC_REQUEST:
			sscanf(this->data.c_str(),SYNC_REQ_FORMAT,&wordIndex,&bitIndex);
			break;
		default:
			break;
	}

	return wordIndex*16+bitIndex;
}