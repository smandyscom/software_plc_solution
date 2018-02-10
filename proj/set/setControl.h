#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;


namespace outDev {

	/// <summary>
	/// Summary for setControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class setControl : public System::Windows::Forms::UserControl
	{
	public:
		setControl(outputDevice*	instance)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->set = (odSet*)instance;
		}
	private: System::Windows::Forms::TextBox^  tb_rhs;

	private: System::Windows::Forms::Label^  label2;
	protected:		
		odSet*			set;
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~setControl()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  tb_lhs;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(setControl::typeid));
			this->tb_lhs = (gcnew System::Windows::Forms::TextBox());
			this->tb_rhs = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// tb_lhs
			// 
			this->tb_lhs->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->tb_lhs->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->tb_lhs->Location = System::Drawing::Point(3, 26);
			this->tb_lhs->Margin = System::Windows::Forms::Padding(3, 0, 0, 3);
			this->tb_lhs->Name = L"tb_lhs";
			this->tb_lhs->Size = System::Drawing::Size(50, 21);
			this->tb_lhs->TabIndex = 0;
			// 
			// tb_rhs
			// 
			this->tb_rhs->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->tb_rhs->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->tb_rhs->Location = System::Drawing::Point(67, 26);
			this->tb_rhs->Margin = System::Windows::Forms::Padding(0, 0, 3, 3);
			this->tb_rhs->Name = L"tb_rhs";
			this->tb_rhs->Size = System::Drawing::Size(50, 21);
			this->tb_rhs->TabIndex = 1;
			// 
			// label2
			// 
			this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"PMingLiU", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->label2->Location = System::Drawing::Point(53, 34);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(12, 12);
			this->label2->TabIndex = 3;
			this->label2->Text = L"=";
			// 
			// setControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Control;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->Controls->Add(this->label2);
			this->Controls->Add(this->tb_rhs);
			this->Controls->Add(this->tb_lhs);
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"setControl";
			this->Size = System::Drawing::Size(120, 50);
			this->Load += gcnew System::EventHandler(this, &setControl::setControl_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void tcInput(System::Object^  /*sender*/, System::EventArgs^  /*e*/) {
				 this->set->setLhs((char*)(void*)Marshal::StringToHGlobalAnsi(this->tb_lhs->Text));
				 this->set->setRhs((char*)(void*)Marshal::StringToHGlobalAnsi(this->tb_rhs->Text));
			 }
private: System::Void setControl_Load(System::Object^  /*sender*/, System::EventArgs^  /*e*/) {
			 this->tb_lhs->Text = gcnew String(this->set->getLhs().c_str());
			 this->tb_rhs->Text = gcnew String(this->set->getRhs().c_str());
			 //----------
			 //	late binding
			 //----------
			 this->tb_lhs->TextChanged += gcnew System::EventHandler(this,&setControl::tcInput);
			 this->tb_rhs->TextChanged += gcnew System::EventHandler(this,&setControl::tcInput);
		 }
};
}
