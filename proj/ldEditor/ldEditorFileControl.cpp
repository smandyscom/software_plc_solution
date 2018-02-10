#include"stdafx.h"
#include "ldEditor.h"

using namespace _ldEditor;

//#define	LDE_SUCCESS_CANCELED	1
//#define	LDE_SUCCESS				0
//#define	LDE_ERROR			-1

void ldEditor::ldeFileDragIn(System::Object ^sender, System::Windows::Forms::DragEventArgs ^e)
{	//----------------------
	//	Implement of File Drag in and open mechanism
	//		1. setup main form property: AllowDrop to be true
	//		2. establish this event handler when dragEnter event comes
	//		3. translate DragEventArgs into System::Array , which stored FIleName String ( static cast only)
	//----------------------
	// Hsien 2013/11/15
	System::Array^ info
		= static_cast<System::Array^>(e->Data->GetData(Windows::Forms::DataFormats::FileDrop));
	this->load(safe_cast<String^>(info->GetValue(0)));
}

void ldEditor::ldeFileDragEnter(System::Object ^sender, System::Windows::Forms::DragEventArgs ^e)
{
	//----------------------
	//	Setup drag effect ( this drapDrog will filter out dragin event if Effect doesnt setup
	//----------------------
	// Hsien 2013/11/15
	e->Effect = System::Windows::Forms::DragDropEffects::All;
}

void ldEditor::newLD	(Windows::Forms::TableLayoutPanel^ parentTlp)
{
	array<rungControl^>^	tmpArray
		= gcnew	array<rungControl^>(parentTlp->Controls->Count);

	parentTlp->SuspendLayout();
	//----------------------------------------
	//	Create a new rung when this form loaded
	//----------------------------------------
	parentTlp->Controls->CopyTo(tmpArray,0);
	parentTlp->Controls->Clear();
	for each(rungControl^ e in tmpArray)	// call destructor iteratedlly
		e->~rungControl();

	_ldEditor::rungControl	^rungHdl = gcnew /*rung::*/rungControl(this->selectedInpItem,this->selectedOutItem,this->_errorHdl);
//	rungHdl->addRow(1);		// Hsien 2013/11/15
	parentTlp->Controls->Add(
		rungHdl
//		gcnew /*rung::*/rungControl(this->selectedInpItem,this->selectedOutItem,this->_errorHdl)
		,-1
		,-1);

	parentTlp->ResumeLayout();
}
int ldEditor::store	(CLST_METADATA		**lmdsContainer)
{
	//---------------------------
	//	storing info to meta data
	//		1. write in header info
	//		2. allocating memory
	//		3. soft-link to child rungs
	//		4. duplicate all data from soft-link one to real-one, whose data were allocated in local memory(this form)
	//			Done , Works Well , Hsien , 2012.09.27
	//---------------------------
	std::string		tmpString;
	CLST_METADATA	*tmpRmdsData = 0;
	size_t			dataIndex;		// last-write-in index

	//------------------------------
	//	release last lmds before store new lmds
	//------------------------------
	if(*lmdsContainer){
		cluster::finalize(*lmdsContainer);
		*lmdsContainer = NULL;
	}

	//*lmdsContainer = cluster::initialize(
	//	(CP_CONSTRUCTOR)cluster::duplicate
	//	,(DESTRUCTOR)cluster::finalize
	//	,this->tlpRung->Controls->Count);		// number of rungs
	*lmdsContainer = clstCreate(
		(DUPLICATOR)cluster::duplicate
		,(DESTRUCTOR)cluster::finalize
		,(FILEWRITER)cluster::fWrite
		,(FILEREADER)cluster::fRead
		,tlpRung->Controls->Count);				// updataed , Hsien, 2012.10.3

	//------------
	//	Head Info
	//------------
	tmpString.clear();
	if(device::writeIn((*lmdsContainer)->headData
		,this->tlpRung->Controls->Count				// item code
		,tmpString.c_str()) != META_SUCCESS){
			//------------------
			//	Write-in failure
			//------------------
			return 0;
	}						// data

	//-----------
	//	Rungs' Info
	//-----------
	dataIndex = -1;
	for each(rungControl^ e in this->tlpRung->Controls){
		if(!e->store(&tmpRmdsData)){
				//-----------------------------------
				//	Store Process Fail , do finalize
				//-----------------------------------
			if(tmpRmdsData != NULL)
				cluster::finalize(tmpRmdsData);
			return 0;
		}
		dataIndex++;
		cluster::writeIn(*lmdsContainer
			,dataIndex
			,cluster::duplicate(tmpRmdsData));
	}

	cluster::finalize(tmpRmdsData);
	return 1;
}

