#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;		// for marshal string , Hsien , 2013.01.21

namespace outDev {

	/// <summary>
	/// Summary for stBlockControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class stBlockControl : public System::Windows::Forms::UserControl
	{
	public:
		stBlockControl(outputDevice*	instance)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->_stBlock		= (odStBlock*)instance;
			this->editorForm	= gcnew stBlockEditor();		// the editor form
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~stBlockControl()
		{
			if (components)
			{
				delete components;
			}
		}
	protected:
		stBlockEditor^		editorForm;
		odStBlock*			_stBlock;
	private: System::Windows::Forms::Button^  btOpen;
	protected: 
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(stBlockControl::typeid));
			this->btOpen = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// btOpen
			// 
			this->btOpen->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btOpen.BackgroundImage")));
			this->btOpen->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->btOpen->Location = System::Drawing::Point(97, 0);
			this->btOpen->Name = L"btOpen";
			this->btOpen->Size = System::Drawing::Size(20, 20);
			this->btOpen->TabIndex = 4;
			this->btOpen->UseVisualStyleBackColor = true;
			this->btOpen->Click += gcnew System::EventHandler(this, &stBlockControl::openStBlockEditor);
			// 
			// stBlockControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->Controls->Add(this->btOpen);
			this->Name = L"stBlockControl";
			this->Size = System::Drawing::Size(120, 50);
			this->Load += gcnew System::EventHandler(this, &stBlockControl::stBlockControlLoad);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void stBlockControlLoad(System::Object^  /*sender*/, System::EventArgs^  /*e*/){
				 //-----------
				 //	Load String from odStBlock(Native Object) to TextBox
				 //-----------	
				 this->editorForm->tbStBlock->Text = gcnew String(this->_stBlock->getGenStr().c_str());
			 }
	private : System::Void openStBlockEditor(System::Object^  /*sender*/, System::EventArgs^  /*e*/){
				  if(this->editorForm->ShowDialog()
					  == Windows::Forms::DialogResult::OK){
						  //------------------
						  //	Input the content in Textbox to odStBlock
						  //------------------
						  this->_stBlock->setStStatement(
							  (char*)(void*)Marshal::StringToHGlobalAnsi(editorForm->tbStBlock->Text)
							  );
				  }//if
			  }
	};
}
