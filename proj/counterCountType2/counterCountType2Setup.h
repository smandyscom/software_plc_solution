#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace outDev {

	/// <summary>
	/// Summary for counterCountType2Setup
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class counterCountType2Setup : public System::Windows::Forms::Form
	{
	public:
		counterCountType2Setup(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			counterData = gcnew array<String^>(DATA_COUNTS);
		}
	public:
		int					counterIndex;
		array<String^>^		counterData;
						// TABINDEX_GOAL		0
						// TABINDEX_INITIAL	1
						// TABINDEX_STEP		2
						// Hsien 2013/5/24
		int		refresh(){
			this->tbGoal->Text		= counterData[TABINDEX_GOAL];
			this->tbInitial->Text	= counterData[TABINDEX_INITIAL];
			this->tbStep->Text		= counterData[TABINDEX_STEP];

			this->numericUpDownCounter->Value = this->counterIndex;

			return 1;
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~counterCountType2Setup()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::NumericUpDown^  numericUpDownCounter;
	private: System::Windows::Forms::Button^  btCancel;
	private: System::Windows::Forms::Button^  btOk;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  tbInitial;
	private: System::Windows::Forms::TextBox^  tbGoal;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  tbStep;
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
			this->numericUpDownCounter = (gcnew System::Windows::Forms::NumericUpDown());
			this->btCancel = (gcnew System::Windows::Forms::Button());
			this->btOk = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->tbInitial = (gcnew System::Windows::Forms::TextBox());
			this->tbGoal = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->tbStep = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownCounter))->BeginInit();
			this->SuspendLayout();
			// 
			// numericUpDownCounter
			// 
			this->numericUpDownCounter->Location = System::Drawing::Point(91, 7);
			this->numericUpDownCounter->Name = L"numericUpDownCounter";
			this->numericUpDownCounter->Size = System::Drawing::Size(75, 22);
			this->numericUpDownCounter->TabIndex = 4;
			this->numericUpDownCounter->ValueChanged += gcnew System::EventHandler(this, &counterCountType2Setup::ncInput);
			// 
			// btCancel
			// 
			this->btCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btCancel->Location = System::Drawing::Point(91, 118);
			this->btCancel->Name = L"btCancel";
			this->btCancel->Size = System::Drawing::Size(75, 23);
			this->btCancel->TabIndex = 17;
			this->btCancel->Text = L"Cancel";
			this->btCancel->UseVisualStyleBackColor = true;
			// 
			// btOk
			// 
			this->btOk->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->btOk->Location = System::Drawing::Point(10, 118);
			this->btOk->Name = L"btOk";
			this->btOk->Size = System::Drawing::Size(75, 23);
			this->btOk->TabIndex = 16;
			this->btOk->Text = L"OK";
			this->btOk->UseVisualStyleBackColor = true;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(53, 65);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(32, 12);
			this->label3->TabIndex = 20;
			this->label3->Text = L"Initial";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(58, 37);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(27, 12);
			this->label2->TabIndex = 19;
			this->label2->Text = L"Goal";
			// 
			// tbInitial
			// 
			this->tbInitial->Location = System::Drawing::Point(91, 62);
			this->tbInitial->Name = L"tbInitial";
			this->tbInitial->Size = System::Drawing::Size(75, 22);
			this->tbInitial->TabIndex = 1;
			this->tbInitial->TextChanged += gcnew System::EventHandler(this, &counterCountType2Setup::tcInput);
			// 
			// tbGoal
			// 
			this->tbGoal->Location = System::Drawing::Point(91, 34);
			this->tbGoal->Name = L"tbGoal";
			this->tbGoal->Size = System::Drawing::Size(75, 22);
			this->tbGoal->TabIndex = 0;
			this->tbGoal->TextChanged += gcnew System::EventHandler(this, &counterCountType2Setup::tcInput);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(42, 7);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(43, 12);
			this->label1->TabIndex = 18;
			this->label1->Text = L"Counter";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(60, 93);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(25, 12);
			this->label4->TabIndex = 23;
			this->label4->Text = L"Step";
			// 
			// tbStep
			// 
			this->tbStep->Location = System::Drawing::Point(91, 90);
			this->tbStep->Name = L"tbStep";
			this->tbStep->Size = System::Drawing::Size(75, 22);
			this->tbStep->TabIndex = 2;
			this->tbStep->TextChanged += gcnew System::EventHandler(this, &counterCountType2Setup::tcInput);
			// 
			// counterCountType2Setup
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(174, 151);
			this->ControlBox = false;
			this->Controls->Add(this->label4);
			this->Controls->Add(this->tbStep);
			this->Controls->Add(this->numericUpDownCounter);
			this->Controls->Add(this->btCancel);
			this->Controls->Add(this->btOk);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->tbInitial);
			this->Controls->Add(this->tbGoal);
			this->Controls->Add(this->label1);
			this->Name = L"counterCountType2Setup";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Ring Counter Count - Type2";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownCounter))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void tcInput(System::Object^  sender, System::EventArgs^  e) {
			 //----------------------
			 //	Table-Match , all text box shared the same handler
			 // Hsien 2013/5/24
			 //----------------------
			 Windows::Forms::TextBox^ tb =  safe_cast<Windows::Forms::TextBox^>(sender);
			 counterData[tb->TabIndex] = tb->Text;
		 }
private: System::Void ncInput(System::Object^  sender, System::EventArgs^  e) {
			 //----------------------
			 //
			 //----------------------
			 // Hsien 2013/5/24
			 this->counterIndex = System::Decimal::ToInt32(this->numericUpDownCounter->Value);
		 }
};
}
