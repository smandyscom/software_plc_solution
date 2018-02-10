#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace _ldEditor {

	/// <summary>
	/// Summary for ldeVarWatchControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class ldSimulatorControl : public System::Windows::Forms::UserControl
	{
	public:
		ldSimulatorControl(/*void**//*sqlite3*	__db*//*void*/)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			//-------------------------------------
			//	Initialz Data-Base for watch window
			//-------------------------------------
			this->_db	= new sqlite3*;
			*this->_db	= NULL;
			dqiInit(this->_db,"PLC.DB");

			int	errorCode;

			this->pvmSimulator = new PVM;

			errorCode = pvmInitialize(this->pvmSimulator
				,10
				,(int(*)(void*))startViaDqiWithoutRs
				,*_db);								// start PVM but wait for loading process

			this->sharedCommand = new SIMU_IMPORT_SIGNAL;	// the shared data to communicate with simulator thread and GUI thread
			this->sharedInfo	= new SIMU_EXPORT_SIGNAL;	// the shared data to communicate with simulator thread and GUI thread 

			//---------------------------------
			//	Hand-Shake Signal initialization
			//---------------------------------
			hsInitial((HS_SIGNAL*)this->sharedCommand	
				,&this->sharedCommand->command
				,sizeof(SIMU_DATA_COMMAND));
			hsInitial((HS_SIGNAL*)this->sharedInfo		
				,&this->sharedInfo->information
				,sizeof(SIMU_DATA_INFORMATION/*SIMU_EXPORT_SIGNAL*/)); 
			// bug found , incorrect data-size induce
			// heap curruption , Hsien , 2013.01.03

			//--------------------------------
			//	Add the first watch-box
			//--------------------------------
			this->addWatchClick(nullptr,nullptr);
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ldSimulatorControl()
		{
			if (components)
			{
				delete components;
			}
			
			FSMI::finalize();					// release Frame memory
			pvmFinalize(this->pvmSimulator
				,(int(*)(void*))NULL
				,(void*)NULL);	// release Instruction memory

			delete this->pvmSimulator;

			delete this->sharedCommand;
			delete this->sharedInfo;

			//------------------------
			//	Data-Base Closing
			//------------------------
			dqiClose(*this->_db);
			delete this->_db;
		}
		//----------------
		//	PUBLIC INTERFACE OF MAIN-THREAD
		//----------------
	public:
		//----------------
		//	LOADING and UNLOADING
		//----------------
		int	load(void *ircInsturctions,const size_t insLength);	// after loaded , it's ready to launch
		int	load(const char*	filename);						// load via file
		int load(FILE*	ircFile);

		int	terminate();										// unload and reset

		//-----------------
		//	SINGAL STEP DEBUGGING
		//-----------------
		int	singalStep		(void*	simuInfo);					// trigger singal-step
		//------------------------------------
		//	CONTINOUS DEBUGGING
		//------------------------------------
		//int	startDebugging	();				// trigger
		//int	setBreakPoint	(const int rungSn);
		//int	rstBreakPoint	(const int rungSn);
		//int	clrBreakPoint	(void);
		//-----------------------
		//	Query
		//-----------------------
		int queryInfo		(void*	simuInfo);
	protected:
		//int	 simuPid;									// pid mechanism is going to abundant , Hsien , 2013.01.02

		//SIMU_DATA_COMMAND*		externalCommand;		// the command input from external via packaged method
		//SIMU_DATA_INFORMATION*	monitorInfomation;		// the information ready to pass to external monitor
		int	triggerWatchTable();

		SIMU_IMPORT_SIGNAL*		sharedCommand;
		SIMU_EXPORT_SIGNAL*		sharedInfo;

		PVM*			pvmSimulator;			// the instance for PVM-simulator

		PFC_OPERAND*	watchTable;				// the table store the user's request

		sqlite3**		_db;					// the data-base instance owned by this control
												// not implement yet, Hsien , 2013.01.17

	private: System::Windows::Forms::TableLayoutPanel^  tlpWatchTable;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorkerPvm;
	private: System::Windows::Forms::ContextMenuStrip^  cmsWatchTable;
	private: System::Windows::Forms::ToolStripMenuItem^  addWatchToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  removeWatchToolStripMenuItem;
	private: System::Windows::Forms::Label^  labelVariable;
	private: System::Windows::Forms::Label^  labelValue;
	private: System::ComponentModel::IContainer^  components;
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->tlpWatchTable = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->cmsWatchTable = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->addWatchToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->removeWatchToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->backgroundWorkerPvm = (gcnew System::ComponentModel::BackgroundWorker());
			this->labelVariable = (gcnew System::Windows::Forms::Label());
			this->labelValue = (gcnew System::Windows::Forms::Label());
			this->cmsWatchTable->SuspendLayout();
			this->SuspendLayout();
			// 
			// tlpWatchTable
			// 
			this->tlpWatchTable->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->tlpWatchTable->AutoSize = true;
			this->tlpWatchTable->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->tlpWatchTable->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Single;
			this->tlpWatchTable->ColumnCount = 2;
			this->tlpWatchTable->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				80)));
			this->tlpWatchTable->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				80)));
			this->tlpWatchTable->Location = System::Drawing::Point(7, 27);
			this->tlpWatchTable->Margin = System::Windows::Forms::Padding(5, 5, 5, 10);
			this->tlpWatchTable->MaximumSize = System::Drawing::Size(600, 480);
			this->tlpWatchTable->Name = L"tlpWatchTable";
			this->tlpWatchTable->RowCount = 1;
			this->tlpWatchTable->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
			this->tlpWatchTable->Size = System::Drawing::Size(163, 32);
			this->tlpWatchTable->TabIndex = 0;
			this->tlpWatchTable->ControlAdded += gcnew System::Windows::Forms::ControlEventHandler(this, &ldSimulatorControl::attachHandler);
			// 
			// cmsWatchTable
			// 
			this->cmsWatchTable->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->addWatchToolStripMenuItem, 
				this->removeWatchToolStripMenuItem});
			this->cmsWatchTable->Name = L"contextMenuStripWatchTable";
			this->cmsWatchTable->Size = System::Drawing::Size(163, 48);
			// 
			// addWatchToolStripMenuItem
			// 
			this->addWatchToolStripMenuItem->Name = L"addWatchToolStripMenuItem";
			this->addWatchToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->addWatchToolStripMenuItem->Text = L"Add Watch";
			this->addWatchToolStripMenuItem->Click += gcnew System::EventHandler(this, &ldSimulatorControl::addWatchClick);
			// 
			// removeWatchToolStripMenuItem
			// 
			this->removeWatchToolStripMenuItem->Name = L"removeWatchToolStripMenuItem";
			this->removeWatchToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->removeWatchToolStripMenuItem->Text = L"Remove Watch";
			this->removeWatchToolStripMenuItem->Click += gcnew System::EventHandler(this, &ldSimulatorControl::removeWatchClick);
			// 
			// backgroundWorkerPvm
			// 
			this->backgroundWorkerPvm->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &ldSimulatorControl::bgworkerPvmThread);
			// 
			// labelVariable
			// 
			this->labelVariable->AutoSize = true;
			this->labelVariable->Location = System::Drawing::Point(5, 5);
			this->labelVariable->Margin = System::Windows::Forms::Padding(5);
			this->labelVariable->Name = L"labelVariable";
			this->labelVariable->Size = System::Drawing::Size(44, 12);
			this->labelVariable->TabIndex = 4;
			this->labelVariable->Text = L"Variable";
			// 
			// labelValue
			// 
			this->labelValue->AutoSize = true;
			this->labelValue->Location = System::Drawing::Point(86, 5);
			this->labelValue->Margin = System::Windows::Forms::Padding(5);
			this->labelValue->Name = L"labelValue";
			this->labelValue->Size = System::Drawing::Size(32, 12);
			this->labelValue->TabIndex = 5;
			this->labelValue->Text = L"Value";
			// 
			// ldSimulatorControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->Controls->Add(this->labelVariable);
			this->Controls->Add(this->labelValue);
			this->Controls->Add(this->tlpWatchTable);
			this->Name = L"ldSimulatorControl";
			this->Size = System::Drawing::Size(185, 202);
			this->cmsWatchTable->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void kdEnterValue(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			 //----------------------
			 //	Tracking which textbox send this request
			 //		Sender: value
			 //		Neibor: variable
			 //----------------------
			 Windows::Forms::TextBox^			senderTextBox = safe_cast<Windows::Forms::TextBox^>(sender);
			 Windows::Forms::TableLayoutPanel^	parentPanel	  = safe_cast<Windows::Forms::TableLayoutPanel^>(senderTextBox->Parent);
			 Windows::Forms::TextBox^			neiborTextBox = safe_cast<Windows::Forms::TextBox^>(parentPanel->GetNextControl(senderTextBox,false));  // search backward
			 
			 std::string os;
			 VAR			varAttr;
			 PFC_OPERAND	pfcOp;
			 int			inputValue = 0;

			 if(e->KeyCode == Keys::Enter){
				 MarshalString(neiborTextBox->Text/*this->textBox1->Text*/,os);		
				 if(varString2Var(*_db,os.c_str(),&varAttr) == VAR_SUCCESS)		// turns string to var-data
					 if(var2PfcOperand(*_db,varAttr,&pfcOp) == VAR_SUCCESS)		// turns var-data to pfc-data
						 pfcOp.set(pfcOp.data,inputValue.Parse(senderTextBox->Text));
			 }
		 }
