//----------------------------------------------------------------------------------------------------------------
// FILENAME			: HS_MECH.h
// ORIGIN AUTHOR	: Hsien
// EDITOR			: Hsien, 2012.12.04
// NOTES			: Generic Hand-Shake Mechanism Provider
//----------------------------------------------------------------------------------------------------------------
#define	HS_ABLE_TO_READ		1								// definition of hand-shake-flag
#define HS_ABLE_TO_WRITE	0								// definition of hand-shake-flag
#define	HS_UNABLE_TO_READ	HS_ABLE_TO_WRITE
#define	HS_UNABLE_TO_WRITE	HS_ABLE_TO_READ

#define	HS_SUCCESS	0x00
#define	HS_WAIT		0x01

struct	hs_signal_st
{
	//-----------------------------------------
	//	THE COMMON HEADER OF THE PROTECTED DATA 
	//	PROVIDED BY HAND-SHAKE MECHANISM
	//	The Share Data should inheritant this Header
	//-----------------------------------------
	char	flag;			//	indicate	the data status
	void*	dataPtr;		//	indicate	the data address  , Hsien , 2013.01.03
	size_t	dataSize;		//	indicate	data block size
	/* data body */
};

typedef hs_signal_st				HS_SIGNAL;

int	hsInitial	(HS_SIGNAL*	sharedData
				 ,void*			_dataPtr
				 ,const size_t	_dataSize);	
int hsRead		(HS_SIGNAL* sharedData,void*	localData);		// share-data should be casted from derived class
int hsWrite		(HS_SIGNAL* sharedData,void*	localData);		// share-data should be casted from derived class
