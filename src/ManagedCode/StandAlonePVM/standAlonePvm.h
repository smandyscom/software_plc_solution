#pragma once


namespace _standAlonePvm {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace _virtualDevice;
	using namespace procInterface;

	/// <summary>
	/// Summary for standAlonePvm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class standAlonePvm : public System::Windows::Forms::Form
	{
	public:
		standAlonePvm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			//----------------------
			//	Under Version COntrol via app.rc file// Hsien 2013/4/29
			//----------------------
			this->tsbVersion->Text =
				"Version: " + System::Windows::Forms::Application::ProductVersion;

			this->markPID(this->tlpLevel2,nullptr);

			//-------------------------------------
			//	Initialz Data-Base for watch window
			//-------------------------------------
			this->_db	= new sqlite3*;
			*this->_db	= NULL;
			if(dqiInit(this->_db,"PLC.DB") != DQI_SUCCESS)
				MessageBox::Show("Data Base Missing");

			this->pvmInstance = new PVM;
			//pvmInitialize(
			//	this->pvmInstance,
			//	SAP_TIME_BASE,
			//	(int(*)(void*))startWithRs,		// forced-casting
			//	*_db/*(void*)&filename*/);						// the start script

			//-------------------------
			//	Timer Initialize , waiting for launch
			//-------------------------
			System::Threading::TimerCallback^	timerCb	= 
				gcnew System::Threading::TimerCallback(
				this,									// arg1 : ?? what's that mean?
				&standAlonePvm::pvmRunMethod);					// arg2 : the method would be delegate
			this->timerPvm	= 
				gcnew System::Threading::Timer(
				timerCb,							// the delegate function
				nullptr,								// the object send to delegate function
				System::Threading::Timeout::Infinite,	// starting time , Infinite: disable
				SAP_TIME_BASE);							// interval

			//--------------------------------------
			//	Set-up The Level-1 Process Attribute
			//--------------------------------------
			procInterfaceControlLevel1 = gcnew procInterfaceControl(PIC_LEVEL1);
			this->procInterfaceControlLevel1->markPid(PVM_PRIORITY_LEVEL1);
			this->procInterfaceControlLevel1->tbLevel->Text = "1";
			this->pnlLevel1->Controls->Add(procInterfaceControlLevel1);
			//--------------------------------------
			//	Set-up The Level-2 Process Attribute
			//--------------------------------------
			this->tlpLevel2->Controls->Add(gcnew procInterfaceControl(PIC_LEVEL2),-1,-1);
			//--------------------------------------
			//	Initializing Virtual Device
			//--------------------------------------
			this->_virDev		= gcnew virtualDeviceControl(
				virInpImage,VIR_INP_IMG_SIZE,
				virOutImage,VIR_OUT_IMG_SIZE);
			this->pn1Vir->Controls->Add(this->_virDev);
//			this->_setupForm	= gcnew sapSetup();	// Hsien 2013/5/2
			//-------------------------------------
			//	Driver Initializing , 
			//-------------------------------------
			driverInit();								// clear the memory
			driverAssign(startIODevice,_DEVICE_START);	// Hsien 2013/5/2

			this->textBoxRenew(L"SAP Stopped");
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~standAlonePvm()
		{
			FSMI::finalize();	// Hsien 2013/5/15
			closeMachine();		// Hsien 2013/5/2
			if (components)
			{
				delete components;
			}
		}
		//------------------------------
		//
		//------------------------------
		PVM*								pvmInstance;
		System::Threading::Timer^			timerPvm;
		virtualDeviceControl^				_virDev;
//		int									_currentDevCode;	// to record current main-output device
//		sapSetup^							_setupForm;
//		int									_deviceState;		// to indicate the state of drive
//		void**								_deviceStartScrips; // the list stored device start scripts
		sqlite3**		_db;					// the data-base instance owned by this control
												// not implement yet, Hsien , 2013.01.17

		procInterfaceControl^				procInterfaceControlLevel1;

	private: System::Windows::Forms::TableLayoutPanel^  tlpLevel2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::ToolStrip^  tsSetup;
	private: System::Windows::Forms::ToolStripButton^  tsPvmStart;
	private: System::Windows::Forms::ToolStripButton^  tsPvmStop;
	private: System::Windows::Forms::ToolStripButton^  tsSave;
	private: System::Windows::Forms::ToolStripButton^  tsLoad;
	private: System::Windows::Forms::TextBox^  tbPvmStatus;
	private: System::Windows::Forms::ToolStripButton^  tsbSetup;
private: System::Windows::Forms::Panel^  pn1Vir;
private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::Label^  label7;
private: System::Windows::Forms::Label^  label8;
private: System::Windows::Forms::Panel^  pnlLevel1;
private: System::Windows::Forms::ToolStripLabel^  tsbVersion;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(standAlonePvm::typeid));
			this->tlpLevel2 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->tsSetup = (gcnew System::Windows::Forms::ToolStrip());
			this->tsPvmStart = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsPvmStop = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsSave = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsLoad = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbSetup = (gcnew System::Windows::Forms::ToolStripButton());
			this->tbPvmStatus = (gcnew System::Windows::Forms::TextBox());
			this->pn1Vir = (gcnew System::Windows::Forms::Panel());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->pnlLevel1 = (gcnew System::Windows::Forms::Panel());
			this->tsbVersion = (gcnew System::Windows::Forms::ToolStripLabel());
			this->tsSetup->SuspendLayout();
			this->SuspendLayout();
			// 
			// tlpLevel2
			// 
			this->tlpLevel2->AutoScroll = true;
			this->tlpLevel2->AutoSize = true;
			this->tlpLevel2->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->tlpLevel2->ColumnCount = 1;
			this->tlpLevel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 610)));
			this->tlpLevel2->Location = System::Drawing::Point(12, 132);
			this->tlpLevel2->MaximumSize = System::Drawing::Size(610, 180);
			this->tlpLevel2->Name = L"tlpLevel2";
			this->tlpLevel2->RowCount = 1;
			this->tlpLevel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 36)));
			this->tlpLevel2->Size = System::Drawing::Size(610, 36);
			this->tlpLevel2->TabIndex = 1;
			this->tlpLevel2->ControlAdded += gcnew System::Windows::Forms::ControlEventHandler(this, &standAlonePvm::markPID);
			this->tlpLevel2->ControlRemoved += gcnew System::Windows::Forms::ControlEventHandler(this, &standAlonePvm::markPID);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(16, 56);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(23, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"PID";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(70, 56);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(29, 12);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Time";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(170, 56);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(47, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Filename";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(399, 56);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(31, 12);
			this->label4->TabIndex = 5;
			this->label4->Text = L"Level";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(451, 56);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(23, 12);
			this->label5->TabIndex = 6;
			this->label5->Text = L"Slot";
			// 
			// tsSetup
			// 
			this->tsSetup->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->tsPvmStart, this->tsPvmStop, 
				this->tsSave, this->tsLoad, this->tsbSetup, this->tsbVersion});
			this->tsSetup->Location = System::Drawing::Point(0, 0);
			this->tsSetup->Name = L"tsSetup";
			this->tsSetup->Size = System::Drawing::Size(624, 25);
			this->tsSetup->TabIndex = 9;
			this->tsSetup->Text = L"toolStrip1";
			// 
			// tsPvmStart
			// 
			this->tsPvmStart->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsPvmStart->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsPvmStart.Image")));
			this->tsPvmStart->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsPvmStart->Name = L"tsPvmStart";
			this->tsPvmStart->Size = System::Drawing::Size(23, 22);
			this->tsPvmStart->Tag = L"0";
			this->tsPvmStart->Text = L"Start";
			this->tsPvmStart->Click += gcnew System::EventHandler(this, &standAlonePvm::btClick);
			// 
			// tsPvmStop
			// 
			this->tsPvmStop->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsPvmStop->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsPvmStop.Image")));
			this->tsPvmStop->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsPvmStop->Name = L"tsPvmStop";
			this->tsPvmStop->Size = System::Drawing::Size(23, 22);
			this->tsPvmStop->Tag = L"1";
			this->tsPvmStop->Text = L"Stop";
			this->tsPvmStop->Click += gcnew System::EventHandler(this, &standAlonePvm::btClick);
			// 
			// tsSave
			// 
			this->tsSave->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsSave->Enabled = false;
			this->tsSave->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsSave.Image")));
			this->tsSave->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsSave->Name = L"tsSave";
			this->tsSave->Size = System::Drawing::Size(23, 22);
			this->tsSave->Tag = L"3";
			this->tsSave->Text = L"toolStripButton3";
			// 
			// tsLoad
			// 
			this->tsLoad->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsLoad->Enabled = false;
			this->tsLoad->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsLoad.Image")));
			this->tsLoad->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsLoad->Name = L"tsLoad";
			this->tsLoad->Size = System::Drawing::Size(23, 22);
			this->tsLoad->Tag = L"4";
			this->tsLoad->Text = L"toolStripButton1";
			// 
			// tsbSetup
			// 
			this->tsbSetup->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsbSetup->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbSetup.Image")));
			this->tsbSetup->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbSetup->Name = L"tsbSetup";
			this->tsbSetup->Size = System::Drawing::Size(23, 22);
			this->tsbSetup->Tag = L"2";
			this->tsbSetup->Text = L"Setup";
			this->tsbSetup->Click += gcnew System::EventHandler(this, &standAlonePvm::btClick);
			// 
			// tbPvmStatus
			// 
			this->tbPvmStatus->Location = System::Drawing::Point(521, 3);
			this->tbPvmStatus->Name = L"tbPvmStatus";
			this->tbPvmStatus->Size = System::Drawing::Size(100, 22);
			this->tbPvmStatus->TabIndex = 10;
			// 
			// pn1Vir
			// 
			this->pn1Vir->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->pn1Vir->AutoScroll = true;
			this->pn1Vir->AutoSize = true;
			this->pn1Vir->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->pn1Vir->Location = System::Drawing::Point(612, 430);
			this->pn1Vir->Name = L"pn1Vir";
			this->pn1Vir->Size = System::Drawing::Size(0, 0);
			this->pn1Vir->TabIndex = 11;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"PMingLiU", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->label6->Location = System::Drawing::Point(9, 110);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(69, 16);
			this->label6->TabIndex = 12;
			this->label6->Text = L"LEVEL2";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"PMingLiU", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label7->Location = System::Drawing::Point(8, 36);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(69, 16);
			this->label7->TabIndex = 13;
			this->label7->Text = L"LEVEL1";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(500, 56);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(51, 12);
			this->label8->TabIndex = 14;
			this->label8->Text = L"Comment";
			// 
			// pnlLevel1
			// 
			this->pnlLevel1->AutoSize = true;
			this->pnlLevel1->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->pnlLevel1->Location = System::Drawing::Point(11, 72);
			this->pnlLevel1->Name = L"pnlLevel1";
			this->pnlLevel1->Size = System::Drawing::Size(0, 0);
			this->pnlLevel1->TabIndex = 15;
			// 
			// tsbVersion
			// 
			this->tsbVersion->Name = L"tsbVersion";
			this->tsbVersion->Size = System::Drawing::Size(95, 22);
			this->tsbVersion->Text = L"toolStripLabel1";
			// 
			// standAlonePvm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(624, 442);
			this->Controls->Add(this->pnlLevel1);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->pn1Vir);
			this->Controls->Add(this->tbPvmStatus);
			this->Controls->Add(this->tsSetup);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->tlpLevel2);
			this->Name = L"standAlonePvm";
			this->Text = L"Stand-Alone PVM";
			this->tsSetup->ResumeLayout(false);
			this->tsSetup->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void markPID(System::Object^  sender, System::Windows::Forms::ControlEventArgs^  e) {
				 int									count = PVM_PRIORITY_LEVEL2;
				 procInterface::procInterfaceControl^	hdl;

				 for each(Control^ e in safe_cast<Windows::Forms::TableLayoutPanel^>(sender)->Controls){
					 hdl = safe_cast<procInterface::procInterfaceControl^>(e);
					 hdl->markPid(count);
					 //-----------------------
					 //	Show-up Level and Slot
					 //-----------------------
					 hdl->tbLevel->Text		= "2";
					 hdl->tbSlot->Text		= (count-1).ToString();

					 count++;
				 }
			 }
		 //-------------
		 //	Timer Thread
		 //-------------
