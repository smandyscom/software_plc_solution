#pragma once


namespace testDriver {

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
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			image = new int;
			*image = 0;

			imageWord = new short;
			*imageWord = 0x8000;

			this->bit = gcnew _virtualDevice::virtualBitAccessor();
			this->bit->linkToImage(imageWord,0);
			this->bit->linkToImage(imageWord,1);
			this->tableLayoutPanel1->Controls->Add(this->bit,-1,-1);
			this->byte = gcnew _virtualDevice::virtualByteInteger();
			this->byte->linkToImage(imageWord,0);
			this->tableLayoutPanel1->Controls->Add(this->byte,-1,-1);
			this->Word = gcnew _virtualDevice::virtualWordInteger();
			this->Word->linkToImage(imageWord,0);
			this->tableLayoutPanel1->Controls->Add(this->Word,-1,-1);
			this->Maintain = gcnew _virtualDevice::virtualMaintainerControl();
			this->Maintain->registerImage(imageWord,0);
			this->Maintain->registerImage(imageWord,1);
			this->tableLayoutPanel1->Controls->Add(this->Maintain,-1,-1);
			this->Maintain->enableOverride(0);

			//this->bit->BeginInvoke(bit->bitRefresh);
			//this->bit->turnTheLabel(12);

			btn		= gcnew _virtualDevice::virtualButtonControl("X",(char*)&(image[3]),3,7);
			light	= gcnew _virtualDevice::virtualLightControl	("X",(char*)&(image[3]),3,7);

			this->tlpTest->Controls->Add(btn,-1,-1);
			this->tlpTest->Controls->Add(light,-1,-1);

//			dev = gcnew _virtualDevice::virtualDeviceControl();
			this->pnlVirDevice->Controls->Add(dev);

			//this->timer->Enabled = true;
			this->textBox1->Text = (*this->imageWord).ToString();

			//-------------------------
			//	Timer Initialize , waiting for launch
			//-------------------------
			System::Threading::TimerCallback^	timerCb	= 
				gcnew System::Threading::TimerCallback(
				this,									// arg1 : ?? what's that mean?
				&Form1::monitorsRenew);					// arg2 : the method would be delegate
			this->testTimer	= 
				gcnew System::Threading::Timer(
				timerCb,							// the delegate function
				nullptr,								// the object send to delegate function
				System::Threading::Timeout::Infinite,	// starting time , Infinite: disable
				15);							// interval
			
			//----------------------
			//	Start Timer
			//----------------------
			this->testTimer->Change(0,15);			// start the timer immediatly
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
		_virtualDevice::virtualButtonControl		^btn;
		_virtualDevice::virtualLightControl			^light;
		_virtualDevice::virtualDeviceControl		^dev;
		_virtualDevice::virtualBitAccessor			^bit;
		_virtualDevice::virtualByteInteger			^byte;
		_virtualDevice::virtualWordInteger			^Word;
		_virtualDevice::virtualMaintainerControl	^Maintain;

	private: delegate		/*System::Void*/int theDelegate	(const int /*System::String^ arg*/);
		System::Threading::Timer^			testTimer;

		int*	image;
		short*	imageWord;
	private: System::Windows::Forms::TableLayoutPanel^  tlpTest;
	private: System::Windows::Forms::Timer^  timer;
	private: System::Windows::Forms::Panel^  pnlVirDevice;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::ComponentModel::IContainer^  components;
	protected: 
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
			this->tlpTest = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->pnlVirDevice = (gcnew System::Windows::Forms::Panel());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->SuspendLayout();
			// 
			// tlpTest
			// 
			this->tlpTest->AutoSize = true;
			this->tlpTest->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->tlpTest->ColumnCount = 1;
			this->tlpTest->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tlpTest->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpTest->Location = System::Drawing::Point(13, 13);
			this->tlpTest->Name = L"tlpTest";
			this->tlpTest->RowCount = 2;
			this->tlpTest->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tlpTest->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tlpTest->Size = System::Drawing::Size(0, 0);
			this->tlpTest->TabIndex = 0;
			// 
			// timer
			// 
			this->timer->Tick += gcnew System::EventHandler(this, &Form1::timer_Tick);
			// 
			// pnlVirDevice
			// 
			this->pnlVirDevice->AutoScroll = true;
			this->pnlVirDevice->Location = System::Drawing::Point(12, 149);
			this->pnlVirDevice->Name = L"pnlVirDevice";
			this->pnlVirDevice->Size = System::Drawing::Size(490, 194);
			this->pnlVirDevice->TabIndex = 1;
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(402, 12);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(100, 22);
			this->textBox1->TabIndex = 2;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &Form1::textBox1_TextChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(363, 21);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(32, 12);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Value";
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->AutoSize = true;
			this->tableLayoutPanel1->ColumnCount = 1;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				20)));
			this->tableLayoutPanel1->Location = System::Drawing::Point(13, 12);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 1;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(200, 100);
			this->tableLayoutPanel1->TabIndex = 4;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(514, 355);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->pnlVirDevice);
			this->Controls->Add(this->tlpTest);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void timer_Tick(System::Object^  sender, System::EventArgs^  e) {
			this->light->setLight((char*)&(image[3]));
//			memcpy(this->dev->outImage,this->dev->inpImage,16);
//			this->dev->outDeviceRefresh();
			 }
	private: System::Void monitorsRenew(Object^){
				 //----------------------
				 //	Every Compoent beginInvoke here
				 //----------------------
				 this->bit->BeginInvoke(gcnew theDelegate(this->bit,&_virtualDevice::virtualBitAccessor::turnTheLabel),0);
				 this->byte->BeginInvoke(gcnew theDelegate(this->byte,&_virtualDevice::virtualByteInteger::showBytes),0);
				 this->Word->BeginInvoke(gcnew theDelegate(this->Word,&_virtualDevice::virtualWordInteger::showWord),0);
				 this->Maintain->BeginInvoke(gcnew theDelegate(this->Maintain,&_virtualDevice::virtualMaintainerControl::monitorRefresh),0);
			 }
	private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
				 try{
				 *this->imageWord = System::Convert::ToInt16(this->textBox1->Text,10);
				 }
				 catch(FormatException^){
				 }
			 }
};
}

