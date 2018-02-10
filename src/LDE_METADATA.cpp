#include<LDE_METADATA.h>
#include<string.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//-------------------------
//	function codes list
//		not suitable for multithread use
//		Hsien , 2012.10.3
//-------------------------
//using namespace metaData;
//static	void*	methodList[] = 
//{
//	&device::duplicate,
//	&device::finalize,
//	&device::fWrite,
//	&device::fRead,
//	&cluster::duplicate,
//	&cluster::finalize,
//	&cluster::fWrite,
//	&cluster::fRead,
//};

//#define	DEV_DUPLICATE	0x01
//#define	DEV_FINALIZE	0x02
//#define DEV_FWRITE		0x03
//#define	DEV_FREAD		0x04
//#define	CLST_DUPLICATE	0x05
//#define	CLST_FINALIZE	0x06
//#define	CLST_FWRITE		0x07
//#define	CLST_FREAD		0x08



namespace metaData
{
	CLST_METADATA*	clstCreate(			
		DUPLICATOR	_leafDuplicator
		,DESTRUCTOR	_leafDestructor
		,FILEWRITER	_leafFileWriter
		,FILEREADER _leafFileReader
			,const size_t dataCounts)
	{
		//-------------------------------
		//	C-style namespace constructor
		//-------------------------------
		CLST_METADATA*	tmp 
			= (CLST_METADATA*)malloc(sizeof(CLST_METADATA));

		tmp->headData		 = device::initialize();		
		tmp->leafDuplicator	 = _leafDuplicator;
		tmp->leafDestructor	 = _leafDestructor;

		tmp->leafFileWriter	 = _leafFileWriter;
		tmp->leafFileReader	 = _leafFileReader;

		//-----------------------------------------
		//	meta Codes to record which methods used
		//-----------------------------------------
		((char*)&tmp->methodCodes)[0] = metaFunc2Code(tmp->leafDuplicator);
		((char*)&tmp->methodCodes)[1] = metaFunc2Code(tmp->leafDestructor);
		((char*)&tmp->methodCodes)[2] = metaFunc2Code(tmp->leafFileWriter);
		((char*)&tmp->methodCodes)[3] = metaFunc2Code(tmp->leafFileReader);

		tmp->dataCounts = dataCounts;
		tmp->dataCluster
			= (void**)malloc(dataCounts * sizeof(void*));
		memset(tmp->dataCluster
			,0
			,tmp->dataCounts * sizeof(void*)); 

		return tmp;
	}

	namespace device{
		DEV_METADATA*	initialize	()
		{
			//------------------
			//	allocating DEV_METADATA only, without allocating data*
			//------------------
			DEV_METADATA	*tmp;

			tmp = (DEV_METADATA*)malloc(sizeof(DEV_METADATA));
			
			memset(tmp,0,sizeof(DEV_METADATA));

			return tmp;
		}
		int			finalize	(DEV_METADATA		*dmds)
		{
			if(dmds->data != NULL)
				free(dmds->data);
			if(dmds != NULL)
				free(dmds);

			return 1;
		}
		DEV_METADATA*	duplicate	(const DEV_METADATA* source)
		{		
			//----------------------------------------
			// preassumption : source has been initilized
			//----------------------------------------
			DEV_METADATA* tmp = initialize();

			tmp->itemCode = source->itemCode;
			
			tmp->data = 
					(char*)malloc(source->dataSize);
			memcpy(tmp->data,source->data,source->dataSize);
			tmp->dataSize = source->dataSize;

			return tmp;
		}

		int				writeIn		(DEV_METADATA *dmds,const int itemCode,const char* data/*,size_t __dataSize*//*added by Hsien , 2013.4.16*/)
		{
			//------------------------------------
			//	reallocating instead of allocating
			//		1. definitly write-in itemCode
			//		2. check source data , if NULL means no data writing needed
			//			, then freeing dmds->data
			//		3. re-allocating , and write-in 
			//------------------------------------
			dmds->itemCode = itemCode;

			if(data == NULL){
				//-----------------
				//		When No any Data imported(Mean Null-Data)
				//	Problem May Happend Here? Hsien , 2013.04.15
				//-----------------
				dmds->dataSize = /*1*/0;
				free(dmds->data);
				dmds->data = NULL;

				return META_SUCCESS;
			}


			dmds->dataSize = /*__dataSize*/strlen(data) + 1;					
																// bugs may happend here , Hsien , 2013.04.15
																// unsafe string counting method
																// added by Hsien  ,2013.04.16
			dmds->data
				= (char*)realloc(dmds->data,dmds->dataSize);


			if(dmds->data == NULL){
				dmds->dataSize = 0;
				return META_ERR_NO_MEM;				// memory exhausted
			}

			//if(dmds->dataSize == 1){
			//	//--------------
			//	//	Means Null-Data, Hsien , 2013.04.16
			//	//--------------
			//	*dmds->data = 0;
			//}
			//else{
				memcpy(dmds->data,data,dmds->dataSize);
			//}

			return META_SUCCESS;
		}
		const char*		readOut		(const DEV_METADATA *dmds,int	&__itemCode,size_t &__dataSize	/*added by Hsien , 2013.04.19*/)
		{						
			//-----------------------------------------------------------
			// in order to trigger expected move of following realloc
			//	critical , Hsien , 2012.07.06
			//	'static' description will raise DEBUG_ASSERTION ERROR in CLR environment
			//------------------------------------------------------------
			
			//----------------------
			//	Bad implementation , Hsien , 2013.04.19
			//----------------------
			__itemCode	=	dmds->itemCode;
			__dataSize	=	dmds->dataSize;	// Hsien , 2013.04.19

			return dmds->data;
		}
}//namespace device

