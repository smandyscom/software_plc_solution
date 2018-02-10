#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;			// for stream-writer/reader
									// Hsien , 2013.01.21

namespace outDev {

	/// <summary>
	/// Summary for stBlockEditor
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class stBlockEditor : public System::Windows::Forms::Form
	{
	public:
		stBlockEditor(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->saveFileDialog->Filter = "Structured Text File (*.st) |*.st";
			this->openFileDialog->Filter = "Structured Text File (*.st) |*.st";
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~stBlockEditor()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::TextBox^			tbStBlock;
	private: System::Windows::Forms::ToolStrip^			toolStrip;
	private: System::Windows::Forms::ToolStripButton^	tsbSave;
	private: System::Windows::Forms::ToolStripButton^	tsbLoad;
	private: System::Windows::Forms::Button^			btOk;
	private: System::Windows::Forms::Button^			btCancel;
	private: System::Windows::Forms::SaveFileDialog^	saveFileDialog;
	private: System::Windows::Forms::OpenFileDialog^	openFileDialog;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(stBlockEditor::typeid));
			this->tbStBlock = (gcnew System::Windows::Forms::TextBox());
			this->toolStrip = (gcnew System::Windows::Forms::ToolStrip());
			this->tsbSave = (gcnew System::Windows::Forms::ToolStripButton());
			this->tsbLoad = (gcnew System::Windows::Forms::ToolStripButton());
			this->btOk = (gcnew System::Windows::Forms::Button());
			this->btCancel = (gcnew System::Windows::Forms::Button());
			this->saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->toolStrip->SuspendLayout();
			this->SuspendLayout();
			// 
			// tbStBlock
			// 
			this->tbStBlock->Location = System::Drawing::Point(12, 28);
			this->tbStBlock->Multiline = true;
			this->tbStBlock->Name = L"tbStBlock";
			this->tbStBlock->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->tbStBlock->Size = System::Drawing::Size(280, 133);
			this->tbStBlock->TabIndex = 0;
			// 
			// toolStrip
			// 
			this->toolStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->tsbSave, this->tsbLoad});
			this->toolStrip->Location = System::Drawing::Point(0, 0);
			this->toolStrip->Name = L"toolStrip";
			this->toolStrip->Size = System::Drawing::Size(304, 25);
			this->toolStrip->TabIndex = 1;
			this->toolStrip->Text = L"toolStrip";
			// 
			// tsbSave
			// 
			this->tsbSave->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsbSave->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbSave.Image")));
			this->tsbSave->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbSave->Name = L"tsbSave";
			this->tsbSave->Size = System::Drawing::Size(23, 22);
			this->tsbSave->Tag = L"1";
			this->tsbSave->Text = L"toolStripButton1";
			this->tsbSave->Click += gcnew System::EventHandler(this, &stBlockEditor::btClick);
			// 
			// tsbLoad
			// 
			this->tsbLoad->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->tsbLoad->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"tsbLoad.Image")));
			this->tsbLoad->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->tsbLoad->Name = L"tsbLoad";
			this->tsbLoad->Size = System::Drawing::Size(23, 22);
			this->tsbLoad->Tag = L"2";
			this->tsbLoad->Text = L"toolStripButton2";
			this->tsbLoad->Click += gcnew System::EventHandler(this, &stBlockEditor::btClick);
			// 
			// btOk
			// 
			this->btOk->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->btOk->Location = System::Drawing::Point(136, 167);
			this->btOk->Name = L"btOk";
			this->btOk->Size = System::Drawing::Size(75, 23);
			this->btOk->TabIndex = 2;
			this->btOk->Tag = L"3";
			this->btOk->Text = L"OK";
			this->btOk->UseVisualStyleBackColor = true;
			// 
			// btCancel
			// 
			this->btCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btCancel->Location = System::Drawing::Point(217, 167);
			this->btCancel->Name = L"btCancel";
			this->btCancel->Size = System::Drawing::Size(75, 23);
			this->btCancel->TabIndex = 3;
			this->btCancel->Tag = L"4";
			this->btCancel->Text = L"Cancel";
			this->btCancel->UseVisualStyleBackColor = true;
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog1";
			// 
			// stBlockEditor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(304, 202);
			this->ControlBox = false;
			this->Controls->Add(this->btCancel);
			this->Controls->Add(this->btOk);
			this->Controls->Add(this->toolStrip);
			this->Controls->Add(this->tbStBlock);
			this->Name = L"stBlockEditor";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"ST Editor";
			this->toolStrip->ResumeLayout(false);
			this->toolStrip->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		private:	System::Void btClick(System::Object^  sender, System::EventArgs^  e) {
						if(sender == this->tsbSave){
							//----------------------
							//	Save the ST-Block
							//----------------------
							if(this->saveFileDialog->ShowDialog()
								== Windows::Forms::DialogResult::OK){
									StreamWriter^ sw
										= gcnew StreamWriter(this->saveFileDialog->FileName);
									sw->Write(this->tbStBlock->Text);
									sw->Close();
							}//if show dialog
						}
						else if(sender == this->tsbLoad){
							//----------------------
							//	Load the ST block
							//----------------------
							if(this->openFileDialog->ShowDialog()
								== Windows::Forms::DialogResult::OK){
									StreamReader^ sr
										= gcnew StreamReader(this->openFileDialog->FileName);
									this->tbStBlock->Text = sr->ReadToEnd();
									sr->Close();
							}//if show dialog
						}
						//else if(sender == this->btOk){
						//	//-----------------------
						//	//	Button OK , Accept the input from tbStBlock
						//	//-----------------------
						//	this->DialogResult = 
						//		Windows::Forms::DialogResult::OK;
						//}// 
						//else if(sender == this->btCancel){
						//	//-----------------------
						//	//	Button Cancel 
						//	//-----------------------
						//	this->DialogResult = 
						//		Windows::Forms::DialogResult::Cancel;
						//}
					}
	};
}
