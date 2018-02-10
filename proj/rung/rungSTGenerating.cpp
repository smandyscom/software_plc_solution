#include"stdafx.h"

using namespace _ldEditor;

//------------------------
//	interpreting routine
//------------------------
int		rungControl::exprGenerate(){
	//-------------------------------------------------------------------------------------------------------
	//	1. semetic checking(won't happen here)		: check if individual variable is valid	(considering)
	//	2. connection checking	: check if all connection is well done , achived by exprGenerator in LDE_LIB2
	//	3. if above checking are passed , means it's definity able to generate statement.
	//-------------------------------------------------------------------------------------------------------
	LD_ARRAY		la			;
	LD_ROW			lr			;		// if allocating statically , this vector can work,
	OD_ARRAY		oa			;
	string			expr,rungStmt	;

	//---------------------------------------------------------
	//	System::Array(Controls) -> LD elements 2D vector(Class)
	//---------------------------------------------------------
	for each(Control^	e in tlpRung->Controls){
		if(e->GetType() == plainInputControl::typeid){
			lr.push_back(*safe_cast<plainInputControl^>(e)->le);	// copy-constructor operated
			if(lr.size() == innerInpNum+2){							// element fullfilled , row switched
				la.push_back(lr);
				lr.clear();
			}
		}//if input plain

		if(e->GetType() == plainOutputControl::typeid){
			safe_cast<plainOutputControl^>(e)->od->getGenStr();		//	generating statment according to derived class's manipulators
			oa.push_back(*safe_cast<plainOutputControl^>(e)->od);	//  copy-constructor operated , all derived class were cast-back to base-class
		}
	}// for all controls in tlpRung

	//-----------------------------------------------
	//	covered by error handler , Hsien , 2012.10.12
	//-----------------------------------------------
#if	  RUNG_INDEPENDENT_DEVELOP
	exprGenerator(la,expr);
	stmtGenerator(oa,expr,rungStmt);
#else 
	if(stcErrorHandler(_errHandler,MOD_STG,exprGenerator(la,expr),"",this->rungSn) != STG_SUCCESS)
		return STG_ERROR;		// added by Hsein , 2013.01.07 , to unite the error report mechanism
	if(stcErrorHandler(_errHandler,MOD_STG,stmtGenerator(oa,expr,rungStmt),"",this->rungSn) != STG_SUCCESS)
		return STG_ERROR;
#endif

	//this->_rungStmt = (const char*)malloc(sizeof(char) * rungStmt.length());	// string.length without including terminal
	//memset((void*)this->_rungStmt,0,rungStmt.length());
	//strcpy((char*)this->_rungStmt,rungStmt.c_str());

	this->rungStmt = gcnew String(rungStmt.c_str());
	//this->expr = gcnew String(stmt.c_str());//debug usage

	return STG_SUCCESS;
}
