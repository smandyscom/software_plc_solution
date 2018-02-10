// plainOutput.cpp : main project file.

#include "stdafx.h"
#include "plainOutputControl.h"

using namespace plain;
int plainOutputControl::deviceEdit(int outItemCode
								   ,const char *importData
								   ,const char	_mode)
{
	//--------------
	//	when outItemCode != currentItemCode
	//			release current out item
	//	logic defect found by Hsien ,2012.09.28
	//--------------
	//--------------------------------
	//	highest priority : NONE(RESET)
	//--------------------------------
	 //if(outItemCode == NONE){
		// if(this->od){
		//	 delete this->od;
		//	 this->od = NULL;
		// }
		// ////-----
		// ////	Once NONE-device assigned , allocating outputDevice()
		// ////		Hsien ,  2012.09.28 , in order to solving memory-leakage problem
		// ////-----
		// //if(*importData == NONE)
		//	// this->od = new outputDevice();
		// this->Controls->Clear();
		// this->AutoSize = false;
		// this->BackgroundImage = safe_cast<Image^>(res->GetObject(L"ST"));
		// return 1;
	 //}

	//-------------------------
	//	Reject Repeative input , When on Edit , edited by Hsien , 2013.01.21
	//-------------------------
	if(this->currentItemCode == outItemCode
		&& (_mode == OD_ON_EDIT))
		return 0;
	 //----------------------------------
	 //	before doing change , reset first
	 //----------------------------------
	if(this->od){
		delete this->od;
		this->od = NULL;
	}
	this->Controls->Clear();

	//------------------
	//	Allocating
	//------------------
	 if(!(this->od = createOd(outItemCode)))
		 return 0;
	 if(importData != NULL)
		 this->od->importData(importData);	// when data is non-null , import it!

	 this->TabStop = true;						// default: enable the Tab nevgating , Hsien , 2013.01.25
	 switch(outItemCode){
		 case ODCOIL:	this->Controls->Add(gcnew coilControl(this->od)	);			break;
		 case ODSET:	this->Controls->Add(gcnew setControl(this->od)	);			break;
			//-------
			//	TIMER
			//-------
		 case ODSETTN:	
		 case ODRSTTN:	
			 this->Controls->Add(gcnew nonStopTimerControl(this->od));	
			 break;
			 //------
			 //	ST-BLOCK  , added by Hsien , 2013.01.21
			 //------
		 case ODSTBLOCK:this->Controls->Add(gcnew stBlockControl(this->od));		break;
			 //------
			 //	Ring-Counter , Added by Hsien , 2013.01.22
			 //------
		 case ODRINGCTR_SET:	this->Controls->Add(gcnew counterSetControl(this->od));	break;
		 case ODRINGCTR_COUNTUP:
		 case ODRINGCTR_COUNTDOWN:
			 this->Controls->Add(gcnew counterCountControl(this->od));
			 break;
			 //----------------------
			 //	NC Sync-IO confirm send , implemented on indexer-coil // Hsien 2013/5/22 
			 //----------------------
		 case ODNC_CONFIRM:
			 this->Controls->Add(gcnew indexerCoilControl(this->od));
			 break;
			 //--------------------------------------------------
			 //	Ring Counter Type 2 , much more concised  // Hsien 2013/5/23
			 //--------------------------------------------------
		 case ODRINGCTR_TYPE2_COUNTUP:
		 case ODRINGCTR_TYPE2_COUNTDOWN:
			 this->Controls->Add(gcnew counterCountType2Control(this->od));
		 case NONE:		
			 this->AutoSize			= false	;
			 this->BackgroundImage	= safe_cast<Image^>(res->GetObject(L"NONE_OUTPUT"));
			 this->TabStop = false;	// disable the Tab nevgating , Hsien , 2013.01.25
		 default:
			 break;
	 }//switch()

	 //this->od->getGenStr();		//debugging usage
	 this->currentItemCode = outItemCode;		// memory leakge solved , Hsien , 2012.09.28

	 return 1;
}


int plainOutputControl::store	(DEV_METADATA	**omds)
{
	//------------------------------
	//	fill in meta data by content
	//------------------------------
	if(*omds){
		finalize(*omds);			// try to finalize , if 
		*omds = NULL;
	}
	*omds = initialize();

	if(writeIn(*omds
		,this->od->getType()			// od type
		,this->od->exportData()) != META_SUCCESS)		// content of od data
	{
		return 0;		// write-in fail
	}

	return 1;
}

int plainOutputControl::rebuild	(const DEV_METADATA	*omds)
{
	//---------------------------------
	//	rebuild this plain by meta data
	//---------------------------------
	int				code;
	size_t		dataSize;

	char*			data;

	data = (char*)readOut((DEV_METADATA*)omds
		,code
		,dataSize);		// Hsien , 2013.04.19
	if(!this->deviceEdit(code,data,OD_ON_REBUILD))
		return 0;

	return 1;
}