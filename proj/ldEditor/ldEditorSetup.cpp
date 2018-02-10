#include "StdAfx.h"
#include "ldEditor.h"
#include "ldEditorSetup.h"

using namespace _ldEditor;

System::Void ldEditor::setupClick	(System::Object^  /*sender*/, System::EventArgs^  /*e*/) {
			 std::string s;
			 
			 if(this->setupForm->ShowDialog()
				 == Windows::Forms::DialogResult::OK){
					 //---------
					 //	1. unload current data base
					 // 2. string conversion
					 //	2. load new data base
					 //	3. check and set status
					 //---------

					 if(this->dataBaseStatus)
						 stcClose();				// unload data base

					 MarshalString(this->setupForm->tbDataBase->Text,s);
					 if(stcInit(*this->_db,this->_errorHdl) == STC_SUCCESS){
						 //-------
						 //	SUCESS
						 //--------
						 this->dataBaseStatus	= 1;
//						 this->tsslStatus->Text		= "Data Base Loaded.";
					 }
					 else{
						 //-------
						 // FAIL
						 //-------
						 this->dataBaseStatus	= 0;
//						 this->tsslStatus->Text		= "Data Base Missing";
					 }

					 this->saveFileDialog->InitialDirectory = this->setupForm->tbHomePath->Text;
					 this->openFileDialog->InitialDirectory = this->setupForm->tbHomePath->Text;
			 }
		 }