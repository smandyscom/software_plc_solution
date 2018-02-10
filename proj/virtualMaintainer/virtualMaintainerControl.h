#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace _virtualDevice {

	/// <summary>
	/// Summary for virtualMaintainerControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class virtualMaintainerControl : public System::Windows::Forms::UserControl
	{
	public:
		virtualMaintainerControl(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			//----------------------
			//	Childs initializing
			//----------------------
			this->monitorsContainer = gcnew array<virtualDeviceCommonWord^>(MAX_MONITOR_COUNT);
			this->monitorsContainer[INDEX_MONITOR_BIT]	= safe_cast<virtualDeviceCommonWord^>(gcnew virtualBitAccessor());
			this->monitorsContainer[INDEX_MONITOR_BYTE] = safe_cast<virtualDeviceCommonWord^>(gcnew virtualByteInteger());
			this->monitorsContainer[INDEX_MONITOR_WORD] = safe_cast<virtualDeviceCommonWord^>(gcnew virtualWordInteger());
			//----------------------
			//	Default: bit monitor
			//----------------------
			this->pnl->Controls->Add(this->monitorsContainer[INDEX_MONITOR_BIT]);
		}
	public:
		int	registerImage(short*		address		,const int imgIndex){
			//----------------------
			//	After initializing , should call this to link to image
			//----------------------
			for each(virtualDeviceCommonWord^ e in this->monitorsContainer)
				e->linkToImage(address,imgIndex);
			return 1;
		}
		int	monitorRefresh(const int){
			//----------------------
			//	Call common delegate 
			//----------------------
			for each(virtualDeviceCommonWord^ e in this->monitorsContainer)
				e->monitorRefresh(0);
			return 1;
		}
		int registerName(String^ addressName){
			// Hsien 2013/6/4
			this->lbAddressName->Text = addressName;
			return 1;
		}
		int	enableOverride(const int ){
			//----------------------
			//	Call common delegate 
			//----------------------
			for each(virtualDeviceCommonWord^ e in this->monitorsContainer)
				e->actOvrride(0);
			return 0;
		}
	protected:
		array<virtualDeviceCommonWord^>^	monitorsContainer;		// Hsien 2013/5/31
	/*	virtualBitAccessor^		monitorBit;
		virtualByteInteger^		monitorByte;
		virtualWordInteger^		monitorWord;*/
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~virtualMaintainerControl()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  lbAddressName;
	private: System::Windows::Forms::Panel^  pnl;
	private: System::Windows::Forms::ContextMenuStrip^  cmsFormat;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmiBit;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmiByte;
	private: System::Windows::Forms::ToolStripMenuItem^  tsmiWord;
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
			this->lbAddressName = (gcnew System::Windows::Forms::Label());
			this->cmsFormat = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->tsmiBit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiByte = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiWord = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pnl = (gcnew System::Windows::Forms::Panel());
			this->cmsFormat->SuspendLayout();
			this->SuspendLayout();
			// 
			// lbAddressName
			// 
			this->lbAddressName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->lbAddressName->AutoSize = true;
			this->lbAddressName->ContextMenuStrip = this->cmsFormat;
			this->lbAddressName->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->lbAddressName->Location = System::Drawing::Point(0, 4);
			this->lbAddressName->Name = L"lbAddressName";
			this->lbAddressName->Size = System::Drawing::Size(113, 21);
			this->lbAddressName->TabIndex = 0;
			this->lbAddressName->Text = L"SYNC_REQ00";
			this->lbAddressName->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// cmsFormat
			// 
			this->cmsFormat->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->tsmiBit, this->tsmiByte, 
				this->tsmiWord});
			this->cmsFormat->Name = L"cmsFormat";
			this->cmsFormat->Size = System::Drawing::Size(109, 70);
			// 
			// tsmiBit
			// 
			this->tsmiBit->Name = L"tsmiBit";
			this->tsmiBit->Size = System::Drawing::Size(108, 22);
			this->tsmiBit->Tag = L"0";
			this->tsmiBit->Text = L"Bit";
			this->tsmiBit->Click += gcnew System::EventHandler(this, &virtualMaintainerControl::contextMenuClick);
			// 
			// tsmiByte
			// 
			this->tsmiByte->Name = L"tsmiByte";
			this->tsmiByte->Size = System::Drawing::Size(108, 22);
			this->tsmiByte->Tag = L"1";
			this->tsmiByte->Text = L"Byte";
			this->tsmiByte->Click += gcnew System::EventHandler(this, &virtualMaintainerControl::contextMenuClick);
			// 
			// tsmiWord
			// 
			this->tsmiWord->Name = L"tsmiWord";
			this->tsmiWord->Size = System::Drawing::Size(108, 22);
			this->tsmiWord->Tag = L"2";
			this->tsmiWord->Text = L"Word";
			this->tsmiWord->Click += gcnew System::EventHandler(this, &virtualMaintainerControl::contextMenuClick);
			// 
			// pnl
			// 
			this->pnl->Location = System::Drawing::Point(116, 0);
			this->pnl->Margin = System::Windows::Forms::Padding(0);
			this->pnl->Name = L"pnl";
			this->pnl->Size = System::Drawing::Size(330, 30);
			this->pnl->TabIndex = 1;
			// 
			// virtualMaintainerControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->Controls->Add(this->lbAddressName);
			this->Controls->Add(this->pnl);
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"virtualMaintainerControl";
			this->Size = System::Drawing::Size(446, 30);
			this->cmsFormat->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void contextMenuClick(System::Object^  sender, System::EventArgs^  e) {
				 //----------------------
				 //	Convert tag into index
				 //	Add Control according to index
				 //----------------------
				 Windows::Forms::ToolStripMenuItem^ tsmi 
					 = safe_cast<Windows::Forms::ToolStripMenuItem^>(sender);
				 int	index = System::Convert::ToInt32(tsmi->Tag->ToString());
				 this->pnl->Controls->Clear();		// remove all control Hsien 2013/5/31
				 this->pnl->Controls->Add(this->monitorsContainer[index]);
			 }
};
}