void ldEditor::rebuild	(const CLST_METADATA*	lmds)
{
	//------------------------------------------------------
	//	rebuild info from meta data
	//		1. adjest rungs ,according to indication of lmds
	//		3. data link , and do rebuilding operation
	//		4. then call child's rebuiding function
	//			Done , Works well , Hsien , 2012.09.27
	//------------------------------------------------------
	rungControl^	rungHandle;
	int			_rungCount		;
	size_t		ldCommentSize	;	// length of comments
									// Hsien , 2013.04.19
	size_t dataIndex;

	device::readOut(lmds->headData,_rungCount,ldCommentSize);	// rebuilding according to header data

	//--------------------------
	//	rung adjesting mechanism
	//	performance would enhanced in expect.
	//	Hsien , 2012.09.30
	//--------------------------
	this->tspbLoadingLmds->Value = 0;		// Hsien , 2013.04.19
	this->tlpRung->SuspendLayout();

	while(_rungCount != this->tlpRung->Controls->Count){
		if(this->tlpRung->Controls->Count < _rungCount){
			//--------------------------------------------------
			//	if current rungs is less than required , creating
			//---------------------------------------------------
			rungControl^ hdl = gcnew rungControl(this->selectedInpItem,this->selectedOutItem,this->_errorHdl);
			//hdl->addRow(1);					// added initial row , Hsien 2013/11/15 
			this->tlpRung->Controls->Add(hdl
				,-1
				,-1);		// added this->errorStack , Hsien , 2012.10.12
//			hdl->_ldResImages = this->_ldResImages;
		}
		else if(this->tlpRung->Controls->Count > _rungCount){
			//---------------------------------------------------
			//	if current rungs is greater than required , remove and destruct
			//---------------------------------------------------
			rungHandle = safe_cast<rungControl^>(this->tlpRung->Controls[this->tlpRung->Controls->Count-1]);
			this->tlpRung->Controls->RemoveAt(this->tlpRung->Controls->Count-1);
			rungHandle->~rungControl();
		}
	}

	//-------------
	//	Rebuilding Each Rung
	//-------------
	this->tspbLoadingLmds->Maximum = _rungCount;							// recovered by Hsien , 2013.01.25
	dataIndex = 0;
	for each(rungControl^ e in this->tlpRung->Controls){
		e->rebuild((CLST_METADATA*)cluster::readOut(lmds,dataIndex));
		dataIndex++;
		this->tspbLoadingLmds->PerformStep();								// recovered by Hsien , 2013.01.25
	}

	this->tlpRung->ResumeLayout();
}

//int ldEditor::lmdsFileSave(FILE*	outFp)
//{
//	//----------------------------------
//	//	Presumpation : store() had been excuted , so that , within a 
//	//----------------------------------
//	return 1;
//}
//
//int ldEditor::lmdsFileLoad(FILE*	inpFp)
//{
//	return 1;
//}
//
int	ldEditor::save(String^ saveFileName/*in*/	/*,String^ resultOutput*//* used to redirecting the place to output message*/)
{
	//----------------------
	//	Bound As Save into this->CurrentFile
	//----------------------
	// Hsien 2013/5/20
	std::string		fileName;					
	FILE*			destFile = NULL;
	CLST_METADATA	*lmds = NULL;

	if(saveFileName == nullptr)
		return LDE_ERROR/*LDE_ERROR*/;

	//----------------------
	//	currentFileName existed: file had been opened or saved
	//----------------------
	destFile = fopenString(saveFileName,"wb+");
	if(destFile == NULL)
		return LDE_ERROR/*LDE_ERROR*/;

	//-----------------
	//	call store function to store current status of LDE
	//-----------------
	if(!this->store(&lmds)){
		//-----------------
		//	Store Process Failure , DO NOT DO FURTHER FILE-IO PROCESS
		//-----------------
		//this->tsslStatus->Text		= this->_ldResMessage->GetString("lmdsSaveError");
		this->postOnOutput(this->_ldResMessage->GetString("lmdsSaveError"),L"Error");
	}
	else{
		//-----------------
		//	write into FILE
		//-----------------
		cluster::fWrite(lmds,destFile);
		//this->tsslStatus->Text		= this->_ldResMessage->GetString("lmdsSaveSuccess");	// Hsien , 2013.04.15
		this->postOnOutput(this->_ldResMessage->GetString("lmdsSaveSuccess"),L"Message");

	}
	//-----------------
	//	release resource 
	//-----------------
	cluster::finalize(lmds);
	fclose(destFile);

	return LDE_SUCCESS;
}

