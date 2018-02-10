#pragma once


namespace _ldEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ldEditor : public System::Windows::Forms::Form
	{
	public:
		ldEditor(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			std::string dbPath;
			//----------------------
			//	Under Version COntrol via app.rc file// Hsien 2013/4/29
			//----------------------
			this->tsbVersion->Text =
				"Version: " + System::Windows::Forms::Application::ProductVersion;

			this->_ldResSetting =	gcnew System::Resources::ResXResourceSet("ldeSetting.resx");
			this->_ldResMessage =	gcnew System::Resources::ResXResourceSet("ldeMessage.resx");

			this->saveFileDialog->Filter = this->_ldResSetting->GetString("fileFilter");
			this->openFileDialog->Filter = this->_ldResSetting->GetString("fileFilter");

			this->selectedInpItem = new int;	*this->selectedInpItem = NULL;
			this->selectedOutItem = new int;	*this->selectedOutItem = NULL;

			this->metaDataContainer = new CLST_METADATA*;
			*this->metaDataContainer = NULL;

			//----------------------
			//	Initializing The Button Table
			// Hsien 2013/5/19
			//----------------------
			this->selectionInit();

			//----------------------
			//	File Saving/Loading System init
			//----------------------
			this->currentFileNames = gcnew array<String^>(LDE_FILE_TYPES_COUNT);	// Hsien 2013/5/24
//			this->currentFileName = nullptr;

			//-----------------------------------
			//	Link To Data-Base by Default Path
			//-----------------------------------
			MarshalString(this->_ldResSetting->GetString("dbFilename"),dbPath);
			this->_db	= new sqlite3*;
			*this->_db	= NULL;
			if(dqiInit(this->_db,dbPath.c_str()) == DQI_ERR_NO_DB){
				dataBaseStatus = LDE_DBSTATUS_MISS;		// link failed. possible to re-link in without closing program, Hsien , 2012.06.11
///				this->tsslStatus->Text = this->_ldResMessage->GetString("dbMiss");
				this->postOnOutput(this->_ldResMessage->GetString("dbMiss"),L"Error");

			}
			else{
				dataBaseStatus = LDE_DBSTATUS_LINK;
				//this->tsslStatus->Text = this->_ldResMessage->GetString("dbLinked");
				this->postOnOutput(this->_ldResMessage->GetString("dbLinked"),L"Message");
			}

			//---------------------------------
			//	error block container initializing
			//	added by Hsien , 2012.10.13
			//---------------------------------
			//this->errorStack = new PCDPM_FAMILY;
			//pcdpmInit(this->errorStack,tmpfile(),sizeof(STG_ERRBLK));	// need a anchor to link tmpfile() , so that possible to release it , Hsien , 2012.10.31
			this->_errorHdl = new STC_ERRHDL;
			stcErrorHandlerInit(this->_errorHdl,*this->_db);						// redirect error stack for STC , Hsien , 2012.10.13

			//---------------------------------------
			//	ST-Compiler initializing
			//---------------------------------------
			stcInit(*this->_db,this->_errorHdl);
		
			this->tlpRung->RowCount			= 0;
			this->tlpRung->RowStyles->Clear()	;

			this->setupForm = gcnew setup();		// sub-form constucting
			this->setupForm->Hide();

//			this->auxiliaryForm = gcnew _ldEditor::ldEditorOutput();
			//this->auxiliaryForm->Show();
//			this->splitContainerUpper->Panel2->Controls->Add(this->auxiliaryForm);

			//------------
			//	The Simulator and Watcher , Hsien ,2013.01.03
			//------------
			this->simulatorControl = gcnew ldSimulatorControl(/**_db*/)		;		// constructor adjested
//			this->auxiliaryForm->tabPageWatch->Controls->Add(this->simulatorControl);		// Hsien , 2013.01.31
			this->tpWatch->Controls->Add(this->simulatorControl);						// Hsien 2013/5/17
																						// change UI layout for convinence use

			this->status = LDE_STATUS_EDITTING;

#if LDE_ON_DEVELOP
			//this->tsbStoreTest->Click += gcnew System::EventHandler(this,ldEditor::storeLMDS);
			//this->tsbRebuildTest->Click += gcnew System::EventHandler(this,ldEditor::rebuildLMDS);			
			//this->tsbFileDebug->Click += gcnew System::EventHandler(this,ldEditor::tsbFileDebugClick);
#else
			this->tsbStoreTest->Visible		= false;
			this->tsbRebuildTest->Visible	= false;
			this->tsbFileDebug->Visible		= false;
			this->tsbSetUp->Visible			= false;
#endif
		}
		
		CLST_METADATA**							metaDataContainer;	

		int*									selectedInpItem;
		int*									selectedOutItem;			 

		_ldEditor::setup^						setupForm;
//		_ldEditor::ldEditorOutput^				auxiliaryForm;			// output and watch Tab
																		// Hsien 2013/5/17 , no more use
		_ldEditor::ldSimulatorControl^			simulatorControl;
		String^									applicationPath;		// Hsien 2013/4/29

		System::Collections::Queue^				inpTable;				// Hsien 2013/5/19 , collection on inpButtons
		System::Collections::Queue^				outTable;				// Hsien 2013/5/19 , collection on outButtons
//		String^									currentFileName;		// Hsien 2013/5/20 , for focusing on specific file
		array<String^>^							currentFileNames;		// Hsien 2013/5/24
																		// for managing irc and lmds file name
																		//	LDE_FILE_INDEX_LMDS			1
																		//	LDE_FILE_INDEX_ST			2
																		//	LDE_FILE_INDEX_IRC			3

		//-------------------------------------------------
		//	Data-Base and Error Handler
		//-------------------------------------------------
	public:		sqlite3**								_db;
		STC_ERRHDL*								_errorHdl;
		//PCDPM_FAMILY*							errorStack;			// for error-reporting mechanism , Hsien , 2012.10.12
																	// for STG and STC
		//----------------
		//	Status
		//----------------
		int										status;
		int										compileResult;
		int										dataBaseStatus;

		//----------------
		//	Resource Management
		//----------------
		System::Resources::ResXResourceSet^				_ldResSetting;	// the setting data
		System::Resources::ResXResourceSet^				_ldResMessage;	// the message data
private: System::Windows::Forms::Panel^  pnlRung;
private: System::Windows::Forms::ToolStrip^  tSFunctions;
private: System::Windows::Forms::TableLayoutPanel^  tlpInpDevice;
private: System::Windows::Forms::TableLayoutPanel^  tlpOdpanel;
private: System::Windows::Forms::ToolStripButton^  tsbSaveFile;
private: System::Windows::Forms::ToolStripButton^  tsbLoadFile;
private: System::Windows::Forms::SaveFileDialog^  saveFileDialog;
private: System::Windows::Forms::OpenFileDialog^  openFileDialog;
private: System::Windows::Forms::ToolStripButton^  tsbCompile;
private: System::Windows::Forms::ToolStripButton^  tsbSetUp;
private: System::Windows::Forms::ToolStripProgressBar^  tspbLoadingLmds;
private: System::Windows::Forms::ToolStripButton^  tsbNewFile;
private: System::Windows::Forms::ToolStripButton^  tsbStoreTest;
private: System::Windows::Forms::ToolStripButton^  tsbRebuildTest;
private: System::Windows::Forms::ToolStripButton^  tsbFileDebug;
private: System::Windows::Forms::ToolStripButton^  tsbStep;
private: System::Windows::Forms::ToolStripButton^  tsbEndDebug;
private: System::Windows::Forms::ToolStripLabel^  tsbVersion;
private: System::Windows::Forms::SplitContainer^  splitContainerUpper;
private: System::Windows::Forms::TabControl^  tabControlUpper;
private: System::Windows::Forms::TabPage^  tpOutput;
private: System::Windows::Forms::TabPage^  tpWatch;
private: System::Windows::Forms::TextBox^  tbOutput;
private: System::Windows::Forms::Button^  inpBdTerminal;
private: System::Windows::Forms::Button^  inpDelete;
private: System::Windows::Forms::Button^  inpBdTsection;
private: System::Windows::Forms::Button^  inpNormalClose;
private: System::Windows::Forms::Button^  inpBfTerminal;
private: System::Windows::Forms::Button^  inpBfTsection;
private: System::Windows::Forms::Button^  inpBufT1;
private: System::Windows::Forms::Button^  inpBufT2;
private: System::Windows::Forms::Button^  inpComparator;
private: System::Windows::Forms::Button^  inpIndexer;
private: System::Windows::Forms::Button^  inpShort;
private: System::Windows::Forms::Button^  outCoil;

private: System::Windows::Forms::Button^  outSet;
private: System::Windows::Forms::Button^  outTNSet;
private: System::Windows::Forms::Button^  outStBlock;

private: System::Windows::Forms::ToolStripButton^  tsbSaveAsFile;
private: System::Windows::Forms::Button^  outIndexerSetNcCfm;
private: System::Windows::Forms::Button^  outCounterCountType2;
private: System::Windows::Forms::ToolStripButton^  tsbCompileTo;
private: System::Windows::Forms::Button^  outCounterCount;
private: System::Windows::Forms::Button^  outCounterSet;

private: System::Windows::Forms::Button^  inpNormalOpen;
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ldEditor()
		{				
			//------------
			//	Finalizer
			//------------
			if(*this->metaDataContainer)
				cluster::finalize(*this->metaDataContainer);
			delete this->metaDataContainer;
			delete this->selectedInpItem;
			delete this->selectedOutItem;

			stcClose();										// compiler-close
			//----------------------------------
			//	Error-Stack And  Data-Base Close
			//----------------------------------
			stcErrorHandlerFinalize(this->_errorHdl);
			delete this->_errorHdl;
			dqiClose(*this->_db);
			delete this->_db;

			if (components)
			{
				delete components;
			}
		}
private: System::Windows::Forms::TableLayoutPanel^  tlpMainframe;
	private: System::Windows::Forms::TableLayoutPanel^  tlpRung;
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(ldEditor::typeid));
			this->tlpMainframe = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->tlpOdpanel = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->outCounterCountType2 = (gcnew System::Windows::Forms::Button());
			this->outIndexerSetNcCfm = (gcnew System::Windows::Forms::Button());
			this->outCoil = (gcnew System::Windows::Forms::Button());
			this->outSet = (gcnew System::Windows::Forms::Button());
			this->outTNSet = (gcnew System::Windows::Forms::Button());
			this->outStBlock = (gcnew System::Windows::Forms::Button());
			this->pnlRung = (gcnew System::Windows::Forms::Panel());
			this->tlpRung = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->splitContainerUpper = (gcnew System::Windows::Forms::SplitContainer());
			this->tlpInpDevice = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->inpNormalClose = (gcnew System::Windows::Forms::Button());
			this->inpBdTerminal = (gcnew System::Windows::Forms::Button());
			this->inpDelete = (gcnew System::Windows::Forms::Button());
			this->inpBdTsection = (gcnew System::Windows::Forms::Button());
			this->inpBfTerminal = (gcnew System::Windows::Forms::Button());
			this->inpBfTsection = (gcnew System::Windows::Forms::Button());
			this->inpBufT1 = (gcnew System::Windows::Forms::Button());
			this->inpBufT2 = (gcnew System::Windows::Forms::Button());
			this->inpComparator = (gcnew System::Windows::Forms::Button());
			this->inpIndexer = (gcnew System::Windows::Forms::Button());
			this->inpShort = (gcnew System::Windows::Forms::Button());
			this->inpNormalOpen = (gcnew System::Windows::Forms::Button());
			this->tabControlUpper = (gcnew System::Windows::Forms::TabControl());
			this->tpOutput = (gcnew System::Windows::Forms::TabPage());
			this->tbOutput = (gcnew System::Windows::Forms::TextBox());
			this->tpWatch = (gcnew System::Windows::Forms::TabPage());
			this->tSFunctions = (gcnew System::Windows::Forms::ToolStrip());
			this->tsbNewFile = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbLoadFile = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbSaveFile = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbSaveAsFile = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbCompile = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbCompileTo = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbStep = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbEndDebug = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbSetUp = (gcnew System::Windows::Forms::ToolStripButton());
			this->tspbLoadingLmds = (gcnew System::Windows::Forms::ToolStripProgressBar());
			this->tsbStoreTest = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbRebuildTest = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbFileDebug = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbVersion = (gcnew System::Windows::Forms::ToolStripLabel());
			this->saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->outCounterCount = (gcnew System::Windows::Forms::Button());
			this->outCounterSet = (gcnew System::Windows::Forms::Button());
			this->tlpMainframe->SuspendLayout();
			this->tlpOdpanel->SuspendLayout();
			this->pnlRung->SuspendLayout();
			this->splitContainerUpper->Panel1->SuspendLayout();
			this->splitContainerUpper->Panel2->SuspendLayout();
			this->splitContainerUpper->SuspendLayout();
			this->tlpInpDevice->SuspendLayout();
			this->tabControlUpper->SuspendLayout();
			this->tpOutput->SuspendLayout();
			this->tSFunctions->SuspendLayout();
			this->SuspendLayout();
			// 
			// tlpMainframe
			// 
			this->tlpMainframe->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->tlpMainframe->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Single;
			this->tlpMainframe->ColumnCount = 2;
			this->tlpMainframe->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				1135)));
			this->tlpMainframe->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				141)));
			this->tlpMainframe->Controls->Add(this->tlpOdpanel, 1, 1);
			this->tlpMainframe->Controls->Add(this->pnlRung, 0, 1);
			this->tlpMainframe->Controls->Add(this->splitContainerUpper, 0, 0);
			this->tlpMainframe->Location = System::Drawing::Point(0, 25);
			this->tlpMainframe->Margin = System::Windows::Forms::Padding(0);
			this->tlpMainframe->Name = L"tlpMainframe";
			this->tlpMainframe->RowCount = 2;
			this->tlpMainframe->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 16.36364F)));
			this->tlpMainframe->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 83.63636F)));
			this->tlpMainframe->Size = System::Drawing::Size(1260, 682);
			this->tlpMainframe->TabIndex = 0;
			// 
			// tlpOdpanel
			// 
			this->tlpOdpanel->AutoSize = true;
			this->tlpOdpanel->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->tlpOdpanel->ColumnCount = 1;
			this->tlpOdpanel->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tlpOdpanel->Controls->Add(this->outCoil, 0, 0);
			this->tlpOdpanel->Controls->Add(this->outSet, 0, 1);
			this->tlpOdpanel->Controls->Add(this->outTNSet, 0, 2);
			this->tlpOdpanel->Controls->Add(this->outStBlock, 0, 3);
			this->tlpOdpanel->Controls->Add(this->outIndexerSetNcCfm, 0, 4);
			this->tlpOdpanel->Controls->Add(this->outCounterCount, 0, 7);
			this->tlpOdpanel->Controls->Add(this->outCounterSet, 0, 6);
			this->tlpOdpanel->Controls->Add(this->outCounterCountType2, 0, 5);
			this->tlpOdpanel->Location = System::Drawing::Point(1137, 113);
			this->tlpOdpanel->Margin = System::Windows::Forms::Padding(0);
			this->tlpOdpanel->Name = L"tlpOdpanel";
			this->tlpOdpanel->RowCount = 9;
			this->tlpOdpanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpOdpanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpOdpanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpOdpanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpOdpanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpOdpanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpOdpanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpOdpanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpOdpanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpOdpanel->Size = System::Drawing::Size(120, 450);
			this->tlpOdpanel->TabIndex = 6;
			// 
			// outCounterCountType2
			// 
			this->outCounterCountType2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->outCounterCountType2->BackColor = System::Drawing::SystemColors::Control;
			this->outCounterCountType2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->outCounterCountType2->FlatAppearance->BorderColor = System::Drawing::Color::DarkRed;
			this->outCounterCountType2->FlatAppearance->BorderSize = 2;
			this->outCounterCountType2->ImageKey = L"(none)";
			this->outCounterCountType2->Location = System::Drawing::Point(0, 250);
			this->outCounterCountType2->Margin = System::Windows::Forms::Padding(0);
			this->outCounterCountType2->Name = L"outCounterCountType2";
			this->outCounterCountType2->Size = System::Drawing::Size(120, 50);
			this->outCounterCountType2->TabIndex = 22;
			this->outCounterCountType2->Tag = L"0x0000000d";
			this->outCounterCountType2->UseVisualStyleBackColor = true;
			// 
			// outIndexerSetNcCfm
			// 
			this->outIndexerSetNcCfm->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->outIndexerSetNcCfm->BackColor = System::Drawing::SystemColors::Control;
			this->outIndexerSetNcCfm->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->outIndexerSetNcCfm->FlatAppearance->BorderColor = System::Drawing::Color::DarkRed;
			this->outIndexerSetNcCfm->FlatAppearance->BorderSize = 2;
			this->outIndexerSetNcCfm->ImageKey = L"(none)";
			this->outIndexerSetNcCfm->Location = System::Drawing::Point(0, 200);
			this->outIndexerSetNcCfm->Margin = System::Windows::Forms::Padding(0);
			this->outIndexerSetNcCfm->Name = L"outIndexerSetNcCfm";
			this->outIndexerSetNcCfm->Size = System::Drawing::Size(120, 50);
			this->outIndexerSetNcCfm->TabIndex = 21;
			this->outIndexerSetNcCfm->Tag = L"0x0000000c";
			this->outIndexerSetNcCfm->UseVisualStyleBackColor = true;
			// 
			// outCoil
			// 
			this->outCoil->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->outCoil->BackColor = System::Drawing::SystemColors::Control;
			this->outCoil->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->outCoil->FlatAppearance->BorderColor = System::Drawing::Color::DarkRed;
			this->outCoil->FlatAppearance->BorderSize = 2;
			this->outCoil->ImageKey = L"(none)";
			this->outCoil->Location = System::Drawing::Point(0, 0);
			this->outCoil->Margin = System::Windows::Forms::Padding(0);
			this->outCoil->Name = L"outCoil";
			this->outCoil->Size = System::Drawing::Size(120, 50);
			this->outCoil->TabIndex = 15;
			this->outCoil->Tag = L"0x00000004";
			this->outCoil->UseVisualStyleBackColor = true;
			// 
			// outSet
			// 
			this->outSet->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->outSet->BackColor = System::Drawing::SystemColors::Control;
			this->outSet->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->outSet->FlatAppearance->BorderColor = System::Drawing::Color::DarkRed;
			this->outSet->FlatAppearance->BorderSize = 2;
			this->outSet->ImageKey = L"(none)";
			this->outSet->Location = System::Drawing::Point(0, 50);
			this->outSet->Margin = System::Windows::Forms::Padding(0);
			this->outSet->Name = L"outSet";
			this->outSet->Size = System::Drawing::Size(120, 50);
			this->outSet->TabIndex = 16;
			this->outSet->Tag = L"0x00000005";
			this->outSet->UseVisualStyleBackColor = true;
			// 
			// outTNSet
			// 
			this->outTNSet->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->outTNSet->BackColor = System::Drawing::SystemColors::Control;
			this->outTNSet->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->outTNSet->FlatAppearance->BorderColor = System::Drawing::Color::DarkRed;
			this->outTNSet->FlatAppearance->BorderSize = 2;
			this->outTNSet->ImageKey = L"(none)";
			this->outTNSet->Location = System::Drawing::Point(0, 100);
			this->outTNSet->Margin = System::Windows::Forms::Padding(0);
			this->outTNSet->Name = L"outTNSet";
			this->outTNSet->Size = System::Drawing::Size(120, 50);
			this->outTNSet->TabIndex = 17;
			this->outTNSet->Tag = L"0x00000006";
			this->outTNSet->UseVisualStyleBackColor = true;
			// 
			// outStBlock
			// 
			this->outStBlock->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->outStBlock->BackColor = System::Drawing::SystemColors::Control;
			this->outStBlock->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->outStBlock->FlatAppearance->BorderColor = System::Drawing::Color::DarkRed;
			this->outStBlock->FlatAppearance->BorderSize = 2;
			this->outStBlock->ImageKey = L"(none)";
			this->outStBlock->Location = System::Drawing::Point(0, 150);
			this->outStBlock->Margin = System::Windows::Forms::Padding(0);
			this->outStBlock->Name = L"outStBlock";
			this->outStBlock->Size = System::Drawing::Size(120, 50);
			this->outStBlock->TabIndex = 18;
			this->outStBlock->Tag = L"0x00000008";
			this->outStBlock->UseVisualStyleBackColor = true;
			// 
			// pnlRung
			// 
			this->pnlRung->AutoScroll = true;
			this->pnlRung->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->pnlRung->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->pnlRung->Controls->Add(this->tlpRung);
			this->pnlRung->Location = System::Drawing::Point(1, 113);
			this->pnlRung->Margin = System::Windows::Forms::Padding(0);
			this->pnlRung->Name = L"pnlRung";
			this->pnlRung->Size = System::Drawing::Size(1135, 547);
			this->pnlRung->TabIndex = 3;
			// 
			// tlpRung
			// 
			this->tlpRung->AutoSize = true;
			this->tlpRung->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->tlpRung->ColumnCount = 1;
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tlpRung->Location = System::Drawing::Point(0, 0);
			this->tlpRung->Margin = System::Windows::Forms::Padding(0);
			this->tlpRung->Name = L"tlpRung";
			this->tlpRung->RowCount = 1;
			this->tlpRung->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tlpRung->Size = System::Drawing::Size(0, 0);
			this->tlpRung->TabIndex = 0;
			this->tlpRung->ControlAdded += gcnew System::Windows::Forms::ControlEventHandler(this, &ldEditor::rungSnMark);
			this->tlpRung->ControlRemoved += gcnew System::Windows::Forms::ControlEventHandler(this, &ldEditor::rungSnMark);
			// 
			// splitContainerUpper
			// 
			this->splitContainerUpper->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainerUpper->Location = System::Drawing::Point(1, 1);
			this->splitContainerUpper->Margin = System::Windows::Forms::Padding(0);
			this->splitContainerUpper->Name = L"splitContainerUpper";
			// 
			// splitContainerUpper.Panel1
			// 
			this->splitContainerUpper->Panel1->AccessibleRole = System::Windows::Forms::AccessibleRole::None;
			this->splitContainerUpper->Panel1->Controls->Add(this->tlpInpDevice);
			// 
			// splitContainerUpper.Panel2
			// 
			this->splitContainerUpper->Panel2->Controls->Add(this->tabControlUpper);
			this->splitContainerUpper->Size = System::Drawing::Size(1135, 111);
			this->splitContainerUpper->SplitterDistance = 270;
			this->splitContainerUpper->TabIndex = 7;
			// 
			// tlpInpDevice
			// 
			this->tlpInpDevice->AutoSize = true;
			this->tlpInpDevice->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->tlpInpDevice->ColumnCount = 7;
			this->tlpInpDevice->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				35)));
			this->tlpInpDevice->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				35)));
			this->tlpInpDevice->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				35)));
			this->tlpInpDevice->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				35)));
			this->tlpInpDevice->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				35)));
			this->tlpInpDevice->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				35)));
			this->tlpInpDevice->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				38)));
			this->tlpInpDevice->Controls->Add(this->inpNormalClose, 2, 1);
			this->tlpInpDevice->Controls->Add(this->inpBdTerminal, 1, 0);
			this->tlpInpDevice->Controls->Add(this->inpDelete, 0, 0);
			this->tlpInpDevice->Controls->Add(this->inpBdTsection, 2, 0);
			this->tlpInpDevice->Controls->Add(this->inpBfTerminal, 3, 0);
			this->tlpInpDevice->Controls->Add(this->inpBfTsection, 4, 0);
			this->tlpInpDevice->Controls->Add(this->inpBufT1, 5, 0);
			this->tlpInpDevice->Controls->Add(this->inpBufT2, 6, 0);
			this->tlpInpDevice->Controls->Add(this->inpComparator, 5, 1);
			this->tlpInpDevice->Controls->Add(this->inpIndexer, 4, 1);
			this->tlpInpDevice->Controls->Add(this->inpShort, 3, 1);
			this->tlpInpDevice->Controls->Add(this->inpNormalOpen, 1, 1);
			this->tlpInpDevice->Location = System::Drawing::Point(3, 3);
			this->tlpInpDevice->Margin = System::Windows::Forms::Padding(0);
			this->tlpInpDevice->Name = L"tlpInpDevice";
			this->tlpInpDevice->RowCount = 2;
			this->tlpInpDevice->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 35)));
			this->tlpInpDevice->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 35)));
			this->tlpInpDevice->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpInpDevice->Size = System::Drawing::Size(248, 70);
			this->tlpInpDevice->TabIndex = 5;
			// 
			// inpNormalClose
			// 
			this->inpNormalClose->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->inpNormalClose->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->inpNormalClose->FlatAppearance->BorderSize = 0;
			this->inpNormalClose->ImageKey = L"(none)";
			this->inpNormalClose->Location = System::Drawing::Point(70, 35);
			this->inpNormalClose->Margin = System::Windows::Forms::Padding(0);
			this->inpNormalClose->Name = L"inpNormalClose";
			this->inpNormalClose->Size = System::Drawing::Size(35, 35);
			this->inpNormalClose->TabIndex = 24;
			this->inpNormalClose->Tag = L"0x0002000a";
			this->inpNormalClose->UseVisualStyleBackColor = true;
			// 
			// inpBdTerminal
			// 
			this->inpBdTerminal->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->inpBdTerminal->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->inpBdTerminal->FlatAppearance->BorderSize = 0;
			this->inpBdTerminal->ImageKey = L"(none)";
			this->inpBdTerminal->Location = System::Drawing::Point(35, 0);
			this->inpBdTerminal->Margin = System::Windows::Forms::Padding(0);
			this->inpBdTerminal->Name = L"inpBdTerminal";
			this->inpBdTerminal->Size = System::Drawing::Size(35, 35);
			this->inpBdTerminal->TabIndex = 13;
			this->inpBdTerminal->Tag = L"0x00000007";
			this->inpBdTerminal->UseVisualStyleBackColor = true;
			// 
			// inpDelete
			// 
			this->inpDelete->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->inpDelete->BackColor = System::Drawing::SystemColors::Control;
			this->inpDelete->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->inpDelete->FlatAppearance->BorderColor = System::Drawing::Color::DarkRed;
			this->inpDelete->FlatAppearance->BorderSize = 2;
			this->inpDelete->ImageKey = L"(none)";
			this->inpDelete->Location = System::Drawing::Point(0, 0);
			this->inpDelete->Margin = System::Windows::Forms::Padding(0);
			this->inpDelete->Name = L"inpDelete";
			this->inpDelete->Size = System::Drawing::Size(35, 35);
			this->inpDelete->TabIndex = 14;
			this->inpDelete->Tag = L"0x00000009";
			this->inpDelete->UseVisualStyleBackColor = true;
			// 
			// inpBdTsection
			// 
			this->inpBdTsection->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->inpBdTsection->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->inpBdTsection->FlatAppearance->BorderSize = 0;
			this->inpBdTsection->ImageKey = L"(none)";
			this->inpBdTsection->Location = System::Drawing::Point(70, 0);
			this->inpBdTsection->Margin = System::Windows::Forms::Padding(0);
			this->inpBdTsection->Name = L"inpBdTsection";
			this->inpBdTsection->Size = System::Drawing::Size(35, 35);
			this->inpBdTsection->TabIndex = 15;
			this->inpBdTsection->Tag = L"0x00000008";
			this->inpBdTsection->UseVisualStyleBackColor = true;
			// 
			// inpBfTerminal
			// 
			this->inpBfTerminal->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->inpBfTerminal->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->inpBfTerminal->FlatAppearance->BorderSize = 0;
			this->inpBfTerminal->ImageKey = L"(none)";
			this->inpBfTerminal->Location = System::Drawing::Point(105, 0);
			this->inpBfTerminal->Margin = System::Windows::Forms::Padding(0);
			this->inpBfTerminal->Name = L"inpBfTerminal";
			this->inpBfTerminal->Size = System::Drawing::Size(35, 35);
			this->inpBfTerminal->TabIndex = 16;
			this->inpBfTerminal->Tag = L"0x00000001";
			this->inpBfTerminal->UseVisualStyleBackColor = true;
			// 
			// inpBfTsection
			// 
			this->inpBfTsection->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->inpBfTsection->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->inpBfTsection->FlatAppearance->BorderSize = 0;
			this->inpBfTsection->ImageKey = L"(none)";
			this->inpBfTsection->Location = System::Drawing::Point(140, 0);
			this->inpBfTsection->Margin = System::Windows::Forms::Padding(0);
			this->inpBfTsection->Name = L"inpBfTsection";
			this->inpBfTsection->Size = System::Drawing::Size(35, 35);
			this->inpBfTsection->TabIndex = 17;
			this->inpBfTsection->Tag = L"0x00000002";
			this->inpBfTsection->UseVisualStyleBackColor = true;
			// 
			// inpBufT1
			// 
			this->inpBufT1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->inpBufT1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->inpBufT1->FlatAppearance->BorderSize = 0;
			this->inpBufT1->ImageKey = L"(none)";
			this->inpBufT1->Location = System::Drawing::Point(175, 0);
			this->inpBufT1->Margin = System::Windows::Forms::Padding(0);
			this->inpBufT1->Name = L"inpBufT1";
			this->inpBufT1->Size = System::Drawing::Size(35, 35);
			this->inpBufT1->TabIndex = 18;
			this->inpBufT1->Tag = L"0x00000004";
			this->inpBufT1->UseVisualStyleBackColor = true;
			// 
			// inpBufT2
			// 
			this->inpBufT2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->inpBufT2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->inpBufT2->FlatAppearance->BorderSize = 0;
			this->inpBufT2->ImageKey = L"(none)";
			this->inpBufT2->Location = System::Drawing::Point(210, 0);
			this->inpBufT2->Margin = System::Windows::Forms::Padding(0);
			this->inpBufT2->Name = L"inpBufT2";
			this->inpBufT2->Size = System::Drawing::Size(38, 35);
			this->inpBufT2->TabIndex = 19;
			this->inpBufT2->Tag = L"0x00000005";
			this->inpBufT2->UseVisualStyleBackColor = true;
			// 
			// inpComparator
			// 
			this->inpComparator->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->inpComparator->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->inpComparator->FlatAppearance->BorderSize = 0;
			this->inpComparator->ImageKey = L"(none)";
			this->inpComparator->Location = System::Drawing::Point(175, 35);
			this->inpComparator->Margin = System::Windows::Forms::Padding(0);
			this->inpComparator->Name = L"inpComparator";
			this->inpComparator->Size = System::Drawing::Size(35, 35);
			this->inpComparator->TabIndex = 23;
			this->inpComparator->Tag = L"0x0008000a";
			this->inpComparator->UseVisualStyleBackColor = true;
			// 
			// inpIndexer
			// 
			this->inpIndexer->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->inpIndexer->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->inpIndexer->FlatAppearance->BorderSize = 0;
			this->inpIndexer->ImageKey = L"(none)";
			this->inpIndexer->Location = System::Drawing::Point(140, 35);
			this->inpIndexer->Margin = System::Windows::Forms::Padding(0);
			this->inpIndexer->Name = L"inpIndexer";
			this->inpIndexer->Size = System::Drawing::Size(35, 35);
			this->inpIndexer->TabIndex = 21;
			this->inpIndexer->Tag = L"0x0006000a";
			this->inpIndexer->UseVisualStyleBackColor = true;
			// 
			// inpShort
			// 
			this->inpShort->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->inpShort->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->inpShort->FlatAppearance->BorderSize = 0;
			this->inpShort->ImageKey = L"(none)";
			this->inpShort->Location = System::Drawing::Point(105, 35);
			this->inpShort->Margin = System::Windows::Forms::Padding(0);
			this->inpShort->Name = L"inpShort";
			this->inpShort->Size = System::Drawing::Size(35, 35);
			this->inpShort->TabIndex = 20;
			this->inpShort->Tag = L"0x0000000a";
			this->inpShort->UseVisualStyleBackColor = true;
			// 
			// inpNormalOpen
			// 
			this->inpNormalOpen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->inpNormalOpen->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->inpNormalOpen->FlatAppearance->BorderSize = 0;
			this->inpNormalOpen->ImageKey = L"(none)";
			this->inpNormalOpen->Location = System::Drawing::Point(35, 35);
			this->inpNormalOpen->Margin = System::Windows::Forms::Padding(0);
			this->inpNormalOpen->Name = L"inpNormalOpen";
			this->inpNormalOpen->Size = System::Drawing::Size(35, 35);
			this->inpNormalOpen->TabIndex = 22;
			this->inpNormalOpen->Tag = L"0x0001000a";
			this->inpNormalOpen->UseVisualStyleBackColor = true;
			// 
			// tabControlUpper
			// 
			this->tabControlUpper->Alignment = System::Windows::Forms::TabAlignment::Left;
			this->tabControlUpper->Controls->Add(this->tpOutput);
			this->tabControlUpper->Controls->Add(this->tpWatch);
			this->tabControlUpper->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControlUpper->Location = System::Drawing::Point(0, 0);
			this->tabControlUpper->Margin = System::Windows::Forms::Padding(0);
			this->tabControlUpper->Multiline = true;
			this->tabControlUpper->Name = L"tabControlUpper";
			this->tabControlUpper->SelectedIndex = 0;
			this->tabControlUpper->Size = System::Drawing::Size(861, 111);
			this->tabControlUpper->TabIndex = 0;
			// 
			// tpOutput
			// 
			this->tpOutput->AutoScroll = true;
			this->tpOutput->Controls->Add(this->tbOutput);
			this->tpOutput->Location = System::Drawing::Point(22, 4);
			this->tpOutput->Name = L"tpOutput";
			this->tpOutput->Padding = System::Windows::Forms::Padding(3);
			this->tpOutput->Size = System::Drawing::Size(835, 103);
			this->tpOutput->TabIndex = 0;
			this->tpOutput->Text = L"Output";
			this->tpOutput->UseVisualStyleBackColor = true;
			// 
			// tbOutput
			// 
			this->tbOutput->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tbOutput->Location = System::Drawing::Point(3, 3);
			this->tbOutput->Multiline = true;
			this->tbOutput->Name = L"tbOutput";
			this->tbOutput->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->tbOutput->Size = System::Drawing::Size(829, 97);
			this->tbOutput->TabIndex = 0;
			// 
			// tpWatch
			// 
			this->tpWatch->AutoScroll = true;
			this->tpWatch->Location = System::Drawing::Point(22, 4);
			this->tpWatch->Name = L"tpWatch";
			this->tpWatch->Padding = System::Windows::Forms::Padding(3);
			this->tpWatch->Size = System::Drawing::Size(835, 103);
			this->tpWatch->TabIndex = 1;
			this->tpWatch->Text = L"Watch";
			this->tpWatch->UseVisualStyleBackColor = true;
			// 
			// tSFunctions
			// 
			this->tSFunctions->AutoSize = false;
			this->tSFunctions->Dock = System::Windows::Forms::DockStyle::None;
			this->tSFunctions->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(14) {this->tsbNewFile, this->tsbLoadFile, 
				this->tsbSaveFile, this->tsbSaveAsFile, this->tsbCompile, this->tsbCompileTo, this->tsbStep, this->tsbEndDebug, this->tsbSetUp, 
				this->tspbLoadingLmds, this->tsbStoreTest, this->tsbRebuildTest, this->tsbFileDebug, this->tsbVersion});
			this->tSFunctions->Location = System::Drawing::Point(1, 0);
			this->tSFunctions->Name = L"tSFunctions";
			this->tSFunctions->Size = System::Drawing::Size(1115, 25);
			this->tSFunctions->TabIndex = 1;
			this->tSFunctions->Text = L"toolStrip1";
			// 
			// tsbNewFile
			// 
			this->tsbNewFile->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->tsbNewFile->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsbNewFile->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbNewFile.Image")));
			this->tsbNewFile->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbNewFile->Name = L"tsbNewFile";
			this->tsbNewFile->Size = System::Drawing::Size(23, 22);
			this->tsbNewFile->Tag = L"0";
			this->tsbNewFile->Text = L"New File";
			this->tsbNewFile->Click += gcnew System::EventHandler(this, &ldEditor::tsbFileClick);
			// 
			// tsbLoadFile
			// 
			this->tsbLoadFile->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsbLoadFile->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbLoadFile.Image")));
			this->tsbLoadFile->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbLoadFile->Name = L"tsbLoadFile";
			this->tsbLoadFile->Size = System::Drawing::Size(23, 22);
			this->tsbLoadFile->Tag = L"1";
			this->tsbLoadFile->Text = L"Load LMDS";
			this->tsbLoadFile->ToolTipText = L"Load LMDS";
			this->tsbLoadFile->Click += gcnew System::EventHandler(this, &ldEditor::tsbFileClick);
			// 
			// tsbSaveFile
			// 
			this->tsbSaveFile->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsbSaveFile->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbSaveFile.Image")));
			this->tsbSaveFile->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbSaveFile->Name = L"tsbSaveFile";
			this->tsbSaveFile->Size = System::Drawing::Size(23, 22);
			this->tsbSaveFile->Tag = L"2";
			this->tsbSaveFile->Text = L"save";
			this->tsbSaveFile->ToolTipText = L"Save LMDS ¡]Ctrl+S¡^";
			this->tsbSaveFile->Click += gcnew System::EventHandler(this, &ldEditor::tsbFileClick);
			// 
			// tsbSaveAsFile
			// 
			this->tsbSaveAsFile->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsbSaveAsFile->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbSaveAsFile.Image")));
			this->tsbSaveAsFile->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbSaveAsFile->Name = L"tsbSaveAsFile";
			this->tsbSaveAsFile->Size = System::Drawing::Size(23, 22);
			this->tsbSaveAsFile->Tag = L"3";
			this->tsbSaveAsFile->Text = L"Save As";
			this->tsbSaveAsFile->Click += gcnew System::EventHandler(this, &ldEditor::tsbFileClick);
			// 
			// tsbCompile
			// 
			this->tsbCompile->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsbCompile->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbCompile.Image")));
			this->tsbCompile->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbCompile->Name = L"tsbCompile";
			this->tsbCompile->Size = System::Drawing::Size(23, 22);
			this->tsbCompile->Tag = L"0";
			this->tsbCompile->Text = L"Compile ¡]Ctrl+B¡^";
			this->tsbCompile->ToolTipText = L"Compile ¡]Ctrl+B¡^";
			this->tsbCompile->Click += gcnew System::EventHandler(this, &ldEditor::tsbCompileClick);
			// 
			// tsbCompileTo
			// 
			this->tsbCompileTo->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsbCompileTo->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbCompileTo.Image")));
			this->tsbCompileTo->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbCompileTo->Name = L"tsbCompileTo";
			this->tsbCompileTo->Size = System::Drawing::Size(23, 22);
			this->tsbCompileTo->Tag = L"1";
			this->tsbCompileTo->Text = L"Compile To";
			this->tsbCompileTo->ToolTipText = L"Compile To";
			this->tsbCompileTo->Click += gcnew System::EventHandler(this, &ldEditor::tsbCompileClick);
			// 
			// tsbStep
			// 
			this->tsbStep->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsbStep->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbStep.Image")));
			this->tsbStep->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbStep->Name = L"tsbStep";
			this->tsbStep->Size = System::Drawing::Size(23, 22);
			this->tsbStep->Tag = L"2";
			this->tsbStep->Text = L"Step Debug¡]F10¡^";
			this->tsbStep->ToolTipText = L"Step Debug¡]F10¡^";
			this->tsbStep->Click += gcnew System::EventHandler(this, &ldEditor::tsbCompileClick);
			// 
			// tsbEndDebug
			// 
			this->tsbEndDebug->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsbEndDebug->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbEndDebug.Image")));
			this->tsbEndDebug->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbEndDebug->Name = L"tsbEndDebug";
			this->tsbEndDebug->Size = System::Drawing::Size(23, 22);
			this->tsbEndDebug->Tag = L"3";
			this->tsbEndDebug->Text = L"End Debug";
			this->tsbEndDebug->Click += gcnew System::EventHandler(this, &ldEditor::tsbCompileClick);
			// 
			// tsbSetUp
			// 
			this->tsbSetUp->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsbSetUp->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbSetUp.Image")));
			this->tsbSetUp->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbSetUp->Name = L"tsbSetUp";
			this->tsbSetUp->Size = System::Drawing::Size(23, 22);
			this->tsbSetUp->Text = L"Setup";
			this->tsbSetUp->ToolTipText = L"Setup";
			this->tsbSetUp->Click += gcnew System::EventHandler(this, &ldEditor::setupClick);
			// 
			// tspbLoadingLmds
			// 
			this->tspbLoadingLmds->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
			this->tspbLoadingLmds->Name = L"tspbLoadingLmds";
			this->tspbLoadingLmds->Size = System::Drawing::Size(100, 22);
			// 
			// tsbStoreTest
			// 
			this->tsbStoreTest->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsbStoreTest->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbStoreTest.Image")));
			this->tsbStoreTest->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbStoreTest->Name = L"tsbStoreTest";
			this->tsbStoreTest->Size = System::Drawing::Size(23, 22);
			this->tsbStoreTest->Text = L"STORE";
			// 
			// tsbRebuildTest
			// 
			this->tsbRebuildTest->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsbRebuildTest->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbRebuildTest.Image")));
			this->tsbRebuildTest->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbRebuildTest->Name = L"tsbRebuildTest";
			this->tsbRebuildTest->Size = System::Drawing::Size(23, 22);
			this->tsbRebuildTest->Text = L"REBUILD";
			// 
			// tsbFileDebug
			// 
			this->tsbFileDebug->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsbFileDebug->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbFileDebug.Image")));
			this->tsbFileDebug->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbFileDebug->Name = L"tsbFileDebug";
			this->tsbFileDebug->Size = System::Drawing::Size(23, 22);
			this->tsbFileDebug->Text = L"STEP DEBUG";
			// 
			// tsbVersion
			// 
			this->tsbVersion->Name = L"tsbVersion";
			this->tsbVersion->Size = System::Drawing::Size(95, 22);
			this->tsbVersion->Text = L"toolStripLabel1";
			// 
			// saveFileDialog
			// 
			this->saveFileDialog->InitialDirectory = L"files";
			// 
			// openFileDialog
			// 
			this->openFileDialog->InitialDirectory = L"files";
			// 
			// outCounterCount
			// 
			this->outCounterCount->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->outCounterCount->BackColor = System::Drawing::SystemColors::Control;
			this->outCounterCount->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->outCounterCount->Enabled = false;
			this->outCounterCount->FlatAppearance->BorderColor = System::Drawing::Color::DarkRed;
			this->outCounterCount->FlatAppearance->BorderSize = 2;
			this->outCounterCount->ImageKey = L"(none)";
			this->outCounterCount->Location = System::Drawing::Point(0, 350);
			this->outCounterCount->Margin = System::Windows::Forms::Padding(0);
			this->outCounterCount->Name = L"outCounterCount";
			this->outCounterCount->Size = System::Drawing::Size(120, 50);
			this->outCounterCount->TabIndex = 20;
			this->outCounterCount->Tag = L"0x0000000a";
			this->outCounterCount->UseVisualStyleBackColor = true;
			this->outCounterCount->Visible = false;
			// 
			// outCounterSet
			// 
			this->outCounterSet->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->outCounterSet->BackColor = System::Drawing::SystemColors::Control;
			this->outCounterSet->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->outCounterSet->Enabled = false;
			this->outCounterSet->FlatAppearance->BorderColor = System::Drawing::Color::DarkRed;
			this->outCounterSet->FlatAppearance->BorderSize = 2;
			this->outCounterSet->ImageKey = L"(none)";
			this->outCounterSet->Location = System::Drawing::Point(0, 300);
			this->outCounterSet->Margin = System::Windows::Forms::Padding(0);
			this->outCounterSet->Name = L"outCounterSet";
			this->outCounterSet->Size = System::Drawing::Size(120, 50);
			this->outCounterSet->TabIndex = 19;
			this->outCounterSet->Tag = L"0x00000009";
			this->outCounterSet->UseVisualStyleBackColor = true;
			this->outCounterSet->Visible = false;
			// 
			// ldEditor
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(1264, 682);
			this->Controls->Add(this->tSFunctions);
			this->Controls->Add(this->tlpMainframe);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->KeyPreview = true;
			this->Location = System::Drawing::Point(350, 0);
			this->MaximumSize = System::Drawing::Size(1280, 1080);
			this->MinimumSize = System::Drawing::Size(1280, 720);
			this->Name = L"ldEditor";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"XX";
			this->Load += gcnew System::EventHandler(this, &ldEditor::ldeLoading);
			this->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &ldEditor::ldeFileDragIn);
			this->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &ldEditor::ldeFileDragEnter);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &ldEditor::keyDownHandler);
			this->tlpMainframe->ResumeLayout(false);
			this->tlpMainframe->PerformLayout();
			this->tlpOdpanel->ResumeLayout(false);
			this->pnlRung->ResumeLayout(false);
			this->pnlRung->PerformLayout();
			this->splitContainerUpper->Panel1->ResumeLayout(false);
			this->splitContainerUpper->Panel1->PerformLayout();
			this->splitContainerUpper->Panel2->ResumeLayout(false);
			this->splitContainerUpper->ResumeLayout(false);
			this->tlpInpDevice->ResumeLayout(false);
			this->tabControlUpper->ResumeLayout(false);
			this->tpOutput->ResumeLayout(false);
			this->tpOutput->PerformLayout();
			this->tSFunctions->ResumeLayout(false);
			this->tSFunctions->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void ldeLoading(System::Object^  /*sender*/, System::EventArgs^  /*e*/) {
			 this->Text = L"New Ladder-Diagram";
			 //this->Text = String::Format("Rung: {0:D3}",10); //test
		 }
