#include"stdafx.h"
#include"ldEditor.h"

using namespace _ldEditor;

int fileCopy(FILE*	source,FILE* dest)
{
	//-----------------------
	//	Native File Copy Utility , Not Tested , Hsien , 2013.01.08
	//-----------------------

	//-----------------------
	//	Make the file indicator to the begin of FILE
	//-----------------------
	fseek(source,0,SEEK_SET);
	fseek(dest,0,SEEK_SET);
	//-----------------------
	//
	//-----------------------
	while(!feof(source))
		fputc(fgetc(source),dest);

	return 1;
}

int	errorStackOutput(Windows::Forms::TextBox^ e
					 ,STC_ERRHDL*	__errorHdl
					 ,String^	_format
					 ,const int rungSn)
{
	//------------------------------------------------------------
	//	There's semetic/connection error happened, do Error output
	//		Would Use Windows:Format , to Modify Output Formattion , Hsien , 2013.01.08
	//	Presumption: Error Stack Handler/Container should existed some where else
	//------------------------------------------------------------	
	STC_ERRBLK			errorBlock;
	int					errorCount = 0;

	while(stcErrorRead(__errorHdl,&errorBlock) != STC_NO_MORE_ERROR){
		errorCount++;
		e->Text += 
			String::Format(_format
			,errorCount
			,rungSn
			,errorBlock.errCode
			,gcnew String(errorBlock.errContent)
			,gcnew String(errorBlock.errExplain));	// Append Mode	
		e->Text += "\r\n";
	 }

	return 1;
}

int	compileSingleRung(_ldEditor::rungControl^ e/*input*/
					  ,STC_ERRHDL*	__errorHdl
					  ,FILE*	ircBuffer/*output*/
					  ,const int arg)
{
	//-------------------------------------------
	//	The Compiling Function Turns Rung into Segment of IRC-Instructions
	//		Hsien , 2013.01.08
	//-------------------------------------------
	std::string		stdString;
	FILE*			fileBuffer;
	IRC_UNIT		debugAssertion;

	stcErrorReset(__errorHdl);	// reset the error stack

	//--------------------------
	//	Stage 1 : ST generating
	//--------------------------
	if(e->exprGenerate() != STG_SUCCESS){
		//----------------
		//	Error Handling : the Error Block had been write into Error-Stack
		//----------------
		return LDE_ERROR;		// connection error
	}

	 //------------------------------------
	 //	Stage 2: ST compiling/ IRC generating
	 //------------------------------------	
	 MarshalString(e->rungStmt,stdString);									// conversion from .net string to standard string
	 fileBuffer =	tmpfile();												// create file buffer
	 fwrite(stdString.c_str(),sizeof(char),stdString.length(),fileBuffer);	// appending contents into buffer
	 rewind(fileBuffer);													// rewind to the beginning

	 if(stcCompile(fileBuffer,ircBuffer,STC_MODE_NORMAL) == STC_ERROR){
		 //----------------
		//	Error Handling
		//----------------
		 fclose(fileBuffer);
		 return LDE_ERROR;
	 }
	 
	 if(arg == LDE_COMPILE_MODE_DEBUG){
		 //----------------------------------
		 //	Debug Assertion Block
		 //----------------------------------
		 debugAssertion.type						= _PROGCTRL;
		 debugAssertion.progCtrlData.ctrlCmd		= _PROG_WAIT;
		 debugAssertion.progCtrlData.sectionIndex	= (short)e->rungSn;	// fill-in the rung serial number	
		 fwrite(&debugAssertion,sizeof(IRC_UNIT),1,ircBuffer);			// write-in debug assertion
	 }

	 fclose(fileBuffer);					// close buffer

	return LDE_SUCCESS;
}

int openFile(Windows::Forms::OpenFileDialog^ e,FILE*	destFile,const int filterIndex)
{
	//-------------------------
	//	Open FILE Utility , Added by Hsien , 2013.01.08
	//-------------------------
	FILE*				openedFile;
	std::string			stdFileName;

	e->FilterIndex	= filterIndex;
	e->FileName		= nullptr;
	if(e->ShowDialog() == System::Windows::Forms::DialogResult::OK){
		MarshalString(e->FileName,stdFileName);
		openedFile = fopen(stdFileName.c_str(),"wb+");	// w+ , or w? , questioned by Hsien , 2013.01.14
	}													// wb+ setted , Hsien , 2013.04.25

	fileCopy(openedFile,destFile);
	fclose(openedFile);

	return LDE_SUCCESS;
}

