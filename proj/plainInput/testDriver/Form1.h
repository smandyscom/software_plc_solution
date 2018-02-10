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
			this->gateway = new int;
			*this->gateway = 2;
			this->tlp_plain->Controls->Add(gcnew plain::plainInputControl(0,plain::constPlainCodes::ST_HEAD),-1,-1);
			this->tlp_plain->Controls->Add(gcnew plain::plainInputControl(0,plain::constPlainCodes::ST_TAIL),-1,-1);
			this->tlp_plain->Controls->Add(gcnew plain::plainInputControl(0,plain::constPlainCodes::E_VERTICAL),-1,-1);
			this->tlp_plain->Controls->Add(gcnew plain::plainInputControl(0,plain::constPlainCodes::E_TERMINAL),-1,-1);
		
			this->anchor = gcnew plain::plainInputControl(this->gateway,plain::constPlainCodes::NULL_CODE);
			this->tlp_plain->Controls->Add(this->anchor,-1,-1);

			this->data = new metaData::DEV_METADATA*;
			*this->data = NULL;
		}
	private: System::Windows::Forms::Button^  Store;
	public: 
	private: System::Windows::Forms::Button^  Rebuild;

	protected:
		plain::plainInputControl^	anchor;
		int*	gateway;
	private: System::Windows::Forms::Button^  btTn;
	private: System::Windows::Forms::Button^  btIndexer;
	private: System::Windows::Forms::Button^  button1;
	protected: 
		metaData::DEV_METADATA**		data;
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if(*this->data != NULL)
				metaData::device::finalize(*data);
			delete this->gateway;
			delete this->data;
			if (components)
			{
				delete components;
			}
			GC::Collect();

		}
	private: System::Windows::Forms::Label^  EMPTY;
	private: System::Windows::Forms::Label^  BUF_T2;
	private: System::Windows::Forms::Label^  BUF_T1;
	private: System::Windows::Forms::Label^  BF_TSECTION;
	private: System::Windows::Forms::Label^  BF_TERMINAL;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::Label^  NC;
	private: System::Windows::Forms::Label^  BD_TSECTION;
	private: System::Windows::Forms::Label^  BD_TERMINAL;
	private: System::Windows::Forms::Label^  SHORT;
	private: System::Windows::Forms::Label^  NO;
	private: System::Windows::Forms::TableLayoutPanel^  tlp_plain;
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
			this->EMPTY = (gcnew System::Windows::Forms::Label());
			this->BUF_T2 = (gcnew System::Windows::Forms::Label());
			this->BUF_T1 = (gcnew System::Windows::Forms::Label());
			this->BF_TSECTION = (gcnew System::Windows::Forms::Label());
			this->BF_TERMINAL = (gcnew System::Windows::Forms::Label());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->SHORT = (gcnew System::Windows::Forms::Label());
			this->NC = (gcnew System::Windows::Forms::Label());
			this->BD_TSECTION = (gcnew System::Windows::Forms::Label());
			this->BD_TERMINAL = (gcnew System::Windows::Forms::Label());
			this->NO = (gcnew System::Windows::Forms::Label());
			this->tlp_plain = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->Store = (gcnew System::Windows::Forms::Button());
			this->Rebuild = (gcnew System::Windows::Forms::Button());
			this->btTn = (gcnew System::Windows::Forms::Button());
			this->btIndexer = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->tableLayoutPanel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// EMPTY
			// 
			this->EMPTY->AutoSize = true;
			this->EMPTY->Location = System::Drawing::Point(3, 0);
			this->EMPTY->Name = L"EMPTY";
			this->EMPTY->Size = System::Drawing::Size(43, 12);
			this->EMPTY->TabIndex = 0;
			this->EMPTY->Text = L"EMPTY";
			this->EMPTY->Click += gcnew System::EventHandler(this, &Form1::EMPTY_Click);
			// 
			// BUF_T2
			// 
			this->BUF_T2->AutoSize = true;
			this->BUF_T2->Location = System::Drawing::Point(3, 200);
			this->BUF_T2->Name = L"BUF_T2";
			this->BUF_T2->Size = System::Drawing::Size(46, 12);
			this->BUF_T2->TabIndex = 1;
			this->BUF_T2->Text = L"BUF_T2";
			this->BUF_T2->Click += gcnew System::EventHandler(this, &Form1::BUF_T2_Click);
			// 
			// BUF_T1
			// 
			this->BUF_T1->AutoSize = true;
			this->BUF_T1->Location = System::Drawing::Point(3, 150);
			this->BUF_T1->Name = L"BUF_T1";
			this->BUF_T1->Size = System::Drawing::Size(46, 12);
			this->BUF_T1->TabIndex = 2;
			this->BUF_T1->Text = L"BUF_T1";
			this->BUF_T1->Click += gcnew System::EventHandler(this, &Form1::BUF_T1_Click);
			// 
			// BF_TSECTION
			// 
			this->BF_TSECTION->AutoSize = true;
			this->BF_TSECTION->Location = System::Drawing::Point(3, 100);
			this->BF_TSECTION->Name = L"BF_TSECTION";
			this->BF_TSECTION->Size = System::Drawing::Size(80, 12);
			this->BF_TSECTION->TabIndex = 3;
			this->BF_TSECTION->Text = L"BF_TSECTION";
			this->BF_TSECTION->Click += gcnew System::EventHandler(this, &Form1::BF_TSECTION_Click);
			// 
			// BF_TERMINAL
			// 
			this->BF_TERMINAL->AutoSize = true;
			this->BF_TERMINAL->Location = System::Drawing::Point(3, 50);
			this->BF_TERMINAL->Name = L"BF_TERMINAL";
			this->BF_TERMINAL->Size = System::Drawing::Size(84, 12);
			this->BF_TERMINAL->TabIndex = 4;
			this->BF_TERMINAL->Text = L"BF_TERMINAL";
			this->BF_TERMINAL->Click += gcnew System::EventHandler(this, &Form1::BF_TERMINAL_Click);
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->AutoSize = true;
			this->tableLayoutPanel1->ColumnCount = 1;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 
				200)));
			this->tableLayoutPanel1->Controls->Add(this->SHORT, 0, 9);
			this->tableLayoutPanel1->Controls->Add(this->NC, 0, 7);
			this->tableLayoutPanel1->Controls->Add(this->BD_TSECTION, 0, 6);
			this->tableLayoutPanel1->Controls->Add(this->BD_TERMINAL, 0, 5);
			this->tableLayoutPanel1->Controls->Add(this->EMPTY, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->BUF_T2, 0, 4);
			this->tableLayoutPanel1->Controls->Add(this->BUF_T1, 0, 3);
			this->tableLayoutPanel1->Controls->Add(this->BF_TSECTION, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->BF_TERMINAL, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->NO, 0, 8);
			this->tableLayoutPanel1->Location = System::Drawing::Point(617, 13);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 10;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(200, 500);
			this->tableLayoutPanel1->TabIndex = 5;
			// 
			// SHORT
			// 
			this->SHORT->AutoSize = true;
			this->SHORT->Location = System::Drawing::Point(3, 450);
			this->SHORT->Name = L"SHORT";
			this->SHORT->Size = System::Drawing::Size(42, 12);
			this->SHORT->TabIndex = 9;
			this->SHORT->Text = L"SHORT";
			this->SHORT->Click += gcnew System::EventHandler(this, &Form1::SHORT_Click);
			// 
			// NC
			// 
			this->NC->AutoSize = true;
			this->NC->Location = System::Drawing::Point(3, 350);
			this->NC->Name = L"NC";
			this->NC->Size = System::Drawing::Size(21, 12);
			this->NC->TabIndex = 7;
			this->NC->Text = L"NC";
			this->NC->Click += gcnew System::EventHandler(this, &Form1::NC_Click);
			// 
			// BD_TSECTION
			// 
			this->BD_TSECTION->AutoSize = true;
			this->BD_TSECTION->Location = System::Drawing::Point(3, 300);
			this->BD_TSECTION->Name = L"BD_TSECTION";
			this->BD_TSECTION->Size = System::Drawing::Size(82, 12);
			this->BD_TSECTION->TabIndex = 6;
			this->BD_TSECTION->Text = L"BD_TSECTION";
			this->BD_TSECTION->Click += gcnew System::EventHandler(this, &Form1::BD_TSECTION_Click);
			// 
			// BD_TERMINAL
			// 
			this->BD_TERMINAL->AutoSize = true;
			this->BD_TERMINAL->Location = System::Drawing::Point(3, 250);
			this->BD_TERMINAL->Name = L"BD_TERMINAL";
			this->BD_TERMINAL->Size = System::Drawing::Size(86, 12);
			this->BD_TERMINAL->TabIndex = 5;
			this->BD_TERMINAL->Text = L"BD_TERMINAL";
			this->BD_TERMINAL->Click += gcnew System::EventHandler(this, &Form1::BD_TERMINAL_Click);
			// 
			// NO
			// 
			this->NO->AutoSize = true;
			this->NO->Location = System::Drawing::Point(3, 400);
			this->NO->Name = L"NO";
			this->NO->Size = System::Drawing::Size(21, 12);
			this->NO->TabIndex = 8;
			this->NO->Text = L"NO";
			this->NO->Click += gcnew System::EventHandler(this, &Form1::NO_Click);
			// 
			// tlp_plain
			// 
			this->tlp_plain->AutoSize = true;
			this->tlp_plain->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->tlp_plain->ColumnCount = 1;
			this->tlp_plain->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tlp_plain->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlp_plain->Location = System::Drawing::Point(13, 13);
			this->tlp_plain->Name = L"tlp_plain";
			this->tlp_plain->RowCount = 5;
			this->tlp_plain->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tlp_plain->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tlp_plain->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tlp_plain->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tlp_plain->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tlp_plain->Size = System::Drawing::Size(0, 0);
			this->tlp_plain->TabIndex = 6;
			// 
			// Store
			// 
			this->Store->Location = System::Drawing::Point(536, 1);
			this->Store->Name = L"Store";
			this->Store->Size = System::Drawing::Size(75, 23);
			this->Store->TabIndex = 7;
			this->Store->Text = L"Store";
			this->Store->UseVisualStyleBackColor = true;
			this->Store->Click += gcnew System::EventHandler(this, &Form1::Store_Click);
			// 
			// Rebuild
			// 
			this->Rebuild->Location = System::Drawing::Point(536, 30);
			this->Rebuild->Name = L"Rebuild";
			this->Rebuild->Size = System::Drawing::Size(75, 23);
			this->Rebuild->TabIndex = 8;
			this->Rebuild->Text = L"Rebuild";
			this->Rebuild->UseVisualStyleBackColor = true;
			this->Rebuild->Click += gcnew System::EventHandler(this, &Form1::Rebuild_Click);
			// 
			// btTn
			// 
			this->btTn->Location = System::Drawing::Point(536, 63);
			this->btTn->Name = L"btTn";
			this->btTn->Size = System::Drawing::Size(75, 23);
			this->btTn->TabIndex = 9;
			this->btTn->Text = L"TN";
			this->btTn->UseVisualStyleBackColor = true;
			this->btTn->Click += gcnew System::EventHandler(this, &Form1::TNClick);
			// 
			// btIndexer
			// 
			this->btIndexer->Location = System::Drawing::Point(536, 92);
			this->btIndexer->Name = L"btIndexer";
			this->btIndexer->Size = System::Drawing::Size(75, 23);
			this->btIndexer->TabIndex = 10;
			this->btIndexer->Text = L"INDEXER";
			this->btIndexer->UseVisualStyleBackColor = true;
			this->btIndexer->Click += gcnew System::EventHandler(this, &Form1::indexerCLick);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(536, 121);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 11;
			this->button1->Text = L"COMPARATOR";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(829, 520);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->btIndexer);
			this->Controls->Add(this->btTn);
			this->Controls->Add(this->Rebuild);
			this->Controls->Add(this->Store);
			this->Controls->Add(this->tlp_plain);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void EMPTY_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->gateway = ldElem_type_enum::EMPTY;
		 }
