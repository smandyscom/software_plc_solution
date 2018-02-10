#pragma once
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace _ldEditor {

	/// <summary>
	/// Summary for rung_control_exprControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.

	public ref class rungControl : public System::Windows::Forms::UserControl
	{
	public:
		rungControl(int* inpGateway 
			,int*				outGateway
			,STC_ERRHDL*		__errHandler/*PCDPM_FAMILY*		errorStack*/)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			//----------------------
			//	Get Current Executing Path ,  Hsien 2013/4/29
			//----------------------
#ifdef ON_SETUPPACK
			this->assemblyPath	= System::IO::Path::GetDirectoryName
				(System::Reflection::Assembly::GetExecutingAssembly()->Location);
#else
			this->assemblyPath = "D:\\workdir\\vs2005projects\\PLC framework\\trunk\\resource";
#endif

			this->_errHandler = __errHandler;
			this->inpItem = inpGateway;
			this->outItem = outGateway;

			this->innerInpNum	= this->tlpRung->ColumnCount-4;	// maxInnerInpNum = wholeInnerInpLength - 1(output column) -1(end image column) , N
																// editable inner input numbers
																// Hsien 2013/5/27
			this->rowCount		= 0;

			this->tlpRung->RowCount =0;

			//this->res  = 
			//	gcnew System::ComponentModel::ComponentResourceManager(rungControl::typeid);
			this->ContextMenuStrip = this->cmsRung;
	//		this->errStack = errorStack;	//	external link , for collecting connection/semetic error

			this->addRow(INITIAL);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~rungControl()
		{
			if (components)
			{
				delete components;		// would automatically call destructor of tlpRung
			}
			this->!rungControl();
		}
		!rungControl()
		{
			//-----------------
			//	Finalizer
			//-----------------
		}
	public: 
		int*				inpItem	;
		int*				outItem	;
		const char*			_rungStmt;
		//----------------------------
		//	Error Handling
		//----------------------------
		//PCDPM_FAMILY*		errStack;		// link to external error-stack , for collecting connection/semetic error
		STC_ERRHDL*			_errHandler;

		int		innerInpNum;				// the column number of inp array of inner layer , N columns
		int		rowCount;					// used to fast-recovering rung
		int		rungSn;						// serial number of this rung
		String^					rungStmt;	// statement of for this rung
		String^					assemblyPath;	// current executing parth ,  Hsien 2013/4/29

		//System::ComponentModel::ComponentResourceManager^	res;			
	//protected:
	public:
		//System::Resources::ResXResourceSet^					_ldResImages;			//Hsien , 2013.01.25,  read images
		System::ComponentModel::ComponentResourceManager^		_ldResImages;			//Hsien , 2013.01.25,  read images

	private: System::Windows::Forms::TextBox^  tb_Comments;
	private: System::Windows::Forms::ContextMenuStrip^  cmsRung;
	private: System::Windows::Forms::ToolStripMenuItem^  insertAboveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  insertBelowToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  deleteThisRungToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  addRowToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  deleteARowToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
	private: System::Windows::Forms::ToolStripMenuItem^  syntaxCheckToolStripMenuItem;
	private: System::Windows::Forms::Label^  labelRungSN;
	private: System::Windows::Forms::ToolStripMenuItem^  clearToolStripMenuItem;
	private: System::Windows::Forms::TableLayoutPanel^  tlpRung;
	private: System::ComponentModel::IContainer^  components;
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
			this->tb_Comments = (gcnew System::Windows::Forms::TextBox());
			this->cmsRung = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->insertAboveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->insertBelowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->deleteThisRungToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->addRowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->deleteARowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->syntaxCheckToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->clearToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->labelRungSN = (gcnew System::Windows::Forms::Label());
			this->tlpRung = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->cmsRung->SuspendLayout();
			this->SuspendLayout();
			// 
			// tb_Comments
			// 
			this->tb_Comments->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->tb_Comments->Location = System::Drawing::Point(0, 15);
			this->tb_Comments->Margin = System::Windows::Forms::Padding(0);
			this->tb_Comments->Multiline = true;
			this->tb_Comments->Name = L"tb_Comments";
			this->tb_Comments->Size = System::Drawing::Size(80, 35);
			this->tb_Comments->TabIndex = 0;
			// 
			// cmsRung
			// 
			this->cmsRung->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(10) {this->insertAboveToolStripMenuItem, 
				this->insertBelowToolStripMenuItem, this->deleteThisRungToolStripMenuItem, this->toolStripSeparator1, this->addRowToolStripMenuItem, 
				this->deleteARowToolStripMenuItem, this->toolStripSeparator2, this->syntaxCheckToolStripMenuItem, this->toolStripSeparator3, 
				this->clearToolStripMenuItem});
			this->cmsRung->Name = L"cms_rung";
			this->cmsRung->Size = System::Drawing::Size(180, 176);
			// 
			// insertAboveToolStripMenuItem
			// 
			this->insertAboveToolStripMenuItem->Name = L"insertAboveToolStripMenuItem";
			this->insertAboveToolStripMenuItem->Size = System::Drawing::Size(179, 22);
			this->insertAboveToolStripMenuItem->Text = L"Insert Rung Above";
			this->insertAboveToolStripMenuItem->Click += gcnew System::EventHandler(this, &rungControl::rungInsertAboveClick);
			// 
			// insertBelowToolStripMenuItem
			// 
			this->insertBelowToolStripMenuItem->Name = L"insertBelowToolStripMenuItem";
			this->insertBelowToolStripMenuItem->Size = System::Drawing::Size(179, 22);
			this->insertBelowToolStripMenuItem->Text = L"Insert Rung Below";
			this->insertBelowToolStripMenuItem->Click += gcnew System::EventHandler(this, &rungControl::rungInsertBelowClick);
			// 
			// deleteThisRungToolStripMenuItem
			// 
			this->deleteThisRungToolStripMenuItem->Name = L"deleteThisRungToolStripMenuItem";
			this->deleteThisRungToolStripMenuItem->Size = System::Drawing::Size(179, 22);
			this->deleteThisRungToolStripMenuItem->Text = L"Delete This Rung";
			this->deleteThisRungToolStripMenuItem->Click += gcnew System::EventHandler(this, &rungControl::rungDeleteClick);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(176, 6);
			// 
			// addRowToolStripMenuItem
			// 
			this->addRowToolStripMenuItem->Name = L"addRowToolStripMenuItem";
			this->addRowToolStripMenuItem->Size = System::Drawing::Size(179, 22);
			this->addRowToolStripMenuItem->Text = L"Add One Row";
			this->addRowToolStripMenuItem->Click += gcnew System::EventHandler(this, &rungControl::addRowClick);
			// 
			// deleteARowToolStripMenuItem
			// 
			this->deleteARowToolStripMenuItem->Name = L"deleteARowToolStripMenuItem";
			this->deleteARowToolStripMenuItem->Size = System::Drawing::Size(179, 22);
			this->deleteARowToolStripMenuItem->Text = L"Delete One Row";
			this->deleteARowToolStripMenuItem->Click += gcnew System::EventHandler(this, &rungControl::deleteRowClick);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(176, 6);
			// 
			// syntaxCheckToolStripMenuItem
			// 
			this->syntaxCheckToolStripMenuItem->Name = L"syntaxCheckToolStripMenuItem";
			this->syntaxCheckToolStripMenuItem->Size = System::Drawing::Size(179, 22);
			this->syntaxCheckToolStripMenuItem->Text = L"Syntax Check";
			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			this->toolStripSeparator3->Size = System::Drawing::Size(176, 6);
			// 
			// clearToolStripMenuItem
			// 
			this->clearToolStripMenuItem->Name = L"clearToolStripMenuItem";
			this->clearToolStripMenuItem->Size = System::Drawing::Size(179, 22);
			this->clearToolStripMenuItem->Text = L"Clear";
			this->clearToolStripMenuItem->Click += gcnew System::EventHandler(this, &rungControl::clearRowClick);
			// 
			// labelRungSN
			// 
			this->labelRungSN->BackColor = System::Drawing::SystemColors::ActiveBorder;
			this->labelRungSN->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->labelRungSN->Location = System::Drawing::Point(0, 0);
			this->labelRungSN->Margin = System::Windows::Forms::Padding(0);
			this->labelRungSN->Name = L"labelRungSN";
			this->labelRungSN->Size = System::Drawing::Size(80, 15);
			this->labelRungSN->TabIndex = 1;
			this->labelRungSN->Text = L"label1";
			this->labelRungSN->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// tlpRung
			// 
			this->tlpRung->AutoSize = true;
			this->tlpRung->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->tlpRung->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->tlpRung->ColumnCount = 21;
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 25)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 120)));
			this->tlpRung->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpRung->ForeColor = System::Drawing::SystemColors::ControlText;
			this->tlpRung->Location = System::Drawing::Point(80, 0);
			this->tlpRung->Margin = System::Windows::Forms::Padding(0);
			this->tlpRung->Name = L"tlpRung";
			this->tlpRung->RowCount = 1;
			this->tlpRung->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tlpRung->Size = System::Drawing::Size(1035, 50);
			this->tlpRung->TabIndex = 0;
			this->tlpRung->ControlAdded += gcnew System::Windows::Forms::ControlEventHandler(this, &rungControl::attachRightClick);
			// 
			// rungControl
			// 
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->Controls->Add(this->labelRungSN);
			this->Controls->Add(this->tb_Comments);
			this->Controls->Add(this->tlpRung);
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"rungControl";
			this->Size = System::Drawing::Size(1115, 50);
			this->Load += gcnew System::EventHandler(this, &rungControl::rungLoad);
			this->cmsRung->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void rungLoad(System::Object^  /*sender*/, System::EventArgs^  /*e*/);
		 int getIndexFromTlp(void);			// get serial number from parent tableLayoutPanel