int saveFile(Windows::Forms::SaveFileDialog^ e,FILE*	sourceFile,const int filterIndex)
{
	//-------------------------
	//	Save FILE Utility , Added by Hsien , 2013.01.08
	//-------------------------
	FILE*				outFile;
	std::string			stdFileName;

	e->FilterIndex	= filterIndex;
	e->FileName		= nullptr;

	//-----------------------
	//	Try to fix bug when cancel the file-saving process
	//		Hsien ,  2013.01.24
	//-----------------------	
	switch(e->ShowDialog()){
		case System::Windows::Forms::DialogResult::OK:
			MarshalString(e->FileName,stdFileName);
			outFile = fopen(stdFileName.c_str(),"wb+");	// w+ , or w? , questioned by Hsien , 2013.01.14
														// wb+ setted , Hsien , 2013.04.25
//			outFile = fopen(stdFileName.c_str(),"w+");	// w+ , or w? , questioned by Hsien , 2013.01.14

			//---------------------------
			//	Copy To Destination File
			//---------------------------
			fileCopy(sourceFile,outFile);
			fclose(outFile);
			break;
		case System::Windows::Forms::DialogResult::Cancel:
			break;
	}

	return LDE_SUCCESS;
}

//System::Void ldEditor::ircGenerate(System::Object^  sender, System::EventArgs^  /*e*/) 
//{
//	//----------------------------------------------------------------------------------
//	//	Flow:
//	//		1. generate ST statement and compile ST  rung by rung	, any report connection error if any
//	//		3. if above process are done successfully , output IRC file , to simulator or FILE
//	//----------------------------------------------------------------------------------
//
//	FILE*		ircInstruction = tmpfile();		// for temprority buffer of ircInstruction
//	std::string stdFileName;					// for connect to Native library
//	IRC_UNIT	endOfIrcInstruction;			// for PROG_END		
//
//	char	errorFlag		= LDE_SUCCESS;										// indicate overall compile status
//	int		compileMode;																// the behavior when compile
//	int		outputMode;																	// the target to output
////	int		senderHashCode	= safe_cast<System::Object^>(sender)->GetHashCode();		// used to identify sender
////----------------------
////
//// Hsien 2013/5/24
////----------------------
//#define	TAG_COMPILE		0
//#define TAG_COMPILE_TO	1
//#define	TAG_SIMU		2
//	int	tagIndex = 0 ;
//	Int32::TryParse(
//		safe_cast<System::Windows::Forms::ToolStripButton^>(sender)->Tag->ToString(),tagIndex);
//
//	//---------------------------
//	//	Choose running mode according the sender
//	//---------------------------
//	switch(tagIndex){
//		case TAG_COMPILE:
//			compileMode	=	LDE_COMPILE_MODE_NORMAL;
//			outputMode	=	LDE_OUTPUT_MODE_IRCFILE;
//			break;
//		case TAG_COMPILE_TO:
//			break;
//		case TAG_SIMU:
//			compileMode = 	LDE_COMPILE_MODE_DEBUG;
//			outputMode	=	LDE_OUTPUT_MODE_SIMULATOR;
//			break;
//		default:
//			break;
//	}
//	//if(senderHashCode == this->tsbStep->GetHashCode()){
//	//		compileMode = 	LDE_COMPILE_MODE_DEBUG;
//	//		outputMode	=	LDE_OUTPUT_MODE_SIMULATOR;
//	//}
//	//else if(senderHashCode == this->tsbIrcGen->GetHashCode()){
//	//		compileMode	=	LDE_COMPILE_MODE_NORMAL;
//	//		outputMode	=	LDE_OUTPUT_MODE_IRCFILE;
//	//}
//	/* else : error */
//
//	//-------------------
//	//	Compiling Whole LDE rung by rung  ,output error message when each rung complished
//	//-------------------
//	this->tbOutput->Clear();		// Hsien 2013/5/20 
//
//	String^			singleError	;	// Hsien 2013/5/20
//	STC_ERRBLK		errorBlock	;
//
//	for each(rungControl^ e in this->tlpRung->Controls){
//		//-------------------------------------------------------
//		//	When sender is normal output , compile in normal mode
//		//					simulator	 , compile in debug	 mode
//		//-------------------------------------------------------
//		if(compileSingleRung(e/*input*/,_errorHdl,ircInstruction/*output*/,compileMode) != LDE_SUCCESS){
//			//----------------
//			//	Error Handling
//			//----------------
//			stcErrorRead(_errorHdl,&errorBlock);
//			singleError = String::Format(this->_ldResSetting->GetString("errorOutputFormat2")
//				,e->rungSn
//				,gcnew String(errorBlock.errContent)
//				,gcnew String(errorBlock.errExplain)
//				);
//			this->postOnOutput(singleError,"Error");
//			//errorStackOutput(/*this->auxiliaryForm->textBoxOutput*/
//			//	this->tbOutput	// Hsien 2013/5/17
//			//	,_errorHdl
//			//	,this->_ldResSetting->GetString("errorOutputFormat")
//			//	,e->rungSn);
//			errorFlag |= LDE_ERROR;				// mark 
//		}
//	}// for each
//
//	if(errorFlag == LDE_SUCCESS){
//		//----------------------------------------------------------
//		// No any Error , Possible to Output
//		//	Complish IRC-Instruction
//		//----------------------------------------------------------
//		endOfIrcInstruction.type						= _PROGCTRL;			// temporary facility , pvm core would 
//		endOfIrcInstruction.progCtrlData.ctrlCmd		= _PROG_END;			// fall in infinite-loop , if lack of PROG_END
//		fwrite(&endOfIrcInstruction,sizeof(IRC_UNIT),1,ircInstruction);			// Hsien , 2013.01.07
//		fflush(ircInstruction);													// forced write-in
//		rewind(ircInstruction);													// rewind to the begin
//
//		switch(outputMode){
//			case LDE_OUTPUT_MODE_SIMULATOR:
//				//------------------------
//				//	Load and Run Simulator  , 
//				//------------------------
//				this->simulatorControl->load(ircInstruction);
//				break;
//			case LDE_OUTPUT_MODE_IRCFILE:
//				//------------------------------
//				//	Output to the File , Save To
//				//------------------------------
//				saveFile(this->saveFileDialog,ircInstruction,LDE_FILE_INDEX_IRC);
//				break;
//			default:
//				break;
//		}
//		this->compileResult			= LDE_SUCCESS;	// 
//		this->postOnOutput(this->_ldResMessage->GetString("cpSuccess"),L"Message");
//	}
//	else{
//		//----------------
//		//	Error Happened
//		//----------------
//		this->compileResult			= LDE_ERROR;
//		this->postOnOutput(this->_ldResMessage->GetString("cpFail"),L"Error");
//
//	}//else
//
//	fclose(ircInstruction);
//
//}// irc generate
//
int	ldEditor::ircGenerate				(Windows::Forms::TableLayoutControlCollection^ tlp/*in*/,FILE* outputFile/*out*/,const int mode)
{
	//----------------------
	//	tlp		: the tablePanelLayout to be feed-in for compiling from graphic
	//	FILE*	: the output file
	//	mode	: DEBUG-ASSERTION , or NORMAL
	//	Flow:
	//		1. generate ST statement and compile ST  rung by rung	, any report connection error if any
	//		3. if above process are done successfully , output IRC to output file 
	// Hsien 2013/5/24
	//----------------------------------------------------------------------------------

	FILE*		ircInstruction = tmpfile();		// for temprority buffer of ircInstruction
	IRC_UNIT	endOfIrcInstruction;			// for PROG_END		

	int		errorFlag		= LDE_SUCCESS;										// indicate overall compile status

	//-------------------
	//	Compiling Whole LDE rung by rung  ,output error message when each rung complished
	//-------------------

	String^			singleError	;	// Hsien 2013/5/20
	STC_ERRBLK		errorBlock	;

	for each(rungControl^ e in tlp){
		//-------------------------------------------------------
		//	When sender is normal output , compile in normal mode
		//					simulator	 , compile in debug	 mode
		//-------------------------------------------------------
		if(compileSingleRung(e/*input*/,_errorHdl,ircInstruction/*output*/,mode) != LDE_SUCCESS){
			//----------------
			//	Error Handling
			//----------------
			if(errorFlag != LDE_ERROR)			// first time the error occurs
				this->tbOutput->Clear();		// Hsien 2013/5/20 

			errorFlag |= LDE_ERROR;				// mark 

			stcErrorRead(_errorHdl,&errorBlock);
			singleError = String::Format(this->_ldResSetting->GetString("errorOutputFormat2")
				,e->rungSn
				,gcnew String(errorBlock.errContent)
				,gcnew String(errorBlock.errExplain)
				);
			this->postOnOutput(singleError,"Error");
		}
	}// for each

	if(errorFlag == LDE_SUCCESS){
		//----------------------------------------------------------
		// No any Error , Possible to Output
		//	Complish IRC-Instruction
		//----------------------------------------------------------
		endOfIrcInstruction.type						= _PROGCTRL;			// temporary facility , pvm core would 
		endOfIrcInstruction.progCtrlData.ctrlCmd		= _PROG_END;			// fall in infinite-loop , if lack of PROG_END
		fwrite(&endOfIrcInstruction,sizeof(IRC_UNIT),1,ircInstruction);			// Hsien , 2013.01.07
		fflush(ircInstruction);													// forced write-in
		rewind(ircInstruction);													// rewind to the begin

		fileCopy(ircInstruction,outputFile);									// Hsien 2013/5/24 , copy contents in buffer to output
	}

	fclose(ircInstruction);

	return errorFlag;
}

