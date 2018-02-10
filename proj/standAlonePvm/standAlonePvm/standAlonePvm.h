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

//			this->markPID(this->tlpLevel2,nullptr);

			//-------------------------------------
			//	Initialz Data-Base for watch window
			//-------------------------------------
			this->_db	= new sqlite3*;
			*this->_db	= NULL;
			if(dqiInit(this->_db,"PLC.DB") != DQI_SUCCESS)
				MessageBox::Show("Data Base Missing");

			this->pvmInstance = new PVM;

			//-------------------------
			//	Timer PVM Initialize , waiting for launch
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

			//-------------------------
			//	Timer Monitor Initialize , waiting for launch
			//-------------------------
			timerCb	= 
				gcnew System::Threading::TimerCallback(
				this,									// arg1 : ?? what's that mean?
				&standAlonePvm::monitorRefreshMethod);					// arg2 : the method would be delegate
			this->timerMonitor	= 
				gcnew System::Threading::Timer(
				timerCb,							// the delegate function
				nullptr,								// the object send to delegate function
				System::Threading::Timeout::Infinite,	// starting time , Infinite: disable
				SAP_MONITOR_TIME_BASE);							// interval
			//----------------------
			//	According to MSDN , it recommened using form timer rather than threading timer
			//		http://msdn.microsoft.com/zh-tw/library/system.threading.timer(v=vs.110).aspx
			// Hsien 2013/11/18	
			//----------------------
			//winFormTimerPvm			= gcnew System::Windows::Forms::Timer();
			//winFormTimerMonitor		= gcnew System::Windows::Forms::Timer();
			//this->winFormTimerPvm->Interval = SAP_TIME_BASE;
			//this->winFormTimerMonitor->Interval = SAP_MONITOR_TIME_BASE;
			//this->winFormTimerPvm->Tick += gcnew System::EventHandler(this,&standAlonePvm::pvmRunMethod);
			//this->winFormTimerMonitor->Tick += gcnew System::EventHandler(this,&standAlonePvm::monitorRefreshMethod);
			//this->mut = gcnew System::Threading::Mutex(false);

			//----------------------
			//	Presetting Tab Directory Path
			// Hsien 2013/12/2
			//----------------------
			pvmProcDirPreset(".//example//");	// Hsien 2013/12/2 , current directory , related to 
			this->defaultFp = NULL;
			this->defaultFp = fopen("default.tab","rb");
			if(this->defaultFp == NULL)
				this->textBoxRenew("Tab Not Found");
			else{
				fclose(this->defaultFp);
				this->tbTab->Text = System::IO::File::ReadAllText("default.tab");
			}
		
			//--------------------------------------
			//	Set-up The Level-1 Process Attribute
			//--------------------------------------
			//procInterfaceControlLevel1 = gcnew procInterfaceControl(PIC_LEVEL1);
			//this->procInterfaceControlLevel1->markPid(PVM_PRIORITY_LEVEL1);
			//this->procInterfaceControlLevel1->tbLevel->Text = "1";
			//this->pnlLevel1->Controls->Add(procInterfaceControlLevel1);
			//--------------------------------------
			//	Set-up The Level-2 Process Attribute
			//--------------------------------------
			//this->tlpLevel2->Controls->Add(gcnew procInterfaceControl(PIC_LEVEL2),-1,-1);
			//--------------------------------------
			//	Initializing Virtual Device
			//--------------------------------------
			//this->_virDev		= gcnew virtualDeviceControl(
			//	virInpImage,VIR_INP_IMG_SIZE,
			//	virOutImage,VIR_OUT_IMG_SIZE);
			//this->pn1Vir->Controls->Add(this->_virDev);
//			this->_setupForm	= gcnew sapSetup();	// Hsien 2013/5/2
			//-------------------------------------
			//	Driver Initializing , 
			//-------------------------------------
			driverInit();								// clear the memory
			driverAssign(ps400Start/*startMachine*//*startIODevice*/,_DEVICE_START);	// Hsien 2013/5/2

			//----------------------
			//	Restart PVM
			//----------------------
			pvmInitialize(
				this->pvmInstance,
				SAP_TIME_BASE,
				(int(*)(void*))startWithRs,		// forced-casting
				*_db/*(void*)&filename*/);						// the start script

			//----------------------
			//	Monitor Initializing:
			//		1. gcnew Monitor
			//		2. link to image
			//		3. register name
			//		4. adding into TLP
			//----------------------
			this->monitorInit(*_db);

			//----------------------
			//	Launch Timer 
			//----------------------
			this->textBoxRenew(L"SAP Stopped");
//			this->timerMonitor->Change(0,SAP_MONITOR_TIME_BASE);
			dqiClose(*_db);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~standAlonePvm()
		{
			//FSMI::finalize();	// Hsien 2013/5/15
			//----------------------
			//	Finalize PVM
			//----------------------
			pvmFinalize(this->pvmInstance
				,(int(*)(void*))stopWithFileOut
				,(void*)NULL);

			delete this->_db;
			delete this->pvmInstance;
	//		closeMachine();		// Hsien 2013/5/2
			if (components)
			{
				delete components;
			}
		}
		//------------------------------
		//
		//------------------------------
		PVM*								pvmInstance;

