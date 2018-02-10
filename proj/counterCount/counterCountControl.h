#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace outDev {

	/// <summary>
	/// Summary for counterCountControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class counterCountControl : public System::Windows::Forms::UserControl
	{
	public:
		counterCountControl(outputDevice*	instance)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->_rcCount = (odRingCounterCount*)instance;
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~counterCountControl()
		{
			if (components)
			{
				delete components;
			}
		}
	protected:
		odRingCounterCount*	_rcCount;

	private: System::Windows::Forms::CheckBox^  cbDown;
	private: System::Windows::Forms::NumericUpDown^  numericUpDownCounter;
	private: System::Windows::Forms::Label^  labelIndex;
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(counterCountControl::typeid));
			this->cbDown = (gcnew System::Windows::Forms::CheckBox());
			this->numericUpDownCounter = (gcnew System::Windows::Forms::NumericUpDown());
			this->labelIndex = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownCounter))->BeginInit();
			this->SuspendLayout();
			// 
			// cbDown
			// 
			this->cbDown->AutoSize = true;
			this->cbDown->Location = System::Drawing::Point(65, 0);
			this->cbDown->Name = L"cbDown";
			this->cbDown->Size = System::Drawing::Size(52, 16);
			this->cbDown->TabIndex = 2;
			this->cbDown->Text = L"Down";
			this->cbDown->UseVisualStyleBackColor = true;
			this->cbDown->CheckedChanged += gcnew System::EventHandler(this, &counterCountControl::downCount);
			// 
			// numericUpDownCounter
			// 
			this->numericUpDownCounter->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->numericUpDownCounter->Location = System::Drawing::Point(72, 27);
			this->numericUpDownCounter->Margin = System::Windows::Forms::Padding(3, 0, 3, 0);
			this->numericUpDownCounter->Name = L"numericUpDownCounter";
			this->numericUpDownCounter->Size = System::Drawing::Size(45, 21);
			this->numericUpDownCounter->TabIndex = 3;
			this->numericUpDownCounter->ValueChanged += gcnew System::EventHandler(this, &counterCountControl::ncInput);
			// 
			// labelIndex
			// 
			this->labelIndex->AutoSize = true;
			this->labelIndex->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->labelIndex->Location = System::Drawing::Point(27, 29);
			this->labelIndex->Name = L"labelIndex";
			this->labelIndex->Size = System::Drawing::Size(39, 11);
			this->labelIndex->TabIndex = 4;
			this->labelIndex->Text = L"Index¡G";
			// 
			// counterCountControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->Controls->Add(this->labelIndex);
			this->Controls->Add(this->numericUpDownCounter);
			this->Controls->Add(this->cbDown);
			this->Name = L"counterCountControl";
			this->Size = System::Drawing::Size(120, 50);
			this->Load += gcnew System::EventHandler(this, &counterCountControl::counterCountControlLoad);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownCounter))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	//private: System::Void tcInput(System::Object^ sender,System::EventArgs^ e){
	//			 try{
	//				 this->_rcCount->setCounterIndex(Int32::Parse(this->tbCountIndex->Text));
	//				 this->tbCountIndex->ForeColor = Color::Black;
	//			 }
	//			 catch(FormatException^){
	//				 //----------------------
	//				 //	Invalid Format(Can't Forming a Integer , notice user by changing the fore color
	//				 //----------------------
	//				 this->tbCountIndex->ForeColor = Color::Red;
	//			 }
	//		 }
	private: System::Void counterCountControlLoad(System::Object^ sender,System::EventArgs^ e){
				 //-------------------
				 //	GUI rebuild
				 //-------------------
//				 this->tbCountIndex->Text = this->_rcCount->getCounterIndex().ToString();
				 this->numericUpDownCounter->Value = this->_rcCount->getCounterIndex();
				 switch(this->_rcCount->getType()){
					 case ODRINGCTR_COUNTUP:									break;
					 case ODRINGCTR_COUNTDOWN:	this->cbDown->Checked = true;	break;
					 default:
						 break;
				 }
			 }
	private: System::Void downCount(System::Object^  sender, System::EventArgs^  e) {
				 //-----------------------
				 //	If THE check box had been checked , change the counter type as down-counting
				 //		Hsien , 2013.01.22
				 //-----------------------
				 if(this->cbDown->Checked)
					 this->_rcCount->setCounterType(ODRINGCTR_COUNTDOWN);
				 else
					 this->_rcCount->setCounterType(ODRINGCTR_COUNTUP);
								// Hsien 2013/5/8 , otherwise , the od type won't change when un-checked
			 }
	private: System::Void ncInput(System::Object^  sender, System::EventArgs^  e) {
				 // Hsien 2013/5/21
				 this->_rcCount->setCounterIndex(System::Decimal::ToInt32(this->numericUpDownCounter->Value));
			 }
};
}