int	ldEditor::compileAction(const int tagIndex)
{
	//----------------------
	//
	// Hsien 2013/5/24
	//----------------------
#define	TAG_COMPILE		0
#define TAG_COMPILE_TO	1
#define	TAG_SIMU		2
#define	TAG_SIMU_STOP	3
	//----------------------
	//	return value as Message Index
	//----------------------
	FILE*	fp =  NULL/*= tmpfile()*/;
	FILE*	outputFp;
	int	errorCode;

	//---------------------------
	//	Choose running mode according the sender
	//---------------------------
	switch(tagIndex){
		case TAG_COMPILE:
		case TAG_COMPILE_TO:
			//-------------------
			//	1. Save file dialog open
			//	2. Compiling
			//	3. fileName memorize
			//-------------------
			fp = tmpfile();
			errorCode = ircGenerate(this->tlpRung->Controls,fp,LDE_COMPILE_MODE_NORMAL);

			if(errorCode != LDE_SUCCESS){
				fclose(fp);
				return LDE_MSG_COMPILE_FAIL;
			}
			
			if((this->currentFileNames[LDE_FILE_INDEX_IRC] == nullptr)
				|| tagIndex == TAG_COMPILE_TO){
					//----------------------
					//	COMPILE_TO pressed or no currentIRCFile setted
					//----------------------
					if(this->saveDialogGetFilename	(this->saveFileDialog,LDE_FILE_INDEX_IRC) != LDE_SUCCESS){
						fclose(fp);
						return LDE_MSG_DIALOG_CANCEL ;				//	User Cancelled
					}// inner if
			}//if
			//----------------------
			//	File Selected
			//----------------------
			outputFp = fopenString(this->currentFileNames[LDE_FILE_INDEX_IRC],"wb+");
			fileCopy(fp,outputFp);

			fclose(outputFp);
			fclose(fp);
			
			return  LDE_MSG_COMPILE_SUCCESS;
			break;
		case TAG_SIMU:
			//-----------------
			//	1. Open tmp file
			//	2. Compiling in Debug mode
			//	3. feed in simulator if successed
			//-----------------
			errorCode = this->stepDebugMode(this->tlpRung->Controls);
			if(errorCode != LDE_SUCCESS)
				return LDE_MSG_DEBUG_FAIL;

			if(this->status == LDE_STATUS_EDITTING){
				//----------------------
				//	1.Transit Status
				//. Step forward
				//	3. Lock TSB
				//----------------------
				this->status = LDE_STATUS_DEBUGGING;
				this->stepDebugMode(nullptr);		// step forward
				//----------------------
				// 3. Lock TSB
				//----------------------
				for each(ToolStripItem^ e in this->tSFunctions->Items)
					if( e->GetType() == ToolStripButton::typeid)
						if(e->Name != this->tsbStep->Name 
							&& e->Name != this->tsbEndDebug->Name)
						e->Enabled = false;

				return  LDE_MSG_DEBUG_START;
			}
			else
				return  LDE_MSG_DEBUGGING;
			break;
		case TAG_SIMU_STOP:
			//----------------------
			//	End Procedure if Stop signal comes
			//----------------------
			if(this->status == LDE_STATUS_DEBUGGING){
				this->simulatorControl->terminate();
				for each(rungControl^ e in this->tlpRung->Controls)
					e->isMe(-1);		// turns all color gray
				this->status = LDE_STATUS_EDITTING;
				for each(ToolStripItem^ e in this->tSFunctions->Items)
					e->Enabled = true;		// Release all functions Hsien 2013/5/24
				return  LDE_MSG_DEBUG_STOP;
			}
			break;
		default:
			break;
	}//switch
	
	return	LDE_MSG_NONE;
}
System::Void ldEditor::tsbCompileClick	(System::Object^  sender, System::EventArgs^  e){
	// Hsien 2013/5/20 , common handler for compile,compile-to,simu,simuStop

	int	msg			= LDE_MSG_NONE;			// Hsien 2013/5/24 message code
	int	tagIndex = 0 ;
	Int32::TryParse(
		safe_cast<System::Windows::Forms::ToolStripButton^>(sender)->Tag->ToString(),tagIndex);
	msg = compileAction(tagIndex);

	//----------------------
	//	Message Output
	//----------------------
	switch(msg){
		case	LDE_MSG_DEBUGGING				:
			break;
		case LDE_MSG_DEBUG_STOP:
			this->postOnOutput("Debug stopped",L"Message");
			break;
		case	LDE_MSG_DEBUG_START				:
			this->postOnOutput("Debug started",L"Message");
			break;
		case	LDE_MSG_COMPILE_SUCCESS			:
			this->postOnOutput(this->_ldResMessage->GetString("cpSuccess"),L"Message");
			break;		
		case	LDE_MSG_DEBUG_FAIL				:
		case	LDE_MSG_COMPILE_FAIL			:
			this->postOnOutput(this->_ldResMessage->GetString("cpFail"),L"Error");
			break;
		case	LDE_MSG_DIALOG_CANCEL			:
			this->postOnOutput("User canceled",L"Message");
			break;
		default:
			break;
	}
}