//		virtualDeviceControl^				_virDev;
//		int									_currentDevCode;	// to record current main-output device
//		sapSetup^							_setupForm;
//		int									_deviceState;		// to indicate the state of drive
//		void**								_deviceStartScrips; // the list stored device start scripts
		sqlite3**		_db;					// the data-base instance owned by this control
												// not implement yet, Hsien , 2013.01.17

//		procInterfaceControl^				procInterfaceControlLevel1;

		//----------------------
		//	Not stable
		// Hsien 2013/11/18
		//----------------------
		System::Threading::Timer^			timerPvm;
		System::Threading::Timer^			timerMonitor;	// Hsien 2013/6/4 , for showing up monitor

		//System::Windows::Forms::Timer^		winFormTimerPvm;
		//System::Windows::Forms::Timer^		winFormTimerMonitor;
		//System::Threading::Mutex^			mut;

		//----------------------
		//	Default Tab file
		//----------------------
		FILE*	defaultFp;

private: System::Windows::Forms::CheckBox^  cbOverride;
private: System::Windows::Forms::ToolStrip^  tsSetup;
private: System::Windows::Forms::ToolStripButton^  tsPvmStart;
private: System::Windows::Forms::ToolStripButton^  tsPvmStop;
private: System::Windows::Forms::ToolStripButton^  tsSave;
private: System::Windows::Forms::ToolStripButton^  tsLoad;
private: System::Windows::Forms::TextBox^  tbPvmStatus;
private: System::Windows::Forms::ToolStripButton^  tsbSetup;
private: System::Windows::Forms::Panel^  pn1Vir;
private: System::Windows::Forms::Panel^  pnlLevel1;
private: System::Windows::Forms::ToolStripLabel^  tsbVersion;
private: System::Windows::Forms::TabControl^  tcMonitor;
private: System::Windows::Forms::TextBox^  tbTab;
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
			this->tsSetup = (gcnew System::Windows::Forms::ToolStrip());
			this->tsPvmStart = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsPvmStop = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsSave = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsLoad = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbSetup = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbVersion = (gcnew System::Windows::Forms::ToolStripLabel());
			this->tbPvmStatus = (gcnew System::Windows::Forms::TextBox());
			this->pn1Vir = (gcnew System::Windows::Forms::Panel());
			this->pnlLevel1 = (gcnew System::Windows::Forms::Panel());
			this->tcMonitor = (gcnew System::Windows::Forms::TabControl());
			this->cbOverride = (gcnew System::Windows::Forms::CheckBox());
			this->tbTab = (gcnew System::Windows::Forms::TextBox());
			this->tsSetup->SuspendLayout();
			this->SuspendLayout();
			// 
			// tsSetup
			// 
			this->tsSetup->Dock = System::Windows::Forms::DockStyle::None;
			this->tsSetup->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->tsPvmStart, this->tsPvmStop, 
				this->tsSave, this->tsLoad, this->tsbSetup, this->tsbVersion});
			this->tsSetup->Location = System::Drawing::Point(0, 0);
			this->tsSetup->Name = L"tsSetup";
			this->tsSetup->Size = System::Drawing::Size(222, 25);
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
			// tsbVersion
			// 
			this->tsbVersion->Name = L"tsbVersion";
			this->tsbVersion->Size = System::Drawing::Size(95, 22);
			this->tsbVersion->Text = L"toolStripLabel1";
			// 
			// tbPvmStatus
			// 
			this->tbPvmStatus->Location = System::Drawing::Point(225, 0);
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
			this->pn1Vir->Location = System::Drawing::Point(877, 550);
			this->pn1Vir->Name = L"pn1Vir";
			this->pn1Vir->Size = System::Drawing::Size(0, 0);
			this->pn1Vir->TabIndex = 11;
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
			// tcMonitor
			// 
			this->tcMonitor->Location = System::Drawing::Point(404, 3);
			this->tcMonitor->Multiline = true;
			this->tcMonitor->Name = L"tcMonitor";
			this->tcMonitor->SelectedIndex = 0;
			this->tcMonitor->Size = System::Drawing::Size(480, 547);
			this->tcMonitor->TabIndex = 16;
			// 
			// cbOverride
			// 
			this->cbOverride->AutoSize = true;
			this->cbOverride->Location = System::Drawing::Point(307, 534);
			this->cbOverride->Name = L"cbOverride";
			this->cbOverride->Size = System::Drawing::Size(91, 16);
			this->cbOverride->TabIndex = 17;
			this->cbOverride->Text = L"Forced Output";
			this->cbOverride->UseVisualStyleBackColor = true;
			this->cbOverride->CheckedChanged += gcnew System::EventHandler(this, &standAlonePvm::overrideToggle);
			// 
			// tbTab
			// 
			this->tbTab->AcceptsReturn = true;
			this->tbTab->AcceptsTab = true;
			this->tbTab->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->tbTab->Location = System::Drawing::Point(11, 28);
			this->tbTab->Multiline = true;
			this->tbTab->Name = L"tbTab";
			this->tbTab->Size = System::Drawing::Size(387, 500);
			this->tbTab->TabIndex = 18;
			this->tbTab->WordWrap = false;
			// 
			// standAlonePvm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(889, 562);
			this->Controls->Add(this->tbTab);
			this->Controls->Add(this->tcMonitor);
			this->Controls->Add(this->cbOverride);
			this->Controls->Add(this->pnlLevel1);
			this->Controls->Add(this->pn1Vir);
			this->Controls->Add(this->tbPvmStatus);
			this->Controls->Add(this->tsSetup);
			this->Name = L"standAlonePvm";
			this->Text = L"Stand-Alone PVM";
			this->Load += gcnew System::EventHandler(this, &standAlonePvm::afterSAPLoaded);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &standAlonePvm::afterSAPClosed);
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
		 //	Timer PVM Thread
		 //-------------
