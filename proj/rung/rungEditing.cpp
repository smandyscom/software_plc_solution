#include"stdafx.h"

using namespace _ldEditor;

System::Void rungControl::addRowClick		(System::Object^  /*sender*/, System::EventArgs^  /*e*/) {
				 this->tlpRung->SuspendLayout();
				 this->addRow(FOLLOWER);				// initial case
				 this->tlpRung->ResumeLayout();
}

System::Void rungControl::deleteRowClick	(System::Object^  /*sender*/, System::EventArgs^  /*e*/) {
				 this->tlpRung->SuspendLayout();
				 this->deleteRow();
				 this->tlpRung->ResumeLayout();				 
		 }
System::Void rungControl::attachRightClick	(System::Object^  /*sender*/, System::Windows::Forms::ControlEventArgs^  e) {
			 //------------------------------------------------------
			 //	Control-added event handler , attach routines for every child-control
			 //-----------------------------------------------------
	e->Control->ContextMenuStrip = this->cmsRung;
		 }
System::Void rungControl::clearRowClick		(System::Object^  /*sender*/, System::EventArgs^  /*e*/) {
			 this->tlpRung->SuspendLayout();
			 this->reset();
			 this->tlpRung->ResumeLayout();
		 }


int rungControl::addRow(int isFirstRow)
{
	//--------------------------------------
	//	1st		column: E_VERTICAL
	//	2nd-Nth	column:	Editable Plain Input
	//	N+1th	column:	E_NOTHING
	//	last	column: Plain Output
	//--------------------------------------
	plainInputControl^	handle;
	plainOutputControl^	handleOc;
	PictureBox^			pb;

	if(isFirstRow)
		tlpRung->Controls->Add(gcnew plainInputControl(NULL, constPlainCodes::ST_HEAD),-1,-1);
	else
		tlpRung->Controls->Add(gcnew plainInputControl(NULL, constPlainCodes::E_VERTICAL),-1,-1);	// automatic layout

	//--------------------------
	//	add editable plain input
	//  [7/27/2012 smandyscom]
	//--------------------------
	for(int i=0;i < innerInpNum;i++){
		if (isFirstRow){
			handle = gcnew plainInputControl(inpItem, constPlainCodes::NULL_CODE);
			handle->deviceEdit( VERTICE,__SHORT,NULL,0x01/* casual facility , Hsien , 2013.01.22*/);
			tlpRung->Controls->Add(handle,-1,-1);
		} 
		else{
			this->tlpRung->Controls->Add(gcnew plainInputControl(this->inpItem, constPlainCodes::NULL_CODE),-1,-1);
		}
	}//for

	//----------------
	//	Add the BRIDGE
	//----------------
	if (isFirstRow){			
		tlpRung->Controls->Add(gcnew plainInputControl(NULL, constPlainCodes::ST_TAIL),-1,-1);
	} 
	else{	
		tlpRung->Controls->Add(gcnew plainInputControl(NULL, constPlainCodes::E_NOTHING),-1,-1);
	}

	//-------------------------
	// add plain output control
	//-------------------------
	handleOc = gcnew plainOutputControl(outItem);
	tlpRung->Controls->Add(handleOc,-1,-1);

	//------------------------------
	//	Valid Setting of Picture Box
	//  [7/28/2012 smandyscom]
	//------------------------------
	pb				= gcnew PictureBox();
	System::Resources::ResXResourceSet ^_ldResImages	
		= gcnew System::Resources::ResXResourceSet(this->assemblyPath + "\\ldeImages.resx");	// Hsien ,2013.03.21
																							// Hsien 2013/4/29	
	tlpRung->Controls->Add(pb,-1,-1);
	pb->Margin	= Windows::Forms::Padding(0);
	pb->Dock	= DockStyle::Fill;
	pb->BackgroundImage			= safe_cast<Image^>(_ldResImages->GetObject("END_VERTICAL"));

	rowCount++;			// count

	//------------------
	//	Layout Adjesting
	//------------------
	this->bridgeAdjest();

	//----------------------
	//	Check Out if row counts is over the limit of adjecent matrix
	//		If it was , lock the addRow() function
	// Hsien 2013/5/27
	//----------------------
	if(rowCount >= (ADJMAT_SIZE/(this->innerInpNum+2 /* total amount of input device */)))
		this->addRowToolStripMenuItem->Enabled = false;		// Hsien 2013/5/27 , lock the add one row to prevent error
	
	return 1;
}

