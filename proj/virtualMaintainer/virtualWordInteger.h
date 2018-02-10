#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace _virtualDevice {

	/// <summary>
	/// Summary for virtualWordInteger
	/// </summary>
	public ref class virtualWordInteger : public _virtualDevice::virtualDeviceCommonWord/*public System::Windows::Forms::UserControl*/
	{
	public:
		virtualWordInteger(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->tbWord->ReadOnly = true;
			this->tbWordOverride->Enabled = false;
			this->tbWordOverride->Hide();
			//----------------------
			//	Common delegate initializing
			// Hsien 2013/5/31
			//----------------------
			this->monitorRefresh	= gcnew delegatePrototype(this,&virtualWordInteger::showWord);
			this->actOvrride		= gcnew delegatePrototype(this,&virtualWordInteger::loadOverrideHandler);	// Hsien 2013/6/4
		}
		int showWord(const int status){
			//----------------------
			//
			//----------------------
			this->tbWord->Text = (*this->monitorAddress).ToString();
			return 1;
		}
		int		loadOverrideHandler(const int){
			//----------------------
			//
			// Hsien 2013/6/4
			//----------------------
			//this->tbWord->TextChanged += gcnew System::EventHandler(this, &virtualWordInteger::tcInput);

			//this->tbWord->ReadOnly = false;
			this->tbWordOverride->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this,&virtualWordInteger::kpOverride);
			this->tbWordOverride->Enabled = true;
			this->tbWordOverride->Show();
			return 1;
		}


	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~virtualWordInteger()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  tbWord;
	private: System::Windows::Forms::TextBox^  tbWordOverride;
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
			this->tbWord = (gcnew System::Windows::Forms::TextBox());
			this->tbWordOverride = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// tbWord
			// 
			this->tbWord->Location = System::Drawing::Point(4, 2);
			this->tbWord->Name = L"tbWord";
			this->tbWord->ReadOnly = true;
			this->tbWord->Size = System::Drawing::Size(100, 22);
			this->tbWord->TabIndex = 0;
			this->tbWord->Text = L"-32767";
			this->tbWord->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// tbWordOverride
			// 
			this->tbWordOverride->Location = System::Drawing::Point(113, 2);
			this->tbWordOverride->Name = L"tbWordOverride";
			this->tbWordOverride->Size = System::Drawing::Size(100, 22);
			this->tbWordOverride->TabIndex = 1;
			this->tbWordOverride->Text = L"-32767";
			this->tbWordOverride->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->tbWordOverride->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &virtualWordInteger::kpOverride);
			// 
			// virtualWordInteger
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->tbWordOverride);
			this->Controls->Add(this->tbWord);
			this->Name = L"virtualWordInteger";
			this->Size = System::Drawing::Size(326, 26);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	//private: System::Void tcInput(System::Object^  sender, System::EventArgs^  e) {
	//			 //----------------------
	//			 //
	//			 //----------------------
	//			 *this->monitorAddress = System::Convert::ToInt16(this->tbWord->Text);
	//		 }
	private: System::Void kpOverride(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
				 //----------------------
				 //
				 //----------------------
				 if(e->KeyChar == (char)System::Windows::Forms::Keys::Enter)
					 *this->ovrrideAddress = System::Convert::ToInt16(this->tbWordOverride->Text);
			 }
};
}
