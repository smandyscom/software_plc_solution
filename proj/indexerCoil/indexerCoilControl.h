#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace outDev {

	/// <summary>
	/// Summary for indexerCoilControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class indexerCoilControl : public System::Windows::Forms::UserControl
	{
	public:
		indexerCoilControl(outputDevice*	instance)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->_iCoil = (odIndexerCoil*)instance;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~indexerCoilControl()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		odIndexerCoil*	_iCoil;
	private: System::Windows::Forms::Label^  labelIndex;
	private: System::Windows::Forms::NumericUpDown^  numericUpDownCounter;


			 System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(indexerCoilControl::typeid));
			this->labelIndex = (gcnew System::Windows::Forms::Label());
			this->numericUpDownCounter = (gcnew System::Windows::Forms::NumericUpDown());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownCounter))->BeginInit();
			this->SuspendLayout();
			// 
			// labelIndex
			// 
			this->labelIndex->AutoSize = true;
			this->labelIndex->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->labelIndex->Location = System::Drawing::Point(28, 30);
			this->labelIndex->Margin = System::Windows::Forms::Padding(3, 0, 0, 0);
			this->labelIndex->Name = L"labelIndex";
			this->labelIndex->Size = System::Drawing::Size(41, 11);
			this->labelIndex->TabIndex = 6;
			this->labelIndex->Text = L"MCODE";
			// 
			// numericUpDownCounter
			// 
			this->numericUpDownCounter->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->numericUpDownCounter->Location = System::Drawing::Point(72, 28);
			this->numericUpDownCounter->Margin = System::Windows::Forms::Padding(3, 0, 3, 0);
			this->numericUpDownCounter->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {127, 0, 0, 0});
			this->numericUpDownCounter->Name = L"numericUpDownCounter";
			this->numericUpDownCounter->Size = System::Drawing::Size(45, 21);
			this->numericUpDownCounter->TabIndex = 5;
			this->numericUpDownCounter->ValueChanged += gcnew System::EventHandler(this, &indexerCoilControl::ncInput);
			// 
			// indexerCoilControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->Controls->Add(this->labelIndex);
			this->Controls->Add(this->numericUpDownCounter);
			this->Name = L"indexerCoilControl";
			this->Size = System::Drawing::Size(120, 50);
			this->Load += gcnew System::EventHandler(this, &indexerCoilControl::indexerCoilControlLoad);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownCounter))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void indexerCoilControlLoad(System::Object^  sender, System::EventArgs^  e) {
				 //----------------------
				 //	Select background image according to 
				 //----------------------
				 System::ComponentModel::ComponentResourceManager^  resources = 
					 (gcnew System::ComponentModel::ComponentResourceManager(indexerCoilControl::typeid));
				 switch(_iCoil->getType()){
					 //----------------------
					 //	NC Sync-IO confirm send , implemented on indexer-coil // Hsien 2013/5/22 
					 //----------------------
					 case ODNC_CONFIRM:
						 this->BackgroundImage	= safe_cast<Drawing::Image^>(resources->GetObject("outIndexerSetNcCfm"));
						 this->labelIndex->Text = L"MCODE";
						 break;
					 default:
						 break;
				 }//switch

				 this->numericUpDownCounter->Value = _iCoil->getIndex();	// Hsien 2013/5/23
			 }
	private: System::Void ncInput(System::Object^  sender, System::EventArgs^  e) {
				 // Hsien 2013/5/23
				 this->_iCoil->setIndex(System::Decimal::ToInt32(this->numericUpDownCounter->Value));
			 }
};
}
