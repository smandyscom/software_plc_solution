#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;			// for using marshal


namespace outDev {

	/// <summary>
	/// Summary for coilControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class coilControl : public System::Windows::Forms::UserControl
	{
	public:
		coilControl(outputDevice*	instance)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			this->coil = (odCoil*)instance;
		}
	public:
		odCoil*	coil;					// when to change kind? at the moment before compiling?
										// and how to change?
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~coilControl()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  tb_coilName;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(coilControl::typeid));
			this->tb_coilName = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// tb_coilName
			// 
			this->tb_coilName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->tb_coilName->BackColor = System::Drawing::SystemColors::Control;
			this->tb_coilName->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->tb_coilName->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->tb_coilName->Location = System::Drawing::Point(0, 36);
			this->tb_coilName->Margin = System::Windows::Forms::Padding(3, 0, 3, 0);
			this->tb_coilName->Name = L"tb_coilName";
			this->tb_coilName->Size = System::Drawing::Size(120, 14);
			this->tb_coilName->TabIndex = 0;
			this->tb_coilName->TextChanged += gcnew System::EventHandler(this, &coilControl::tcInput);
			// 
			// coilControl
			// 
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->BackColor = System::Drawing::SystemColors::Control;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->Controls->Add(this->tb_coilName);
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"coilControl";
			this->Size = System::Drawing::Size(120, 50);
			this->Load += gcnew System::EventHandler(this, &coilControl::coilControlLoad);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
 	private: System::Void tcInput(System::Object^  /*sender*/, System::EventArgs^  /*e*/) {
				 this->coil->setBitName((char*)(void*)Marshal::StringToHGlobalAnsi(this->tb_coilName->Text));
			 }
	//----------------------------------------------------------------------------------------------------------------------------------
	// data packing/extracting functions
	//	 policy:  1. do not store any real data instance in such base controls.
	//	 packing: 1. evaluating data size in parent layer , then allocating buffer
	//			  2. pass allocated memory's address , do data write-in
	//			  3. reason: 
    //					a. in the memory managed environment , it's unable to allocationg unmanged memory in the scope of local function
	//						momery would be returned when existing function.
    //					b. so that , the alternaitve way is , asking data size before receive meta data.
    //						then allocating memory in control layer , it won't be forced returning. writing data in the end.
	//----------------------------------------------------------------------------------------------------------------------------------
	private: System::Void coilControlLoad(System::Object^  /*sender*/, System::EventArgs^  /*e*/) {
				 this->tb_coilName->Text = gcnew String(this->coil->getBitName().c_str());
			 }
	};
}