int	ldEditor::openDialogGetFilename(OpenFileDialog^	dialog, const int fileIndex)
{
	//----------------------
	//	Request File name by opening dialog
	// Hsien 2013/5/20
	//----------------------
	dialog->FilterIndex = /*1*/fileIndex;
	dialog->FileName = nullptr;

	if(dialog->ShowDialog() 
		== System::Windows::Forms::DialogResult::OK){
			this->currentFileNames[fileIndex]	= dialog->FileName;	// Hsien 2013/5/24

		return LDE_SUCCESS;	// successed
	}

	return LDE_ERROR;		// cancelled
}

int	ldEditor::saveDialogGetFilename(SaveFileDialog^	dialog, const int fileIndex)
{
	//----------------------
	//	Request File name by opening dialog
	// Hsien 2013/5/20
	//----------------------
	dialog->FilterIndex = /*1*/fileIndex;
	dialog->FileName = nullptr;

	if(dialog->ShowDialog() 
		== System::Windows::Forms::DialogResult::OK){
			this->currentFileNames[fileIndex]	= dialog->FileName;	// Hsien 2013/5/24

		return LDE_SUCCESS;	// successed
	}

	return LDE_ERROR;		// cancelled
}


int	ldEditor::load(String^			openFileName/*out*/	/*,String^ resultOutput*//* used to redirecting the place to output message*/)
{
	FILE*			sourceFile = NULL;
	CLST_METADATA	*lmds = NULL;

	this->postOnOutput(this->_ldResMessage->GetString("lmdsLoading"),L"Message");

	//-----------------
	//	read from FILE
	//-----------------	
	if(openFileName == nullptr){
	this->postOnOutput(this->_ldResMessage->GetString("lmdsOpenError"),L"Error");
		return LDE_ERROR;
	}

	//this->tbOutput->Text = System::DateTime::Now.ToString();

	sourceFile = fopenString(openFileName,"rb");
	if(sourceFile == NULL)
		return LDE_ERROR;

	if(cluster::fRead(&lmds,sourceFile) == NULL){
		//----------------------
		//	aborting reading in rb mode , this file is possibly writing in r mode  
		//	, insteadly , reading in r mode
		//----------------------
		cluster::finalize(lmds);
		lmds = NULL;			//	otherwise , error will occurs when re-creating process
								// Hsien 2013/4/28
		fclose(sourceFile);
		
		//----------------------
		//	Of course  ,there's still some broken file writing in r mode
		//		but no way to fix it, ignore the failure information
		//----------------------
		sourceFile = fopenString(openFileName,"r");
		if(sourceFile == NULL)
			return LDE_ERROR;
		
		if(cluster::fRead(&lmds,sourceFile) == NULL)
			return LDE_ERROR;		// Hsien 2013/5/20 can't opened with r , rb , serious brokened file
	}

	//-----------------
	//	call rebuidd function to store current status of LDE
	//-----------------
	this->rebuild(lmds);
	//-----------------
	//	release resource 
	//-----------------
	cluster::finalize(lmds);
	fclose(sourceFile);

	//this->tsslStatus->Text 
	//	= this->_ldResMessage->GetString("lmdsLoaded");
	this->postOnOutput(this->_ldResMessage->GetString("lmdsLoaded"),L"Message");

	//----------------------
	//	Updating Banner // Hsien 2013/11/15
	//----------------------
	this->Text = openFileName;						// Hsien 2013/12/3 , to renew header
	this->currentFileNames[LDE_FILE_INDEX_LMDS] = openFileName;/*currentFileName*/;
	this->currentFileNames[LDE_FILE_INDEX_IRC]	= nullptr;	// Hsien 2013/5/24 , reset IRC filename


	return LDE_SUCCESS;
}


