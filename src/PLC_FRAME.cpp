//---------------------------------------------------------
// FILENAME			: PLC_FRAME.cpp
// ORIGIN AUTHOR	: Hsien
// EDITOR			: Hsien, 2012.02.16
// NOTES			: abc
//---------------------------------------------------------
#include<PLC_FRAME.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

static bool	frmCheck(const FRAME* frm,const size_t elemSize,const size_t index)	
{
	float	log2Res = 0;

	//---------
	//	elemSize should be in 2^n , n is interger
	//---------
	log2Res = log((float)elemSize)/log((float)2);
	if(ceilf(log2Res) != log2Res && log2Res != 0)			// if log2Res is not integer either nonzero, check failed
		return false;
	
	//-------
	//	elemSize,frmSize were represented in unit of BYTE
	//	index was represeted in unit of elemSize
	//-------
	if(elemSize * (index + 1) > frm->frmSize)	// less or equal is acceptable
		return false;
	return true;
}

int	frmInit			(FRAME* frm,
						 const FID frmId,
						 const size_t frmSize,
						 char* baseAdr)
{
//---------------------------------------------------------
// frmInit : initialize frame and link to memory if existed
// Following condition must be hold:
//					1.	frame ID has not to be zero
//					2.	frame frmSize	should be nonzero
//					3.	frame Count		should be 2^n , where n is positive integer
//---------------------------------------------------------
	float log2Res;								// to load DEFAULT_LENG's result after squared

	memset(frm,0,sizeof(FRAME));
	//------------------
	//	condition check
	//------------------
	if(		frmId		== 0 
		||	frmSize		== 0)
		return 0;

	log2Res = log((float)frmSize)/log((float)2);
	if(ceilf(log2Res) != log2Res && log2Res != 0)			// if log2Res is not integer either nonzero, check failed
		return 0;

	//-------------------------------------
	//	assign frame size & id
	//-------------------------------------	
	frm->frmId   = frmId;
	frm->frmSize = frmSize;

	//-------------------------------------
	// link to memory if existed and refresh em
	//-------------------------------------
	if(baseAdr == NULL)
		return 1;

	//--------------------------------------
	//	link and check if any memory invalid
	//--------------------------------------
	frm->baseAdr = baseAdr;
	for(size_t i=0;i<frm->frmSize;i++)
		if(((frm->baseAdr)+i) == NULL)
			return 0;

	memset(frm->baseAdr,0,frm->frmSize);
	return 1;
}
FRAME*			frmAllocate		(const FID frmId,
								 const size_t frmSize)
{
	//------------------------------------------------------
	// initialize frame and link to dynamically allocated memory
	//------------------------------------------------------
	FRAME*		 frm;

	//------
	// allocate frame header
	//------
	frm = (FRAME*)malloc(sizeof(FRAME));
	if(frm == NULL)
		return NULL;

	//-----------------
	// initialize frame
	//-----------------
	if(!frmInit(frm,frmId,frmSize,NULL)){
		free(frm);
		return NULL;
	}

	//------
	// allocate frame's content
	//------
	frm->baseAdr
		= (char*)malloc(frm->frmSize);
	if(!frm->baseAdr){
		free(frm);
		return NULL;
	}

	memset(frm->baseAdr,0,frm->frmSize);

	return frm;
}
void			frmDeallocate	(FRAME* frm)
{
	free(frm->baseAdr);
	free(frm);
	frm = NULL;
}
void*			frmQueryAdr		(FRAME* frm,const size_t elemSize,const size_t index)
{
	//----------------------------------------------------
	//	Query required address if existed
	//----------------------------------------------------
	if(!frmCheck(frm,elemSize,index))
		return 	NULL;
	return (&(frm->baseAdr[elemSize * index]));
}
int		frmReset(const FRAME*	frm)
{
	//-------------------------------
	//	Reset Values in Frame to Zero
	//		Hsien , 2013.01.07
	//-------------------------------
	memset(frm->baseAdr,0,frm->frmSize);
	return 1;
}
int		frmGetTotalSize(const FRAME*	frm)
{
	//----------------
	//	Used To Return the Frame Size
	//----------------
	return (int)frm->frmSize;
}
//-------------------------------------------------------
//	rountine service manager , added by Hsien, 2012.06.12
//-------------------------------------------------------
int			frmAddRs		(FRAME*	frm,routineService rs)
{
	//----------------------------------
	// push a routine service into stack
	//----------------------------------
	if(!rs)
		return 0;
	if(frm->rsCounts >= MAX_RS_NUM)
		return 0;
	
	frm->rs[frm->rsCounts] = rs;
	++frm->rsCounts;

	return 1;
}
int			frmRemoveRs		(FRAME* frm)
{
	//---------------------------------
	// pop a routine service from stack
	//---------------------------------
	if(frm->rsCounts < 1)
		return 0;				// no rs in stack

	frm->rs[frm->rsCounts] = NULL;
	--frm->rsCounts;

	return 1;
}
int			frmRemoveRs		(FRAME* frm,routineService rs)
{
	//----------------------
	//	Compare and remove , but would cause blob
	//		not good algo
	// Hsien 2013/6/4
	//----------------------
	if(frm->rsCounts < 1)
		return 0;				// no rs in stack

	for(int i=0;i<(int)frm->rsCounts;i++){
		if(frm->rs[i] == rs){
			frm->rs[i] = NULL;	
			--frm->rsCounts;
		}
	}

	return 1;
}

int			frmRunRs		(FRAME*	frm)
{
	//------------------------------------------------
	// run routine service , if any. Hsien, 2012.06.12
	//------------------------------------------------
	for(size_t i=0;i<frm->rsCounts;i++)
		(frm->rs[i])(frm);

	return 1;
}