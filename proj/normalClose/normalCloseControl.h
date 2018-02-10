#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace inpDev {

	/// <summary>
	/// Summary for normalCloseControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class normalCloseControl : public System::Windows::Forms::UserControl
	{
	public:
		normalCloseControl(/*normalClose**/vertice*	instance)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->nc = (normalClose*)instance;
		}
	protected:
		normalClose*	nc;
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~normalCloseControl()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  tb_deviceName;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(normalCloseControl::typeid));
			this->tb_deviceName = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// tb_deviceName
			// 
			this->tb_deviceName->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->tb_deviceName->BackColor = System::Drawing::SystemColors::Control;
			this->tb_deviceName->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->tb_deviceName->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->tb_deviceName->Location = System::Drawing::Point(0, 36);
			this->tb_deviceName->Margin = System::Windows::Forms::Padding(3, 0, 3, 0);
			this->tb_deviceName->Name = L"tb_deviceName";
			this->tb_deviceName->Size = System::Drawing::Size(50, 14);
			this->tb_deviceName->TabIndex = 0;
			this->tb_deviceName->TextChanged += gcnew System::EventHandler(this, &normalCloseControl::tcInput);
			// 
			// normalCloseControl
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->BackColor = System::Drawing::SystemColors::Control;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->Controls->Add(this->tb_deviceName);
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"normalCloseControl";
			this->Size = System::Drawing::Size(50, 50);
			this->Load += gcnew System::EventHandler(this, &normalCloseControl::normalCloseControlLoad);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void tcInput(System::Object^  sender, System::EventArgs^  e) {
				 std::string s;
				 MarshalString(this->tb_deviceName->Text,s);
				 this->nc->setBitName(s);	// always calling vertice's setData
		 }
	private: System::Void normalCloseControlLoad(System::Object^  sender, System::EventArgs^  e) {
				 this->tb_deviceName->Text = gcnew String(this->nc->getBitName().c_str());
			 }
	};
}
