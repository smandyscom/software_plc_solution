#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace _standAlonePvm {

	/// <summary>
	/// Summary for sapSetup
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class sapSetup : public System::Windows::Forms::Form
	{
	public:
		sapSetup(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			//----------------
			//	Setup the tabindex as same as device code
			//----------------
			//this->rbEPCIO->TabIndex		= SAP_DEVCODE_EPCIO6000;
			//this->rbVirtual->TabIndex	= SAP_DEVCODE_VIRDEV;
			//this->rbNull->TabIndex		= SAP_DEVCODE_NULL;
			//
			//this->rbNull->Checked		= true;						// as default driver
			//this->deviceCode			= SAP_DEVCODE_NULL;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~sapSetup()
		{
			if (components)
			{
				delete components;
			}
		}
	public:
		int	deviceCode;			// to indicate which device is selected
	private: System::Windows::Forms::RadioButton^  rbEPCIO;
	private: System::Windows::Forms::RadioButton^  rbVirtual;
	private: System::Windows::Forms::Button^  btOK;
	private: System::Windows::Forms::Button^  btCancel;
	private: System::Windows::Forms::TableLayoutPanel^  tlpDevice;
	private: System::Windows::Forms::RadioButton^  rbNull;
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
			this->rbEPCIO = (gcnew System::Windows::Forms::RadioButton());
			this->rbVirtual = (gcnew System::Windows::Forms::RadioButton());
			this->btOK = (gcnew System::Windows::Forms::Button());
			this->btCancel = (gcnew System::Windows::Forms::Button());
			this->tlpDevice = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->rbNull = (gcnew System::Windows::Forms::RadioButton());
			this->tlpDevice->SuspendLayout();
			this->SuspendLayout();
			// 
			// rbEPCIO
			// 
			this->rbEPCIO->AutoSize = true;
			this->rbEPCIO->Location = System::Drawing::Point(3, 3);
			this->rbEPCIO->Name = L"rbEPCIO";
			this->rbEPCIO->Size = System::Drawing::Size(84, 14);
			this->rbEPCIO->TabIndex = 0;
			this->rbEPCIO->TabStop = true;
			this->rbEPCIO->Text = L"EPCIO-6000";
			this->rbEPCIO->UseVisualStyleBackColor = true;
			this->rbEPCIO->CheckedChanged += gcnew System::EventHandler(this, &sapSetup::selectDrive);
			// 
			// rbVirtual
			// 
			this->rbVirtual->AutoSize = true;
			this->rbVirtual->Location = System::Drawing::Point(3, 23);
			this->rbVirtual->Name = L"rbVirtual";
			this->rbVirtual->Size = System::Drawing::Size(90, 14);
			this->rbVirtual->TabIndex = 1;
			this->rbVirtual->TabStop = true;
			this->rbVirtual->Text = L"Virtual Device";
			this->rbVirtual->UseVisualStyleBackColor = true;
			this->rbVirtual->CheckedChanged += gcnew System::EventHandler(this, &sapSetup::selectDrive);
			// 
			// btOK
			// 
			this->btOK->Location = System::Drawing::Point(75, 78);
			this->btOK->Name = L"btOK";
			this->btOK->Size = System::Drawing::Size(75, 23);
			this->btOK->TabIndex = 0;
			this->btOK->Text = L"OK";
			this->btOK->UseVisualStyleBackColor = true;
			this->btOK->Click += gcnew System::EventHandler(this, &sapSetup::btClick);
			// 
			// btCancel
			// 
			this->btCancel->Location = System::Drawing::Point(156, 78);
			this->btCancel->Name = L"btCancel";
			this->btCancel->Size = System::Drawing::Size(75, 23);
			this->btCancel->TabIndex = 1;
			this->btCancel->Text = L"Cancel";
			this->btCancel->UseVisualStyleBackColor = true;
			this->btCancel->Click += gcnew System::EventHandler(this, &sapSetup::btClick);
			// 
			// tlpDevice
			// 
			this->tlpDevice->AutoSize = true;
			this->tlpDevice->ColumnCount = 1;
			this->tlpDevice->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->tlpDevice->Controls->Add(this->rbNull, 0, 2);
			this->tlpDevice->Controls->Add(this->rbEPCIO, 0, 0);
			this->tlpDevice->Controls->Add(this->rbVirtual, 0, 1);
			this->tlpDevice->Location = System::Drawing::Point(12, 12);
			this->tlpDevice->Name = L"tlpDevice";
			this->tlpDevice->RowCount = 3;
			this->tlpDevice->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpDevice->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpDevice->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tlpDevice->Size = System::Drawing::Size(96, 60);
			this->tlpDevice->TabIndex = 4;
			// 
			// rbNull
			// 
			this->rbNull->AutoSize = true;
			this->rbNull->Location = System::Drawing::Point(3, 43);
			this->rbNull->Name = L"rbNull";
			this->rbNull->Size = System::Drawing::Size(53, 14);
			this->rbNull->TabIndex = 2;
			this->rbNull->TabStop = true;
			this->rbNull->Text = L"NULL";
			this->rbNull->UseVisualStyleBackColor = true;
			// 
			// sapSetup
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(243, 113);
			this->Controls->Add(this->tlpDevice);
			this->Controls->Add(this->btCancel);
			this->Controls->Add(this->btOK);
			this->Name = L"sapSetup";
			this->Text = L"Select Output Drive";
			this->tlpDevice->ResumeLayout(false);
			this->tlpDevice->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void selectDrive(System::Object^  sender, System::EventArgs^  e) {
				 //-------------------
				 //	Scan All the Device  , set the device code as the one be checked
				 //--------------------
				 for each(System::Windows::Forms::RadioButton^ e in this->tlpDevice->Controls)
					 if(e->Checked)
						 deviceCode = e->TabIndex;
			 }
private: System::Void btClick(System::Object^  sender, System::EventArgs^  e) {
#define	SAPSETUP_TABINDEX_OK		0
#define SAPSETUP_TABINDEX_CANCEL	1
			 switch(safe_cast<Control^>(sender)->TabIndex){
				 case SAPSETUP_TABINDEX_OK:			this->DialogResult = Windows::Forms::DialogResult::OK;		break;
				 case SAPSETUP_TABINDEX_CANCEL:		this->DialogResult = Windows::Forms::DialogResult::Cancel;	break;
				 default:
					 break;
			 }
		 }
};
}
