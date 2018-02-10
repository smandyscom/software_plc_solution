#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace procInterface {

	/// <summary>
	/// Summary for procInterfaceControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class procInterfaceControl : public System::Windows::Forms::UserControl
	{
	public:
		procInterfaceControl(const char args)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			//----------------------
			//	For XP environmnet use // Hsien 2013/4/29
			//----------------------
#ifdef ON_SETUPPACK
			this->assemblyPath	= System::IO::Path::GetDirectoryName
				(System::Reflection::Assembly::GetExecutingAssembly()->Location);
#else
			this->assemblyPath = "D:\\workdir\\vs2005projects\\PLC framework\\trunk\\resource";
#endif

			//---------------------------------------------
			//	Link ContextMenu into every TextBox
			//---------------------------------------------
			switch(args){
				case PIC_LEVEL2:
					for each(Control^ e in this->tlpProc->Controls)
						if(e->GetType() == Windows::Forms::TextBox::typeid)
							e->ContextMenuStrip = this->cmsEdit;
					break;
				case PIC_LEVEL1:
				default:
					break;
			}

			//---------------------------------------------
			//
			//---------------------------------------------
			this->cbTimeInterval->SelectedIndex = ITEM_INDEX_15MS;
			this->timeInterval = 15;

			this->openFileDialog->DefaultExt = L".irc";
			this->openFileDialog->Filter	= L"Intermidiate Representation Codes Image (*.irc) | *.irc";
			this->filename					= this->assemblyPath + "\\NULL_INSTRUCTION.irc";			// the default instruction , Hsien , 2013.01.30
												// Hsien 2013/4/29
		}
	public:
		int		pid;								//
		int		timeInterval;						//
		String^	filename;					//
		int		runningLevel;						//
		String^	assemblyPath;						// Hsien 2013/4/29
	private: System::Windows::Forms::ComboBox^  cbTimeInterval;
	public: 
		int		runningSlot;						//
		//--------------
		//
		//---------------
		int		markPid	(const int	_pid);			// automatic PID mark

	private: System::Windows::Forms::ContextMenuStrip^		cmsEdit;
	private: System::Windows::Forms::ToolStripMenuItem^		btInsertBelow;
	private: System::Windows::Forms::ToolStripMenuItem^		btInsertAbove;
	private: System::Windows::Forms::ToolStripMenuItem^		btRemove;
	private: System::Windows::Forms::TextBox^				tbComment;	
	private: System::Windows::Forms::OpenFileDialog^		openFileDialog;
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~procInterfaceControl()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^  tlpProc;
	private: System::Windows::Forms::TextBox^			tbPid;
	private: System::Windows::Forms::TextBox^			tbFilename;
	private: System::Windows::Forms::Button^			btOpenFile;
	public:	 System::Windows::Forms::TextBox^			tbLevel;
	public:  System::Windows::Forms::TextBox^			tbSlot;
	private: System::ComponentModel::IContainer^		components;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(procInterfaceControl::typeid));
			this->tlpProc = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->tbPid = (gcnew System::Windows::Forms::TextBox());
			this->tbFilename = (gcnew System::Windows::Forms::TextBox());
			this->btOpenFile = (gcnew System::Windows::Forms::Button());
			this->tbLevel = (gcnew System::Windows::Forms::TextBox());
			this->tbSlot = (gcnew System::Windows::Forms::TextBox());
			this->tbComment = (gcnew System::Windows::Forms::TextBox());
			this->cbTimeInterval = (gcnew System::Windows::Forms::ComboBox());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->cmsEdit = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->btInsertBelow = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->btInsertAbove = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->btRemove = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tlpProc->SuspendLayout();
			this->cmsEdit->SuspendLayout();
			this->SuspendLayout();
			// 
			// tlpProc
			// 
			this->tlpProc->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Single;
			this->tlpProc->ColumnCount = 7;
			this->tlpProc->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpProc->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpProc->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpProc->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tlpProc->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpProc->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpProc->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 122)));
			this->tlpProc->Controls->Add(this->tbPid, 0, 0);
			this->tlpProc->Controls->Add(this->tbFilename, 3, 0);
			this->tlpProc->Controls->Add(this->btOpenFile, 2, 0);
			this->tlpProc->Controls->Add(this->tbLevel, 4, 0);
			this->tlpProc->Controls->Add(this->tbSlot, 5, 0);
			this->tlpProc->Controls->Add(this->tbComment, 6, 0);
			this->tlpProc->Controls->Add(this->cbTimeInterval, 1, 0);
			this->tlpProc->Location = System::Drawing::Point(3, 3);
			this->tlpProc->Name = L"tlpProc";
			this->tlpProc->RowCount = 1;
			this->tlpProc->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 62)));
			this->tlpProc->Size = System::Drawing::Size(600, 30);
			this->tlpProc->TabIndex = 0;
			// 
			// tbPid
			// 
			this->tbPid->Location = System::Drawing::Point(4, 4);
			this->tbPid->Name = L"tbPid";
			this->tbPid->ReadOnly = true;
			this->tbPid->Size = System::Drawing::Size(44, 22);
			this->tbPid->TabIndex = 0;
			// 
			// tbFilename
			// 
			this->tbFilename->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tbFilename->Location = System::Drawing::Point(157, 4);
			this->tbFilename->Name = L"tbFilename";
			this->tbFilename->Size = System::Drawing::Size(214, 22);
			this->tbFilename->TabIndex = 3;
			// 
			// btOpenFile
			// 
			this->btOpenFile->BackColor = System::Drawing::Color::Transparent;
			this->btOpenFile->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btOpenFile.BackgroundImage")));
			this->btOpenFile->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->btOpenFile->Location = System::Drawing::Point(106, 4);
			this->btOpenFile->Name = L"btOpenFile";
			this->btOpenFile->Size = System::Drawing::Size(44, 22);
			this->btOpenFile->TabIndex = 5;
			this->btOpenFile->TextImageRelation = System::Windows::Forms::TextImageRelation::ImageBeforeText;
			this->btOpenFile->UseVisualStyleBackColor = false;
			this->btOpenFile->Click += gcnew System::EventHandler(this, &procInterfaceControl::openFile);
			// 
			// tbLevel
			// 
			this->tbLevel->Location = System::Drawing::Point(378, 4);
			this->tbLevel->Name = L"tbLevel";
			this->tbLevel->ReadOnly = true;
			this->tbLevel->Size = System::Drawing::Size(44, 22);
			this->tbLevel->TabIndex = 6;
			// 
			// tbSlot
			// 
			this->tbSlot->Location = System::Drawing::Point(429, 4);
			this->tbSlot->Name = L"tbSlot";
			this->tbSlot->ReadOnly = true;
			this->tbSlot->Size = System::Drawing::Size(44, 22);
			this->tbSlot->TabIndex = 7;
			// 
			// tbComment
			// 
			this->tbComment->Location = System::Drawing::Point(480, 4);
			this->tbComment->Name = L"tbComment";
			this->tbComment->Size = System::Drawing::Size(101, 22);
			this->tbComment->TabIndex = 8;
			// 
			// cbTimeInterval
			// 
			this->cbTimeInterval->Enabled = false;
			this->cbTimeInterval->FormattingEnabled = true;
			this->cbTimeInterval->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"15", L"30", L"60", L"120"});
			this->cbTimeInterval->Location = System::Drawing::Point(55, 4);
			this->cbTimeInterval->MaxDropDownItems = 4;
			this->cbTimeInterval->Name = L"cbTimeInterval";
			this->cbTimeInterval->Size = System::Drawing::Size(44, 20);
			this->cbTimeInterval->TabIndex = 9;
			// 
			// cmsEdit
			// 
			this->cmsEdit->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->btInsertBelow, this->btInsertAbove, 
				this->btRemove});
			this->cmsEdit->Name = L"cmsEdit";
			this->cmsEdit->Size = System::Drawing::Size(208, 70);
			// 
			// btInsertBelow
			// 
			this->btInsertBelow->Name = L"btInsertBelow";
			this->btInsertBelow->Size = System::Drawing::Size(207, 22);
			this->btInsertBelow->Text = L"Insert Instruction Below";
			this->btInsertBelow->Click += gcnew System::EventHandler(this, &procInterfaceControl::insertInstruction);
			// 
			// btInsertAbove
			// 
			this->btInsertAbove->Name = L"btInsertAbove";
			this->btInsertAbove->Size = System::Drawing::Size(207, 22);
			this->btInsertAbove->Text = L"Insert Instruction Above";
			this->btInsertAbove->Click += gcnew System::EventHandler(this, &procInterfaceControl::insertInstruction);
			// 
			// btRemove
			// 
			this->btRemove->Name = L"btRemove";
			this->btRemove->Size = System::Drawing::Size(207, 22);
			this->btRemove->Text = L"Remove Instruction";
			this->btRemove->Click += gcnew System::EventHandler(this, &procInterfaceControl::removeInstruction);
			// 
			// procInterfaceControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->Controls->Add(this->tlpProc);
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"procInterfaceControl";
			this->Size = System::Drawing::Size(606, 36);
			this->tlpProc->ResumeLayout(false);
			this->tlpProc->PerformLayout();
			this->cmsEdit->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void openFile(System::Object^  sender, System::EventArgs^  e) {
				 if( this->openFileDialog->ShowDialog()
					 == Windows::Forms::DialogResult::OK){
						 this->tbFilename->Text = this->openFileDialog->FileName;
						 this->filename			= gcnew String(this->openFileDialog->FileName);
															// Hsien  ,2013.04.26
				 }
				 else
					 this->filename =  this->assemblyPath + "\\NULL_INSTRUCTION.irc";			// the default instruction , Hsien , 2013.01.30
																							// Hsien 2013/4/29
			 }