private: System::Void kdEnterVariable(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
				 std::string os;
				 VAR			varAttr;
				 PFC_OPERAND	pfcOp;

				 Windows::Forms::TextBox^			senderTextBox = safe_cast<Windows::Forms::TextBox^>(sender);
				 Windows::Forms::TableLayoutPanel^	parentPanel	  = safe_cast<Windows::Forms::TableLayoutPanel^>(senderTextBox->Parent);
				 Windows::Forms::TextBox^			neiborTextBox = 
					 safe_cast<Windows::Forms::TextBox^>(parentPanel->GetNextControl(senderTextBox,true));
				 int	signedValue;

				 if(e->KeyCode == Keys::Enter){
					 //---------------------------------------------------
					 // replace fixed variable(textBox1->Text) binding with input argument(sender) 
					 // , so that , it's able to reuse
					 //---------------------------------------------------
					 MarshalString(senderTextBox->Text/*this->textBox1->Text*/,os);		
					 if(varString2Var(*_db,os.c_str(),&varAttr) == VAR_SUCCESS)		// turns string to var-data
						 if(var2PfcOperand(*_db,varAttr,&pfcOp) == VAR_SUCCESS)		// turns var-data to pfc-data
							 signedValue = pfcOp.get(pfcOp.data);
							 neiborTextBox->Text = /*pfcOp.get(pfcOp.data)*/signedValue.ToString();
				 }
			 }
