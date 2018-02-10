//---------------------------------------------------------
// FILENAME				: LDE_OUTDEV_STBLOCK.cpp
// ORIGIN AYTHOR		: Hsien , 2013.01.21
// EDITOR				: Hsien
// NOTE					: 
//---------------------------------------------------------
#include<LDE_OUTDEV.h>
#pragma warning(disable:4996)		// warning about sprintf
#include<string.h>


void odStBlock::importData(const char* dataPack)
{
	this->generatedStr = dataPack;
}

const char* odStBlock::exportData()
{
	return this->generatedStr.c_str();
}