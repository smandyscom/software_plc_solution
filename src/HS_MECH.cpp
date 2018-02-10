#include<HS_MECH.h>
#include<string.h>
//----------------------------------------------------------------------------------------------------------------
// FILENAME			: HS_MECH.cpp
// ORIGIN AUTHOR	: Hsien
// EDITOR			: Hsien, 2012.12.04
// NOTES			: Generic Hand-Shake Mechanism Provider
//----------------------------------------------------------------------------------------------------------------

int	hsInitial(HS_SIGNAL*	sharedData
			  ,void*		_dataPtr
			  ,const size_t	_dataSize)
{
	sharedData->flag		= HS_ABLE_TO_WRITE;
	sharedData->dataPtr		= _dataPtr; 
	sharedData->dataSize	= _dataSize;

	return HS_SUCCESS;
}

int hsRead(HS_SIGNAL* sharedData,void*	localData)
{
	if(sharedData->flag == HS_UNABLE_TO_READ)
		return HS_WAIT;
	
	//-------------
	//	ABLE TO READ , read shared data to local data
	//-------------
	memcpy(localData
		,sharedData->dataPtr/*+sizeof(HS_SIGNAL)*/		// offset is not the effective method , Hsien , 2013.01.03
		,sharedData->dataSize);
	sharedData->flag = HS_ABLE_TO_WRITE;	// reset signal

	return HS_SUCCESS;
}

int hsWrite(HS_SIGNAL* sharedData,void*	localData)
{
	if(sharedData->flag == HS_UNABLE_TO_WRITE)
		return HS_WAIT;
	
	//-------------
	//	ABLE TO WRITE , write shared data from local data
	//-------------
	memcpy(sharedData->dataPtr/*+sizeof(HS_SIGNAL)*/	// offset is not the effective method , Hsien , 2013.01.03
		,localData
		,sharedData->dataSize);
	sharedData->flag = HS_ABLE_TO_READ;	// reset signal

	return HS_SUCCESS;
}