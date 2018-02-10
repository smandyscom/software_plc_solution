#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace _virtualDevice {

	/// <summary>
	/// Summary for virtualBitAccessor
	/// </summary>
	public ref class virtualBitAccessor : public _virtualDevice::virtualDeviceCommonWord/*public System::Windows::Forms::UserControl*/
	{
	public:
		virtualBitAccessor(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			short table[] = {
				0x0001,
				0x0002,
				0x0004,
				0x0008,
				0x0010,
				0x0020,
				0x0040,
				0x0080,
				0x0100,
				0x0200,
				0x0400,
				0x0800,
				0x1000,
				0x2000,
				0x4000,
				0x8000,
			};
			//Label^ lb;
			//------------------------------------------------------------
			//	Bit Accessor , to monitor each bit masked from source word
			//------------------------------------------------------------
			// Hsien 2013/5/30
			for(int i=0;i<this->tlpBit->ColumnCount;i++){
				this->tlpBit->Controls[i]->Tag	= table[i].ToString();
				this->tlpBit->Controls[i]->Text = 
					String::Format(".{0:D2}",i);
				//----------------------
				//	Addin Event Handle of one-shot operation
				//----------------------
				//lb = safe_cast<Label^>(this->tlpBit->Controls[i]);
				//lb->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &virtualBitAccessor::bitSet);
				//lb->MouseUp	  += gcnew System::Windows::Forms::MouseEventHandler(this, &virtualBitAccessor::bitUnset);
				//lb->Click	  += gcnew System::EventHandler(this, &virtualBitAccessor::bitToggle);
			}
			
			//----------------------
			//	Set-up bitRefresh delegate
			//----------------------
//			this->bitRefresh = gcnew validiateMethod(this,&virtualBitAccessor::turnTheLabel);
			this->monitorRefresh	= gcnew /*monitorRefreshMethod*/delegatePrototype(this,&virtualBitAccessor::turnTheLabel);
			this->actOvrride		= gcnew delegatePrototype(this,&virtualBitAccessor::loadOverrideHandler);	// Hsien 2013/6/4
		}
	protected:
		int isToggleMode;							// Hsien 2013/6/4 , triggered by Key event
	public:

//		validiateMethod^	bitRefresh;				// the delegate  of "turnTheLabel"
		int		turnTheLabel(/*String^*/const int _status){
			//--------------------------------------------------
			//	Turn the Lable Lighted According to Image Status
			//--------------------------------------------------
			for each(Windows::Forms::Label^ e in this->tlpBit->Controls){
				if(*this->monitorAddress & System::Convert::ToInt16(e->Tag->ToString(),10)){
					// Corresponding Bit is Ture , turn on 
					e->BackColor = Color::YellowGreen;
				}
				else
					// Hsien 2013/5/30 , turn off
					e->BackColor = Color::Gray;
			}

			return 1;
		}

		int		loadOverrideHandler(const int){
			//----------------------
			//
			// Hsien 2013/6/4
			//----------------------
			Label^ lb;

			for(int i=0;i<this->tlpBit->ColumnCount;i++){
				//----------------------
				//	Addin Event Handle of one-shot operation
				//----------------------
				lb = safe_cast<Label^>(this->tlpBit->Controls[i]);
				lb->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &virtualBitAccessor::bitSet);
				lb->MouseUp	  += gcnew System::Windows::Forms::MouseEventHandler(this, &virtualBitAccessor::bitUnset);
				lb->Click	  += gcnew System::EventHandler(this, &virtualBitAccessor::bitToggle);
			}
			return 1;
		}
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~virtualBitAccessor()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^  tlpBit;
	private: System::Windows::Forms::Label^  labelBit15;
	private: System::Windows::Forms::Label^  labelBit0;
	private: System::Windows::Forms::Label^  labelBit1;
	private: System::Windows::Forms::Label^  labelBit2;
	private: System::Windows::Forms::Label^  label1Bit3;
	private: System::Windows::Forms::Label^  label1Bit4;
	private: System::Windows::Forms::Label^  label1Bit5;
	private: System::Windows::Forms::Label^  labelBit6;
	private: System::Windows::Forms::Label^  labelBit7;
	private: System::Windows::Forms::Label^  labelBit8;
	private: System::Windows::Forms::Label^  labelBit9;
	private: System::Windows::Forms::Label^  labelBit10;
	private: System::Windows::Forms::Label^  labelBit11;
	private: System::Windows::Forms::Label^  labelBit12;
	private: System::Windows::Forms::Label^  labelBit13;
	private: System::Windows::Forms::Label^  labelBit14;
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
			this->tlpBit = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->labelBit0 = (gcnew System::Windows::Forms::Label());
			this->labelBit1 = (gcnew System::Windows::Forms::Label());
			this->labelBit2 = (gcnew System::Windows::Forms::Label());
			this->label1Bit3 = (gcnew System::Windows::Forms::Label());
			this->label1Bit4 = (gcnew System::Windows::Forms::Label());
			this->label1Bit5 = (gcnew System::Windows::Forms::Label());
			this->labelBit6 = (gcnew System::Windows::Forms::Label());
			this->labelBit7 = (gcnew System::Windows::Forms::Label());
			this->labelBit8 = (gcnew System::Windows::Forms::Label());
			this->labelBit9 = (gcnew System::Windows::Forms::Label());
			this->labelBit10 = (gcnew System::Windows::Forms::Label());
			this->labelBit11 = (gcnew System::Windows::Forms::Label());
			this->labelBit12 = (gcnew System::Windows::Forms::Label());
			this->labelBit13 = (gcnew System::Windows::Forms::Label());
			this->labelBit14 = (gcnew System::Windows::Forms::Label());
			this->labelBit15 = (gcnew System::Windows::Forms::Label());
			this->tlpBit->SuspendLayout();
			this->SuspendLayout();
			// 
			// tlpBit
			// 
			this->tlpBit->AutoSize = true;
			this->tlpBit->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->tlpBit->ColumnCount = 16;
			this->tlpBit->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpBit->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpBit->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpBit->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpBit->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpBit->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpBit->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpBit->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpBit->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpBit->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpBit->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpBit->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpBit->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpBit->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpBit->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpBit->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpBit->Controls->Add(this->labelBit0, 15, 0);
			this->tlpBit->Controls->Add(this->labelBit1, 14, 0);
			this->tlpBit->Controls->Add(this->labelBit2, 13, 0);
			this->tlpBit->Controls->Add(this->label1Bit3, 12, 0);
			this->tlpBit->Controls->Add(this->label1Bit4, 11, 0);
			this->tlpBit->Controls->Add(this->label1Bit5, 10, 0);
			this->tlpBit->Controls->Add(this->labelBit6, 9, 0);
			this->tlpBit->Controls->Add(this->labelBit7, 8, 0);
			this->tlpBit->Controls->Add(this->labelBit8, 7, 0);
			this->tlpBit->Controls->Add(this->labelBit9, 6, 0);
			this->tlpBit->Controls->Add(this->labelBit10, 5, 0);
			this->tlpBit->Controls->Add(this->labelBit11, 4, 0);
			this->tlpBit->Controls->Add(this->labelBit12, 3, 0);
			this->tlpBit->Controls->Add(this->labelBit13, 2, 0);
			this->tlpBit->Controls->Add(this->labelBit14, 1, 0);
			this->tlpBit->Controls->Add(this->labelBit15, 0, 0);
			this->tlpBit->Location = System::Drawing::Point(3, 2);
			this->tlpBit->Margin = System::Windows::Forms::Padding(0);
			this->tlpBit->Name = L"tlpBit";
			this->tlpBit->RowCount = 1;
			this->tlpBit->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpBit->Size = System::Drawing::Size(320, 20);
			this->tlpBit->TabIndex = 3;
			// 
			// labelBit0
			// 
			this->labelBit0->Location = System::Drawing::Point(300, 0);
			this->labelBit0->Margin = System::Windows::Forms::Padding(0);
			this->labelBit0->Name = L"labelBit0";
			this->labelBit0->Size = System::Drawing::Size(20, 20);
			this->labelBit0->TabIndex = 15;
			this->labelBit0->Tag = L"0x0001";
			// 
			// labelBit1
			// 
			this->labelBit1->Location = System::Drawing::Point(280, 0);
			this->labelBit1->Margin = System::Windows::Forms::Padding(0);
			this->labelBit1->Name = L"labelBit1";
			this->labelBit1->Size = System::Drawing::Size(20, 20);
			this->labelBit1->TabIndex = 14;
			// 
			// labelBit2
			// 
			this->labelBit2->Location = System::Drawing::Point(260, 0);
			this->labelBit2->Margin = System::Windows::Forms::Padding(0);
			this->labelBit2->Name = L"labelBit2";
			this->labelBit2->Size = System::Drawing::Size(20, 20);
			this->labelBit2->TabIndex = 13;
			// 
			// label1Bit3
			// 
			this->label1Bit3->Location = System::Drawing::Point(240, 0);
			this->label1Bit3->Margin = System::Windows::Forms::Padding(0);
			this->label1Bit3->Name = L"label1Bit3";
			this->label1Bit3->Size = System::Drawing::Size(20, 20);
			this->label1Bit3->TabIndex = 12;
			// 
			// label1Bit4
			// 
			this->label1Bit4->Location = System::Drawing::Point(220, 0);
			this->label1Bit4->Margin = System::Windows::Forms::Padding(0);
			this->label1Bit4->Name = L"label1Bit4";
			this->label1Bit4->Size = System::Drawing::Size(20, 20);
			this->label1Bit4->TabIndex = 11;
			// 
			// label1Bit5
			// 
			this->label1Bit5->Location = System::Drawing::Point(200, 0);
			this->label1Bit5->Margin = System::Windows::Forms::Padding(0);
			this->label1Bit5->Name = L"label1Bit5";
			this->label1Bit5->Size = System::Drawing::Size(20, 20);
			this->label1Bit5->TabIndex = 10;
			// 
			// labelBit6
			// 
			this->labelBit6->Location = System::Drawing::Point(180, 0);
			this->labelBit6->Margin = System::Windows::Forms::Padding(0);
			this->labelBit6->Name = L"labelBit6";
			this->labelBit6->Size = System::Drawing::Size(20, 20);
			this->labelBit6->TabIndex = 9;
			// 
			// labelBit7
			// 
			this->labelBit7->Location = System::Drawing::Point(160, 0);
			this->labelBit7->Margin = System::Windows::Forms::Padding(0);
			this->labelBit7->Name = L"labelBit7";
			this->labelBit7->Size = System::Drawing::Size(20, 20);
			this->labelBit7->TabIndex = 8;
			// 
			// labelBit8
			// 
			this->labelBit8->Location = System::Drawing::Point(140, 0);
			this->labelBit8->Margin = System::Windows::Forms::Padding(0);
			this->labelBit8->Name = L"labelBit8";
			this->labelBit8->Size = System::Drawing::Size(20, 20);
			this->labelBit8->TabIndex = 7;
			// 
			// labelBit9
			// 
			this->labelBit9->Location = System::Drawing::Point(120, 0);
			this->labelBit9->Margin = System::Windows::Forms::Padding(0);
			this->labelBit9->Name = L"labelBit9";
			this->labelBit9->Size = System::Drawing::Size(20, 20);
			this->labelBit9->TabIndex = 6;
			// 
			// labelBit10
			// 
			this->labelBit10->Location = System::Drawing::Point(100, 0);
			this->labelBit10->Margin = System::Windows::Forms::Padding(0);
			this->labelBit10->Name = L"labelBit10";
			this->labelBit10->Size = System::Drawing::Size(20, 20);
			this->labelBit10->TabIndex = 5;
			// 
			// labelBit11
			// 
			this->labelBit11->Location = System::Drawing::Point(80, 0);
			this->labelBit11->Margin = System::Windows::Forms::Padding(0);
			this->labelBit11->Name = L"labelBit11";
			this->labelBit11->Size = System::Drawing::Size(20, 20);
			this->labelBit11->TabIndex = 4;
			// 
			// labelBit12
			// 
			this->labelBit12->Location = System::Drawing::Point(60, 0);
			this->labelBit12->Margin = System::Windows::Forms::Padding(0);
			this->labelBit12->Name = L"labelBit12";
			this->labelBit12->Size = System::Drawing::Size(20, 20);
			this->labelBit12->TabIndex = 3;
			// 
			// labelBit13
			// 
			this->labelBit13->BackColor = System::Drawing::Color::Green;
			this->labelBit13->Location = System::Drawing::Point(40, 0);
			this->labelBit13->Margin = System::Windows::Forms::Padding(0);
			this->labelBit13->Name = L"labelBit13";
			this->labelBit13->Size = System::Drawing::Size(20, 20);
			this->labelBit13->TabIndex = 2;
			// 
			// labelBit14
			// 
			this->labelBit14->BackColor = System::Drawing::Color::Red;
			this->labelBit14->Location = System::Drawing::Point(20, 0);
			this->labelBit14->Margin = System::Windows::Forms::Padding(0);
			this->labelBit14->Name = L"labelBit14";
			this->labelBit14->Size = System::Drawing::Size(20, 20);
			this->labelBit14->TabIndex = 1;
			// 
			// labelBit15
			// 
			this->labelBit15->BackColor = System::Drawing::Color::Lime;
			this->labelBit15->Location = System::Drawing::Point(0, 0);
			this->labelBit15->Margin = System::Windows::Forms::Padding(0);
			this->labelBit15->Name = L"labelBit15";
			this->labelBit15->Size = System::Drawing::Size(20, 20);
			this->labelBit15->TabIndex = 0;
			// 
			// virtualBitAccessor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->Controls->Add(this->tlpBit);
			this->Name = L"virtualBitAccessor";
			this->Size = System::Drawing::Size(323, 22);
			this->tlpBit->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void bitSet(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 	//----------------------
				//	Use Tag to mask out taget bit , and write
				//----------------------
			 if(e->Button
				 == Windows::Forms::MouseButtons::Left/*!isToggleMode*/){
				 Control^ ctl = safe_cast<Control^>(sender);
				 *this->ovrrideAddress |= System::Convert::ToInt16(ctl->Tag->ToString(),10);
			 }//if
		 }
