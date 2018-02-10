#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace outDev {

	/// <summary>
	/// Summary for counterSetControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class counterSetControl : public System::Windows::Forms::UserControl
	{
	public:
		counterSetControl(outputDevice*	instance)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->_rcSet		= (odRingCounterSet*)instance;
			this->setupForm		= gcnew counterSetup();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~counterSetControl()
		{
			if (components)
			{
				delete components;
			}
		}
	protected:
		odRingCounterSet*	_rcSet;
		counterSetup^		setupForm;
		int	showBrief(){
			this->tbBrief->Text =  
				String::Format("\({0:D2}\) Goal-{1:G} Init-{2:G}"
				,this->_rcSet->getCounterIndex()
				,gcnew String(this->_rcSet->getCounterGoal().c_str())
				,gcnew String(this->_rcSet->getCounterInit().c_str()));

			return 1;
		}
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::TextBox^  tbBrief;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(counterSetControl::typeid));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->tbBrief = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"button1.BackgroundImage")));
			this->button1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->button1->Location = System::Drawing::Point(97, 0);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(20, 20);
			this->button1->TabIndex = 3;
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &counterSetControl::setupDClick);
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
			this->tbBrief->TabIndex = 3;
			// 
			// counterSetControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->Controls->Add(this->tbBrief);
			this->Controls->Add(this->button1);
			this->Name = L"counterSetControl";
			this->Size = System::Drawing::Size(120, 50);
			this->Load += gcnew System::EventHandler(this, &counterSetControl::counterSetControlLoad);
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

						 this->_rcSet->setCounterIndex	(setupForm->counterIndex/*setupForm->dataArray[TABINDEX_INDEX]*/);

						 MarshalString(setupForm->counterGoal,buffer);
						 this->_rcSet->setCounterGoal(buffer);
						 MarshalString(setupForm->counterInit,buffer);
						 this->_rcSet->setCounterInit(buffer);
						 // Hsien 2013/5/21

						 //this->_rcSet->setCounterGoal	(setupForm->dataArray[TABINDEX_GOAL]);
						 //this->_rcSet->setCounterInit	(setupForm->dataArray[TABINDEX_INITIAL]);
						 this->showBrief();	// Hsien 2013/5/21
				 }
			 }
	private: System::Void counterSetControlLoad(System::Object^ sender,System::EventArgs^ e){
				 //---------------
				 //	Rebuild The Content 
				 //---------------
				 //setupForm->dataArray[TABINDEX_INDEX]	= this->_rcSet->getCounterIndex();
				 setupForm->counterIndex = this->_rcSet->getCounterIndex();	// Hsien 2013/5/21
				 // Hsien 2013/5/21
				 setupForm->counterGoal = gcnew String(this->_rcSet->getCounterGoal().c_str());
				 setupForm->counterInit = gcnew String(this->_rcSet->getCounterInit().c_str());
				 //setupForm->dataArray[TABINDEX_GOAL]	= this->_rcSet->getCounterGoal();
				 //setupForm->dataArray[TABINDEX_INITIAL] = this->_rcSet->getCounterInit();
				 //---------------
				 //	Show the Values on GUI
				 //---------------
				 setupForm->refresh();
				 this->showBrief();	// Hsien 2013/5/21
			 }
	};
}