private: System::Void pvmRunMethod(System::Object^ obj){
				 //--------------------------------------
				 //	The Method would be Delegate by Timer
				 //--------------------------------------
			 this->textBoxRenew(L"SAP Started");								//
			 FSMI::RSMI::runAllRoutineService();						// scan in/out all images
			 pvmRunTick(this->pvmInstance,PVM_MODE_CONVENTIAL);			// pvm
			 virOutRenew(nullptr);										// to drive the virtual device to show 
																		// Hsien , 2013.01.29
		 }
		 //-----------
		 //	Management Thread
		 //-----------
private: 
	System::Void	btClick	(System::Object^  sender, System::EventArgs^  e);
	int				procStart();
	int				procStop();
//	int				devSetup();
		 //---------------------------------
		 //	Monitoring/Virtual Device Thread
		 //---------------------------------
private: delegate		System::Void theDelegate	(System::String^ arg);
private: System::Void	textBoxRenew				(System::String^ arg){
			 if(this->tbPvmStatus->InvokeRequired == true/* if the obj is not own by this Thread , need to delegate*/){
				 //------------------------------------------
				 // wait for UI-thread response , function return until UI-thread finished the required work
				 // indeterministic
				 //		BeginInvoke : Thread would contibue to work
				 //		Invoke		: Thread would fall into waiting until other thread finish its work
				 //------------------------------------------
				 this->tbPvmStatus->BeginInvoke(gcnew standAlonePvm::theDelegate(this,&standAlonePvm::textBoxRenew),arg);		
			 }
			 else{
				 //----------------------------------------------------------
				 // if the obj is owned by timer Thread , 
				 //		rewrite-it without delegating
				 //-----------------------------------------------------------
				 this->tbPvmStatus->Text = arg;
			 }
		 }
private: System::Void	virOutRenew					(System::String^ arg){
			 //------------------------
			 //	Delegate To Drive Virtual Device to Show
			 //------------------------
			 this->_virDev->BeginInvoke(gcnew standAlonePvm::theDelegate(this->_virDev,&virtualDeviceControl::outDeviceRefresh),arg);
		 }
};
}