private: System::Void bitUnset(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 	//----------------------
				//	Use Tag to mask out taget bit , and write
				//----------------------
			 if(e->Button
				 == Windows::Forms::MouseButtons::Left/*!isToggleMode*/){
				 Control^ ctl = safe_cast<Control^>(sender);
				 *this->ovrrideAddress &= ~System::Convert::ToInt16(ctl->Tag->ToString(),10);
			 }
		 }
private: System::Void bitToggle(System::Object^  sender, System::EventArgs^  e) {
			 //----------------------
			 //
			 // Hsien 2013/6/4
			 //----------------------
			 Control^ ctl = safe_cast<Control^>(sender);

			 if(/*isToggleMode*//*safe_cast<Windows::Forms::KeyEventArgs^>(e)->Alt*/safe_cast<Windows::Forms::MouseEventArgs^>(e)->Button
				 == Windows::Forms::MouseButtons::Right){
				 if(*this->ovrrideAddress & System::Convert::ToInt16(ctl->Tag->ToString(),10))
					 //----------------------
					 //	Unset
					 //----------------------
					 *this->ovrrideAddress &= ~(System::Convert::ToInt16(ctl->Tag->ToString(),10));
				 else
					 *this->ovrrideAddress |= (System::Convert::ToInt16(ctl->Tag->ToString(),10));
			 }
		 }
//private: System::Void toggleModeChange(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
//			 //----------------------
//			 //
//			 //----------------------
//			 this->isToggleMode = e->Alt;
//		 }
};
}
