//---------------------------------------------------------
// FILENAME				: LDE_OUTDEV.cpp
// ORIGIN AYTHOR		: Hsien , 2012.05.20
// EDITOR				: Hsien
// NOTE					: 
//---------------------------------------------------------
#include<LDE_OUTDEV.h>
#pragma warning(disable:4996)		// warning about sprintf
#include<string.h>
#include<stdlib.h>

outputDevice::outputDevice(const outputDevice& source)
{
	//---------------
	//	copy constructor , nessarary to build for multithread usage
	//		e.g vector<>
	//	unable to fully reconstruct derive class , 
	//	instead , deep copy all private data from source
	//---------------
	this->type = source.type;
	this->kind = source.kind;
	this->generatedStr = source.generatedStr;
}

void		outputDevice::importData(const char*	dataPack)
{
	this->generatedStr.assign(dataPack);
}

const char*	outputDevice::exportData(void)
{
	return this->generatedStr.c_str();
}

string	odSet::getGenStr()
{
	this->generatedStr.clear();
	this->generatedStr
		= this->lhs + "=" + this->rhs + ";";	// cancelled /n and the end of generatedStr , Hsien , 2012.10.31
	return this->generatedStr;
}
void odSet::importData(const char* dataPack)
{
	string tmp(dataPack);

	this->lhs.assign(tmp,0,tmp.find("\n"));
	this->rhs = tmp.substr(tmp.find("\n")+1);
}
const char* odSet::exportData(void)
{
	this->generatedStr = this->lhs + "\n" + this->rhs;

	return this->generatedStr.c_str();
}
string	odCoil::getGenStr()
{
	switch(this->kind){
		case SINGLE:	this->generatedStr = this->bitName;				break;
		case IF_STMT:	this->generatedStr = this->bitName + "=1;";		break;	// cancelled /n and the end of generatedStr , Hsien , 2012.10.31
	}//switch
	
	return this->generatedStr;
}

void		odCoil::importData(const char *dataPack)
{
	this->bitName.assign(dataPack);
}
const char*	odCoil::exportData(void)
{
	return this->bitName.c_str();
}


#define	SYNC_CFM_FORMAT	"SYNC_CFM%d.%d"	// Hsien 2013/5/23

int	odIndexerCoil::setIndex(const int _index)
{
		//-------------------------------------
	//	coding index form into bit name form
	//		switch the format according to type
	// Hsien 2013/5/23
	//-------------------------------------
	div_t	outWordBit		= div(_index,16);
	char	outString[32]; 
	
	switch(this->type){
			//----------------------
			//	NC Sync-IO request receive , shared the implementation with indexer // Hsien 2013/5/22 
			//----------------------
		case ODNC_CONFIRM:
			sprintf(outString,SYNC_CFM_FORMAT,outWordBit.quot,outWordBit.rem);
			break;
		default:
			break;
	}

	this->setBitName(outString);

	return 1;
}

int odIndexerCoil::getIndex()
{
	//-------------------------------
	//	decode bit name form to index form
	// Hsien 2013/5/23
	//-------------------------------
	int	wordIndex = 0;
	int	bitIndex  = 0;

	switch(this->type){
			//----------------------
			//	NC Sync-IO request receive , shared the implementation with indexer // Hsien 2013/5/22 
			//----------------------
		case ODNC_CONFIRM:
			sscanf(this->bitName.c_str(),SYNC_CFM_FORMAT,&wordIndex,&bitIndex);
			break;
		default:
			break;
	}

	return wordIndex*16+bitIndex;
}

outputDevice*	createOd(const int typeId)
{
	outputDevice*	tmp = NULL;

	switch(typeId){
		case NONE:		tmp = (outputDevice*)new outputDevice();			break;
		case ODCOIL:	tmp = (outputDevice*)new odCoil();					break;
		case ODSET:		tmp = (outputDevice*)new odSet();					break;
			//-------
			//	TIMER
			//-------
		case ODSETTN:	tmp = (outputDevice*)new odNonStopTimer(ODSETTN);	break;
		case ODRSTTN:	tmp = (outputDevice*)new odNonStopTimer(ODRSTTN);	break;
			//-------
			//	ST-BLOCK  , added by Hsien , 2013.01.21
			//-------
		case ODSTBLOCK:	tmp = (outputDevice*)new odStBlock();				break;
			//------
			//	Ring-Counter , Added by Hsien , 2013.01.22
			//------
		case ODRINGCTR_SET:			tmp = (outputDevice*)new odRingCounterSet();						break;
		case ODRINGCTR_COUNTUP:		tmp = (outputDevice*)new odRingCounterCount(ODRINGCTR_COUNTUP)	;	break;
		case ODRINGCTR_COUNTDOWN:	tmp = (outputDevice*)new odRingCounterCount(ODRINGCTR_COUNTDOWN);	break;
			//----------------------
			//	NC Sync-IO confirm send , implemented on indexer-coil // Hsien 2013/5/22 
			//----------------------
		case ODNC_CONFIRM:			tmp = (outputDevice*)new odIndexerCoil(ODNC_CONFIRM);					break;
			//--------------------------------------------------
			//	Ring Counter Type 2 , much more concised  // Hsien 2013/5/23
			//--------------------------------------------------
		case ODRINGCTR_TYPE2_COUNTUP:	tmp = (outputDevice*)new  odRingCounterCountType2(ODRINGCTR_TYPE2_COUNTUP);		break;// Ring-Counter Device Count-UP,	Hsien , 2013.01.21
		case ODRINGCTR_TYPE2_COUNTDOWN:	tmp = (outputDevice*)new  odRingCounterCountType2(ODRINGCTR_TYPE2_COUNTDOWN);	break;	// Ring-Counter Device Count-Down,  Hsien , 2013.01.21
		default:
			tmp = NULL;
			break;
	}

	return tmp;
}