int rungControl::deleteRow()
{
	Windows::Forms::Control^	controlHandle;
	//--------------------------------
	//	DELETE entire row : N controls
	//--------------------------------
	if(tlpRung->Controls->Count > tlpRung->ColumnCount){					// prevent out-index accessing , Hsien , 2012.05,24
		for (int i=0;i<tlpRung->ColumnCount;i++){
			controlHandle = tlpRung->Controls[tlpRung->Controls->Count-1];
			tlpRung->Controls->RemoveAt(tlpRung->Controls->Count-1);		// remove last-index control N times
			if(controlHandle->GetType() == plainInputControl::typeid)
				safe_cast<plainInputControl^>(controlHandle)->~plainInputControl();		// destructor-call
			if(controlHandle->GetType() == plainOutputControl::typeid)
				safe_cast<plainOutputControl^>(controlHandle)->~plainOutputControl();	// destructor-call
			//-----
			//	Got it ! eliminate memory leakage problem by EXPLICIT DESTRUCT CALL
			//		Hsien , 2012.09.29
			//-----
		}
	}
	rowCount--;

	//-----------------
	//	Layout Adjesting
	//-----------------
	this->bridgeAdjest();

	//----------------------
	//	Check Out if row counts is over the limit of adjecent matrix
	//		If it was , lock the addRow() function
	// Hsien 2013/5/27
	//----------------------
	if(rowCount < (ADJMAT_SIZE/(this->innerInpNum+2 /* total amount of input device */)))
		this->addRowToolStripMenuItem->Enabled = true;		// Hsien 2013/5/27 , lock the add one row to prevent error

	return 1;
}

int	rungControl::reset()
{
	//------------------------------------------
	//	reset rung status as like rung beginning
	//	1. reset	comments
	//	2. remove		controls
	//			a. duplicate tlpRung's control collection to tmp array
	//			b. clear tlpRung
	//			c. destructor things in tmp array
	//	3. initializing first row.
	//------------------------------------------
	array<Windows::Forms::Control^>^	handle = 
		gcnew array<Windows::Forms::Control^>(this->tlpRung->Controls->Count);

	tlpRung->Controls->CopyTo(handle,0);	// copy content from tlpRung to array
	tlpRung->Controls->Clear();			// clear tlpRung
	for each(Windows::Forms::Control^ e in handle){
		//-------------
		//	Destruct em
		//-------------
		if(e->GetType() == plainInputControl::typeid)
			safe_cast<plainInputControl^>(e)->~plainInputControl();
		if(e->GetType() == plainOutputControl::typeid)
			safe_cast<plainOutputControl^>(e)->~plainOutputControl();
	}
	//this->BeginInvoke(this->Load);

	rowCount = 0;					// set 0
	tb_Comments->Text = nullptr;	// clear comments
	addRow(INITIAL);				// initialize new row

	return 1;
}

int	rungControl::bridgeAdjest()
{
	//---------------------------
	//	Used to Change the Images , to make representation much better
	//		Hsien , 2013.01.25
	//---------------------------
	Windows::Forms::Control^ hdl;
	System::Resources::ResXResourceSet ^_ldResImages	
		= gcnew System::Resources::ResXResourceSet(this->assemblyPath + "\\ldeImages.resx");
	// Hsien , 2013.03.21
	// Hsien 2013/4/29
	if(rowCount > 1){
		//-------------------
		//	First Row Adjesting
		//-------------------
		hdl = this->tlpRung->GetControlFromPosition(this->tlpRung->ColumnCount-3,0);
		hdl->BackgroundImage = safe_cast<Image^>(_ldResImages->GetObject("OUT_BRANCH1"));
		//-------------------
		//	Last Row Adjesting
		//-------------------
		hdl = this->tlpRung->GetControlFromPosition(this->tlpRung->ColumnCount-3,rowCount-1);
		hdl->BackgroundImage = safe_cast<Image^>(_ldResImages->GetObject("OUT_BRANCH3"));
		//-------------------
		//	Middle Row Adjesting
		//-------------------
		for(int i=1;i<rowCount-1;i++){
			hdl = this->tlpRung->GetControlFromPosition(this->tlpRung->ColumnCount-3,i);
			hdl->BackgroundImage = safe_cast<Image^>(_ldResImages->GetObject("OUT_BRANCH2"));
		}
	}

	return 1;
}