//----------------------
//	Part : Class-Wide Message Bus(input/output)
//		Key Message Manage , keyDown() event
//		Post On Output Tab
// Hsien 2013/5/20
//----------------------
private:	System::Void keyDownHandler	(System::Object^  sender,	System::Windows::Forms::KeyEventArgs^  e);
private:	int			 postOnOutput	(String^ message		,	String^ group);	// Hsien 2013/5/20
//----------------------------------------------------------------------------
//	Part : EDITING
//	selections , Following sections is possible to be replaced by Custom event
//		so that , would be much suitable in such event-driven infrastructre
//		commented by Hsien , 2012.07.03
//----------------------------------------------------------------------------
private: System::Void selectionInit		();	// Hsien 2013/5/19
private: System::Void outputSelect		(System::Object^  sender	, System::EventArgs^  e);		// collected by Hsien , 2015.01.15	
private: System::Void inputSelect		(System::Object^  sender	, System::EventArgs^  e);		// collected by Hsien , 2015.01.15
//------------------
//	Part : COMPILE/DEBUG
//	FILE output
//-----------------
		 int	ircGenerate				(Windows::Forms::TableLayoutControlCollection^ tlp/*in*/,FILE* outputFile/*out*/,const int mode);	// Hsien 2013/5/24
		 int	stepDebugMode			(Windows::Forms::TableLayoutControlCollection^ tlp/*in*/);	// Hsien 2013/5/24 , state-machine