	namespace cluster{
	
		CLST_METADATA*	initialize	(
			/*CP_CONSTRUCTOR*/DUPLICATOR	_leafConstructor
			,DESTRUCTOR		_leafDestructor
			,const size_t dataCounts)
		{
			CLST_METADATA*	tmp 
				= (CLST_METADATA*)malloc(sizeof(CLST_METADATA));

			tmp->headData		 = device::initialize();		
			tmp->leafDuplicator/*Constructor*/ = _leafConstructor;
			tmp->leafDestructor	 = _leafDestructor;
			tmp->dataCounts = dataCounts;
			tmp->dataCluster
				= (void**)malloc(dataCounts * sizeof(void*));
			memset(tmp->dataCluster,0,tmp->dataCounts * sizeof(void*)); 

			return tmp;
		}
		void finalize	(CLST_METADATA	*cmds)
		{
			device::finalize(cmds->headData);

			//--------------------------
			//	destruct leaf recursivly
			//--------------------------
			if(cmds->leafDestructor != NULL){
				for(size_t i=0;i<cmds->dataCounts;i++)
					cmds->leafDestructor(cmds->dataCluster[i]);
			}

			if(cmds->dataCluster != NULL){
				//--------
				//	Hsien , 2013.04.08
				//--------
				free(cmds->dataCluster);	// destruct array of pointers
			}

			if(cmds != NULL){
				//--------
				//	Hsien , 2013.04.08
				//--------
				free(cmds);
			}
		}

		CLST_METADATA*	duplicate	(const CLST_METADATA* source)
		{
			//------------------------------------------------
			//	initialize tmp cluster data
			//	write in header
			//	duplicate source's leaf by source's duplicator	
			//------------------------------------------------
			CLST_METADATA	*tmp
				= clstCreate(
				source->leafDuplicator
				,source->leafDestructor
				,source->leafFileWriter
				,source->leafFileReader
				,source->dataCounts);			// updated by Hsien , 2012.10.03

			device::finalize(tmp->headData);	// in order to solve memory leakage problem
			tmp->headData = device::duplicate(source->headData);

			tmp->methodCodes = source->methodCodes;

			//----------------------
			//	duplicate every leaf
			//----------------------
			if(source->leafDuplicator != NULL){			// error prevent
				for(size_t i=0;i<source->dataCounts;i++)
					tmp->dataCluster[i] = 
					source->leafDuplicator(source->dataCluster[i]);		// duplicate source's leaf by source's duplicator
			}

			return tmp;
		}


		int writeIn(CLST_METADATA *cmds,const size_t dataIndex,void*	elem)
		{
			//-------------------------------
			//	link cluster's leaf with elem
			//-------------------------------
			if(dataIndex > (cmds->dataCounts-1))
				return 0;

			cmds->dataCluster[dataIndex] = elem;

			return 1;
		}

		void*	readOut(const CLST_METADATA	*cmds,const size_t dataIndex)
		{
			//----------------------------------
			//	given cluster,dataIndex
			//		return object data reference
			//----------------------------------
			if(dataIndex > (cmds->dataCounts-1))
				return NULL;

			return cmds->dataCluster[dataIndex];
		}
	}// namespace cluster

char		metaFunc2Code(void*	fp)
{
//-------------------------
//	function codes list
//		 for multithread use
//		Hsien , 2012.10.3
//-------------------------
	int	code = -1;	// default is error code
	void*	methodList[] = 
	{
		&device::duplicate,
		&device::finalize,
		&device::fWrite,
		&device::fRead,
		&cluster::duplicate,
		&cluster::finalize,
		&cluster::fWrite,
		&cluster::fRead,
	};

	for(char i=0;i<sizeof(methodList)/sizeof(void*);i++){
		if(methodList[i] == fp){
			code = i;
			break;
		}//if
	}//if

	return code;
}

void*	metaCode2Func(char	code)
{
//-------------------------
//	function codes list
//		 for multithread use
//		Hsien , 2012.10.3
//-------------------------
	void*	methodList[] = 
	{
		&device::duplicate,
		&device::finalize,
		&device::fWrite,
		&device::fRead,
		&cluster::duplicate,
		&cluster::finalize,
		&cluster::fWrite,
		&cluster::fRead,
	};

	return methodList[code];
}

}//namespace