private: System::Void BF_TERMINAL_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->gateway = ldElem_type_enum::BRANCH_FORWARD_TERMINAL;
		 }
private: System::Void BF_TSECTION_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->gateway = ldElem_type_enum::BRANCH_FORWARD_TSECTION;
		 }
private: System::Void BUF_T1_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->gateway = ldElem_type_enum::BRANCH_UP_FORWARD_TSECTION_TYPE1;
		 }
private: System::Void BUF_T2_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->gateway = ldElem_type_enum::BRANCH_UP_FORWARD_TSECTION_TYPE2;
		 }
private: System::Void BD_TERMINAL_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->gateway = ldElem_type_enum::BRANCH_DOWN_TERMINAL;
		 }
private: System::Void BD_TSECTION_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->gateway = ldElem_type_enum::BRANCH_DOWN_TSECTION;
		 }
private: System::Void NC_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->gateway = ldElem_type_enum::VERTICE;
			 ((short*)this->gateway)[1] = vertice_enum::NORMAL_OPEN;
		 }
private: System::Void NO_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->gateway = ldElem_type_enum::VERTICE;
			 ((short*)this->gateway)[1] = vertice_enum::NORMAL_CLOSE;
		 }
private: System::Void SHORT_Click(System::Object^  sender, System::EventArgs^  e) {
			 			 *this->gateway = ldElem_type_enum::VERTICE;
						 ((short*)this->gateway)[1] = vertice_enum::__SHORT;
		 }
private: System::Void TNClick(System::Object^  sender, System::EventArgs^  e) {
			 			 *this->gateway = ldElem_type_enum::VERTICE;
						 ((short*)this->gateway)[1] = vertice_enum::NONSTOP_TIMER_OUTPUT_NO;
		 }
private: System::Void Store_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->anchor->store(this->data);
		 }
private: System::Void Rebuild_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->anchor->rebuild(*this->data);
		 }
private: System::Void indexerCLick(System::Object^  sender, System::EventArgs^  e) {
			 			 *this->gateway = ldElem_type_enum::VERTICE;
						 ((short*)this->gateway)[1] = vertice_enum::RING_COUNTER_OUTPUT_NO;
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->gateway = ldElem_type_enum::VERTICE;
			 ((short*)this->gateway)[1] = vertice_enum::COMPARATOR;
		 }
};
}

