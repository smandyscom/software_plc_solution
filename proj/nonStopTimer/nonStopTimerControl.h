#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace outDev {

	/// <summary>
	/// Summary for nonStopTimerControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class nonStopTimerControl : public System::Windows::Forms::UserControl
	{
	public:
		nonStopTimerControl(outputDevice*	instance)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->setupForm = gcnew nonStopTimerSetup();
			this->rsManager = gcnew System::Resources::ResourceManager(nonStopTimerControl::typeid);

			this->_odTimer = (odNonStopTimer*)instance;	// link to instance
		}
	public:
		odNonStopTimer* _odTimer;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::TextBox^  tbTimerName;
	public: 
	private:
		System::Resources::ResourceManager^	rsManager;	// used to manage image resources
		int	_briefUIRefresh(
			const int __timerIndex,
			/*const int*/String^ __timerInterval,
			const int __timerType){
				//--------------------
				// refresh the brief UI
				//--------------------
				this->tbTimerName->Text
					= String::Format("\({0:D2}\) Interval-{1:G}"
					,__timerIndex
					,__timerInterval);

				switch(__timerType){
					case ODSETTN:	
						this->BackgroundImage = safe_cast<Image^>(rsManager->GetObject("TN_SET"));
						break;
					case ODRSTTN:
						this->BackgroundImage = safe_cast<Image^>(rsManager->GetObject("TN_RST"));
						break;
					default:
						break;
				}
				return 1;
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~nonStopTimerControl()
		{
			if (components)
			{
				delete components;
			}
		}
	private: nonStopTimerSetup^					setupForm;		// used to do attribute set-up

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(nonStopTimerControl::typeid));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->tbTimerName = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"button1.BackgroundImage")));
			this->button1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->button1->Location = System::Drawing::Point(97, 3);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(20, 20);
			this->button1->TabIndex = 4;
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &nonStopTimerControl::setupDClick);
			// 
			// tbTimerName
			// 
			this->tbTimerName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->tbTimerName->BackColor = System::Drawing::SystemColors::Control;
			this->tbTimerName->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->tbTimerName->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->tbTimerName->Location = System::Drawing::Point(10, 34);
			this->tbTimerName->Margin = System::Windows::Forms::Padding(3, 0, 3, 0);
			this->tbTimerName->Name = L"tbTimerName";
			this->tbTimerName->ReadOnly = true;
			this->tbTimerName->Size = System::Drawing::Size(100, 14);
			this->tbTimerName->TabIndex = 5;
			// 
			// nonStopTimerControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->Controls->Add(this->tbTimerName);
			this->Controls->Add(this->button1);
			this->Name = L"nonStopTimerControl";
			this->Size = System::Drawing::Size(120, 50);
			this->Load += gcnew System::EventHandler(this, &nonStopTimerControl::nonStopTimerControlLoad);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void setupDClick(System::Object^  sender, System::EventArgs^  e) {
				 if(this->setupForm->ShowDialog()
					 == Windows::Forms::DialogResult::OK){
						 std::string buffer;

						 //--------------------------------
						 //	Property Assignment
						 //--------------------------------
						 this->_odTimer->setTimerIndex		(setupForm->getTimerIndex());
						 MarshalString (setupForm->getTimerInterval(), buffer ) ;
						 this->_odTimer->setTimerInterval	(buffer/*setupForm->getTimerInterval()*/);
						 this->_odTimer->setTimerType		(setupForm->getTimerType());
						 //--------------------------------
						 //	Change Parent UI
						 //--------------------------------
						 this->_briefUIRefresh(setupForm->getTimerIndex()
							 ,setupForm->getTimerInterval()
							 ,setupForm->getTimerType());
				 }//if
			 }//function
	private: System::Void nonStopTimerControlLoad(System::Object^  sender, System::EventArgs^  e) {
				 //------------
				 //	1. Update Brief UI
				 //------------
				 this->_briefUIRefresh(
					 this->_odTimer->getTimerIndex(),
					 gcnew String(this->_odTimer->getTimerInterval().c_str()),	// Hsien 2013/5/21
					 this->_odTimer->getTimerType());
				 //------------
				 //	2. Update Setup UI
				 //-----------
				 this->setupForm->_setupUIRefresh(
					 this->_odTimer->getTimerIndex(),
					 gcnew String(this->_odTimer->getTimerInterval().c_str()),	// Hsien 2013/5/21
					 this->_odTimer->getTimerType());
			 }
	};
}
