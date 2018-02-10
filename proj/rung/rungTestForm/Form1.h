#pragma once


namespace testForm3 {

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
			//this->skeletonKind = new unsigned char;
			//*this->skeletonKind = 1;
			//this->Controls->Add(gcnew rung_control_expr::rung_control_exprControl(this->inpItem,new int));
			//this->Controls->Add(gcnew coil::coilControl(this->skeletonKind));

			//this->Controls->Add(gcnew set::setControl);
			this->outItem = new int;			
			this->inpItem = new int;
			//this->Controls->Add(gcnew plainOutput::plainOutputControl(this->outItem,new unsigned char));
			this->rmds = new metaData::CLST_METADATA*;
			*this->rmds = NULL;

			*this->inpItem = 5;
			*this->outItem = 5;
			this->rc = gcnew _ldEditor::rungControl(this->inpItem,this->outItem,new PCDPM_FAMILY);
			this->Controls->Add(this->rc);

			//this->Controls->Add(gcnew inpDev::normalCloseControl(new int,new bool));
			this->timerConst->Enabled = true;
		}

	public: 
		_ldEditor::rungControl^	rc;
	private: System::Windows::Forms::Button^  buttonCoil;
	private: System::Windows::Forms::Button^  buttonSet;
	private: System::Windows::Forms::TextBox^  textBoxStmt;
	private: System::Windows::Forms::Button^  buttonNO;
	private: System::Windows::Forms::Button^  buttonNC;
	private: System::Windows::Forms::Timer^  timerConst;
	private: System::Windows::Forms::Button^  buttonDelete;
	private: System::Windows::Forms::Button^  buttonBUT1;
	private: System::Windows::Forms::Button^  buttonBFTER;
	private: System::Windows::Forms::Button^  buttonBUT2;
	private: System::Windows::Forms::Button^  buttonBDTER;
	private: System::Windows::Forms::Button^  buttonBDTSEC;
	private: System::Windows::Forms::Button^  buttonBFTSEC;
	private: System::Windows::Forms::Button^  buttonTMR;
	private: System::Windows::Forms::Button^  buttonSTGen;
	private: System::Windows::Forms::Button^  btTn;
	private: System::Windows::Forms::Button^  btSTBlk;
	private: System::Windows::Forms::Button^  btCSet;
	private: System::Windows::Forms::Button^  btCcnt;
	private: System::Windows::Forms::Button^  btIndex;

	protected:
		metaData::CLST_METADATA**	rmds;
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{				
			if(*this->rmds != NULL)
				metaData::cluster::finalize(*this->rmds);	// WARNING! if this->rmds is NULL , the finalizer would fail
															// the rest problem is , delete row had memory leakage proble

			delete this->rmds;
			delete this->inpItem;
			delete this->outItem;

//			 _CrtDumpMemoryLeaks();
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		int* inpItem;
		int* outItem;
	private: System::Windows::Forms::Button^  Store;
	private: System::Windows::Forms::Button^  Rebuild;
	private: System::ComponentModel::IContainer^  components;
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->Store = (gcnew System::Windows::Forms::Button());
			this->Rebuild = (gcnew System::Windows::Forms::Button());
			this->buttonCoil = (gcnew System::Windows::Forms::Button());
			this->buttonSet = (gcnew System::Windows::Forms::Button());
			this->textBoxStmt = (gcnew System::Windows::Forms::TextBox());
			this->buttonNO = (gcnew System::Windows::Forms::Button());
			this->buttonNC = (gcnew System::Windows::Forms::Button());
			this->timerConst = (gcnew System::Windows::Forms::Timer(this->components));
			this->buttonDelete = (gcnew System::Windows::Forms::Button());
			this->buttonBUT1 = (gcnew System::Windows::Forms::Button());
			this->buttonBFTER = (gcnew System::Windows::Forms::Button());
			this->buttonBUT2 = (gcnew System::Windows::Forms::Button());
			this->buttonBDTER = (gcnew System::Windows::Forms::Button());
			this->buttonBDTSEC = (gcnew System::Windows::Forms::Button());
			this->buttonBFTSEC = (gcnew System::Windows::Forms::Button());
			this->buttonTMR = (gcnew System::Windows::Forms::Button());
			this->buttonSTGen = (gcnew System::Windows::Forms::Button());
			this->btTn = (gcnew System::Windows::Forms::Button());
			this->btSTBlk = (gcnew System::Windows::Forms::Button());
			this->btCSet = (gcnew System::Windows::Forms::Button());
			this->btCcnt = (gcnew System::Windows::Forms::Button());
			this->btIndex = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// Store
			// 
			this->Store->Location = System::Drawing::Point(13, 365);
			this->Store->Name = L"Store";
			this->Store->Size = System::Drawing::Size(75, 23);
			this->Store->TabIndex = 13;
			this->Store->Text = L"store";
			this->Store->UseVisualStyleBackColor = true;
			this->Store->Click += gcnew System::EventHandler(this, &Form1::Store_Click);
			// 
			// Rebuild
			// 
			this->Rebuild->Location = System::Drawing::Point(13, 336);
			this->Rebuild->Name = L"Rebuild";
			this->Rebuild->Size = System::Drawing::Size(75, 23);
			this->Rebuild->TabIndex = 14;
			this->Rebuild->Text = L"rebuild";
			this->Rebuild->UseVisualStyleBackColor = true;
			this->Rebuild->Click += gcnew System::EventHandler(this, &Form1::Rebuild_Click);
			// 
			// buttonCoil
			// 
			this->buttonCoil->Location = System::Drawing::Point(13, 394);
			this->buttonCoil->Name = L"buttonCoil";
			this->buttonCoil->Size = System::Drawing::Size(75, 23);
			this->buttonCoil->TabIndex = 15;
			this->buttonCoil->Text = L"coil";
			this->buttonCoil->UseVisualStyleBackColor = true;
			this->buttonCoil->Click += gcnew System::EventHandler(this, &Form1::buttonCoil_Click);
			// 
			// buttonSet
			// 
			this->buttonSet->Location = System::Drawing::Point(94, 394);
			this->buttonSet->Name = L"buttonSet";
			this->buttonSet->Size = System::Drawing::Size(75, 23);
			this->buttonSet->TabIndex = 16;
			this->buttonSet->Text = L"set";
			this->buttonSet->UseVisualStyleBackColor = true;
			this->buttonSet->Click += gcnew System::EventHandler(this, &Form1::buttonSet_Click);
			// 
			// textBoxStmt
			// 
			this->textBoxStmt->Location = System::Drawing::Point(307, 356);
			this->textBoxStmt->Multiline = true;
			this->textBoxStmt->Name = L"textBoxStmt";
			this->textBoxStmt->Size = System::Drawing::Size(190, 116);
			this->textBoxStmt->TabIndex = 17;
			// 
			// buttonNO
			// 
			this->buttonNO->Location = System::Drawing::Point(13, 423);
			this->buttonNO->Name = L"buttonNO";
			this->buttonNO->Size = System::Drawing::Size(75, 23);
			this->buttonNO->TabIndex = 18;
			this->buttonNO->Text = L"NO";
			this->buttonNO->UseVisualStyleBackColor = true;
			this->buttonNO->Click += gcnew System::EventHandler(this, &Form1::buttonNO_Click);
			// 
			// buttonNC
			// 
			this->buttonNC->Location = System::Drawing::Point(94, 423);
			this->buttonNC->Name = L"buttonNC";
			this->buttonNC->Size = System::Drawing::Size(75, 23);
			this->buttonNC->TabIndex = 19;
			this->buttonNC->Text = L"NC";
			this->buttonNC->UseVisualStyleBackColor = true;
			this->buttonNC->Click += gcnew System::EventHandler(this, &Form1::buttonNC_Click);
			// 
			// timerConst
			// 
			this->timerConst->Interval = 1000;
			this->timerConst->Tick += gcnew System::EventHandler(this, &Form1::timerConst_Tick);
			// 
			// buttonDelete
			// 
			this->buttonDelete->Location = System::Drawing::Point(94, 365);
			this->buttonDelete->Name = L"buttonDelete";
			this->buttonDelete->Size = System::Drawing::Size(75, 23);
			this->buttonDelete->TabIndex = 20;
			this->buttonDelete->Text = L"delete";
			this->buttonDelete->UseVisualStyleBackColor = true;
			this->buttonDelete->Click += gcnew System::EventHandler(this, &Form1::buttonDelete_Click);
			// 
			// buttonBUT1
			// 
			this->buttonBUT1->Location = System::Drawing::Point(175, 423);
			this->buttonBUT1->Name = L"buttonBUT1";
			this->buttonBUT1->Size = System::Drawing::Size(75, 23);
			this->buttonBUT1->TabIndex = 21;
			this->buttonBUT1->Text = L"¢sU";
			this->buttonBUT1->UseVisualStyleBackColor = true;
			this->buttonBUT1->Click += gcnew System::EventHandler(this, &Form1::buttonBUT1_Click);
			// 
			// buttonBFTER
			// 
			this->buttonBFTER->Location = System::Drawing::Point(13, 452);
			this->buttonBFTER->Name = L"buttonBFTER";
			this->buttonBFTER->Size = System::Drawing::Size(75, 23);
			this->buttonBFTER->TabIndex = 22;
			this->buttonBFTER->Text = L"¢|";
			this->buttonBFTER->UseVisualStyleBackColor = true;
			this->buttonBFTER->Click += gcnew System::EventHandler(this, &Form1::buttonBFTER_Click);
			// 
			// buttonBUT2
			// 
			this->buttonBUT2->Location = System::Drawing::Point(94, 452);
			this->buttonBUT2->Name = L"buttonBUT2";
			this->buttonBUT2->Size = System::Drawing::Size(75, 23);
			this->buttonBUT2->TabIndex = 23;
			this->buttonBUT2->Text = L"¢u";
			this->buttonBUT2->UseVisualStyleBackColor = true;
			this->buttonBUT2->Click += gcnew System::EventHandler(this, &Form1::buttonBUT2_Click);
			// 
			// buttonBDTER
			// 
			this->buttonBDTER->Location = System::Drawing::Point(175, 452);
			this->buttonBDTER->Name = L"buttonBDTER";
			this->buttonBDTER->Size = System::Drawing::Size(75, 23);
			this->buttonBDTER->TabIndex = 24;
			this->buttonBDTER->Text = L"¢}";
			this->buttonBDTER->UseVisualStyleBackColor = true;
			this->buttonBDTER->Click += gcnew System::EventHandler(this, &Form1::buttonBDTER_Click);
			// 
			// buttonBDTSEC
			// 
			this->buttonBDTSEC->Location = System::Drawing::Point(175, 365);
			this->buttonBDTSEC->Name = L"buttonBDTSEC";
			this->buttonBDTSEC->Size = System::Drawing::Size(75, 23);
			this->buttonBDTSEC->TabIndex = 25;
			this->buttonBDTSEC->Text = L"¢t";
			this->buttonBDTSEC->UseVisualStyleBackColor = true;
			this->buttonBDTSEC->Click += gcnew System::EventHandler(this, &Form1::buttonBDTSEC_Click);
			// 
			// buttonBFTSEC
			// 
			this->buttonBFTSEC->Location = System::Drawing::Point(175, 394);
			this->buttonBFTSEC->Name = L"buttonBFTSEC";
			this->buttonBFTSEC->Size = System::Drawing::Size(75, 23);
			this->buttonBFTSEC->TabIndex = 26;
			this->buttonBFTSEC->Text = L"¢sF";
			this->buttonBFTSEC->UseVisualStyleBackColor = true;
			this->buttonBFTSEC->Click += gcnew System::EventHandler(this, &Form1::buttonBFTSEC_Click);
			// 
			// buttonTMR
			// 
			this->buttonTMR->Location = System::Drawing::Point(95, 336);
			this->buttonTMR->Name = L"buttonTMR";
			this->buttonTMR->Size = System::Drawing::Size(75, 23);
			this->buttonTMR->TabIndex = 27;
			this->buttonTMR->Text = L"TMR";
			this->buttonTMR->UseVisualStyleBackColor = true;
			this->buttonTMR->Click += gcnew System::EventHandler(this, &Form1::buttonTMR_Click);
			// 
			// buttonSTGen
			// 
			this->buttonSTGen->Location = System::Drawing::Point(177, 336);
			this->buttonSTGen->Name = L"buttonSTGen";
			this->buttonSTGen->Size = System::Drawing::Size(75, 23);
			this->buttonSTGen->TabIndex = 28;
			this->buttonSTGen->Text = L"ST GEN";
			this->buttonSTGen->UseVisualStyleBackColor = true;
			this->buttonSTGen->Click += gcnew System::EventHandler(this, &Form1::stGen);
			// 
			// btTn
			// 
			this->btTn->Location = System::Drawing::Point(259, 336);
			this->btTn->Name = L"btTn";
			this->btTn->Size = System::Drawing::Size(75, 23);
			this->btTn->TabIndex = 29;
			this->btTn->Text = L"TN";
			this->btTn->UseVisualStyleBackColor = true;
			this->btTn->Click += gcnew System::EventHandler(this, &Form1::tnInputSelect);
			// 
			// btSTBlk
			// 
			this->btSTBlk->Location = System::Drawing::Point(340, 336);
			this->btSTBlk->Name = L"btSTBlk";
			this->btSTBlk->Size = System::Drawing::Size(75, 23);
			this->btSTBlk->TabIndex = 30;
			this->btSTBlk->Text = L"STBLK";
			this->btSTBlk->UseVisualStyleBackColor = true;
			this->btSTBlk->Click += gcnew System::EventHandler(this, &Form1::btSTBlkClick);
			// 
			// btCSet
			// 
			this->btCSet->Location = System::Drawing::Point(422, 336);
			this->btCSet->Name = L"btCSet";
			this->btCSet->Size = System::Drawing::Size(75, 23);
			this->btCSet->TabIndex = 31;
			this->btCSet->Text = L"CSET";
			this->btCSet->UseVisualStyleBackColor = true;
			this->btCSet->Click += gcnew System::EventHandler(this, &Form1::btCSet_Click);
			// 
			// btCcnt
			// 
			this->btCcnt->Location = System::Drawing::Point(422, 307);
			this->btCcnt->Name = L"btCcnt";
			this->btCcnt->Size = System::Drawing::Size(75, 23);
			this->btCcnt->TabIndex = 32;
			this->btCcnt->Text = L"CCNT";
			this->btCcnt->UseVisualStyleBackColor = true;
			this->btCcnt->Click += gcnew System::EventHandler(this, &Form1::btCcnt_Click);
			// 
			// btIndex
			// 
			this->btIndex->Location = System::Drawing::Point(340, 307);
			this->btIndex->Name = L"btIndex";
			this->btIndex->Size = System::Drawing::Size(75, 23);
			this->btIndex->TabIndex = 33;
			this->btIndex->Text = L"INDEXER";
			this->btIndex->UseVisualStyleBackColor = true;
			this->btIndex->Click += gcnew System::EventHandler(this, &Form1::btIndex_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ClientSize = System::Drawing::Size(509, 484);
			this->Controls->Add(this->btIndex);
			this->Controls->Add(this->btCcnt);
			this->Controls->Add(this->btCSet);
			this->Controls->Add(this->btSTBlk);
			this->Controls->Add(this->btTn);
			this->Controls->Add(this->buttonSTGen);
			this->Controls->Add(this->buttonTMR);
			this->Controls->Add(this->buttonBFTSEC);
			this->Controls->Add(this->buttonBDTSEC);
			this->Controls->Add(this->buttonBDTER);
			this->Controls->Add(this->buttonBUT2);
			this->Controls->Add(this->buttonBFTER);
			this->Controls->Add(this->buttonBUT1);
			this->Controls->Add(this->buttonDelete);
			this->Controls->Add(this->buttonNC);
			this->Controls->Add(this->buttonNO);
			this->Controls->Add(this->textBoxStmt);
			this->Controls->Add(this->buttonSet);
			this->Controls->Add(this->buttonCoil);
			this->Controls->Add(this->Rebuild);
			this->Controls->Add(this->Store);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			 }

private: System::Void Store_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->rc->store(this->rmds);
		 }
