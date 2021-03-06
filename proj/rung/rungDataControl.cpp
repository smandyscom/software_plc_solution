#include"stdafx.h"

using namespace _ldEditor;

//--------------------------------------------------
// data storing/rebuiling , plain traverse functions
//--------------------------------------------------
int rungControl::store(CLST_METADATA	**rmdsContainer)
{
	//---------------------------------------------------------------------
	//	1. cluster meta-data initializing
	//	2. write-in head information
	//		i.	comments
	//		ii.	how many rows
	//	3. call plain's store function then attach these leaf into cluster.
	//---------------------------------------------------------------------
	std::string		tmpString;
	DEV_METADATA	*tmpData = NULL;
	size_t			dataIndex;		// last-write-in index

	//-----
	//	Try to finalize it
	//	Hsien , 2012.09.28
	//-----
	if(*rmdsContainer){
		cluster::finalize(*rmdsContainer);
		*rmdsContainer = NULL;
	}

	using namespace cluster;
	//*rmdsContainer = cluster::initialize(
	//	(CP_CONSTRUCTOR)device::duplicate
	//	,(DESTRUCTOR)device::finalize
	//	,rowCount * (innerInpNum + 1));							//	plus one output device
	*rmdsContainer = clstCreate(
		(DUPLICATOR)device::duplicate
		,(DESTRUCTOR)device::finalize
		,(FILEWRITER)device::fWrite
		,(FILEREADER)device::fRead
		,rowCount * (innerInpNum + 1/*plus one plainOuput Cell */));		// updated , Hsien , 2012.10.03

	//-----------------
	//	Write-In row-counts , and comment
	//------------------
	MarshalString(tb_Comments->Text,tmpString);
	if(device::writeIn((*rmdsContainer)->headData
		,rowCount
		,tmpString.c_str()) != META_SUCCESS)
	{
			//---------
			//	Write-in failure , do finalize
			//---------	
			return 0;
	};	
				//	storing rowCount , and Comments content simultaneously.

	//---------------------------------------------------------------
	//	then find-out where the editable-plain are , and output plain
	//	call their store function and attach them to cluster
	//---------------------------------------------------------------
	dataIndex = -1;
	for each(Control^ e in tlpRung->Controls){												// tranverse all controls in tlp
		if(tlpRung->GetPositionFromControl(e).Column >=1
			&& tlpRung->GetPositionFromControl(e).Column <= innerInpNum)					// editable-plain input part
		{
			if(!safe_cast<plainInputControl^>(e)->store(&tmpData)){
				//----------
				//	Store Process Fail , return error
				//----------
				return 0;
			}								// will finalize last-tempData
			dataIndex++;
			cluster::writeIn(*rmdsContainer
				,dataIndex
				,device::duplicate(tmpData));
			//------------------------------------------------------------
			// tmpData will be finalized in the next cycle of data storing
			// so that, we need to do deep duplication of it.		
			//  [7/30/2012 smandyscom]
			//------------------------------------------------------------
		}
		else if (tlpRung->GetPositionFromControl(e).Column == (tlpRung->ColumnCount - 2))	// plain output part
		{
			if(!safe_cast<plainOutputControl^>(e)->store(&tmpData)){
				return 0;
			};						// will finalize last-tempData
			dataIndex++;		
			cluster::writeIn(*rmdsContainer,dataIndex,device::duplicate(tmpData));	// tmpData will be finalized in the next cycle of data storing
																					// so that, we need to do deep duplication of it.		
																					//  [7/30/2012 smandyscom]
		}
	}

	device::finalize(tmpData);		// reease memory

	return 1;
}
int rungControl::rebuild(const CLST_METADATA	*rmds)
{
	//-----------------------------------------
//	Rebuild rung according to rmds:
	// 0. reset this rung
	// 1. import comments and row count
	// 2. according row counts , creating rows
	// 3. import plainIn/Out data
	//-----------------------------------------
	int							_rowCount;
	size_t						rungCommentSize;	// Hsien , 2013.04.19
	/*std::string*/char*		comment;
	size_t						dataIndex;

	comment				= (char*)device::readOut(rmds->headData
		,_rowCount
		,rungCommentSize);		// read-out header infomation
	//tb_Comments->Text	= gcnew String(comment.c_str());			//	show-up comments
	tb_Comments->Text = gcnew String(comment,0,rungCommentSize);	//	Hsien , 2013.04.19

	//--------------------------
	//	if rowCount doesnt match _rowCount , do some procedure to
	//		make numbers of plainInput/Output as command descriping
	//	performance would enhanced in expect.
	//	Hsien , 2012.09.30
	//--------------------------
	while (this->rowCount != _rowCount){
		if(this->rowCount < _rowCount)
			addRow(FOLLOWER);
		else if(this->rowCount > _rowCount)
			deleteRow();
	}							//	propogate/eliminate rows until rowCount fullfilled

	dataIndex = 0;
	for each(Control^ e in tlpRung->Controls){
		if(tlpRung->GetPositionFromControl(e).Column >=1
			&& tlpRung->GetPositionFromControl(e).Column <= innerInpNum)					// editable-plain input part
		{
			safe_cast<plainInputControl^>(e)->rebuild((const DEV_METADATA*)cluster::readOut(rmds,dataIndex));
			dataIndex++;
		}
		else if (tlpRung->GetPositionFromControl(e).Column == (tlpRung->ColumnCount - 2))	// plain output part
		{
			safe_cast<plainOutputControl^>(e)->rebuild((const DEV_METADATA*)cluster::readOut(rmds,dataIndex));
			dataIndex++;
		}
	}
	// simple ,test ok ,  [7/30/2012 smandyscom]
	//	need to do more further check.
	return 1;
}


