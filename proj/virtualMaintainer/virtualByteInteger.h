#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace _virtualDevice {

	/// <summary>
	/// Summary for virtualByteInteger
	/// </summary>
	public ref class virtualByteInteger : public _virtualDevice::virtualDeviceCommonWord/*public System::Windows::Forms::UserControl*/
	{
	public:
		virtualByteInteger(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->tbLeastByte->ReadOnly = true;
			this->tbMostByte->ReadOnly  = true;
			this->tbMSBOverride->Enabled = false;
			this->tbLSBOverride->Enabled = false;
			this->tbMSBOverride->Hide();
			this->tbLSBOverride->Hide();
			//----------------------
			//	Common delegate initializing
			// Hsien 2013/5/31
			//----------------------
			this->monitorRefresh	= gcnew delegatePrototype(this,&virtualByteInteger::showBytes);
			this->actOvrride		= gcnew delegatePrototype(this,&virtualByteInteger::loadOverrideHandler);	// Hsien 2013/6/4
		}
		int showBytes(const int status){
			//----------------------
			//	
			//----------------------
			this->tbMostByte->Text = (((char*)this->monitorAddress)[1]).ToString();
			this->tbLeastByte->Text = (((char*)this->monitorAddress)[0]).ToString();
			return 1;
		}
		int		loadOverrideHandler(const int){
			//----------------------
			//
			// Hsien 2013/6/4
			//----------------------
			//this->tbMostByte->TextChanged	+= gcnew System::EventHandler(this, &virtualByteInteger::tcInput);
			//this->tbLeastByte->TextChanged	+= gcnew System::EventHandler(this, &virtualByteInteger::tcInput);
			this->tbMSBOverride->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this,&virtualByteInteger::kpOverride);
			this->tbLSBOverride->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this,&virtualByteInteger::kpOverride);

			this->tbMSBOverride->Enabled = true;
			this->tbLSBOverride->Enabled = true;
			this->tbMSBOverride->Show();
			this->tbLSBOverride->Show();
			//this->tbLeastByte->ReadOnly = false;
			//this->tbMostByte->ReadOnly  = false;
			
			return 1;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~virtualByteInteger()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  tbMostByte;
	private: System::Windows::Forms::TextBox^  tbLeastByte;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ContextMenuStrip^  cmsFormat;
	private: System::Windows::Forms::ToolStripMenuItem^  hEXToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  iNTEGERToolStripMenuItem;
	private: System::Windows::Forms::TextBox^  tbMSBOverride;
	private: System::Windows::Forms::TextBox^  tbLSBOverride;


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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->cmsFormat = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->hEXToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->iNTEGERToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tbMostByte = (gcnew System::Windows::Forms::TextBox());
			this->tbLeastByte = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->tbMSBOverride = (gcnew System::Windows::Forms::TextBox());
			this->tbLSBOverride = (gcnew System::Windows::Forms::TextBox());
			this->cmsFormat->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ContextMenuStrip = this->cmsFormat;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->label1->Location = System::Drawing::Point(3, 2);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(24, 21);
			this->label1->TabIndex = 0;
			this->label1->Text = L":1";
			// 
			// cmsFormat
			// 
			this->cmsFormat->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->hEXToolStripMenuItem, 
				this->iNTEGERToolStripMenuItem});
			this->cmsFormat->Name = L"cmsFormat";
			this->cmsFormat->Size = System::Drawing::Size(128, 48);
			// 
			// hEXToolStripMenuItem
			// 
			this->hEXToolStripMenuItem->Name = L"hEXToolStripMenuItem";
			this->hEXToolStripMenuItem->Size = System::Drawing::Size(127, 22);
			this->hEXToolStripMenuItem->Text = L"HEX";
			// 
			// iNTEGERToolStripMenuItem
			// 
			this->iNTEGERToolStripMenuItem->Name = L"iNTEGERToolStripMenuItem";
			this->iNTEGERToolStripMenuItem->Size = System::Drawing::Size(127, 22);
			this->iNTEGERToolStripMenuItem->Text = L"INTEGER";
			// 
			// tbMostByte
			// 
			this->tbMostByte->ContextMenuStrip = this->cmsFormat;
			this->tbMostByte->Location = System::Drawing::Point(23, 2);
			this->tbMostByte->Name = L"tbMostByte";
			this->tbMostByte->ReadOnly = true;
			this->tbMostByte->Size = System::Drawing::Size(50, 22);
			this->tbMostByte->TabIndex = 1;
			this->tbMostByte->Tag = L"1";
			this->tbMostByte->Text = L"-127";
			// 
			// tbLeastByte
			// 
			this->tbLeastByte->ContextMenuStrip = this->cmsFormat;
			this->tbLeastByte->Location = System::Drawing::Point(165, 2);
			this->tbLeastByte->Name = L"tbLeastByte";
			this->tbLeastByte->ReadOnly = true;
			this->tbLeastByte->Size = System::Drawing::Size(50, 22);
			this->tbLeastByte->TabIndex = 3;
			this->tbLeastByte->Tag = L"0";
			this->tbLeastByte->Text = L"-127";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ContextMenuStrip = this->cmsFormat;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->label2->Location = System::Drawing::Point(135, 2);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(24, 21);
			this->label2->TabIndex = 2;
			this->label2->Text = L":0";
			// 
			// tbMSBOverride
			// 
			this->tbMSBOverride->ContextMenuStrip = this->cmsFormat;
			this->tbMSBOverride->Location = System::Drawing::Point(79, 2);
			this->tbMSBOverride->Name = L"tbMSBOverride";
			this->tbMSBOverride->Size = System::Drawing::Size(50, 22);
			this->tbMSBOverride->TabIndex = 4;
			this->tbMSBOverride->Tag = L"1";
			this->tbMSBOverride->Text = L"-127";
			// 
			// tbLSBOverride
			// 
			this->tbLSBOverride->ContextMenuStrip = this->cmsFormat;
			this->tbLSBOverride->Location = System::Drawing::Point(221, 2);
			this->tbLSBOverride->Name = L"tbLSBOverride";
			this->tbLSBOverride->Size = System::Drawing::Size(50, 22);
			this->tbLSBOverride->TabIndex = 5;
			this->tbLSBOverride->Tag = L"0";
			this->tbLSBOverride->Text = L"-127";
			// 
			// virtualByteInteger
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ContextMenuStrip = this->cmsFormat;
			this->Controls->Add(this->tbLSBOverride);
			this->Controls->Add(this->tbMSBOverride);
			this->Controls->Add(this->tbLeastByte);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->tbMostByte);
			this->Controls->Add(this->label1);
			this->Name = L"virtualByteInteger";
			this->Size = System::Drawing::Size(326, 26);
			this->cmsFormat->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	//private: System::Void tcInput(System::Object^  sender, System::EventArgs^  e) {
	//			 //---------------------------
	//			 //	Tag means MSB[1] or LSB[0]
	//			 //---------------------------
	//			 Windows::Forms::TextBox^ tb = safe_cast<Windows::Forms::TextBox^>(sender);
	//			 int index = System::Convert::ToInt32(tb->Tag->ToString());

	//			 ((char*)this->monitorAddress)[index] = System::Convert::ToSByte(tb->Text);	// Hsien 2013/6/4 , signed integer
	//		 }
private: System::Void kpOverride(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
			 if(e->KeyChar == (char)Windows::Forms::Keys::Enter){
				 //---------------------------
				 //	Tag means MSB[1] or LSB[0]
				 //---------------------------
				 Windows::Forms::TextBox^ tb = safe_cast<Windows::Forms::TextBox^>(sender);
				 int index = System::Convert::ToInt32(tb->Tag->ToString());

				 ((char*)this->ovrrideAddress)[index] = System::Convert::ToSByte(tb->Text);	// Hsien 2013/6/4 , signed integer
			 }
		 }
};
}
