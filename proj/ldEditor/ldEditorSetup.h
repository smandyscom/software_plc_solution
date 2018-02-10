#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace _ldEditor {

	/// <summary>
	/// Summary for setup
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class setup : public System::Windows::Forms::Form
	{
	public:
		setup(/*System::String^ %dbPathGateway,System::String^ %homeDirGateway*/)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->openFileDialog_Database->Filter
				= L"Sqlite3 Data Base (*.db) |*.db";

			//--------------------------------------------------------------------------
			//	in .NET , it's impossible to store tracking-reference
			//	because it would cause memory unsafe , key word: c++ cli store reference
			//	commented by Lisa , 2012.07.03
			//--------------------------------------------------------------------------
			
			//this->dbPath	= safe_cast<System::String^>(dbPathGateway);
			//dbPathGateway = "dumb";
			//this->dbPath = ;
			//this->homeDir	= homeDirGateway->MemberwiseClone();
		}
	private: System::Windows::Forms::Button^  btDataBase;
	private: System::Windows::Forms::Button^  btHomePath;
	private: System::Windows::Forms::Button^  btDialogOK;
	public: System::Windows::Forms::TextBox^  tbDataBase;
	private: System::Windows::Forms::Button^  btDialogCancel;
	public: System::Windows::Forms::TextBox^  tbHomePath;
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~setup()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog_Database;
	private: System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog_Homedir;
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
			this->openFileDialog_Database = (gcnew System::Windows::Forms::OpenFileDialog());
			this->folderBrowserDialog_Homedir = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->btDataBase = (gcnew System::Windows::Forms::Button());
			this->btHomePath = (gcnew System::Windows::Forms::Button());
			this->btDialogOK = (gcnew System::Windows::Forms::Button());
			this->tbDataBase = (gcnew System::Windows::Forms::TextBox());
			this->tbHomePath = (gcnew System::Windows::Forms::TextBox());
			this->btDialogCancel = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// folderBrowserDialog_Homedir
			// 
			this->folderBrowserDialog_Homedir->Description = L"Select Your Home Folder";
			this->folderBrowserDialog_Homedir->RootFolder = System::Environment::SpecialFolder::MyComputer;
			// 
			// btDataBase
			// 
			this->btDataBase->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->btDataBase->Location = System::Drawing::Point(12, 11);
			this->btDataBase->Name = L"btDataBase";
			this->btDataBase->Size = System::Drawing::Size(77, 23);
			this->btDataBase->TabIndex = 0;
			this->btDataBase->Text = L"Data Base:";
			this->btDataBase->UseVisualStyleBackColor = true;
			this->btDataBase->Click += gcnew System::EventHandler(this, &setup::selectDbClick);
			// 
			// btHomePath
			// 
			this->btHomePath->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->btHomePath->Location = System::Drawing::Point(13, 43);
			this->btHomePath->Name = L"btHomePath";
			this->btHomePath->Size = System::Drawing::Size(75, 23);
			this->btHomePath->TabIndex = 1;
			this->btHomePath->Text = L"Home Path:";
			this->btHomePath->UseVisualStyleBackColor = true;
			this->btHomePath->Click += gcnew System::EventHandler(this, &setup::selectHomePathClick);
			// 
			// btDialogOK
			// 
			this->btDialogOK->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->btDialogOK->Location = System::Drawing::Point(355, 72);
			this->btDialogOK->Name = L"btDialogOK";
			this->btDialogOK->Size = System::Drawing::Size(75, 23);
			this->btDialogOK->TabIndex = 2;
			this->btDialogOK->Text = L"OK";
			this->btDialogOK->UseVisualStyleBackColor = true;
			this->btDialogOK->Click += gcnew System::EventHandler(this, &setup::dialogOKClick);
			// 
			// tbDataBase
			// 
			this->tbDataBase->Location = System::Drawing::Point(95, 13);
			this->tbDataBase->Name = L"tbDataBase";
			this->tbDataBase->Size = System::Drawing::Size(416, 22);
			this->tbDataBase->TabIndex = 3;
			// 
			// tbHomePath
			// 
			this->tbHomePath->Location = System::Drawing::Point(95, 43);
			this->tbHomePath->Name = L"tbHomePath";
			this->tbHomePath->Size = System::Drawing::Size(416, 22);
			this->tbHomePath->TabIndex = 4;
			// 
			// btDialogCancel
			// 
			this->btDialogCancel->Location = System::Drawing::Point(436, 72);
			this->btDialogCancel->Name = L"btDialogCancel";
			this->btDialogCancel->Size = System::Drawing::Size(75, 23);
			this->btDialogCancel->TabIndex = 5;
			this->btDialogCancel->Text = L"Cancel";
			this->btDialogCancel->UseVisualStyleBackColor = true;
			this->btDialogCancel->Click += gcnew System::EventHandler(this, &setup::dialogCancelClick);
			// 
			// setup
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(523, 106);
			this->ControlBox = false;
			this->Controls->Add(this->btDialogCancel);
			this->Controls->Add(this->tbHomePath);
			this->Controls->Add(this->tbDataBase);
			this->Controls->Add(this->btDialogOK);
			this->Controls->Add(this->btHomePath);
			this->Controls->Add(this->btDataBase);
			this->Name = L"setup";
			this->Text = L"setup";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void dialogOKClick(System::Object^  sender, System::EventArgs^  e) {
				 this->DialogResult = Windows::Forms::DialogResult::OK;

			 }
private: System::Void selectDbClick(System::Object^  sender, System::EventArgs^  e) {
			 if(this->openFileDialog_Database->ShowDialog()
				 == Windows::Forms::DialogResult::OK){
					 this->tbDataBase->Text = this->openFileDialog_Database->FileName;
					 
					 //this->btDialogOK->PerformClick();
			 }
		 }
private: System::Void selectHomePathClick(System::Object^  sender, System::EventArgs^  e) {
			 if(this->folderBrowserDialog_Homedir->ShowDialog()
				 == Windows::Forms::DialogResult::OK){
					 this->tbHomePath->Text = this->folderBrowserDialog_Homedir->SelectedPath;
			 }
		 }
private: System::Void dialogCancelClick(System::Object^  sender, System::EventArgs^  e) {
			 this->DialogResult = Windows::Forms::DialogResult::Cancel;
		 }
};
}