private: System::Void bgworkerPvmThread(System::Object^  /*sender*/, System::ComponentModel::DoWorkEventArgs^  /*e*/);
		 //----
		 //	Debug Use
		 //----
//private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
//			 SIMU_DATA_INFORMATION	localInfo;
//
//			 this->singalStep();
//			 this->queryInfo(&localInfo);
//
//			 //--------------------------------------
//			 //	Trigger the Watch Table simoutanously , to refresh to value of requested variable
//			 //--------------------------------------
//			 Windows::Forms::KeyEventArgs^	eKd = gcnew Windows::Forms::KeyEventArgs(Windows::Forms::Keys::Enter);
//			 Windows::Forms::TableLayoutPanelCellPosition^	cellPos;
//
//			 for each(Control^ e in this->tlpWatchTable->Controls){
//				 cellPos = this->tlpWatchTable->GetPositionFromControl(e);
//				 if(cellPos->Column == 0){
//					 //-------
//					 //	Except the banner
//					 //-------
//					 this->kdEnterVariable(e,eKd);
//				 }
//			 }
//
//			 //--------------------------------------
//			 //	Debug Use
//			 //--------------------------------------
//			 this->textBox3->Text = localInfo.simuState.ToString();
//			 this->textBox4->Text = localInfo.simuDebugInfo.ToString();
//		 }
//
private: System::Void addWatchClick(System::Object^  /*sender*/, System::EventArgs^  /*e*/) {
			 //----------------------------------------
			 //	Add Two Textbox consquently
			 //----------------------------------------
			 this->tlpWatchTable->SuspendLayout();
			 Windows::Forms::TextBox^	tb;
			 for(int i=0; i<2 ; i++){
				 tb = gcnew Windows::Forms::TextBox();
				 tb->Dock = DockStyle::Fill;
				 tb->Margin.All = 0;
				 this->tlpWatchTable->Controls->Add(tb,-1,-1);
			 }
			 tb->Focus();				// to get focus 
			 this->tlpWatchTable->ResumeLayout();
		 }
private: System::Void attachHandler(System::Object^  sender, System::Windows::Forms::ControlEventArgs^  e) {
			 //-------------------
			 //	Attach Some Event-Handler
			 //-------------------	
			 Windows::Forms::TableLayoutPanel^ tlp = safe_cast<Windows::Forms::TableLayoutPanel^>(sender);

			 e->Control->ContextMenuStrip = this->cmsWatchTable;		// right-click menu added

			 if(tlp->GetPositionFromControl(e->Control).Column == 0){
				 //--------------------
				 //	the variable column
				 //--------------------
				 e->Control->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &ldSimulatorControl::kdEnterVariable);
			 }
			 else{
				 //-----------------
				 //	the value column
				 //-----------------
				 e->Control->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this,&ldSimulatorControl::kdEnterValue);
			 }
		 }
private: System::Void removeWatchClick(System::Object^  sender, System::EventArgs^  e) {
			 //----------------------------------------
			 //	Remove Two Textbox consquently , Except the last one
			 //----------------------------------------
			 if(this->tlpWatchTable->Controls->Count > 2){
				 this->tlpWatchTable->Controls->RemoveAt(this->tlpWatchTable->Controls->Count-1);
				 this->tlpWatchTable->Controls->RemoveAt(this->tlpWatchTable->Controls->Count-1);
			 }
		 }
};
}
