//--------------------------------------------------------------------------------------------------------------
// FILENAME			: STC_PCDPM.cpp
// ORIGIN AUTHOR		: Hsien	,	2012.06.08
// EDITOR				: Hsien
// NOTE					: Parent-Child Data structure Propagation Mechanism
//			1. data structure style : linked list(chain) , from root(head) to end
//			2. every node(child) includes a buffer , implemented by file stream
//			3. this mechanism supports merge child's buffer to parent's , when child going to eliminate.
//---------------------------------------------------------------------------------------------------------------
#include<STC_PCDPM.h>
#include<stdlib.h>
#include<string.h>
#pragma warning (disable:4996)		// unsafe tmpfile

//static int					pcdpmMembers	= 0;		// how many members in this inheritance family
//static struct family_st		pcdpmRoot		= {
//	NULL,		// no parent since root
//	0,
//	NULL,
//};
//static struct family_st*	pcdpmYoungest = NULL;

static PCDPM_MEMBER*	queryGeneration(PCDPM_FAMILY* family,const size_t generation)
{
	//-------------------------------------------
	//	tranverse generation via link connection
	//		0: youngest
	//		1: parent of youngest
	//		.
	//		.
	//		N-1(index)
	//-------------------------------------------
	PCDPM_MEMBER*	tmp = family->youngest;

	for(size_t i=0;i<generation;i++){
		tmp = tmp->parent;
	}

	return tmp;
}

int pcdpmInit(PCDPM_FAMILY* family
			  ,FILE* mainTrunk
			  ,const size_t	elemSize)
{
	//------------------------------------------------------------------------------
	//	initilizing root , including self-loop linking , external file buffer linking
	//	set youngest as root
	//------------------------------------------------------------------------------
	//if(family->memberCounts)	// if had been initialized
	//	return PCDPM_ERR_INITIALIZED;
	if(!mainTrunk)		// if mainTrunk is not valid
		return PCDPM_ERR_INVALID_TRUNK;

	family->memberElemSize = elemSize;		

	/*pcdpmRoot.parent*/ family->root.parent= &family->root/*pcdpmRoot*/;
	/*pcdpmRoot.counts*/ family->root.counts= 0;
	/*pcdpmRoot.output*/ family->root.output= mainTrunk;

	/*pcdpmYoungest*/family->youngest		= &family->root/*pcdpmRoot*/;

	/*pcdpmMembers*/ family->memberCounts	= 1;

	return PCDPM_SUCCESS;
}

int pcdpmCreateChild(PCDPM_FAMILY* family)
{
	//----------------------------------------------------------------
	//	allocating and initializing new child , and link its parent to
	//	 current youngest parent
	//	then set youngest as newborn child
	//----------------------------------------------------------------
	/*struct family_st*/PCDPM_MEMBER *newborn = (/*FAMILY*/PCDPM_MEMBER*)malloc(sizeof(PCDPM_MEMBER));

	newborn->parent = family->youngest/*pcdpmYoungest*/;
	newborn->counts = 0;
	newborn->output = tmpfile();		// all member other than root , using temporaroy internal buffer.

	/*pcdpmYoungest*/family->youngest = newborn;

	++family->memberCounts/*pcdpmMembers*/;

	return PCDPM_SUCCESS;
}

int pcdpmCloseChild(PCDPM_FAMILY* family)
{
	//------------------------------------
	//	copy all things stroed in youngest's output to its parent's output
	//	then cut link , set its parent as youngest
	//	eliminating this youngest child
	//------------------------------------
	char*  buffer;
	size_t fileSize;
	/*struct family_st*/PCDPM_MEMBER* death;

	if(family->memberCounts/*pcdpmMembers*/ == 1)	// only root left.
		return PCDPM_ERR_NO_CHILD;

	fileSize	= ftell(/*pcdpmYoungest*/family->youngest->output);	// get current position in stream
	buffer		= (char*)malloc(fileSize);							// allocating transmission buffer
	if(!buffer)
		return PCDPM_ERR_NO_MEM;			// memory error

	//-------------------------------------------
	//	copy contents of youngest to its parent
	//-------------------------------------------
	rewind	(/*pcdpmYoungest*/family->youngest->output);
	if((fread	(buffer
		,sizeof(char)
		,fileSize
		,/*pcdpmYoungest*/family->youngest->output)) != fileSize){
			free(buffer);
			return PCDPM_ERR_FILEIO;
	}
	if((fwrite	(buffer
		,sizeof(char)
		,fileSize
		,/*pcdpmYoungest*/family->youngest->parent->output)) != fileSize){
			free(buffer);
			return PCDPM_ERR_FILEIO;
	}		// append(merge) child's content to its parent
	/*pcdpmYoungest*/family->youngest->parent->counts += /*pcdpmYoungest*/family->youngest->counts;					// sum counts

	death			= family->youngest/*pcdpmYoungest*/;
	/*pcdpmYoungest*/family->youngest	= family->youngest->parent/*pcdpmYoungest->parent*/;		// cut link  , and set parent as youngest

	fclose(death->output);
	free(buffer);
	free(death);

	--family->memberCounts/*pcdpmMembers*/;

	return PCDPM_SUCCESS;
}

