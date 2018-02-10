#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace inpDev {

	/// <summary>
	/// Summary for indexerOutControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class indexerOutControl : public System::Windows::Forms::UserControl
	{
	public:
		indexerOutControl(vertice*	instance)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->_iNo = (indexerNo*)instance;

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~indexerOutControl()
		{
			if (components)
			{
				delete components;
			}
		}
	protected:
		indexerNo*	_iNo;
	private: System::Windows::Forms::ComboBox^  cbSelect;
	private: System::Windows::Forms::NumericUpDown^  nudIndex;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(indexerOutControl::typeid));
			this->cbSelect = (gcnew System::Windows::Forms::ComboBox());
			this->nudIndex = (gcnew System::Windows::Forms::NumericUpDown());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nudIndex))->BeginInit();
			this->SuspendLayout();
			// 
			// cbSelect
			// 
			this->cbSelect->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->cbSelect->FormattingEnabled = true;
			this->cbSelect->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"TMR", L"CTR", L"MCODE"});
			this->cbSelect->Location = System::Drawing::Point(3, -2);
			this->cbSelect->Name = L"cbSelect";
			this->cbSelect->Size = System::Drawing::Size(44, 19);
			this->cbSelect->TabIndex = 0;
			// 
			// nudIndex
			// 
			this->nudIndex->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->nudIndex->Location = System::Drawing::Point(6, 29);
			this->nudIndex->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {127, 0, 0, 0});
			this->nudIndex->Name = L"nudIndex";
			this->nudIndex->Size = System::Drawing::Size(40, 21);
			this->nudIndex->TabIndex = 1;
			// 
			// indexerOutControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->Controls->Add(this->nudIndex);
			this->Controls->Add(this->cbSelect);
			this->Name = L"indexerOutControl";
			this->Size = System::Drawing::Size(50, 50);
			this->Load += gcnew System::EventHandler(this, &indexerOutControl::indexexOutLoad);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nudIndex))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void indexexOutLoad(System::Object^ sender, System::EventArgs^){
				 //------------------------
				 //	Rebuild The Information
				 //------------------------
				 switch(this->_iNo->getType()){
					 case NONSTOP_TIMER_OUTPUT_NO:
						 this->cbSelect->SelectedIndex	= ITEM_TIMER;
						 break;
					 case RING_COUNTER_OUTPUT_NO:
						 this->cbSelect->SelectedIndex = ITEM_COUNTER;
						 // Hsien 2013/5/8
						 //		After this , would doing selectEventHandler
						 break;
						 //----------------------
						//	NC Sync-IO request receive , shared the implementation with indexer // Hsien 2013/5/22 
						//----------------------
					case NC_REQUEST:
						this->cbSelect->SelectedIndex = ITEM_SYNCREQ;
						break;
					 default:
						 break;
				 }
				 this->nudIndex->Value = this->_iNo->getIndex();

				 this->cbSelect->SelectedIndexChanged	+= gcnew System::EventHandler(this,&indexerOutControl::select);
				 this->nudIndex->ValueChanged			+= gcnew System::EventHandler(this,&indexerOutControl::select);
				 // Hsien 2013/5/8 , delayed loading eventHandler , for avoiding lead to select() on initializing

			 }
	private: System::Void select(System::Object^  sender, System::EventArgs^  e) {
				 //-------------------------------------
				 //	Refresh the infos : Type and Index
				 //-------------------------------------
				 switch(this->cbSelect->SelectedIndex){
					 case ITEM_TIMER:
						 this->_iNo->setType(NONSTOP_TIMER_OUTPUT_NO);
						 break;
					 case ITEM_COUNTER:
						 this->_iNo->setType(RING_COUNTER_OUTPUT_NO);
						 break;
						 //----------------------
						 //	NC Sync-IO request receive , shared the implementation with indexer // Hsien 2013/5/22 
						 //----------------------
					 case ITEM_SYNCREQ: 
						 this->_iNo->setType(NC_REQUEST);
						 break;
					 default:
						 break;
				 }
				 //---------------
				 //	Set Index
				 //---------------
				 this->_iNo->setIndex(System::Decimal::ToInt32(this->nudIndex->Value));
			 }
};
}
