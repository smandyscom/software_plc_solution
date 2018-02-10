#pragma once


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

using namespace inpDev;

namespace plain {

	/// <summary>
	/// Summary for plainControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.

	public enum class constPlainCodes
	{
		ST_HEAD,
		ST_TAIL,
		E_VERTICAL,
		E_TERMINAL,
		E_NOTHING,
		NULL_CODE,
	};

	public ref class plainInputControl : public System::Windows::Forms::UserControl
	{
	public:
		plainInputControl(int*	gateway,constPlainCodes constDevCode) 
		{
			InitializeComponent();
			this->res	= gcnew System::ComponentModel::ComponentResourceManager(plainInputControl::typeid);
			this->le	= new	ldElement();
			this->currentInpItemCode = 0xffffffff;
			this->currentVerticeCode = 0xffffffff;

			//--------------------------------------------------------------------
			//	if gateway is NULL , means constant plain initialization requested
			//--------------------------------------------------------------------
			if(!gateway)
				this->constantPlainEdit(constDevCode);	// hold a constant(uneditable) object
			else
				this->editablePlainInit(gateway);		// establish link to gateway

		}
	public:
		ldElement*							le;				// used to store meta information , and this is which child's ldElement point to
	protected:		
		System::ComponentModel::ComponentResourceManager^	res;
		short*								itemRequest	;
		short*								devRequest	;	// used to identity which componenet is selected
															// point to names variable on paraent
		short						currentInpItemCode;
		short						currentVerticeCode;
		array</*System::Drawing::Image^*/String^>^						images;
	public:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~plainInputControl()
		{
			if (components)
			{
				delete components;
			}
			this->!plainInputControl();
		}
		!plainInputControl()
		{
			//-----------
			//	finalizer , overrides Finalize Method
			//-----------
			delete this->le;// destructor of ldElement
			this->le = NULL;
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
			// plainInputControl
			// 
			this->AccessibleName = L"";
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Window;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"plainInputControl";
			this->Size = System::Drawing::Size(50, 50);
			this->ControlAdded += gcnew System::Windows::Forms::ControlEventHandler(this, &plainInputControl::attachFuncsOnChild);
			this->ResumeLayout(false);

		}
#pragma endregion
	private	:	int constantPlainEdit	(constPlainCodes	);
	private	:	int editablePlainInit	(int*				);
	public	:	int	deviceEdit			(short inpItemCode
					,short verticeCode
					,const char* data
					,const char _mode);
	private	:	System::Void addClick(System::Object^  sender, System::EventArgs^  e) {
					//--------------------------------------------------------------------------------
					//	when child control existed , click signal would not pass throught this control
					//		very important property for windows object
					//	so that , not be afraid event collision.
					//	commented by Hsien , 2012.06.05
					//--------------------------------------------------------------------------------
					if(safe_cast<System::Windows::Forms::MouseEventArgs^>(e)->Button
						== Windows::Forms::MouseButtons::Left){
							this->deviceEdit(*this->itemRequest
								,*this->devRequest
								,NULL
								,ID_ON_EDIT);
					}//if
					this->Focus();			// gain the focus , Hsien , 2013.01.25
			 }
				//--------------------------------
				//	data packing/unpacking routine
				//--------------------------------
	public: int		store	(DEV_METADATA		**imds	);		// store internal data into meta-data form
	public: int		rebuild	(const DEV_METADATA	*imds	);		// rebuild this control by metadata

	private: System::Void attachFuncsOnChild(System::Object^  /*sender*/, System::Windows::Forms::ControlEventArgs^  e) {
			 e->Control->Click	+= gcnew System::EventHandler(this,&plainInputControl::addClick);
			 e->Control->Dock	= DockStyle::Fill;
			 this->BackgroundImage = nullptr;
		 }

};
}
