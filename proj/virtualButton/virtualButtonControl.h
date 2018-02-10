#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace _virtualDevice {

	/// <summary>
	/// Summary for virtualButtonControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class virtualButtonControl : _virtualDevice::virtualDeviceCommon/*public System::Windows::Forms::UserControl*/
	{
	public:
		virtualButtonControl(String^ __deviceName
			,char*	imageAddress
			,int	byteIndex			// Hsien 2013/5/2 , show up only
			,int	bitIndex/*void*/)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			//-----------------
			//	Set-up the name , turns the format from 8-bit coded to 16-bit coded
			//-----------------
			int	wordIndexHex = (byteIndex*8+bitIndex)/16;
			int	bitIndexHex	 = (byteIndex*8+bitIndex)%16;

			this->setDeviceName(
				String::Format(VIR_BUTTON_DEVICE_NAME_FORMAT,__deviceName,wordIndexHex,bitIndexHex));
			this->btInput->Text = 
				this->_deviceName;

			//-------------------
			//	Set-up the image
			//-------------------
			this->linkToImage(imageAddress,bitIndex);

			//--------------------
			//	Event-Handler Link
			//--------------------
			this->btInput->MouseDown	+= gcnew System::Windows::Forms::MouseEventHandler(this,&virtualButtonControl::mouseDown);
			this->btInput->MouseUp		+= gcnew System::Windows::Forms::MouseEventHandler(this,&virtualButtonControl::mouseUp);
			this->tbDescription->TextChanged += gcnew System::EventHandler(this,&virtualButtonControl::tcInput);
		}
	public:
		int	postSymbol(){
			this->tbDescription->Text = this->_deviceSymbol;
			return 1;
		}
	protected:

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~virtualButtonControl()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^	btInput;
	private: System::Windows::Forms::TextBox^	tbDescription;
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
			this->btInput = (gcnew System::Windows::Forms::Button());
			this->tbDescription = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// btInput
			// 
			this->btInput->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->btInput->Location = System::Drawing::Point(0, 0);
			this->btInput->Margin = System::Windows::Forms::Padding(0);
			this->btInput->Name = L"btInput";
			this->btInput->Size = System::Drawing::Size(50, 25);
			this->btInput->TabIndex = 0;
			this->btInput->Text = L"button1";
			this->btInput->UseVisualStyleBackColor = true;
			// 
			// tbDescription
			// 
			this->tbDescription->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(136)));
			this->tbDescription->Location = System::Drawing::Point(0, 25);
			this->tbDescription->Margin = System::Windows::Forms::Padding(0);
			this->tbDescription->Name = L"tbDescription";
			this->tbDescription->Size = System::Drawing::Size(49, 21);
			this->tbDescription->TabIndex = 1;
			// 
			// virtualButtonControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->Controls->Add(this->tbDescription);
			this->Controls->Add(this->btInput);
			this->Name = L"virtualButtonControl";
			this->Size = System::Drawing::Size(50, 46);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:	System::Void mouseDown	(System::Object^ sender,System::Windows::Forms::MouseEventArgs^ e){
					this->oneShotPress(VIR_ONE_SHOT_PRESS_DOWN);
				}
	private:	System::Void mouseUp	(System::Object^ sender,System::Windows::Forms::MouseEventArgs^ e){
					this->oneShotPress(VIR_ONE_SHOT_PRESS_UP);
				}
	private: System::Void	tcInput		(System::Object^ sender,System::EventArgs^ e){
				 //---------------------------------------------
				 //	Stored in Base Class
				 //---------------------------------------------
				 this->_deviceSymbol = this->tbDescription->Text;
			 }
	};
}
