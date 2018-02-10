#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace inpDev {

	/// <summary>
	/// Summary for comparatorControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class comparatorControl : public System::Windows::Forms::UserControl
	{
	public:
		comparatorControl(vertice*	instance)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			setupForm = gcnew comparatorSetup();
			this->com = (comparator*)instance;
		}

	protected:
		comparator*		com;
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~comparatorControl()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btSetup;
			 comparatorSetup^				  setupForm;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(comparatorControl::typeid));
			this->btSetup = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// btSetup
			// 
			this->btSetup->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btSetup.BackgroundImage")));
			this->btSetup->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->btSetup->Location = System::Drawing::Point(28, -1);
			this->btSetup->Name = L"btSetup";
			this->btSetup->Size = System::Drawing::Size(20, 20);
			this->btSetup->TabIndex = 0;
			this->btSetup->UseVisualStyleBackColor = true;
			this->btSetup->Click += gcnew System::EventHandler(this, &comparatorControl::setupClick);
			// 
			// comparatorControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->Controls->Add(this->btSetup);
			this->Name = L"comparatorControl";
			this->Size = System::Drawing::Size(50, 50);
			this->Load += gcnew System::EventHandler(this, &comparatorControl::comparatorControlLoad);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void setupClick(System::Object^  sender, System::EventArgs^  e) {
				 //----------------------
				 //	After Setup-Form Confirmed , input the value stored in Setup-Form
				 //----------------------
				 std::string	buffer;

				 if(this->setupForm->ShowDialog()
					 == Windows::Forms::DialogResult::OK){
						 MarshalString(this->setupForm->dataArray[TABINDEX_LHS],buffer);
						 this->com->setLhs(buffer);

						 MarshalString(this->setupForm->dataArray[TABINDEX_RHS],buffer);
						 this->com->setRhs(buffer);

						 MarshalString(this->setupForm->dataArray[TABINDEX_OP],buffer);
						 this->com->setOp(buffer);
				 }
			 }
	private: System::Void comparatorControlLoad(System::Object^  sender, System::EventArgs^  e) {
				 //----------------------
				 //	Grab Information From native component-comparator , and showup on setup-Form
				 //----------------------
				 // Hsien 2013/5/17

				 //----------------------
				 //	Rebuild Content
				 //----------------------
				 this->setupForm->dataArray[TABINDEX_LHS] = gcnew String(this->com->getLhs().c_str());
				 this->setupForm->dataArray[TABINDEX_RHS] = gcnew String(this->com->getRhs().c_str());
				 this->setupForm->dataArray[TABINDEX_OP] = gcnew String(this->com->getOp().c_str());
				 //----------------------
				 //	Post them on Setup-Form
				 //----------------------
				 this->setupForm->refresh();
			 }
	};
}