private: System::Void pvmRunMethod(System::Object^ obj/*,System::EventArgs^*/){
				 //--------------------------------------
				 //	The Method would be Delegate by Timer
				 //--------------------------------------
//			 this->mut->WaitOne();
			 FSMI::RSMI::runAllRoutineService();						// scan in/out all images
//			 pvmRunTick(this->pvmInstance,PVM_MODE_CONVENTIAL);			// pvm
//			 pvmRunPriority(this->pvmInstance,0);						// Hsien 2013/7/22 , normalized to Priority mode with CNC
			 //----------------------
			 //
			 // Hsien 2013/12/3
			 //----------------------
			 pvmRunPriority2(this->pvmInstance,0);
//			 virOutRenew(nullptr);										// to drive the virtual device to show 
//			 this->mut->ReleaseMutex();
																		// Hsien , 2013.01.29
		 }
		 //-------------
		 //	Timer Monitor Thread
		 //-------------
private: System::Void monitorRefreshMethod(System::Object^ obj/*,System::EventArgs^*/){ 
				 //--------------------------------------
				 //	The Method would be Delegate by Timer
				 //--------------------------------------
			 for each(Windows::Forms::TabPage^ e in this->tcMonitor->Controls){
				 for each(virtualMaintainerControl^ e in safe_cast<Windows::Forms::TableLayoutPanel^>(e->Controls[0])->Controls)
					 e->Invoke/*BeginInvoke*/(gcnew virtualDeviceCommonWord::delegatePrototype/*monitorRefreshMethod*/(e,&virtualMaintainerControl::monitorRefresh),0/*nullptr*//*0*/);
			 }//outter for 
		 }
		 //-----------
		 //	Management Thread
		 //-----------
private: 
	System::Void	btClick	(System::Object^  sender, System::EventArgs^  e);
	int				procStart();
	int				procStop();
//	int				devSetup();
	//----------------------
	//
	//----------------------
protected:
			int monitorInit(void* arg);
			int	startService(void)	;		// Hsien 2013/6/5 , including timer start
			int	stopService(void)	;		// includeing timer stop , clean FSMI
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
//private: System::Void	virOutRenew					(System::String^ arg){
//			 //------------------------
//			 //	Delegate To Drive Virtual Device to Show
//			 //------------------------
//			 this->_virDev->BeginInvoke(gcnew standAlonePvm::theDelegate(this->_virDev,&virtualDeviceControl::outDeviceRefresh),arg);
//			 //for each(virtualMaintainerControl^ e in this->tlpMonitor->Controls)
//				// e->BeginInvoke(gcnew virtualDeviceCommonWord::monitorRefreshMethod(e,&virtualMaintainerControl::monitorRefresh),0);
//		 }
private: System::Void afterSAPLoaded(System::Object^  sender, System::EventArgs^  e) {
			 //----------------------
			 //	Should be Launch after Window loaded
			 //----------------------
			 //-------------------
			 //	Start The Device
			 //-------------------
			 startDevice();
			 this->startService();// Hsien 2013/6/5
		 }
private: System::Void afterSAPClosed(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			 //----------------------
			 //	Should be Launch after Window Closed
			 //----------------------
			 //-------------------
			 //	Stop The Device
			 //-------------------
			 this->stopService();
			 stopDevice();
		 }
private: System::Void overrideToggle(System::Object^  sender, System::EventArgs^  e) {
			 //----------------------
			 //
			 //----------------------
			 this->stopService();
			 if(this->cbOverride->Checked){
				 overrideEnable();
				 this->tsPvmStart->Enabled	= false;
				 this->tsPvmStop->Enabled	= false;
			 }
			 else{
				 overrideDisable();
				 this->tsPvmStart->Enabled	= true;
				 this->tsPvmStop->Enabled	= true;
			 }
			 this->startService();
		 }

};
}