//private: System::Void ircGenerate		(System::Object^  /*sender*/, System::EventArgs^  /*e*/);
private: System::Void rungSnMark		(System::Object^  /*sender*/, System::Windows::Forms::ControlEventArgs^  /*e*/);
//private: System::Void kdF10				(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
//private: System::Void tsbStepClick		(System::Object^  sender, System::EventArgs^  e);
//private: System::Void tsbEndDebugClick	(System::Object^  sender, System::EventArgs^  e);
		 int	compileAction			(const int tagIndex);	// Hsien 2013/5/24
private: System::Void tsbCompileClick	(System::Object^  sender, System::EventArgs^  e);	// Hsien 2013/5/20 , common handler for compile,compile-to,simu,simuStop
//------------------------------------------------------------
//	Part : FILE CONTROL
// data storing/rebuiling LMDS FILE , rung traverse functions , FILE I/O
//------------------------------------------------------------
private:
	void	newLD	(Windows::Forms::TableLayoutPanel^ parentTlp);
	int		store	(CLST_METADATA**		container);
	void	rebuild(const CLST_METADATA*	lmds);
	int		save					(String^			saveFileName/*in*/	/*,String^ resultOutput*//* used to redirecting the place to output message*/); // Hsien 2013/5/20
	int		openDialogGetFilename	(OpenFileDialog^	dialog, const int fileIndex);		// Hsien 2013/5/24											// Hsien 2013/5/20 , request file name from user
	int		saveDialogGetFilename	(SaveFileDialog^	dialog, const int fileIndex);												// Hsien 2013/5/20 , request file name from user
