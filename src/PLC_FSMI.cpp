//---------------------------------------------------------
// FILENAME			: PLC_FSMI.cpp
// ORIGIN AUTHOR	: Hsien
// EDITOR			: Hsien, 2012.03.28
// NOTES			: Frames Management Interface
//---------------------------------------------------------
#include<PLC_FSMI.h>
#include<limits.h>
#include<stdlib.h>
#include<string.h>

static unsigned char	frameCounts;				// how many frame this Management Interface posessed.
static FRAME			frames[MAX_FRAME_NUM];		// add a id table listing mechanism, to showing current added frame's id

static int CompareID( const void *elem1, const void *elem2)
{
	//------------------------------------------------------------------
	//	the custom fucntion offering for bsearch() , which is in stdlib
	//------------------------------------------------------------------
	return	(*((FID*)elem2) - *((FID*)elem1));		//	descand sorting
}

namespace	FSMI
{
	void	initialize	(void)
	{
		memset(frames,0,sizeof(FRAME) * MAX_FRAME_NUM);
		frameCounts = 0;
	}

	int		addFrame	(const FID frmId,const size_t frmSize)
	{
		char*	tmp;

		tmp = (char*)malloc(frmSize);
		if(!tmp)
			return 0;			// memory exhausted
		memset(tmp,0,frmSize);	// memory clearing

		if(!frmInit(&frames[frameCounts],frmId,frmSize,tmp)){
			free(tmp);
			return 0;	// fail to initializing, due to invalid size value or etc..
		}

		frameCounts++;

		qsort(frames,frameCounts,sizeof(FRAME),CompareID);	//	qsort OK , Hsien , 2012.03.27
															//	re-organize frames by its id.

		return 1;
	}

	int		finalize(void)
	{
		//----------------------------------
		// release all frame
		//	added by Hsien , 2012.06.26
		//----------------------------------
		for(int i=0;i<frameCounts;i++)
			free(frames[i].baseAdr);

		initialize();

		return 1;
	}

	int		removeFrame	(const FID frmId)
	{
		FRAME* tmp;

		tmp = queryFrame(frmId);
		if(!tmp)
			return 0;										//	target frame not found
		free(tmp->baseAdr);
		memset(tmp,0,sizeof(FRAME));						//	reset all attribute as zero.
															// zero for frame's id means invalid. Hsien , 2012.06.18

		qsort(frames,frameCounts,sizeof(FRAME),CompareID);	//	re-organize frames by its id.

		--frameCounts;

		return 1;
	}
	void*	queryAddress(const FID frmId,const size_t elemSize,const size_t index)
	{
		FRAME* tmp = queryFrame(frmId);
		if(!tmp)
			return NULL;
		return frmQueryAdr(tmp,elemSize,index);				// return NULL pointer if request in not reasonable.
	}

	size_t	queryCounts	(void)
	{
		return frameCounts;
	}

	FRAME*	queryFrame	(const FID frmId)
	{
		return (FRAME*)bsearch(&frmId,frames,frameCounts,sizeof(FRAME),CompareID);
	}

	int		resetFrame	(/*const FID frmId*/)
	{
		//--------------------
		//	Hsien , 2013.01.07
		//--------------------

		//--------------------
		//	The FID orientation version
		//-----------------------------
		//FRAME* tmp = queryFrame(frmId);
		//if(!tmp)
		//	return NULL;
		//return frmReset(tmp);

		//----------------------------
		//	The Whole Reseting Version
		//----------------------------
		for(int i=0;i<frameCounts;i++)
			frmReset(&frames[i]);

		return 1;
	}

	namespace RSMI
	{
		int		addRoutineService	(const FID frmId,routineService rs)
		{
			FRAME*	tmp = queryFrame(frmId);
			if(!tmp)
				return 0;

			if(!frmAddRs(tmp,rs))
				return 0;

			return 1;
		}
		int		removeRoutineService(const FID frmId)
		{
			FRAME*	tmp = queryFrame(frmId);
			if(!tmp)
				return 0;

			if(!frmRemoveRs(tmp))
				return 0;

			return 1;
		}
		int		removeRoutineService(const FID frmId,routineService rs)
		{
			//----------------------
			//	Compare and remove
			//----------------------
			FRAME*	tmp = queryFrame(frmId);
			if(!tmp)
				return 0;

			if(!frmRemoveRs(tmp,rs))
				return 0;

			return 1;
		}

		int		runAllRoutineService(void)
		{
			//----------------------------------------------------
			// runs all frame's routine service
			//	in sqeunce of id , in the same meaning of priority
			//	Hsien , 2012.06.13
			//----------------------------------------------------
			static int i = 0;
			
			//------------------------------------------------
			// running routine services in order of descending
			//	Hsien , 2012.06.18
			//	warning , for descending loop , the iterator index has to be signed integer
			//	otherwise , underflow occurs.
			//------------------------------------------------
			for(i = frameCounts-1 ;i >= 0 ; i--)		
				frmRunRs(&frames[i]);
			
			return 1;
		}
	}//namespace of RS
}//namespace of FSMI
