//---------------------------------------------------------
// FILENAME			: PLC_FRAME.h
// ORIGIN AUTHOR	: Hsien
// EDITOR			: Hsien, 2012.02.16
// NOTES			: 
//						FRAME , THE SIMULATING MEMORY QUERY INTERFACE (DATA STRUCTURE)
//  ID identified & TYPE CARELESS 1-dimension Array
//		Note:
//		a.	Zero is invalid FRAME_ID
//		b.	accepatable element type is BYTE(8bits) , WORD(16bits) , DWORD(32bits) so far 
//		c.	you can access each byte if request is reasonable
//		d.	send your request into frmQueryAdr , then returned a non-Null pointer , 
//				if your request is valid
//		ABBREVIATION :
//			frm : frame
//			adr : address
//---------------------------------------------------------------------------------
// BYTE , WORD , DWORD was followed by WINDEF , but also in POSIX standard
//---------------------------------------------------------------------------------
#ifndef _PLC_FRAME_H
#define _PLC_FRAME_H

#ifndef		FID									// able to be overrided by outer predefined MARCO , Hsien , 2012.06.15
#define		FID				unsigned short	
#endif	

#ifndef		MAX_RS_NUM
#define		MAX_RS_NUM		8
#endif

typedef struct frame_st FRAME;
typedef int (*routineService)(FRAME*);	// prototype of rountine service , added by Hsien , 2012.06.12
struct	frame_st
{
	FID				frmId;				
	size_t			frmSize;			//	in unit of byte
	char*			baseAdr;
										//	does it need to add a service state? Hsien , 2012.06.12
	size_t			rsCounts;
	routineService	rs[MAX_RS_NUM];		//  added by Hsien, 2012.06.12 , for loading routine service
};

//---------------------------------------------------------------------------------------------
// frmInit : set basic attributes into frm , and link frm to a existed base adress , if valid 
//	if baseAdr is not valid , initializing would be done as well. in initializing stage , 
//	the main purpose is to check if Id and Size valid
//---------------------------------------------------------------------------------------------
int				frmInit			(FRAME* frm			,const FID frmId
													,const size_t frmSize
													,char* baseAdr);
void*			frmQueryAdr		(FRAME* frm,const size_t elemSize,const size_t index);

FRAME*			frmAllocate		(const FID frmId	,const size_t frmSize);		// seldom use
void			frmDeallocate	(FRAME* frm);			// seldom use

int		frmReset		(const FRAME*	frm);			// reset the frame
int		frmGetTotalSize	(const FRAME*	frm);			// get the size info of the frame

//-------------------------------------------------------
//	rountine service manager , added by Hsien, 2012.06.12
//-------------------------------------------------------
int			frmAddRs		(FRAME*	frm,routineService rs);	// push a routine service into stack
int			frmRemoveRs		(FRAME* frm,routineService rs);	// search and remove , // Hsien 2013/6/4
int			frmRemoveRs		(FRAME* frm);					// pop a routine service from stack
int			frmRunRs		(FRAME*	frm);					// run routine service , if any. Hsien, 2012.06.12

#endif

