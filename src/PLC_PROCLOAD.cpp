#include<stdio.h>
#include<string.h>

#define	PROCLOAD_FORMAT_COMMENT		"#%s['\n']"
#define PROCLOAD_FORMAT_PROC		"\[%d\]%s%s"

#define	PROCLOAD_MATCH_COUNT_PROC		3	
#define	PROCLOAD_MATCH_COUNT_COMMENT	1

#define	PROCLOAD_STR_PID_RANDOM			"random"



int procLoadByLine(const char*	_sourceLineStr
				   ,int*		_level
				   ,char*		_filenameStr
				   ,char*		_pidStr)
{
	int matchCount;
	//-------------------------------------
	//	Turns Raw String into Sperated Formatted String
	//-------------------------------------
	matchCount = sscanf(
		_sourceLineStr
		,PROCLOAD_FORMAT_PROC
		,_level
		,_filenameStr
		,_pidStr);
	if(matchCount != PROCLOAD_MATCH_COUNT_PROC){
		//--------------------------------------------------
		//	If this line is not represented as Command Line
		//		Check if comment
		//--------------------------------------------------
		matchCount = sscanf(_sourceLineStr
			,PROCLOAD_FORMAT_PROC);
		if(matchCount != PROCLOAD_MATCH_COUNT_COMMENT)
			//------------
			//	Error
			//------------
			return 0;
		else{
			//--------------
			//	Clear Buffer
			//--------------
			return 2;	// return as comment
		}
	}


	return 1;
}

int	turnsIntoData()
{
	//---------------------------------
	//	Turns Formatted String into Useable Data
	//		Fp
	//		pid(or Randomized Pid)
	//		if any error occurs , reject and report
	//---------------------------------

	return 1;
}