//---------------------
//	Part : Rung Editing
//---------------------
private: System::Void addRowClick	(System::Object^  /*sender*/, System::EventArgs^  /*e*/);
private: System::Void deleteRowClick(System::Object^  /*sender*/, System::EventArgs^  /*e*/);
//private: System::Void rungRightClick	(System::Object^  sender, System::EventArgs^  e);
private: System::Void attachRightClick	(System::Object^  /*sender*/, System::Windows::Forms::ControlEventArgs^  e);
private: System::Void clearRowClick		(System::Object^  /*sender*/, System::EventArgs^  /*e*/);
public:
		 int	deleteRow	();
		 int	addRow		(int isFirstRow);
		 int	reset		();
		 int	bridgeAdjest();							// added by Hsien ,2013.01.25
		 int	markSn		(int serialNumber);			// for storing debug info
		 int	isMe		(const int serialNumber);	// will earn focus and turns the banner colour when called
														// added by Hsien , 2013.01.03
 //---------------------------------
 //	Part : ST-expression Generating:
 //---------------------------------
public:	 int  exprGenerate();
//---------------------------------------------------------------
//	Part : Meta Data storing/rebuiling , plain traverse functions
//---------------------------------------------------------------
public:
			int store	(CLST_METADATA**);
			int rebuild	(const CLST_METADATA	*rmds);
//---------------------------------------
//	Part : Ladder-Diagram Editing 
//---------------------------------------
private: System::Void rungInsertAboveClick	(System::Object^  sender, System::EventArgs^  e);
private: System::Void rungInsertBelowClick	(System::Object^  /*sender*/, System::EventArgs^  /*e*/);
private: System::Void rungDeleteClick		(System::Object^  /*sender*/, System::EventArgs^  /*e*/);
};//class
}// namespace