private: System::Void kdEnter(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			 //-------------------------------
			 //	Enter 
			 //-------------------------------
			// this->timeInterval = Int32::Parse(this->tbTimeInterval->Text);
		 }
private: System::Void insertInstruction(System::Object^  sender, System::EventArgs^  e) {
			 //-------------------------------------------------
			 //	According Sender to Decide Insert Below or Above 
			 //-------------------------------------------------
			 if(sender->GetHashCode() == this->btInsertAbove->GetHashCode()){
				 //-------
				 //	Above : Directly call insert() utility
				 //	Preassuming this control has been loaded in tableLayoutPanel	
				 //-------
				 insertControl(this,gcnew procInterfaceControl(PIC_LEVEL2));
			 }
			 else if(sender->GetHashCode() == this->btInsertBelow->GetHashCode()){
				 //------
				 //	Below : Get the next control than call insert utility
				 //------
				 Windows::Forms::TableLayoutPanel^ parentTable = 
				 safe_cast<Windows::Forms::TableLayoutPanel^>(this->Parent);			// cast-back parent into tlp class
				 Control^ nextControl
				 = parentTable->GetControlFromPosition(
				 parentTable->GetPositionFromControl(this).Column
				 ,parentTable->GetPositionFromControl(this).Row + 1);
				 //------------------------------------------------------------------
				 // no reference when this method applied on last row : no next Rung
				 //	so that , could not reference tablePanel
				 //-------------------------------------------------------------------
				 if(nextControl == nullptr)
					 addControl		(this,gcnew procInterfaceControl(PIC_LEVEL2));
				 else
					 insertControl	(nextControl,gcnew procInterfaceControl(PIC_LEVEL2));	
			 }
		 }
private: System::Void removeInstruction(System::Object^  sender, System::EventArgs^  e) {
			 //----------------------------------------
			 //	impossible to delete the very last rung
			 //----------------------------------------
			 if(this->Parent->Controls->Count > 1){
				 this->Parent->Controls->Remove(this);
				 this->~procInterfaceControl();	
			 }
		 }
};
}