public:	int		load					(String^			openFileName/*out*/	/*,String^ resultOutput*//* used to redirecting the place to output message*/);
private: System::Void tsbFileClick	(System::Object^  /*sender*/, System::EventArgs^  /*e*/);		// Hsien 2013/5/20 , common handler for new,save,saveAs,load
//private: System::Void tsbNewClick	(System::Object^  /*sender*/, System::EventArgs^  /*e*/);
//private: System::Void tsbSaveClick		(System::Object^  /*sender*/, System::EventArgs^  /*e*/);
//private: System::Void tsbSaveAsClick	(System::Object^  /*sender*/, System::EventArgs^  /*e*/);	// Hsien 2013/5/20 
//private: System::Void tsbLoadClick	(System::Object^  /*sender*/, System::EventArgs^  /*e*/);
//------------------------------------------------------------
//	Part : CONFIGURATION
// data storing/rebuiling , rung traverse functions , FILE I/O
//------------------------------------------------------------
private: System::Void setupClick	(System::Object^  /*sender*/, System::EventArgs^  /*e*/);

//------------------------------
//	Valid When LDE is On Develop
//------------------------------
#if LDE_ON_DEVELOP
private: System::Void storeLMDS(System::Object^  sender, System::EventArgs^  e) {
			 this->store(this->metaDataContainer);
		 }
private: System::Void rebuildLMDS(System::Object^  sender, System::EventArgs^  e) {
			 this->rebuild(*this->metaDataContainer);
		 }
private: System::Void tsbFileDebugClick	(System::Object^  /*sender*/, System::EventArgs^  /*e*/){
			 std::string		fileName;				
			 FILE*			sourceFile = NULL;
			 
			 this->openFileDialog->FilterIndex = LDE_FILE_INDEX_IRC;
			 this->openFileDialog->FileName = nullptr;

			 if(this->openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK){
				 this->Text = this->openFileDialog->FileName;
				 MarshalString(this->openFileDialog->FileName,fileName);	// turns System String into standard string
				 
				 this->simulatorControl->load(fileName.c_str());		// instruction engaged , Hsien , 2012.01.03
			 }
		 }
#endif
private: System::Void ldeFileDragIn(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e);
private: System::Void ldeFileDragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e);
};// class
}//namespace

