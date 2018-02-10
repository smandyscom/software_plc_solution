#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace outDev {

	/// <summary>
	/// Summary for counterSetup
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class counterSetup : public System::Windows::Forms::Form
	{
	public:
		counterSetup(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			//this->dataArray = gcnew array<int>(3);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~counterSetup()
		{
			if (components)
			{
				delete components;
			}
		}
	public:
		int		counterIndex;		// Hsien 2013/5/21
		String^	counterInit;		// Hsien 2013/5/21
		String^ counterGoal;		// Hsien 2013/5/21
	private: System::Windows::Forms::NumericUpDown^  numericUpDownCounter;
	public: 

	public: 

		//array<int>^	dataArray;
					// 1. Index		Value
					// 2. Goal		Value
					// 3. Initial	Value
		int		refresh(){
			//-------------
			//	Post the Value on GUI
			//-------------
			//this->tbIndex->Text		= dataArray[TABINDEX_INDEX].ToString();
			//this->tbGoal->Text		= dataArray[TABINDEX_GOAL].ToString();
			//this->tbInitial->Text	= dataArray[TABINDEX_INITIAL].ToString();
			this->tbGoal->Text		= counterGoal;
			this->tbInitial->Text	= counterInit;

			//if(this->counterIndex < System::Decimal::ToInt32(this->numericUpDownCounter->Maximum))
				this->numericUpDownCounter->Value = this->counterIndex;
			//else{
			//	//----------------------
			//	//	Out of bound , marked as Red
			//	// Hsien 2013/5/21
			//	//----------------------
			//	this->numericUpDownCounter->Value = this->numericUpDownCounter->Maximum;
			//	this->numericUpDownCounter->ForeColor = Color::Red;
			//}
			// Hsien 2013/5/21

			return 1;
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  tbGoal;
	private: System::Windows::Forms::TextBox^  tbInitial;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Button^  btOk;
	private: System::Windows::Forms::Button^  btCancel;
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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->tbGoal = (gcnew System::Windows::Forms::TextBox());
			this->tbInitial = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->btOk = (gcnew System::Windows::Forms::Button());
			this->btCancel = (gcnew System::Windows::Forms::Button());
			this->numericUpDownCounter = (gcnew System::Windows::Forms::NumericUpDown());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownCounter))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(40, 10);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(44, 12);
			this->label1->TabIndex = 5;
			this->label1->Text = L"Index¡G";
			// 
			// tbGoal
			// 
			this->tbGoal->Location = System::Drawing::Point(90, 35);
			this->tbGoal->Name = L"tbGoal";
			this->tbGoal->Size = System::Drawing::Size(75, 22);
			this->tbGoal->TabIndex = 1;
			this->tbGoal->TextChanged += gcnew System::EventHandler(this, &counterSetup::tcInput);
			// 
			// tbInitial
			// 
			this->tbInitial->Location = System::Drawing::Point(90, 63);
			this->tbInitial->Name = L"tbInitial";
			this->tbInitial->Size = System::Drawing::Size(75, 22);
			this->tbInitial->TabIndex = 2;
			this->tbInitial->TextChanged += gcnew System::EventHandler(this, &counterSetup::tcInput);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(45, 38);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(39, 12);
			this->label2->TabIndex = 11;
			this->label2->Text = L"Goal¡G";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(40, 66);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(44, 12);
			this->label3->TabIndex = 12;
			this->label3->Text = L"Initial¡G";
			// 
			// btOk
			// 
			this->btOk->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->btOk->Location = System::Drawing::Point(9, 90);
			this->btOk->Name = L"btOk";
			this->btOk->Size = System::Drawing::Size(75, 23);
			this->btOk->TabIndex = 3;
			this->btOk->Text = L"OK";
			this->btOk->UseVisualStyleBackColor = true;
			// 
			// btCancel
			// 
			this->btCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btCancel->Location = System::Drawing::Point(90, 90);
			this->btCancel->Name = L"btCancel";
			this->btCancel->Size = System::Drawing::Size(75, 23);
			this->btCancel->TabIndex = 4;
			this->btCancel->Text = L"Cancel";
			this->btCancel->UseVisualStyleBackColor = true;
			// 
			// numericUpDownCounter
			// 
			this->numericUpDownCounter->Location = System::Drawing::Point(90, 8);
			this->numericUpDownCounter->Name = L"numericUpDownCounter";
			this->numericUpDownCounter->Size = System::Drawing::Size(75, 22);
			this->numericUpDownCounter->TabIndex = 13;
			this->numericUpDownCounter->ValueChanged += gcnew System::EventHandler(this, &counterSetup::ncInput);
			// 
			// counterSetup
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(174, 123);
			this->ControlBox = false;
			this->Controls->Add(this->numericUpDownCounter);
			this->Controls->Add(this->btCancel);
			this->Controls->Add(this->btOk);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->tbInitial);
			this->Controls->Add(this->tbGoal);
			this->Controls->Add(this->label1);
			this->Name = L"counterSetup";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Ring Counter Setup";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownCounter))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void tcInput(System::Object^ sender,System::EventArgs^ e){
			 Windows::Forms::TextBox^ tb 
				 = safe_cast<Windows::Forms::TextBox^>(sender);

			 //----------------------
			 //	
			 // Hsien 2013/5/21
			 //----------------------
			 switch(tb->TabIndex){
				 case TABINDEX_GOAL:
					 this->counterGoal = tb->Text;
					 break;
				 case TABINDEX_INITIAL:
					 this->counterInit = tb->Text;
					 break;
				 default:
					 break;
			 }
			 //try{
				// this->dataArray[tb->TabIndex]		= Int32::Parse(tb->Text);		// try to parse
				// tb->ForeColor = Color::Black;										// valid input, show as black
			 //}//try
			 //catch(FormatException^ ){
				// tb->ForeColor = Color::Red;
			 //}//catch
		 }
//private: System::Void btClick(System::Object^ sender,System::EventArgs^ e){
//			 Windows::Forms::Button^	bt
//				 = safe_cast<Windows::Forms::Button^>(sender);
//
//			 switch(bt->TabIndex){
//				 case TABINDEX_OK:		this->DialogResult = Windows::Forms::DialogResult::OK;		break;
//				 case TABINDEX_CANCEL:	this->DialogResult = Windows::Forms::DialogResult::Cancel;	break;
//				 default:
//					 break;
//			 }
//		 }
	private: System::Void ncInput(System::Object^  sender, System::EventArgs^  e) {
				 // Hsien 2013/5/21
				 this->counterIndex = System::Decimal::ToInt32(this->numericUpDownCounter->Value);
			 }
};
}
