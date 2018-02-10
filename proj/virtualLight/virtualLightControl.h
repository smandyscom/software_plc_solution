#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace _virtualDevice {

	/// <summary>
	/// Summary for virtualLightControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class virtualLightControl : public _virtualDevice::virtualDeviceCommon/*public System::Windows::Forms::UserControl*/
	{
	public:
		virtualLightControl(String^ __deviceName
			,char*	imageAddress
			,int	byteIndex
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
			this->lbLight->Text = this->_deviceName;

			//-------------------
			//	Set-up the image
			//-------------------
			this->linkToImage(imageAddress,bitIndex);

			//------------------
			//	Set-up Delegate
			//------------------
			this->_lbRefresh = gcnew validiateMethod(this,&virtualLightControl::turnTheLabel);


			this->tbDescription->TextChanged += gcnew System::EventHandler(this,&virtualLightControl::tcInput);
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
		~virtualLightControl()
		{
			if (components)
			{
				delete components;
			}
		}
	public:
		int					setLight(){
			//------------------------------
			//	Repack the Base Function , __address stand for
			//		the image
			//	Selected function
			//------------------------------
			this->setUIEffect(_lbRefresh);		
			return 1;
		}
		int					setLight(char*	__address){
			//------------------------------
			//	Repack the Base Function , __address stand for
			//		the image
			//------------------------------
			this->setUIEffect(__address,_lbRefresh);
			return 1;
		}
	protected:
		validiateMethod^	_lbRefresh;				// the delegate  of "turnTheLabel"
		int		turnTheLabel(const char _status){
			//--------------------------------------------------
			//	Turn the Lable Lighted According to Image Status
			//--------------------------------------------------
			if(_status)
				this->lbLight->BackColor = Color::Green;
			else
				this->lbLight->BackColor = Color::Red;

			return _status;
		}
	private: System::Windows::Forms::TextBox^  tbDescription;
	private: System::Windows::Forms::Label^  lbLight;
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
			this->tbDescription = (gcnew System::Windows::Forms::TextBox());
			this->lbLight = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// tbDescription
			// 
			this->tbDescription->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline)), 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(136)));
			this->tbDescription->Location = System::Drawing::Point(0, 27);
			this->tbDescription->Margin = System::Windows::Forms::Padding(0);
			this->tbDescription->Name = L"tbDescription";
			this->tbDescription->Size = System::Drawing::Size(50, 21);
			this->tbDescription->TabIndex = 2;
			// 
			// lbLight
			// 
			this->lbLight->BackColor = System::Drawing::Color::Green;
			this->lbLight->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->lbLight->Location = System::Drawing::Point(0, 0);
			this->lbLight->Margin = System::Windows::Forms::Padding(0);
			this->lbLight->Name = L"lbLight";
			this->lbLight->Size = System::Drawing::Size(50, 25);
			this->lbLight->TabIndex = 3;
			this->lbLight->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// virtualLightControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->Controls->Add(this->lbLight);
			this->Controls->Add(this->tbDescription);
			this->Name = L"virtualLightControl";
			this->Size = System::Drawing::Size(50, 48);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void	tcInput		(System::Object^ sender,System::EventArgs^ e){
				 //---------------------------------------------
				 //	Stored in Base Class
				 //---------------------------------------------
				 this->_deviceSymbol = this->tbDescription->Text;
			 }

	};
}
