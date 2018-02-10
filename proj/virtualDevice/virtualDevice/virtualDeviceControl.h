#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace _virtualDevice {

	/// <summary>
	/// Summary for virtualDeviceControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class virtualDeviceControl : public System::Windows::Forms::UserControl
	{
	public:
		virtualDeviceControl(/*void*/
			char*	_inpImage,const size_t	_inpImageSize,
			char*	_outImage,const size_t	_outImageSize)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->inpForm = gcnew virtualDeviceInp();
			this->outForm = gcnew virtualDeviceOut();

			//---------------------------------------------
			//	Loading Virtual Button
			//---------------------------------------------
			for(size_t byteIndex=0;byteIndex < _inpImageSize/*VIR_INP_IMG_SIZE*//*16*/;byteIndex++)
				for(int bitIndex =0;bitIndex<8;bitIndex++){
					this->inpForm->tlpButtons->Controls->Add(
						gcnew _virtualDevice::virtualButtonControl("X",&/*this->*//*virInpImage*/_inpImage[byteIndex],byteIndex + 32,bitIndex)
						,-1
						,-1);
				}

			//---------------------------------------------
			//	Loading Virtual Light
			//---------------------------------------------
			for(size_t byteIndex=0;byteIndex< _outImageSize/*VIR_OUT_IMG_SIZE*//*16*/;byteIndex++)
				for(int bitIndex =0;bitIndex<8;bitIndex++){
					this->outForm->tlpLights->Controls->Add(
						gcnew _virtualDevice::virtualLightControl("Y",&/*this->*//*virOutImage*/_outImage[byteIndex],byteIndex + 32,bitIndex)
						,-1
						,-1);
				}

				//----
				//	Going to Complish Save/Load Mechanism
				//----
				this->autoLoad();
		}
	public:
		System::Void outDeviceRefresh(String^ args)
		{
			//-----------------------------
			//	The Function Ready to Be Delegating
			//-----------------------------
			this->outForm->refreshLights();
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~virtualDeviceControl()
		{
			//----------------------
			//	Save Before Destruct
			//----------------------
			this->autoSave();
			if (components)
			{
				delete components;
			}
		}
	public:
			 //char*	inpImage;		// 128-bits image
			 //char*	outImage;		// 128-bits image
			 virtualDeviceInp^		inpForm;
			 virtualDeviceOut^		outForm;
	private: System::Windows::Forms::Button^  btKo;
	private: System::Windows::Forms::Button^  btKI;
	private: System::Windows::Forms::Button^  btInp;
	private: System::Windows::Forms::Button^  btOut;
	private: System::Windows::Forms::Label^  label1;
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
			this->btKo = (gcnew System::Windows::Forms::Button());
			this->btKI = (gcnew System::Windows::Forms::Button());
			this->btInp = (gcnew System::Windows::Forms::Button());
			this->btOut = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// btKo
			// 
			this->btKo->Enabled = false;
			this->btKo->Location = System::Drawing::Point(111, 19);
			this->btKo->Name = L"btKo";
			this->btKo->Size = System::Drawing::Size(30, 30);
			this->btKo->TabIndex = 0;
			this->btKo->Text = L"KO";
			this->btKo->UseVisualStyleBackColor = true;
			// 
			// btKI
			// 
			this->btKI->Enabled = false;
			this->btKI->Location = System::Drawing::Point(75, 19);
			this->btKI->Name = L"btKI";
			this->btKI->Size = System::Drawing::Size(30, 30);
			this->btKI->TabIndex = 1;
			this->btKI->Text = L"KI";
			this->btKI->UseVisualStyleBackColor = true;
			// 
			// btInp
			// 
			this->btInp->Location = System::Drawing::Point(3, 19);
			this->btInp->Name = L"btInp";
			this->btInp->Size = System::Drawing::Size(30, 30);
			this->btInp->TabIndex = 2;
			this->btInp->Text = L"X";
			this->btInp->UseVisualStyleBackColor = true;
			this->btInp->Click += gcnew System::EventHandler(this, &virtualDeviceControl::btClick);
			// 
			// btOut
			// 
			this->btOut->Location = System::Drawing::Point(39, 19);
			this->btOut->Name = L"btOut";
			this->btOut->Size = System::Drawing::Size(30, 30);
			this->btOut->TabIndex = 3;
			this->btOut->Text = L"Y";
			this->btOut->UseVisualStyleBackColor = true;
			this->btOut->Click += gcnew System::EventHandler(this, &virtualDeviceControl::btClick);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"PMingLiU", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->label1->Location = System::Drawing::Point(3, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(120, 16);
			this->label1->TabIndex = 4;
			this->label1->Text = L"Virtual Devices";
			// 
			// virtualDeviceControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->Controls->Add(this->label1);
			this->Controls->Add(this->btOut);
			this->Controls->Add(this->btInp);
			this->Controls->Add(this->btKI);
			this->Controls->Add(this->btKo);
			this->Name = L"virtualDeviceControl";
			this->Size = System::Drawing::Size(144, 52);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private:	
	int	autoSave();		// auto save/load the symbols , name...etc
	int autoLoad();		// Hsien , 2013.01.31

#define	VIR_DEV_TABINDEX_X	2
#define	VIR_DEV_TABINDEX_Y	3
		System::Void	btClick(System::Object^ sender,System::EventArgs^ e){
			Windows::Forms::Control^ hdl = 
				safe_cast<Windows::Forms::Control^>(sender);

			switch(hdl->TabIndex){
				case	VIR_DEV_TABINDEX_X:	this->inpForm->Show();break;
				case	VIR_DEV_TABINDEX_Y:	this->outForm->Show();break;
			}
		}
	};
}
