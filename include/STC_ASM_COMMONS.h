//-----------
//
//	The Externed Datas Bounded in STC
//------------
#include<PLC_IRC.h>
#include<STC_PCDPM.h>
#include<STC_ERRHDL.h>
#include<sqlite3.h>

//-------------------
//	Instance Datas
//-------------------
extern IRC_UNIT			outputElement	;			// global buffer for all code-generating process
extern int					generation	;		// used to control which branch to output
extern PCDPM_FAMILY		outputQueue		;			// for standard output(IRC code instructions) of STC
//---------------
//	References
//---------------
extern sqlite3				*refDb			;		// reference from external
extern STC_ERRHDL			*refErrorHdl	;		// reference from external	