private: System::Void Rebuild_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->rc->rebuild(*rmds);
		 }
private: System::Void buttonCoil_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->outItem = 4;		// coil code
		 }
private: System::Void buttonSet_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->outItem = 5;
		 }
private: System::Void buttonNO_Click(System::Object^  sender, System::EventArgs^  e) {
			 *((short*)this->inpItem) = 10;
			 *(((short*)this->inpItem)+1) = 1;
		 }
private: System::Void buttonNC_Click(System::Object^  sender, System::EventArgs^  e) {
			 *((short*)this->inpItem) = 10;
			 *(((short*)this->inpItem)+1) = 2;
		 }
private: System::Void timerConst_Tick(System::Object^  sender, System::EventArgs^  e) {
			 this->textBoxStmt->Text = this->rc->rungStmt;
		 }
private: System::Void buttonBFTER_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->inpItem = 1;	// branch forward terminal
		 }
private: System::Void buttonBUT2_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->inpItem = 5;	// branch up-forward type-2
		 }
private: System::Void buttonBDTER_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->inpItem = 7;	// branch down terminal
		 }
private: System::Void buttonBUT1_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->inpItem = 4;	// branch up-forward type-1
		 }
private: System::Void buttonBFTSEC_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->inpItem = 2;	// branch forward tsection
		 }
private: System::Void buttonBDTSEC_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->inpItem = 8;	// branch down tsection
		 }
private: System::Void buttonDelete_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->inpItem = 9;
			 *this->outItem = 3;
		 }
private: System::Void buttonTMR_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->outItem = 6;
		 }
private: System::Void stGen(System::Object^  sender, System::EventArgs^  e) {
			 this->rc->exprGenerate();
			 this->textBoxStmt->Text = this->rc->rungStmt;
		 }
private: System::Void tnInputSelect(System::Object^  sender, System::EventArgs^  e) {
			 *((short*)this->inpItem) = 10;
			 *(((short*)this->inpItem)+1) = 6;
		 }
private: System::Void btSTBlkClick(System::Object^  sender, System::EventArgs^  e) {
			 *this->outItem = 8;
		 }
private: System::Void btCSet_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->outItem = 9;
		 }
private: System::Void btCcnt_Click(System::Object^  sender, System::EventArgs^  e) {
			 *this->outItem = 10;
		 }
private: System::Void btIndex_Click(System::Object^  sender, System::EventArgs^  e) {
			 *((short*)this->inpItem) = 10;
			 *(((short*)this->inpItem)+1) = 6;
		 }
};
}

