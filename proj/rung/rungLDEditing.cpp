#include"stdafx.h"

using namespace _ldEditor;

System::Void rungMarkSn(Windows::Forms::TableLayoutPanel^ parentTable)
{
	int	serialNumber = 0;

	for each(rungControl^ e in parentTable->Controls){
		e->rungSn		= (++serialNumber);
//		e->labelRungSN	= 
	}
}

System::Void rungControl::rungInsertAboveClick	(System::Object^  /*sender*/, System::EventArgs^  /*e*/) {
			 //-------------------------------------------------------------
			 //	Preassuming this control has been loaded in tableLayoutPanel
			 //-------------------------------------------------------------
			rungControl^ newRung = gcnew rungControl(this->inpItem
				,this->outItem
				,this->_errHandler);
			 insertControl(this,newRung);

			 newRung->Focus();				// get the focus , for convient to use , Hsien , 2013.01.25
		 }
System::Void rungControl::rungInsertBelowClick	(System::Object^  /*sender*/, System::EventArgs^  /*e*/) {
			 //-------------------------------------------------------------
			 //	Preassuming this control has been loaded in tableLayoutPanel
			 //-------------------------------------------------------------
			 Windows::Forms::TableLayoutPanel^ parentTable = 
				 safe_cast<Windows::Forms::TableLayoutPanel^>(this->Parent);			// cast-back parent into tlp class
			 rungControl^ nextRung
				 = safe_cast<rungControl^>(parentTable->GetControlFromPosition(
				 parentTable->GetPositionFromControl(this).Column
				 ,parentTable->GetPositionFromControl(this).Row + 1));

			 //------------------------------------------------------------------
			 // no reference when this method applied on last row : no next Rung
			 //	so that , could not reference tablePanel
			 //-------------------------------------------------------------------

			 rungControl^ newRung = gcnew rungControl(this->inpItem,this->outItem,this->_errHandler);
			 if(nextRung == nullptr)
				 addControl		(this,newRung);
			 else
				 insertControl	(nextRung,newRung);	
		 }
System::Void rungControl::rungDeleteClick		(System::Object^  sender, System::EventArgs^  e) {
			 //----------------------------------------
			 //	impossible to delete the very last rung
			 //		Hsien , 2012.07.03
			 //----------------------------------------

			//----------------------
			 //	Made Focus on neighbor Rung
			 //---------------------
			// Hsien 2013/5/19
	Windows::Forms::TableLayoutPanel^ parentTable = 
		safe_cast<Windows::Forms::TableLayoutPanel^>(this->Parent);			// cast-back parent into tlp class
	Control^ nextControl = parentTable->GetNextControl(this,true);
	if(nextControl == nullptr){
		nextControl = parentTable->GetNextControl(this,false);
	}
	nextControl->Focus();

		if(this->Parent->Controls->Count > 1){
				 this->Parent->Controls->Remove(this);
			 this->~rungControl();		// destruct , Hsien ,2012.10.1
		}
}

