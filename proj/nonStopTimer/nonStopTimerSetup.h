#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace outDev {

	/// <summary>
	/// Summary for nonStopTimerSetup
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class nonStopTimerSetup : public System::Windows::Forms::Form
	{
	public:
		nonStopTimerSetup(void)
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
		~nonStopTimerSetup()
		{
			if (components)
			{
				delete components;
			}
		}
	public:
		int		getTimerIndex	(void){ return this->_timerIndex;}
		/*int*/String^		getTimerInterval(void){	return this->_timerInterval;}
		char	getTimerType	(void){ return this->_timerType;}

		int	_setupUIRefresh	(
			const int __timerIndex,
			/*const int*/String^ __timerInterval,
			const char	__timerType)
		{
			//----------------------------------------
			//	Internal Data Set
			//----------------------------------------
			this->_timerIndex		= __timerIndex;
			this->_timerInterval	= __timerInterval;
			this->_timerType = __timerType;
			//----------------------
			//	Change GUI effect
			// Hsien 2013/7/9
			//----------------------
			switch(__timerType){
				case ODSETTN:	this->rbTurnOn->Checked = true ;	break;
				case ODRSTTN:	this->rbTurnOff->Checked  = true ;	break;
				default:
					break;
			}
			//----------------------------------------
			//	UI refresh
			//----------------------------------------
//			this->tbTimer->Text		= __timerIndex.ToString();
			this->numericUpDownTimer->Value = __timerIndex;
			this->tbInterval->Text	= __timerInterval/*.ToString()*/;

			return 1;
		}
	private:
		System::Windows::Forms::TextBox^  tbInterval;

		int					_timerIndex;			// data would be write-in after text changed
		/*int*/String^		_timerInterval;			// data would be write-in after text changed
		char				_timerType;				// 

	private: System::Windows::Forms::Button^  btOk;
	private: System::Windows::Forms::Button^  btCancel;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::RadioButton^  rbTurnOff;
	private: System::Windows::Forms::RadioButton^  rbTurnOn;
	private: System::Windows::Forms::NumericUpDown^  numericUpDownTimer;


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
			this->btOk = (gcnew System::Windows::Forms::Button());
			this->btCancel = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->tbInterval = (gcnew System::Windows::Forms::TextBox());
			this->rbTurnOff = (gcnew System::Windows::Forms::RadioButton());
			this->rbTurnOn = (gcnew System::Windows::Forms::RadioButton());
			this->numericUpDownTimer = (gcnew System::Windows::Forms::NumericUpDown());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownTimer))->BeginInit();
			this->SuspendLayout();
			// 
			// btOk
			// 
			this->btOk->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->btOk->Location = System::Drawing::Point(64, 67);
			this->btOk->Name = L"btOk";
			this->btOk->Size = System::Drawing::Size(75, 23);
			this->btOk->TabIndex = 2;
			this->btOk->Text = L"OK";
			this->btOk->UseVisualStyleBackColor = true;
			// 
			// btCancel
			// 
			this->btCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btCancel->Location = System::Drawing::Point(145, 65);
			this->btCancel->Name = L"btCancel";
			this->btCancel->Size = System::Drawing::Size(75, 23);
			this->btCancel->TabIndex = 3;
			this->btCancel->Text = L"Cancel";
			this->btCancel->UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(92, 11);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(45, 12);
			this->label1->TabIndex = 4;
			this->label1->Text = L"Timer¡G";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(84, 40);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Interval¡G";
			// 
			// tbInterval
			// 
			this->tbInterval->Location = System::Drawing::Point(143, 37);
			this->tbInterval->Name = L"tbInterval";
			this->tbInterval->Size = System::Drawing::Size(77, 22);
			this->tbInterval->TabIndex = 6;
			this->tbInterval->TextChanged += gcnew System::EventHandler(this, &nonStopTimerSetup::tbInput);
			// 
			// rbTurnOff
			// 
			this->rbTurnOff->AutoSize = true;
			this->rbTurnOff->Location = System::Drawing::Point(10, 38);
			this->rbTurnOff->Name = L"rbTurnOff";
			this->rbTurnOff->Size = System::Drawing::Size(65, 16);
			this->rbTurnOff->TabIndex = 8;
			this->rbTurnOff->Text = L"Turn Off";
			this->rbTurnOff->UseVisualStyleBackColor = true;
			// 
			// rbTurnOn
			// 
			this->rbTurnOn->AutoSize = true;
			this->rbTurnOn->Checked = true;
			this->rbTurnOn->Location = System::Drawing::Point(10, 9);
			this->rbTurnOn->Name = L"rbTurnOn";
			this->rbTurnOn->Size = System::Drawing::Size(63, 16);
			this->rbTurnOn->TabIndex = 9;
			this->rbTurnOn->TabStop = true;
			this->rbTurnOn->Text = L"Turn On";
			this->rbTurnOn->UseVisualStyleBackColor = true;
			this->rbTurnOn->CheckedChanged += gcnew System::EventHandler(this, &nonStopTimerSetup::rbTurnTimer);
			// 
			// numericUpDownTimer
			// 
			this->numericUpDownTimer->Location = System::Drawing::Point(145, 9);
			this->numericUpDownTimer->Name = L"numericUpDownTimer";
			this->numericUpDownTimer->Size = System::Drawing::Size(75, 22);
			this->numericUpDownTimer->TabIndex = 11;
			this->numericUpDownTimer->ValueChanged += gcnew System::EventHandler(this, &nonStopTimerSetup::ncInput);
			// 
			// nonStopTimerSetup
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(234, 102);
			this->ControlBox = false;
			this->Controls->Add(this->numericUpDownTimer);
			this->Controls->Add(this->rbTurnOn);
			this->Controls->Add(this->rbTurnOff);
			this->Controls->Add(this->tbInterval);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->btCancel);
			this->Controls->Add(this->btOk);
			this->Name = L"nonStopTimerSetup";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Non-Stop Timer Setup";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDownTimer))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void tbInput(System::Object^  sender, System::EventArgs^  e) {
				 //if(sender == this->tbTimer){
					// //------------
					// //	in case of "timer" textbox
					// //------------
					// try{
					//	 this->_timerIndex = Int32::Parse(this->tbTimer->Text);
					//	 this->tbTimer->ForeColor = Color::Black;
					// }
					// catch(FormatException^ /*e*/){
					//	 //---------------
					//	 //	Invalid Format
					//	 //---------------
					//	 this->tbTimer->ForeColor = Color::Red;
					// }
				 //}
				 /*else */if(sender == this->tbInterval){
					 //------------
					 //	in case of "timer" textbox
					 //------------
					 this->_timerInterval = this->tbInterval->Text;	// Hsien 2013/5/21
					 //try{
						// this->_timerInterval = Int32::Parse(this->tbInterval->Text);
						// this->tbInterval->ForeColor = Color::Black;
					 //}
					 //catch(FormatException^ /*e*/){
						// //---------------
						// //	Invalid Format
						// //---------------
						// this->tbInterval->ForeColor = Color::Red;
					 //}
				 }
				 //else{
					// //-------
					// //	Error
					// //-------
				 //}
			 }
//private: System::Void btClick(System::Object^  sender, System::EventArgs^  e) {
//			 //---------------------
//			 //	Button OK or Cancel
//			 //---------------------
//			 if(sender == this->btOk){
//				 this->DialogResult = Windows::Forms::DialogResult::OK;
//			 }
//			 else if(sender == this->btCancel){
//				 this->DialogResult = Windows::Forms::DialogResult::Cancel;
//			 }
//		 }
private: System::Void rbTurnTimer(System::Object^  sender, System::EventArgs^  e) {
			 //---------------------
			 //	Timer On/Off
			 //---------------------
			 if(this->rbTurnOn->Checked){
				 this->_timerType			= ODSETTN;
				 this->tbInterval->Enabled	= true;
			 }
			 else{
				 this->_timerType			= ODRSTTN;
				 this->tbInterval->Enabled	= false;
			 }
		 }
private: System::Void ncInput(System::Object^  sender, System::EventArgs^  e) {
			 this->_timerIndex = System::Decimal::ToInt32(this->numericUpDownTimer->Value);
		 }
};
}
