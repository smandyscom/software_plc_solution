#include"stdafx.h"
#include "ldEditor.h"
#include<LDE_OUTDEV.h>
#include<LDE_ELEMENT.h>

using namespace _ldEditor;


System::Void ldEditor::outputSelect(System::Object ^sender, System::EventArgs ^e)
{
	Button^	bt = safe_cast<System::Windows::Forms::Button^>(sender);

		//----------------------
		//	Convert the info stored in Tag into numerical(inpCode)
		// Hsien 2013/5/19
		//----------------------
	*this->selectedOutItem = 
	System::Convert::ToInt32(bt->Tag->ToString(),16);

	//----------------------
	//	Change Style Betwenn Pressed and Release
	//		1. Release All the inp buttons
	//		2. layout that one into pressed style
	//----------------------
	for each(Button^ e in /*outTable*/this->tlpOdpanel->Controls)
		e->FlatStyle = Windows::Forms::FlatStyle::Standard;
	bt->FlatStyle = Windows::Forms::FlatStyle::Flat;

}


System::Void ldEditor::selectionInit(void)
{
	System::Resources::ResXResourceSet^ resources =	
		gcnew System::Resources::ResXResourceSet("ldeItems.resx");

		//----------------------
		//	Much more concised , H// Hsien 2013/5/24
		//----------------------
		for	each(System::Windows::Forms::Button^ e in /*inpTable*/this->tlpInpDevice->Controls){
			e->BackgroundImage = 
				safe_cast<Drawing::Image^>(resources->GetObject(e->Name));
			e->Click += gcnew System::EventHandler(this,&ldEditor::inputSelect);

			//----------------------
			//	Setup Visual Effect when pressed
			//----------------------
			e->FlatAppearance->BorderColor	= Drawing::Color::Red;
			e->FlatAppearance->BorderSize	= 1;
		}

		for each(System::Windows::Forms::Button^ e in /*outTable*/this->tlpOdpanel->Controls){
			e->BackgroundImage = 
				safe_cast<Drawing::Image^>(resources->GetObject(e->Name));
			e->Click += gcnew System::EventHandler(this,&ldEditor::outputSelect);

			//----------------------
			//	Setup Visual Effect when pressed
			//----------------------
			e->FlatAppearance->BorderColor	= Drawing::Color::Red;
			e->FlatAppearance->BorderSize	= 1;
		}
}

System::Void ldEditor::inputSelect		(System::Object^  sender	, System::EventArgs^  e)
{		
	Button^	bt = safe_cast<System::Windows::Forms::Button^>(sender);

		//----------------------
		//	Convert the info stored in Tag into numerical(inpCode)
		// Hsien 2013/5/19
		//----------------------
	*this->selectedInpItem = 
	System::Convert::ToInt32(bt->Tag->ToString(),16);

	if(*this->selectedInpItem == EMPTY){
		//----------------------
		//	Exclusive Case: Deleting
		//----------------------
		// Hsien 2013/5/19
		*this->selectedOutItem = NONE;

		for each(Button^ e in /*inpTable*/this->tlpInpDevice->Controls)
			e->FlatStyle = Windows::Forms::FlatStyle::Standard;
		for each(Button^ e in /*outTable*/this->tlpOdpanel->Controls)
			e->FlatStyle = Windows::Forms::FlatStyle::Standard;
		bt->FlatStyle = Windows::Forms::FlatStyle::Flat;
	}
	else{
	//----------------------
	//	Change Style Betwenn Pressed and Release
	//		1. Release All the inp buttons
	//		2. layout that one into pressed style
	//----------------------
		for each(Button^ e in /*inpTable*/this->tlpInpDevice->Controls)
			e->FlatStyle = Windows::Forms::FlatStyle::Standard;
		bt->FlatStyle = Windows::Forms::FlatStyle::Flat;
	}
			
}

//----------------------
//	Part : Key Message Manage , keyDown() event
// Hsien 2013/5/20
//----------------------
System::Void ldEditor::keyDownHandler	(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
	//----------------------
	//	The KeyDown Message Handling Center
	//----------------------

	switch(e->KeyCode){
		//----------------------
		//	Singal Keys
		//----------------------
		case Keys::F10:
			//-------------
			//	F10 as Step
			//-------------
//			this->tsbStepClick(this->tsbStep,nullptr);
			this->tsbCompileClick(this->tsbStep,nullptr);
			break;
		default:
			//----------------------
			//	Compound Keys
			// Hsien 2013/5/23
			//----------------------
			if((e->KeyCode == Keys::S) && e->Control)/*| Keys::ControlKey*/
				this->tsbFileClick(this->tsbSaveFile,nullptr);
			if((e->KeyCode == Keys::B) && e->Control)/*| Keys::ControlKey*/
				this->tsbCompileClick(this->tsbCompile,nullptr);
			break;
	}		
}
//----------------------
//	Part : Central Message Interface
// Hsien 2013/5/20
//----------------------
int	ldEditor::postOnOutput	(String^ message		,	String^ group)
{	
#define OUTPUT_FORMAT	"{0:G} {1:G}: {2:G}\r\n"

	// Hsien 2013/5/20
	this->tbOutput->Text +=
		String::Format(OUTPUT_FORMAT
		,System::DateTime::Now.ToString()
		,group
		,message);
	//---------------------------------------------
	//	To made text line always in the lastest one
	// Hsien 2013/6/24
	//---------------------------------------------
	tbOutput->SelectionStart = tbOutput->Text->Length;
	tbOutput->ScrollToCaret();

	return 1;
}