int	ldEditor::stepDebugMode			(Windows::Forms::TableLayoutControlCollection^ tlp/*in*/)
{			
	FILE*	fp;
	int		compileResult;

	//---------------
	//	If not on debugging mode:
	//---------------
	switch(this->status){
		case LDE_STATUS_EDITTING:
			//----------------------
			//	Transit to Debugging mode if successed
			//----------------------
			fp = tmpfile();
			compileResult = this->ircGenerate(tlp,fp,LDE_COMPILE_MODE_DEBUG);
			if(compileResult == LDE_SUCCESS){
				fflush(fp);	
				rewind(fp);							// rewind and feed in
				this->simulatorControl->load(fp);

				return LDE_SUCCESS;
			}
			fclose(fp);

			return LDE_ERROR;	// inform caller the result , Hsien 2013/5/24
			break;
		case LDE_STATUS_DEBUGGING:
			//----------------------
			//	Transit to Stopping if stop clicked
			//----------------------
			SIMU_DATA_INFORMATION	localInfo;
			this->simulatorControl->singalStep(&localInfo);
			for each(rungControl^ e in this->tlpRung->Controls)
				e->isMe(localInfo.simuDebugInfo);

			return LDE_SUCCESS;
			break;
		default:
			return LDE_ERROR;
			break;
	}//switch
}