//---------------------------------------------------------------------------------------
//	Generic Library offered the methods manipulating controls in TableLayoutPanel: 
//											insert(add a new item in the middle of array) 
//											, add(add a new item in the tail of array
//											, delete(delete item from any position)
//	Hsien , 2012.09.26
//---------------------------------------------------------------------------------------

using namespace System;
using namespace System::Windows::Forms;

void insertControl(Windows::Forms::Control^ ref
				   ,Windows::Forms::Control^ insert)
{
	//---------------------------------------------------------
	//	ref		:	the reference control to locate position
	//	insert	:	the new device which is going to be inserted
	//	the control going to insert have to be initialize outer
	//---------------------------------------------------------

	int insertIndex , columnIndex;
	Collections::Queue fifo;
	Windows::Forms::TableLayoutPanel^ tblHandle 
		= safe_cast<Windows::Forms::TableLayoutPanel^>(ref->Parent);
	Windows::Forms::TableLayoutPanelCellPosition pos;

	//-------------------------------------------------------------------
	//	TableLayoutPanel::GetRow Method	From MSDN : 
	//	The row position of control
	//	, or -1 if the position of control is determined by LayoutEngine.
	//	so that it is invalid when layoutEngine activated.
	//-------------------------------------------------------------------
	insertIndex			= (tblHandle->GetPositionFromControl(ref)).Row;
	columnIndex			= (tblHandle->GetPositionFromControl(ref)).Column;

	tblHandle->SuspendLayout();
	
	//-------------------------------------------------------------------------
	//	Move Controls into temp FIFO , then remove them from table-panel-layout
	//-------------------------------------------------------------------------
	 while(tblHandle->GetControlFromPosition(columnIndex,insertIndex) != nullptr){
		 fifo.Enqueue(tblHandle->GetControlFromPosition(columnIndex,insertIndex));
		 tblHandle->Controls->Remove(tblHandle->GetControlFromPosition(columnIndex,insertIndex));
	 }

	 //--------------------------
	 //	added new device
	 //--------------------------
	 tblHandle->Controls->Add(insert,columnIndex,-1);

	 //------------------------
	 // recover old devices
	 //------------------------
	 while(fifo.Count != 0){
		 tblHandle->Controls->Add(
			 safe_cast<Control^>(fifo.Dequeue()),columnIndex,-1);
	 }

	 tblHandle->ResumeLayout();

}


void addControl		(Windows::Forms::Control^ ref
					 ,Windows::Forms::Control^ newControl)
{
	//---------------------------------------------------------
	//	adding Control in the tail of array	(table panel layout
	//---------------------------------------------------------
	Windows::Forms::TableLayoutPanel^ tblHandle
		= safe_cast<Windows::Forms::TableLayoutPanel^>(ref->Parent);

	tblHandle->Controls->Add(
		newControl
		,tblHandle->GetPositionFromControl(ref).Column
		,-1);
}
void deleteControl	(Windows::Forms::Control^ ref)
{
	//-------------------------------------
	//	ref : the device is going to delete
	//-------------------------------------
		 int myRowIndex;
		 System::Collections::Queue fifo;				 
		 Windows::Forms::TableLayoutPanel^ tblHandle
			 = safe_cast<Windows::Forms::TableLayoutPanel^>(ref->Parent);

		 tblHandle->SuspendLayout();

		 myRowIndex = tblHandle->GetRow(ref);
		 //-------------------------------------------------------------------------
		 //	remove and enqueue controls  , which are starting next from ref Control
		 //-------------------------------------------------------------------------
		 for(int i = myRowIndex+1; i < tblHandle->RowCount ;i++){
			 fifo.Enqueue(tblHandle->GetControlFromPosition(0,i));
			 tblHandle->Controls->Remove(tblHandle->GetControlFromPosition(0,i));
		 }

		 //--------------------------
		 //	delete ref device
		 //--------------------------
		 tblHandle->Controls->Remove(ref);
		 tblHandle->RowCount--;

		 //------------------------
		 // recover old devices
		 //------------------------
		 int i=0;
		 while(fifo.Count != 0){
			 tblHandle->Controls->Add(
				 safe_cast<Control^>(fifo.Dequeue()),0,myRowIndex+i);
			 i++;
		 }


		 tblHandle->ResumeLayout();

}