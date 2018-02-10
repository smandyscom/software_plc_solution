#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace inpDev {

	/// <summary>
	/// Summary for comparatorSetup
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class comparatorSetup : public System::Windows::Forms::Form
	{
	public:
		comparatorSetup(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->dataArray = gcnew array<String^>(3);
			//for each(String^ e in dataArray){
			//	e = gcnew String("");		// Hsien 2013/5/17	, in case of dirty character
			//}
			//this->tbInput(this->tbLhs,nullptr);	// inject the default strings into dataArray
			//this->tbInput(this->tbRhs,nullptr);	// inject the default strings into dataArray

//			this->cbOp->SelectedIndex = 0;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~comparatorSetup()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  lbExample;
	protected: 
	public:
		array<String^>^	dataArray;
						//	1. LHS	String
						//	2. Op	String
						//	3. RHS	String
		int	refresh(){
			//-------------
			//	Post the Value on GUI
			//-------------
			// Hsien 2013/5/16
			this->tbLhs->Text = dataArray[TABINDEX_LHS]->ToString();
			this->tbRhs->Text = dataArray[TABINDEX_RHS]->ToString();
			this->cbOp->Text  = dataArray[TABINDEX_OP]->ToString();

			return 1;
		}

	private: System::Windows::Forms::Button^  btCancel;
	private: System::Windows::Forms::Button^  btOk;
	private: System::Windows::Forms::ComboBox^  cbOp;
	private: System::Windows::Forms::TextBox^  tbLhs;
	private: System::Windows::Forms::TextBox^  tbRhs;
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
			this->btCancel = (gcnew System::Windows::Forms::Button());
			this->btOk = (gcnew System::Windows::Forms::Button());
			this->cbOp = (gcnew System::Windows::Forms::ComboBox());
			this->tbLhs = (gcnew System::Windows::Forms::TextBox());
			this->tbRhs = (gcnew System::Windows::Forms::TextBox());
			this->lbExample = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// btCancel
			// 
			this->btCancel->Location = System::Drawing::Point(153, 51);
			this->btCancel->Name = L"btCancel";
			this->btCancel->Size = System::Drawing::Size(75, 23);
			this->btCancel->TabIndex = 4;
			this->btCancel->Text = L"Cancel";
			this->btCancel->UseVisualStyleBackColor = true;
			this->btCancel->Click += gcnew System::EventHandler(this, &comparatorSetup::btClick);
			// 
			// btOk
			// 
			this->btOk->Location = System::Drawing::Point(72, 51);
			this->btOk->Name = L"btOk";
			this->btOk->Size = System::Drawing::Size(75, 23);
			this->btOk->TabIndex = 3;
			this->btOk->Text = L"OK";
			this->btOk->UseVisualStyleBackColor = true;
			this->btOk->Click += gcnew System::EventHandler(this, &comparatorSetup::btClick);
			// 
			// cbOp
			// 
			this->cbOp->FormattingEnabled = true;
			this->cbOp->Items->AddRange(gcnew cli::array< System::Object^  >(6) {L">", L"<", L">=", L"<=", L"!=", L"=="});
			this->cbOp->Location = System::Drawing::Point(90, 23);
			this->cbOp->Name = L"cbOp";
			this->cbOp->Size = System::Drawing::Size(57, 20);
			this->cbOp->TabIndex = 1;
			this->cbOp->SelectedIndexChanged += gcnew System::EventHandler(this, &comparatorSetup::select);
			// 
			// tbLhs
			// 
			this->tbLhs->Location = System::Drawing::Point(4, 23);
			this->tbLhs->Name = L"tbLhs";
			this->tbLhs->Size = System::Drawing::Size(80, 22);
			this->tbLhs->TabIndex = 0;
			this->tbLhs->Text = L"X0:0";
			this->tbLhs->TextChanged += gcnew System::EventHandler(this, &comparatorSetup::tbInput);
			// 
			// tbRhs
			// 
			this->tbRhs->Location = System::Drawing::Point(153, 23);
			this->tbRhs->Name = L"tbRhs";
			this->tbRhs->Size = System::Drawing::Size(75, 22);
			this->tbRhs->TabIndex = 2;
			this->tbRhs->Text = L"X1:0";
			this->tbRhs->TextChanged += gcnew System::EventHandler(this, &comparatorSetup::tbInput);
			// 
			// lbExample
			// 
			this->lbExample->AutoSize = true;
			this->lbExample->Location = System::Drawing::Point(4, 5);
			this->lbExample->Name = L"lbExample";
			this->lbExample->Size = System::Drawing::Size(95, 12);
			this->lbExample->TabIndex = 5;
			this->lbExample->Text = L"E.g¡G X0:1 <= 127";
			// 
			// comparatorSetup
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(234, 79);
			this->ControlBox = false;
			this->Controls->Add(this->lbExample);
			this->Controls->Add(this->tbRhs);
			this->Controls->Add(this->tbLhs);
			this->Controls->Add(this->cbOp);
			this->Controls->Add(this->btCancel);
			this->Controls->Add(this->btOk);
			this->Name = L"comparatorSetup";
			this->Text = L"Comparator Setup";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void btClick(System::Object^  sender, System::EventArgs^  e) {
				 //----------------------
				 //	Common Button Handler
				 //----------------------
				 Windows::Forms::Button^	bt
					 = safe_cast<Windows::Forms::Button^>(sender);

				 switch(bt->TabIndex){
					case TABINDEX_OK:		this->DialogResult = Windows::Forms::DialogResult::OK;		break;
					case TABINDEX_CANCEL:	this->DialogResult = Windows::Forms::DialogResult::Cancel;	break;
					default:
						break;
				 }
			 }
private: System::Void tbInput(System::Object^  sender, System::EventArgs^  e) {
			 //----------------------
			 //	All Text Box Shared The same Handler
			 //----------------------
			 Windows::Forms::TextBox^ tb 
				 = safe_cast<Windows::Forms::TextBox^>(sender);

			 dataArray[tb->TabIndex] = tb->Text;
		 }
private: System::Void select(System::Object^  sender, System::EventArgs^  e) {
			 dataArray[TABINDEX_OP] = safe_cast<String^>(this->cbOp->SelectedItem);
		 }
};//class

}//namespace
