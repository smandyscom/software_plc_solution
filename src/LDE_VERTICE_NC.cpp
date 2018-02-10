//---------------------------------------------------------
// FILENAME				: LDE_VERTICE_NC.cpp
// ORIGIN AYTHOR		: Hsien , 2012.05.08
// EDITOR				: Hsien
// NOTE					: Normal Close Manipulating Function
//---------------------------------------------------------
#include<LDE_VERTICE.h>

string normalClose::getData()
{
	this->data = "!" + bitName;
	return this->data;
}

void normalClose::importData(const char* dataPack)
{
	//--------------------------------------------------
	//	preassumption: dataPack were loaded with bitName
	//--------------------------------------------------
	this->bitName.assign(dataPack);
}

const char* normalClose::exportData()
{
	//----------------------------------------
	//	throw out bitName
	//----------------------------------------
	return this->bitName.c_str();
}