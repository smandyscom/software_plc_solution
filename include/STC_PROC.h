//-----------------
//	functions which embedded in LY parser
//-----------------
#ifndef _STC_PROC_H
#define _STC_PROC_H
#include<PLC_IRC.h>
#include<STC_VAROP.h>
#include<STC_PCDPM.h>

#pragma warning(disable:4996)
//-------------------------
//	Branch-Control MARCO
//-------------------------
#define GRAND  PCDPM_GRAND
#define PARENT PCDPM_PARENT
#define CHILD  PCDPM_CHILD

extern PCDPM_FAMILY		outputQueue		;			// for standard output(IRC code instructions) of STC
extern int				generation		;			//	used to indicate which generation the compiler output.
														//  all expression parsing part beneath the expression_statement are following this.
														//	Hsien , 2012.06.08
extern IRC_UNIT			outputElement		;


int		varProcess		(const char*		varName,
						 const char			type,
						 const size_t		wordIndex,
						 const size_t		bitByteIndex,
						 IRC_OPERAND		*output);			//	given raw information , then output IRC_OPERAND info if exists
int		opProcess		(const char *opName,IRC_UNIT *outputElement);
int		branchProcess	(PCDPM_FAMILY* family
						 ,	const int _type
						 ,	const int _jumpOffset);
int		negationProcess(void);	// for handling unary negation , Hsien 2013/12/6


//------------------------------
//	Raw Stream Storage Mechanism , For Error Handling
//------------------------------
extern char	textBuffer	[32];
extern char	outBuffer	[32];
void		pushText	(const char* str);
const char* popText		(void);
void		resetText	(void);


#endif