//int pcdpmWriteInChild(PCDPM_FAMILY* family
//					  ,void* elemData
//					  ,const size_t elemSize)
//{
//	//------------------------------
//	//	write single data in youngest child , then counting
//	//------------------------------
//	if((fwrite(elemData
//		,elemSize
//		,1
//		,/*pcdpmYoungest*/family->youngest->output)) != 1)
//		return PCDPM_ERR_FILEIO;
//	++(/*pcdpmYoungest*/family->youngest->counts);
//
//	return PCDPM_SUCCESS;
//}
//
//int pcdpmWriteInParent(PCDPM_FAMILY* family
//					   ,void* elemData
//					   ,const size_t elemSize)
//{
//	//---------------------------------------
//	//	write data in youngest child's parent
//	//---------------------------------------
//	if((fwrite(elemData
//		,elemSize
//		,1
//		,/*pcdpmYoungest*/family->youngest->parent->output)) != 1)
//		return PCDPM_ERR_FILEIO;
//	++(/*pcdpmYoungest*/family->youngest->parent->counts);
//
//	return PCDPM_SUCCESS;
//}


int pcdpmWriteData(PCDPM_FAMILY* family
				   ,void* elemData
				   //,const size_t elemSize
				   ,const size_t generation)
{
	//---------------------------------------
	//	write data in specific generation , as push a content into a queue
	//---------------------------------------
	/*FAMILY*/PCDPM_MEMBER*	tmp = queryGeneration(family,generation);
	if(tmp == NULL)
		return PCDPM_ERR_INVALID_GEN;

	if((fwrite(elemData
		,family->memberElemSize/*elemSize*/
		,1
		,tmp->output)) != 1)
		return PCDPM_ERR_FILEIO;
	++tmp->counts;

	return PCDPM_SUCCESS;
}
size_t pcdpmQueryCounts(PCDPM_FAMILY* family,const size_t generation)
{
	//-------------------------------------
	//	query counts
	//-------------------------------------
	/*FAMILY**/PCDPM_MEMBER*	tmp = queryGeneration(family,generation);
	if(tmp == NULL)
		return PCDPM_ERR_INVALID_GEN;
	return tmp->counts;
}

int pcdpmClose(PCDPM_FAMILY* family)
{
	int	errCode;
	//----------------------------------------------------------
	//	close all child , then cut file link to external of root
	//----------------------------------------------------------
	while(/*pcdpmYoungest*/ family->memberCounts != 1/*(&pcdpmRoot)*/){
		if((errCode = pcdpmCloseChild(family)) != PCDPM_SUCCESS)
			return errCode;
	}

	/*pcdpmRoot*/family->root.parent = NULL;
	/*pcdpmRoot*/family->root.counts = 0;
	/*pcdpmRoot*/family->root.output = NULL;

	/*pcdpmMembers*/family->memberCounts = 0;

	return PCDPM_SUCCESS;
}

int pcdpmReadData(PCDPM_FAMILY* family
				  ,void* elemData
				  //,const size_t elemSize
				  , const size_t generation)
{
	//---------------------------------------
	//	read data from specific generation , as pop a content on a queue
	//---------------------------------------
	PCDPM_MEMBER*	tmp = queryGeneration(family,generation);
	if(tmp == NULL)
		return PCDPM_ERR_INVALID_GEN;

	memset(elemData,0,family->memberElemSize);
	if(tmp->counts == 0)
		return PCDPM_ERR_FILEIO;
	//---------------------------------
	//	when fseek to very beggining
	//		file-pointer won't move any more
	//	so that fread will start at begin of FILE
	//	Hsien , 2012.10.09
	//---------------------------------
	fseek(tmp->output
		,-((long)family->memberElemSize)
		,SEEK_CUR);		// rewind fp to last elem's head

	if((fread(elemData
		,family->memberElemSize/*elemSize*/
		,1
		,tmp->output)) != 1)
		return PCDPM_ERR_FILEIO;
	
	fseek(tmp->output
		,-((long)family->memberElemSize)
		,SEEK_CUR);		// rewind fp to last elem's head


	--tmp->counts;
	
	return PCDPM_SUCCESS;
}

int	pcdpmResetData(PCDPM_FAMILY*	family,const size_t generation)
{
	//---------------------------------------
	//	reset data of specific generation 
	//---------------------------------------
	PCDPM_MEMBER*	tmp = queryGeneration(family,generation);
	if(tmp == NULL)
		return PCDPM_ERR_INVALID_GEN;

	rewind(tmp->output);
	tmp->counts = 0;

	return PCDPM_SUCCESS;
}

int	pcdpmReadIndexOfData(PCDPM_FAMILY*	family
					 ,void* elemData
					 ,const size_t generation
					 ,const size_t index)
{
	//---------------------------------------------------
	//	get data of specific index of specific generation
	//---------------------------------------------------
	long currentPosition;
	PCDPM_MEMBER*	tmp = queryGeneration(family,generation);
	if(tmp == NULL)
		return PCDPM_ERR_INVALID_GEN;

	currentPosition = ftell(tmp->output);	// back-up current position
	if(!fseek(tmp->output
		,(long)(index * family->memberElemSize)	// this line may raise overflow error , Hsien , 2012.10.5
		,SEEK_SET))
		return PCDPM_ERR_FILEIO;			// set file-pointer to willing position
	if((fread(elemData
		,family->memberElemSize
		,1
		,tmp->output)) != 1){
			return PCDPM_ERR_FILEIO;
	}
	if(!fseek(tmp->output
		,currentPosition
		,SEEK_SET))
		return PCDPM_ERR_FILEIO;			// turns file-pointer back to origin position

	return PCDPM_SUCCESS;
}