//System::Void ldEditor::tsbNewClick(System::Object^  /*sender*/, System::EventArgs^  /*e*/) {
//	this->newLD(this->tlpRung);
//}
//System::Void ldEditor::tsbSaveAsClick	(System::Object^  /*sender*/, System::EventArgs^  /*e*/) {
//	std::string		fileName;					
//	FILE*			destFile = NULL;
//	CLST_METADATA	*lmds = NULL;
//
//
//	//----------------------
//	//	Open File then Pass File name via class-wide variable for tsbSaveClick()
//	// Hsien 2013/5/20
//	//----------------------
//	this->saveFileDialog->FilterIndex = 1;
//	this->saveFileDialog->FileName = nullptr;
//	if(this->saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK){
//		this->Text				= this->saveFileDialog->FileName;
//		this->currentFileName	= this->saveFileDialog->FileName;
//
//		this->tsbSaveClick(nullptr,nullptr);
//	}
//}
//System::Void ldEditor::tsbSaveClick		(System::Object^  /*sender*/, System::EventArgs^  /*e*/) {
//	//----------------------
//	//	Bound As Save into this->CurrentFile
//	//----------------------
//	// Hsien 2013/5/20
//	std::string		fileName;					
//	FILE*			destFile = NULL;
//	CLST_METADATA	*lmds = NULL;
//
//	//this->saveFileDialog->FilterIndex = 1;
//	//this->saveFileDialog->FileName = nullptr;
//	//if(this->saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK){
//	//	this->Text = this->saveFileDialog->FileName;
//	//	MarshalString(this->saveFileDialog->FileName,fileName);	// turns System String into standard string
//	//	destFile = fopen(fileName.c_str(),"wb+");					// w+ , or w? , questioned by Hsien , 2013.01.14
//	//// edited as wb+ mode , worked , Hsien , 2013.04.22
//	if(this->currentFileName != nullptr){
//		//----------------------
//		//	currentFileName existed: file had been opened or saved
//		//----------------------
//		destFile = fopenString(currentFileName,"wb+");
//	}
//	else{
//		//----------------------
//		//	currentFileName inexisted: new file saving
//		//----------------------
//		this->tsbSaveAsClick(nullptr,nullptr);
//	}
//
//	if(destFile != NULL){
//		//-----------------
//		//	call store function to store current status of LDE
//		//-----------------
//		if(!this->store(&lmds)){
//			//-----------------
//			//	Store Process Failure , DO NOT DO FURTHER FILE-IO PROCESS
//			//-----------------
//			this->tsslStatus->Text		= this->_ldResMessage->GetString("lmdsSaveError");
//		}
//		else{
//			//-----------------
//			//	write into FILE
//			//-----------------
//			cluster::fWrite(lmds,destFile);
//			this->tsslStatus->Text		= this->_ldResMessage->GetString("lmdsSaveSuccess");	// Hsien , 2013.04.15
//		}
//		//-----------------
//		//	release resource 
//		//-----------------
//		cluster::finalize(lmds);
//		fclose(destFile);
//	}
//	/*}*///if
// }
//System::Void ldEditor::tsbLoadClick		(System::Object^  /*sender*/, System::EventArgs^  /*e*/) {
//	std::string		fileName;					
//	FILE*			sourceFile = NULL;
//	CLST_METADATA	*lmds = NULL;
//	int				errorCode = 0;		// 0 : SUCCESS
//										// non-zero : Error ,  Hsien 2013/4/29
//			 
//	this->openFileDialog->FilterIndex = 1;
//	this->openFileDialog->FileName = nullptr;
//	
//	if(this->openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK){
//		this->Text				= this->openFileDialog->FileName;
//		this->currentFileName	= this->openFileDialog->FileName;	// Hsien 2013/5/20 , focusing on specific file
//																	// for auto saving
//
//		MarshalString(this->openFileDialog->FileName,fileName);	// turns System String into standard string
//
//		this->tsslStatus->Text = this->_ldResMessage->GetString("lmdsLoading");
//		//-----------------
//		//	read from FILE
//		//-----------------		
//		sourceFile = fopen(fileName.c_str(),"rb");
//		if(cluster::fRead(&lmds,sourceFile) == NULL){
//			//----------------------
//			//	aborting reading in rb mode , this file is possibly writing in r mode  
//			//	, insteadly , reading in r mode
//			//----------------------
//			cluster::finalize(lmds);
//			lmds = NULL;			//	otherwise , error will occurs when re-creating process
//									// Hsien 2013/4/28
//			fclose(sourceFile);
//			
//			//----------------------
//			//	Of course  ,there's still some broken file writing in r mode
//			//		but no way to fix it, ignore the failure information
//			//----------------------
//			sourceFile = fopen(fileName.c_str(),"r");
//			cluster::fRead(&lmds,sourceFile);
//		};
//		//-----------------
//		//	call store function to store current status of LDE
//		//-----------------
//		this->rebuild(lmds);
//		//-----------------
//		//	release resource 
//		//-----------------
// 		cluster::finalize(lmds);
//		fclose(sourceFile);
//
//		this->tsslStatus->Text = this->_ldResMessage->GetString("lmdsLoaded");
//	}//if
//}
//
System::Void ldEditor::tsbFileClick	(System::Object^  sender, System::EventArgs^  /*e*/)
{
	//----------------------
	//	Indicating the data , preseted in Tag properties , used to distinguish items
	//----------------------
#define	TAG_NEW		0
#define TAG_LOAD	1
#define	TAG_SAVE	2
#define	TAG_SAVEAS	3

	// Hsien 2013/5/20 , common handler for new,save,saveAs,load
	int	tagIndex = 0 ;
	Int32::TryParse(
		safe_cast<System::Windows::Forms::ToolStripButton^>(sender)->Tag->ToString(),tagIndex);

	//----------------------
	//	diffent handling flow according to the sender
	//----------------------
	switch(tagIndex){
		case TAG_NEW:
			this->newLD(this->tlpRung);
			this->currentFileNames[LDE_FILE_INDEX_LMDS] = nullptr;	// Hsien 2013/5/20 , reset the focus file
			break;
		case TAG_LOAD:
			//----------------------
			//	1. Get File name via OpenFileDialog
			//	2. starting load and rebuild procedure , if previous move did successfully
			//----------------------
			if(this->openDialogGetFilename(this->openFileDialog,LDE_FILE_INDEX_LMDS) == LDE_SUCCESS)
				this->load(this->currentFileNames[LDE_FILE_INDEX_LMDS]/*currentFileName*//*,this->tsslStatus->Text*/);
			//this->Text = this->currentFileNames[LDE_FILE_INDEX_LMDS]/*currentFileName*/;
			//this->currentFileNames[LDE_FILE_INDEX_IRC] = nullptr;	// Hsien 2013/5/24 , reset IRC filename
			// Hsien 2013/11/15 , moving into load()
			break;
		case TAG_SAVE:
			//----------------------
			//	1. Try to Save
			//	2. IF Saving Failure , Call Save As
			//----------------------
			if(this->save(this->currentFileNames[LDE_FILE_INDEX_LMDS]/*currentFileName*/) != LDE_SUCCESS){
				this->saveDialogGetFilename(this->saveFileDialog,LDE_FILE_INDEX_LMDS);	// Hsien 2013/5/24
				this->save(this->currentFileNames[LDE_FILE_INDEX_LMDS]/*currentFileName*/);
				this->Text = this->currentFileNames[LDE_FILE_INDEX_LMDS]/*currentFileName*/;
			}
			break;
		case TAG_SAVEAS:
			//----------------------
			//	1. Open File Dialog
			//	2. Saving
			//----------------------
			this->saveDialogGetFilename	(this->saveFileDialog,LDE_FILE_INDEX_LMDS);	// Hsien 2013/5/24
			this->save			(this->currentFileNames[LDE_FILE_INDEX_LMDS]/*currentFileName*/);
			this->Text		= this->currentFileNames[LDE_FILE_INDEX_LMDS]/*currentFileName*/;
			break;
		default:
			break;
	}//switch
}
