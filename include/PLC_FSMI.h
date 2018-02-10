//---------------------------------------------------------
// FILENAME			: PLC_FSMI.h
// ORIGIN AUTHOR	: Hsien
// EDITOR			: Hsien, 2012.03.28
// NOTES			: Frames Management Interface
//---------------------------------------------------------
#include<limits.h>
#define		MAX_FRAME_NUM	UCHAR_MAX		//	
#define		FID				unsigned short
#define		MAX_RS_NUM		8
#include<PLC_FRAME.h>


namespace	FSMI{
	
	//------------------------------
	// management interface
	//------------------------------
	void	initialize			(void);	
	int		finalize			(void);									// deallocate all frame
	int		addFrame			(const FID frmId,const size_t frmSize);	// register a block of memory , and allocating it , Hsien , 2012.06.12
	int		removeFrame			(const FID frmId);

	int		resetFrame			(/*const FID frmId*/);						// turns all values in frame to zero , Hsien , 2013.01.07
	//---------------------------
	//	query interface
	//---------------------------
	void*	queryAddress	(const FID frmId,const size_t elemSize,const size_t index);	//
	size_t	queryCounts		(void);							//	added by Hsien , 2012.06.12	, used to get how many frames this CLCT posessed.
	FRAME*	queryFrame		(const FID frmId);				//  added by Hsien , 2012.06.13 , memory monitoring use when debugging.
	//---------------------------
	//	routine service  interface , Hsien , 2012.06.12
	//---------------------------
	namespace RSMI
	{
		int		addRoutineService	(const FID frmId,routineService rs);
		int		removeRoutineService(const FID frmId);
		int		removeRoutineService(const FID frmId,routineService rs);	// Hsien 2013/6/4
		int		runAllRoutineService(void);					// runs all frame's routine service
	}

}

