#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace _ldEditor {

	/// <summary>
	/// Summary for ldEditorOutput
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ldEditorOutput : public System::Windows::Forms::Form
	{
	public:
		ldEditorOutput(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ldEditorOutput()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^  tcWindow;
	public : System::Windows::Forms::TabPage^  tabPageOutput;
	public: System::Windows::Forms::TabPage^  tabPageWatch;
	public: System::Windows::Forms::TextBox^  textBoxOutput;
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
			this->tcWindow = (gcnew System::Windows::Forms::TabControl());
			this->tabPageOutput = (gcnew System::Windows::Forms::TabPage());
			this->textBoxOutput = (gcnew System::Windows::Forms::TextBox());
			this->tabPageWatch = (gcnew System::Windows::Forms::TabPage());
			this->tcWindow->SuspendLayout();
			this->tabPageOutput->SuspendLayout();
			this->SuspendLayout();
			// 
			// tcWindow
			// 
			this->tcWindow->Alignment = System::Windows::Forms::TabAlignment::Left;
			this->tcWindow->Controls->Add(this->tabPageOutput);
			this->tcWindow->Controls->Add(this->tabPageWatch);
			this->tcWindow->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tcWindow->Location = System::Drawing::Point(0, 0);
			this->tcWindow->Multiline = true;
			this->tcWindow->Name = L"tcWindow";
			this->tcWindow->SelectedIndex = 0;
			this->tcWindow->Size = System::Drawing::Size(542, 112);
			this->tcWindow->TabIndex = 0;
			// 
			// tabPageOutput
			// 
			this->tabPageOutput->Controls->Add(this->textBoxOutput);
			this->tabPageOutput->Location = System::Drawing::Point(22, 4);
			this->tabPageOutput->Name = L"tabPageOutput";
			this->tabPageOutput->Padding = System::Windows::Forms::Padding(3);
			this->tabPageOutput->Size = System::Drawing::Size(516, 104);
			this->tabPageOutput->TabIndex = 0;
			this->tabPageOutput->Text = L"Output";
			this->tabPageOutput->UseVisualStyleBackColor = true;
			// 
			// textBoxOutput
			// 
			this->textBoxOutput->Dock = System::Windows::Forms::DockStyle::Fill;
			this->textBoxOutput->Location = System::Drawing::Point(3, 3);
			this->textBoxOutput->Multiline = true;
			this->textBoxOutput->Name = L"textBoxOutput";
			this->textBoxOutput->Size = System::Drawing::Size(510, 98);
			this->textBoxOutput->TabIndex = 0;
			// 
			// tabPageWatch
			// 
			this->tabPageWatch->Location = System::Drawing::Point(22, 4);
			this->tabPageWatch->Name = L"tabPageWatch";
			this->tabPageWatch->Padding = System::Windows::Forms::Padding(3);
			this->tabPageWatch->Size = System::Drawing::Size(516, 104);
			this->tabPageWatch->TabIndex = 1;
			this->tabPageWatch->Text = L"Watch";
			this->tabPageWatch->UseVisualStyleBackColor = true;
			// 
			// ldEditorOutput
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(542, 112);
			this->Controls->Add(this->tcWindow);
			this->Name = L"ldEditorOutput";
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Text = L"ldEditorOutput";
			this->tcWindow->ResumeLayout(false);
			this->tabPageOutput->ResumeLayout(false);
			this->tabPageOutput->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
};
}
