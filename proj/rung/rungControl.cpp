// rung_control_expr.cpp : main project file.

#include "stdafx.h"
#include "rungControl.h"

extern void MarshalString ( String ^ s, string& os );

System::Void rungControl::rungLoad(System::Object^  /*sender*/, System::EventArgs^  /*e*/) {
//			 addRow(INITIAL);	//		// do nothing when rung load , adjesting working flow
										// Hsien 2013/11/15
	//		 this->getIndexFromTlp();
}
int rungControl::getIndexFromTlp(void)
{
	Windows::Forms::TableLayoutPanel^ parentTable = 
		safe_cast<Windows::Forms::TableLayoutPanel^>(this->Parent);			// cast-back parent into tlp class
//	Windows::Forms::TableLayoutPanelCellPosition	pos;

//	pos		= parentTable->GetPositionFromControl(this);

	this->rungSn		= parentTable->GetPositionFromControl(this).Row;
//	this->rungSn		= (parentTable->GetPositionFromControl(this)).Column;
	//this->rungSn		= parentTable->GetCellPosition(this).Row;

	this->labelRungSN->Text = this->rungSn.ToString();

	return 1;
}

int		rungControl::markSn(int serialNumber)
{
	this->rungSn				= serialNumber;
	this->labelRungSN->Text		= this->rungSn.ToString();

	return 1;
}


int		rungControl::isMe(const int serialNumber)
{
	if(serialNumber != this->rungSn){
		//---------
		//	Not this Rung
		//---------
		this->labelRungSN->BackColor = Color::Gray;
		return 0;
	}

	//--------
	//	this Rung is Matched
	//--------
	this->labelRungSN->BackColor = Color::Red;
	this->Focus();			// to earn focus

	return 1;
}