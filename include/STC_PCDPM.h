//--------------------------------------------------------------------------------------------------------------
//// FILENAME			: STC_PCDPM.h
// ORIGIN AUTHOR		: Hsien	,	2012.06.08
// EDITOR				: Hsien
// NOTE					: Parent-Child Data structure Propagation Mechanism
//			1. data structure style : linked list(chain) , from root(head) to end
//										for container , file-stream loaded indentical elements ( array )
//			2. every node(child) includes a buffer , implemented by file stream
//			3. buffer could be used as fifo or random-accessing array
//			4. this mechanism supports merge child's buffer to parent's , when child going to eliminate.
//---------------------------------------------------------------------------------------------------------------
#ifndef _STC_PCDPM_H
#define _STC_PCDPM_H
#include<stdio.h>

typedef struct pcdpm_family_st PCDPM_FAMILY;	// class of PCDPM handler
typedef struct pcdpm_member_st PCDPM_MEMBER;	// element of PCDPM

struct pcdpm_member_st
{
		PCDPM_MEMBER*	parent;		 // link to this member's parent
		size_t			counts;		 // how many times had been write-in
		FILE*			output;		 // countainer
};

struct pcdpm_family_st
{
	//------------------------------------------------------------
	//	such definition is private for caller , Hsien , 2012.10.03
	//------------------------------------------------------------


	int					memberCounts;		// how many members in this inheritance family
	size_t				memberElemSize;		// referenced element size of container of member , should be initialized at first 
	PCDPM_MEMBER		root;
//	NULL,		// no parent since root
//	0,
//	NULL,
//};
	PCDPM_MEMBER*		youngest;
};

//---------------------
//	ERROR/WARNING codes , use 3rd BYTE to indentify
//---------------------
#define	PCDPM_SUCCESS			0x00
#define	PCDPM_ERR_INITIALIZED	0x01
#define	PCDPM_ERR_INVALID_TRUNK	0x02	// given trunk is not valid
#define	PCDPM_ERR_NO_MEM		0x03	// memory exhausted when moving data in closing child
#define	PCDPM_ERR_NO_CHILD		0x04	// warning when try to close child when no any child existed
#define	PCDPM_ERR_INVALID_GEN	0x05	// invalid generation
#define	PCDPM_ERR_FILEIO		0x06	// file IO error

//enum pcdpm_enum
//{
	//-----------------------------------
	//	inheritance indicating when querying
	//-----------------------------------
#define	PCDPM_CHILD/*,*/		0		// newest branch
#define PCDPM_PARENT/*,*/		1
#define PCDPM_GRAND				2
//};

//----------------------
//	management interface
//----------------------
int		pcdpmInit		(PCDPM_FAMILY*	family
						 ,FILE*			mainTrunk
						 ,const size_t	elemSize);
int		pcdpmCreateChild(PCDPM_FAMILY* family);
int		pcdpmCloseChild	(PCDPM_FAMILY* family);
int		pcdpmClose		(PCDPM_FAMILY* family);
//------------------------
//	data control interface
//------------------------
//int		pcdpmWriteInChild	(PCDPM_FAMILY* family,void* elemData,const size_t elemSize); //seems redundant
//int		pcdpmWriteInParent	(PCDPM_FAMILY* family,void* elemData,const size_t elemSize);
int		pcdpmWriteData		(PCDPM_FAMILY* family
							 ,void* elemData
							 //,const size_t elemSize
							 ,const size_t generation);	// push data
int		pcdpmReadData		(PCDPM_FAMILY* family
							 ,void* elemData
							 //,const size_t elemSize
							 ,const size_t generation);	// pop	data

size_t	pcdpmQueryCounts	(PCDPM_FAMILY*	family,const size_t generation);
int		pcdpmResetData		(PCDPM_FAMILY*	family,const size_t generation);	// added by Hsien ,2012.10.5

int		pcdpmReadIndexOfData(PCDPM_FAMILY*	family
					 ,void* elemData
					 ,const size_t generation
					 ,const size_t index);				//get data of specific index of specific generation

#endif