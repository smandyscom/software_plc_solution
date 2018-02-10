#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<LDE_METADATA.h>

namespace metaData
{

namespace device
{
	int	fWrite	(const DEV_METADATA* source,FILE*	destFile)
	{
		//-------------
		//	1. write in determinitic data , such as , itemCode, dataSize
		//	2. trace indetermistic data via source->data , fwrite-in it
		//-------------
		size_t	totalCount = 0;

		totalCount +=	fwrite(&source->itemCode,sizeof(source->itemCode),1,destFile);
		totalCount +=	fwrite(&source->dataSize,sizeof(source->dataSize),1,destFile);

		if( source->dataSize > 0)
			totalCount +=	fwrite(source->data	,sizeof(char),source->dataSize,destFile);
			// write when data existed
			// Hsien , 2013.04.19

		if(totalCount != (source->dataSize+2))
			return META_ERR_UNKNOWN/*0*/;	// fail to write
						// problem occurs here?
		return META_SUCCESS/*1*/;
	}
	void*	fRead	(DEV_METADATA**		destBuf	,FILE*	sourceFile)
	{
		//------------
		//	Presumption: destBuf is not intialized buffer(IMPORTANT: because of in recursive usage , caller wouldn't know what
		//				leaf kind is , so that cannot initialize container externally
		//		1. intialize destBuf
		//		2. read-in data from file
		//		3. return buffer's reference
		//------------
		size_t			totalCount	= 0;
		DEV_METADATA	*tmpData	= NULL;

		//----------------------------
		//	reset dest-buffer at first 
		//----------------------------
		if(*destBuf != NULL){
			device::finalize(*destBuf);
			*destBuf = NULL;
		}

		tmpData = device::initialize();

		totalCount +=	fread(&tmpData->itemCode,sizeof(tmpData->itemCode),1,sourceFile);
		totalCount +=	fread(&tmpData->dataSize,sizeof(tmpData->dataSize),1,sourceFile);

		tmpData->data = (char*)malloc(tmpData->dataSize);
		if(tmpData->data == NULL)
			return NULL;								// Hsien , 2013.04.19
		memset(tmpData->data,0,tmpData->dataSize);		// Hsien , 2013.04.19
		//tmpData->data = (char*)realloc(tmpData->data,sizeof(char) * (tmpData->dataSize));	// reallocating buffer
																							// to complete memory management , Hsien , 2012.10.03
		//--------------------------------------
		//	Read-Out Data , When there's existed
		//		Hsien , 2013.04.19
		//--------------------------------------
		if(tmpData->dataSize > 0)
			totalCount +=	fread(tmpData->data,sizeof(char),tmpData->dataSize,sourceFile);

		if(totalCount != (tmpData->dataSize + 2)){
			device::finalize(tmpData);
			return NULL;	// fail to read
		}

		*destBuf = tmpData;

		return tmpData;
	}
}//namespace device

namespace cluster
{
	int		fWrite	(const CLST_METADATA* source,FILE*	destFile)
	{
		//-----------------------------------------------------------
		//	1. write-in head data
		//	2. write-in	other infos , duplicator code, destructor code...etc
		//	2. write-in leaf data leaf by leaf , using leafFileWriter
		//-----------------------------------------------------------
		int code;

		//----------
		//	Head-Data Write-In
		//----------
		if(device::fWrite(source->headData,destFile) != META_SUCCESS)
			return META_ERR_UNKNOWN;	// fail to write

		fwrite(&source->methodCodes	,sizeof(source->methodCodes),1,destFile);		// should be cata into head-data
																					// stand-for leaf methods
		fwrite(&source->dataCounts	,sizeof(size_t)				,1,destFile);		// stand-for imds+omds number

		for(size_t i=0;i<source->dataCounts;i++)
			if(source->leafFileWriter(source->dataCluster[i],destFile) != META_SUCCESS)
				return META_ERR_UNKNOWN;		// write-in failed

		return META_SUCCESS;
	}
	void*	fRead	(CLST_METADATA**		destBuf	,FILE*	sourceFile)
	{
		//-------------------
		//	read from file and reconstruct
		//		1. read header from file
		//		2. reconstruct cluster by header
		//		3. read leaf from file , and reconstruct leaf
		//		4. do step 4 repeatily , until data count satified
		//-------------------
		DEV_METADATA *tmpHeader = NULL;
		int	duplicatorCode
			,destructorCode
			,fileWriterCode
			,fileReaderCode
			,tmpMethodCode;
		size_t	tmpDataCount = 0;
		void*	tmpLeaf = NULL;

		CLST_METADATA	*tmpData;

		if(*destBuf != NULL){
			cluster::finalize(*destBuf);
			*destBuf = NULL;
		}
		//-------------------------------------------------------
		//	reconstruct cluster , read-out following information:
		//		*1. head data(size-undetermined datas)
		//		2. leaf duplicator
		//		3. leaf destructor
		//		4. leaf filereader
		//		5. leaf filewriter
		//		6. data counts
		//--------------------------------------------------------		
		if(device::fRead(&tmpHeader,sourceFile) == NULL)
			return NULL;					// Hsien , 2013.04.08
											// to prevent unexpected file-end , happens somehow
		//fread(&duplicatorCode	,sizeof(duplicatorCode)	,1,sourceFile);//read leaf duplicator
		//fread(&destructorCode	,sizeof(destructorCode)	,1,sourceFile);//read leaf destructor
		//fread(&fileWriterCode	,sizeof(fileWriterCode)	,1,sourceFile);//read leaf filereader
		//fread(&fileReaderCode	,sizeof(fileReaderCode)	,1,sourceFile);	//read leaf filewriter
		fread(&tmpMethodCode	,sizeof(tmpMethodCode)	,1,sourceFile);
		fread(&tmpDataCount		,sizeof(size_t)			,1,sourceFile);

		tmpData = clstCreate(
			(DUPLICATOR)metaCode2Func(((char*)&tmpMethodCode)[0]/*duplicatorCode*/)
			,(DESTRUCTOR)metaCode2Func(((char*)&tmpMethodCode)[1]/*destructorCode*/)
			,(FILEWRITER)metaCode2Func(((char*)&tmpMethodCode)[2]/*fileWriterCode*/)
			,(FILEREADER)metaCode2Func(((char*)&tmpMethodCode)[3]/*fileReaderCode*/)
			,tmpDataCount);											// initializing clst according to content read from file

		//-------------------------
		//	Read , Duplicate , and Attach to Cluster
		//-------------------------	
		device::finalize(tmpData->headData);	// addby Hsien ,2012.10.03 , to solve memory leakage problem
		tmpData->headData = tmpHeader;

		//----------------------
		//	Error Prevent , Checke the function if in specification
		//	// Hsien 2013/4/28
		//----------------------
		if(tmpData->leafDuplicator != (DUPLICATOR)cluster::duplicate
			&& tmpData->leafDuplicator != (DUPLICATOR)device::duplicate)
			return NULL;
		if(tmpData->leafDestructor != (DESTRUCTOR)cluster::finalize
			&& tmpData->leafDestructor != (DESTRUCTOR)device::finalize)
			return NULL;
		if(tmpData->leafFileWriter != (FILEWRITER)cluster::fWrite
			&& tmpData->leafFileWriter != (FILEWRITER)device::fWrite)
			return NULL;
		if(tmpData->leafFileReader != (FILEREADER)cluster::fRead
			&& tmpData->leafFileReader != (FILEREADER)device::fRead)
			return NULL;

		for(size_t i=0;i<tmpData->dataCounts;i++){
			//------------------------------------------------------------------
			// tmpLeaf is implicity intializing in the scope of leafFileReader
			// recursive reconstruct leaf , by leaf file reader
			//-------------------------------------------------------------------
			if(tmpData->leafFileReader(&tmpLeaf,sourceFile) == NULL){
				//-------------------------
				//	Error Prevent Mechaism
				//-------------------------
				if(tmpLeaf != NULL)
					free(tmpLeaf);
				*destBuf = tmpData;
				tmpData->headData->itemCode = i;		// justify real rung count
				tmpData->dataCounts			= i;
				return NULL;									
			}
																
			tmpData->dataCluster[i] = 
				tmpData->leafDuplicator(tmpLeaf);				// duplicate leaf and link it

			tmpData->leafDestructor(tmpLeaf);					// finalize tmpLeaf( memory free)
			tmpLeaf = NULL;										// after finalize , should set it as NULL
		}

		*destBuf = tmpData;

		return tmpData;
	}
}// namespace cluster
}// namespace metaData