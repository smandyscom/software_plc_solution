#pragma once


namespace testDrive {

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
			this->item = new int;
			*this->item = outD_enum::NONE;

			anc = gcnew plain::plainOutputControl(this->item);
			this->tlp_od->Controls->Add(anc);
			this->omds	= new metaData::DEV_METADATA*;
			*this->omds = NULL;

		//Windows::Forms::Control^	hdl 
		//	= gcnew outDev::nonStopTimerControl(new odNonStopTimer(ODSETTN));			
		//this->panel1->Controls->Add(hdl);

		}
		plain::plainOutputControl^		anc;
		metaData::DEV_METADATA			**omds;
	private: System::Windows::Forms::Button^  STORE;
	private: System::Windows::Forms::Button^  REBUILD;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  btSt;
	private: System::Windows::Forms::Button^  btCset;
	private: System::Windows::Forms::Button^  btCcnt;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;

	protected:
		int*	item;
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if(*this->omds != NULL)
				metaData::device::finalize(*this->omds);
			delete this->omds;
			delete this->item;

			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::Button^  COIL;
	private: System::Windows::Forms::Button^  NONE;
	private: System::Windows::Forms::Button^  SET;
	private: System::Windows::Forms::TableLayoutPanel^  tlp_od;
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
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->btCcnt = (gcnew System::Windows::Forms::Button());
			this->COIL = (gcnew System::Windows::Forms::Button());
			this->NONE = (gcnew System::Windows::Forms::Button());
			this->SET = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->btSt = (gcnew System::Windows::Forms::Button());
			this->btCset = (gcnew System::Windows::Forms::Button());
			this->tlp_od = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->STORE = (gcnew System::Windows::Forms::Button());
			this->REBUILD = (gcnew System::Windows::Forms::Button());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->tableLayoutPanel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->AutoSize = true;
			this->tableLayoutPanel1->ColumnCount = 1;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				100)));
			this->tableLayoutPanel1->Controls->Add(this->btCcnt, 0, 6);
			this->tableLayoutPanel1->Controls->Add(this->COIL, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->NONE, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->SET, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->button1, 0, 3);
			this->tableLayoutPanel1->Controls->Add(this->btSt, 0, 4);
			this->tableLayoutPanel1->Controls->Add(this->btCset, 0, 5);
			this->tableLayoutPanel1->Location = System::Drawing::Point(697, 12);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 7;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 35)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 8)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(200, 329);
			this->tableLayoutPanel1->TabIndex = 0;
			// 
			// btCcnt
			// 
			this->btCcnt->Location = System::Drawing::Point(3, 288);
			this->btCcnt->Name = L"btCcnt";
			this->btCcnt->Size = System::Drawing::Size(75, 23);
			this->btCcnt->TabIndex = 7;
			this->btCcnt->Text = L"CCNT";
			this->btCcnt->UseVisualStyleBackColor = true;
			this->btCcnt->Click += gcnew System::EventHandler(this, &Form1::btCcnt_Click);
			// 
			// COIL
			// 
			this->COIL->Location = System::Drawing::Point(3, 53);
			this->COIL->Name = L"COIL";
			this->COIL->Size = System::Drawing::Size(75, 23);
			this->COIL->TabIndex = 1;
			this->COIL->Text = L"COIL";
			this->COIL->UseVisualStyleBackColor = true;
			this->COIL->Click += gcnew System::EventHandler(this, &Form1::COIL_Click);
			// 
			// NONE
			// 
			this->NONE->Location = System::Drawing::Point(3, 3);
			this->NONE->Name = L"NONE";
			this->NONE->Size = System::Drawing::Size(75, 23);
			this->NONE->TabIndex = 2;
			this->NONE->Text = L"NONE";
			this->NONE->UseVisualStyleBackColor = true;
			this->NONE->Click += gcnew System::EventHandler(this, &Form1::NONE_Click);
			// 
			// SET
			// 
			this->SET->Location = System::Drawing::Point(3, 103);
			this->SET->Name = L"SET";
			this->SET->Size = System::Drawing::Size(75, 23);
			this->SET->TabIndex = 3;
			this->SET->Text = L"SET";
			this->SET->UseVisualStyleBackColor = true;
			this->SET->Click += gcnew System::EventHandler(this, &Form1::SET_Click);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(3, 153);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 4;
			this->button1->Text = L"TIMER";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::TIMER_CLICK);
			// 
			// btSt
			// 
			this->btSt->Location = System::Drawing::Point(3, 203);
			this->btSt->Name = L"btSt";
			this->btSt->Size = System::Drawing::Size(75, 23);
			this->btSt->TabIndex = 5;
			this->btSt->Text = L"ST";
			this->btSt->UseVisualStyleBackColor = true;
			this->btSt->Click += gcnew System::EventHandler(this, &Form1::btSt_Click);
			// 
			// btCset
			// 
			this->btCset->Location = System::Drawing::Point(3, 253);
			this->btCset->Name = L"btCset";
			this->btCset->Size = System::Drawing::Size(75, 23);
			this->btCset->TabIndex = 6;
			this->btCset->Text = L"CSET";
			this->btCset->UseVisualStyleBackColor = true;
			this->btCset->Click += gcnew System::EventHandler(this, &Form1::btCset_Click);
			// 
			// tlp_od
			// 
			this->tlp_od->AutoSize = true;
			this->tlp_od->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->tlp_od->ColumnCount = 1;
			this->tlp_od->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tlp_od->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlp_od->Location = System::Drawing::Point(13, 12);
			this->tlp_od->Name = L"tlp_od";
			this->tlp_od->RowCount = 2;
			this->tlp_od->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tlp_od->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tlp_od->Size = System::Drawing::Size(0, 0);
			this->tlp_od->TabIndex = 1;
			// 
			// STORE
			// 
			this->STORE->Location = System::Drawing::Point(616, 12);
			this->STORE->Name = L"STORE";
			this->STORE->Size = System::Drawing::Size(75, 23);
			this->STORE->TabIndex = 2;
			this->STORE->Text = L"STORE";
			this->STORE->UseVisualStyleBackColor = true;
			this->STORE->Click += gcnew System::EventHandler(this, &Form1::STORE_Click);
			// 
			// REBUILD
			// 
			this->REBUILD->Location = System::Drawing::Point(616, 42);
			this->REBUILD->Name = L"REBUILD";
			this->REBUILD->Size = System::Drawing::Size(75, 23);
			this->REBUILD->TabIndex = 3;
			this->REBUILD->Text = L"REBUILD";
			this->REBUILD->UseVisualStyleBackColor = true;
			this->REBUILD->Click += gcnew System::EventHandler(this, &Form1::REBUILD_Click);
			// 
			// panel1
			// 
			this->panel1->Location = System::Drawing::Point(491, 150);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(200, 100);
			this->panel1->TabIndex = 4;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(700, 347);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 8;
			this->button2->Text = L"ICOIL";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(700, 376);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 9;
			this->button3->Text = L"CCNT2";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(909, 405);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->REBUILD);
			this->Controls->Add(this->STORE);
			this->Controls->Add(this->tlp_od);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->tableLayoutPanel1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void NONE_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->item = outD_enum::NONE;
		 }
private: System::Void COIL_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->item = outD_enum::ODCOIL;
		 }
private: System::Void SET_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->item = outD_enum::ODSET;
		 }
private: System::Void STORE_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->anc->store(this->omds);
		 }
private: System::Void REBUILD_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->anc->rebuild(*this->omds);
		 }
private: System::Void TIMER_CLICK(System::Object^  sender, System::EventArgs^  e) {
			 *this->item = ODSETTN;
		 }
private: System::Void btSt_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->item = ODSTBLOCK;
		 }
private: System::Void btCset_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->item = ODRINGCTR_SET;
		 }
private: System::Void btCcnt_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->item = ODRINGCTR_COUNTUP;
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->item = ODNC_CONFIRM;
		 }
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->item = ODRINGCTR_TYPE2_COUNTUP;
		 }
};
}

