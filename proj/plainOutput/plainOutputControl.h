#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

using namespace outDev;

namespace plain {

	/// <summary>
	/// Summary for plainOutputControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.

	public ref class plainOutputControl : public System::Windows::Forms::UserControl
	{
	public:
		plainOutputControl(int* gateway)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->od		= NULL/*new outputDevice()*/;
			this->outItem			= gateway;
			this->currentItemCode	= 0xffffffff;
			this->res		= gcnew System::ComponentModel::ComponentResourceManager(plainOutputControl::typeid);

			this->deviceEdit(NONE,NULL,OD_ON_EDIT);	// NONE means allocating a outputDevice , Hsien , 2012.09.28
		}
	public:
		outputDevice*										od;
	protected:
		int*											outItem;	// link to external gateway , to receive item request
		int										currentItemCode;
		System::ComponentModel::ComponentResourceManager^	res;
	public:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~plainOutputControl()
		{
			if (components)
			{
				delete components;
			}
			this->!plainOutputControl();		// finalizer
		}
		!plainOutputControl()
		{
			//this->od->~outputDevice();
			delete this->od;
			this->od = NULL;
		}

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
			this->SuspendLayout();
			// 
			// plainOutputControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->BackColor = System::Drawing::SystemColors::Window;
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"plainOutputControl";
			this->Size = System::Drawing::Size(120, 50);
			this->Click += gcnew System::EventHandler(this, &plainOutputControl::editClick);
			this->ControlAdded += gcnew System::Windows::Forms::ControlEventHandler(this, &plainOutputControl::attachFuncOnChilds);
			this->ControlRemoved += gcnew System::Windows::Forms::ControlEventHandler(this, &plainOutputControl::afterChildlRemoved);
			this->ResumeLayout(false);

		}
#pragma endregion
	private	: System::Void attachFuncOnChilds(System::Object^  /*sender*/, System::Windows::Forms::ControlEventArgs^  e) {
				 e->Control->Click += 
					 gcnew System::EventHandler(this,&plainOutputControl::editClick);
				 //-------------------------------------
				 //	apply on grand child control as well
 				 //-------------------------------------
				 for each(Windows::Forms::Control^ e in e->Control->Controls)
					 if(e->GetType() != Windows::Forms::TextBox::typeid
						 && e->GetType() != Windows::Forms::Button::typeid/* added by Hsien , 2013.01.31*/
						 && e->GetType() != Windows::Forms::CheckBox::typeid /* added by Hsien , 2013.04.02*/)			// except Text Box
																													// and button
						 e->Click += gcnew System::EventHandler(this,&plainOutputControl::editClick);
				 this->AutoSize = true;
			 }
	private	: System::Void afterChildlRemoved(System::Object^  /*sender*/, System::Windows::Forms::ControlEventArgs^  /*e*/) {
				 delete this->od;
				 this->od = NULL;
			 }
	private	: System::Void editClick(System::Object^  sender, System::EventArgs^  e) {
				 if(safe_cast<Windows::Forms::MouseEventArgs^>(e)->Button
					 == Windows::Forms::MouseButtons::Left){
						 this->deviceEdit(*this->outItem,NULL,OD_ON_EDIT);
				 }//if

				 this->Focus();
			 }
	public	: int deviceEdit(int outItemCode
				  ,const char*	importData
				  ,const char	_mode);		// given outItemCodes , this plain would genrate image either control
				//--------------------------------
				//	data packing/unpacking routine
				//--------------------------------
	public	: int	store	(DEV_METADATA		**omds);	// store internal data into meta-data form
	public	: int	rebuild	(const DEV_METADATA	*omds);		// rebuild this control by metadata
	};
}
