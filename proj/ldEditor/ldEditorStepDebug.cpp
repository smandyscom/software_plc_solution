#include "StdAfx.h"
#include "ldEditor.h"

using namespace _ldEditor;

System::Void ldEditor::rungSnMark(System::Object^  /*sender*/, System::Windows::Forms::ControlEventArgs^  /*e*/) {
		 //--------------------------------------------------
		 //	Marking Serial-Number to Rung , by the sequence of row
		 //--------------------------------------------------
		 for each(rungControl^ e in this->tlpRung->Controls){
			 e->markSn(this->tlpRung->GetPositionFromControl(e).Row);
		 }
}

//System::Void ldEditor::tsbStepClick(System::Object ^sender, System::EventArgs ^e)
//{
//	SIMU_DATA_INFORMATION	localInfo;
//	
//	//---------------
//	//	If not on debugging mode:
//	//---------------
//	switch(this->status){
//		case LDE_STATUS_EDITTING:
//			this->ircGenerate(sender,e);			// trigger compiling process and load-in simulator
//			if(this->compileResult == LDE_SUCCESS){
//				this->status = LDE_STATUS_DEBUGGING;
//				this->tsbStepClick(sender,nullptr);
//			}
//			break;
//		case LDE_STATUS_DEBUGGING:	
//			this->simulatorControl->singalStep(&localInfo);
//			for each(rungControl^ e in this->tlpRung->Controls)
//				e->isMe(localInfo.simuDebugInfo);
//		default:
//			break;
//	}
//}
//
//System::Void ldEditor::tsbEndDebugClick(System::Object ^sender, System::EventArgs ^e)
//{
//	//---------------------------------------
//	//	Terminate the Debugging Process
//	//---------------------------------------
//	if(this->status == LDE_STATUS_DEBUGGING){
//		this->simulatorControl->terminate();
//		for each(rungControl^ e in this->tlpRung->Controls)
//			e->isMe(-1);		// turns all color gray
//		this->status = LDE_STATUS_EDITTING;
//	}
//}