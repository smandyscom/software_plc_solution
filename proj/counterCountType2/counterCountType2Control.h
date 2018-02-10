#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace outDev {

	/// <summary>
	/// Summary for counterCountType2Control
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class counterCountType2Control : public System::Windows::Forms::UserControl
	{
	public:
		counterCountType2Control(outputDevice*	instance)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->_rcCountType2 = (odRingCounterCountType2*)instance;
			this->setupForm		= gcnew counterCountType2Setup();
		}
	protected:
		odRingCounterCountType2*	_rcCountType2;
	private: System::Windows::Forms::CheckBox^  cbDown;
	protected: 
		counterCountType2Setup^		setupForm;
		int	showBrief(void){
			this->tbBrief->Text =  
				String::Format("({0:D2}) Goal-{1:G} Init-{2:G} Step-{3:G}"
				,this->_rcCountType2->getCounterIndex()
				,gcnew String(this->_rcCountType2->getCounterGoal().c_str())
				,gcnew String(this->_rcCountType2->getCounterInit().c_str())
				,gcnew String(this->_rcCountType2->getCounterStep().c_str()));

			return 1;
		}
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~counterCountType2Control()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  tbBrief;
	private: System::Windows::Forms::Button^  button1;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(counterCountType2Control::typeid));
			this->tbBrief = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->cbDown = (gcnew System::Windows::Forms::CheckBox());
			this->SuspendLayout();
			// 
			// tbBrief
			// 
			this->tbBrief->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->tbBrief->BackColor = System::Drawing::SystemColors::Control;
			this->tbBrief->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->tbBrief->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->tbBrief->Location = System::Drawing::Point(10, 34);
			this->tbBrief->Margin = System::Windows::Forms::Padding(3, 0, 3, 0);
			this->tbBrief->Name = L"tbBrief";
			this->tbBrief->ReadOnly = true;
			this->tbBrief->Size = System::Drawing::Size(100, 14);
			this->tbBrief->TabIndex = 5;
			// 
			// button1
			// 
			this->button1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"button1.BackgroundImage")));
			this->button1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->button1->Location = System::Drawing::Point(97, 0);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(20, 20);
			this->button1->TabIndex = 4;
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &counterCountType2Control::setupDClick);
			// 
			// cbDown
			// 
			this->cbDown->AutoSize = true;
			this->cbDown->Location = System::Drawing::Point(39, 0);
			this->cbDown->Name = L"cbDown";
			this->cbDown->Size = System::Drawing::Size(52, 16);
			this->cbDown->TabIndex = 6;
			this->cbDown->Text = L"Down";
			this->cbDown->UseVisualStyleBackColor = true;
			this->cbDown->CheckedChanged += gcnew System::EventHandler(this, &counterCountType2Control::downCount);
			// 
			// counterCountType2Control
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->Controls->Add(this->cbDown);
			this->Controls->Add(this->tbBrief);
			this->Controls->Add(this->button1);
			this->Name = L"counterCountType2Control";
			this->Size = System::Drawing::Size(120, 50);
			this->Load += gcnew System::EventHandler(this, &counterCountType2Control::counterSetControlLoad);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		private: System::Void setupDClick(System::Object^ sender,System::EventArgs^ e){
				 if(this->setupForm->ShowDialog()
					 == Windows::Forms::DialogResult::OK){
						 //--------------
						 //	Dialog Confirmed , Input the Values from Setup GUI
						 //--------------
						 std::string buffer;

						 this->_rcCountType2->setCounterIndex	(setupForm->counterIndex);

						 MarshalString(setupForm->counterData[TABINDEX_GOAL],buffer);
						 this->_rcCountType2->setCounterGoal(buffer);

						 MarshalString(setupForm->counterData[TABINDEX_INITIAL],buffer);
						 this->_rcCountType2->setCounterInit(buffer);

						 MarshalString(setupForm->counterData[TABINDEX_STEP],buffer);
						 this->_rcCountType2->setCounterStep(buffer);

						 this->showBrief();
				 }
			 }
private: System::Void counterSetControlLoad(System::Object^ sender,System::EventArgs^ e){
				 //---------------
				 //	Rebuild The Content 
				 //---------------
				 setupForm->counterIndex = this->_rcCountType2->getCounterIndex();	// Hsien 2013/5/21
				 // Hsien 2013/5/21
				 setupForm->counterData[TABINDEX_GOAL]		= gcnew String(this->_rcCountType2->getCounterGoal().c_str());
				 setupForm->counterData[TABINDEX_INITIAL]	= gcnew String(this->_rcCountType2->getCounterInit().c_str());
				 setupForm->counterData[TABINDEX_STEP]		= gcnew String(this->_rcCountType2->getCounterStep().c_str());
				 //----------------------
				 //
				 //----------------------
				 switch(this->_rcCountType2->getType()){
					 case ODRINGCTR_TYPE2_COUNTUP:										break;
					 case ODRINGCTR_TYPE2_COUNTDOWN:	this->cbDown->Checked = true;	break;
					 default:
						 break;
				 }
				 //---------------
				 //	Show the Values on GUI
				 //---------------
				 setupForm->refresh();
				 this->showBrief();	// Hsien 2013/5/21
			 }
private: System::Void downCount(System::Object^  sender, System::EventArgs^  e) {
				 //-----------------------
				 //	If THE check box had been checked , change the counter type as down-counting
				 //		Hsien , 2013.01.22
				 //-----------------------
				 if(this->cbDown->Checked)
					 this->_rcCountType2->setCounterType(ODRINGCTR_TYPE2_COUNTDOWN);
				 else
					 this->_rcCountType2->setCounterType(ODRINGCTR_TYPE2_COUNTUP);
								// Hsien 2013/5/8 , otherwise , the od type won't change when un-checked
			 }


	};
}
