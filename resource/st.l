%{
#define _cplusplus					//	compile as C++ code , rather default setting : compile as C code
									//	add /TP option for VC compiler
#include	<STC_PROC.h>
#include	<STC_ERRHDL.h>	
#include	<string.h>
#include	"st.tab.h"

#include	<STC_ASM_COMMONS.h>		// for accessing error handler

void	comment();
void	getString();
void	getInteger();
%}

/****************************************************************************************************/
/*	The macro yy_set_interactive(is_interactive) can be used to control whether						*/
/*	the current buffer is considered interactive. An interactive buffer is processed more slowly,	*/
/*	but must be used when the scanner's input source is indeed interactive to						*/
/*	avoid problems due to waiting to fill buffers (see the discussion of the `-I'					*/
/*	flag in Scanner Options). A non-zero value in the macro invocation marks						*/
/*	the buffer as interactive, a zero value as non-interactive. Note that							*/
/*	use of this macro overrides %option always-interactive or										*/
/*	%option never-interactive (see Scanner Options). yy_set_interactive()							*/
/*	must be invoked prior to beginning to scan the buffer that is (or is not)						*/
/*	to be considered interactive.																	*/
/*	cite from http://flex.sourceforge.net/manual/Misc-Macros.html#Misc-Macros						*/
/****************************************************************************************************/

/* enable this when formal using , from file , fread	*/
%option never-interactive

/* enable this when testing	, from stdio , getc			*/			
/*%option always-interactive*/			

%option noyywrap
%option yylineno

D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*



%%

[A-Z]+_?[A-Z]*	{getString();return(FRAMENAME);}	/*	upper-case	only			*/
"."				{getString();return('.');}

{D}+			{getInteger();	return(INTEGER);}	/* {D}+"."{D}+		{return(FLOAT);}	not surpport so far*/ 

"/*"			{comment();}				

"goto"		{getString();return(GOTO); }
"if"		{
				getString();
				stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmCreateChild(&outputQueue),popText(),yylineno);	/* branch generate */
				generation = PARENT;																		/* for condition expression , it still written in PARENT branch */
				return(IF); 
				}
"else"		{
				getString();
				stcErrorHandler(refErrorHdl,MOD_PCDPM,branchProcess		(&outputQueue	,_COND_JUMP	,2	/*	should be "2" step more jumping forward	*/		),popText(),stlineno)	;	/* pack the IF-BRANCH */
				stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmCreateChild	(&outputQueue)	,popText()	,yylineno																	)	;	/* create ELSE-BRANCH */
				generation = CHILD; 
				return(ELSE);
				}
"end"		{getString();return(END);/* added by Hsien , 2013.01.31 , to solve serious look-ahead problem of LY*/}
"while"		{getString();return(WHILE); }

">>"		{getString();return(RIGHT_OP); }
"<<"		{getString();return(LEFT_OP); }
"&&"		{getString();return(AND_OP); }
"||"		{getString();return(OR_OP); }
"<="		{getString();return(LE_OP); }
">="		{getString();return(GE_OP); }
"=="		{getString();return(EQ_OP); }
"!="		{getString();return(NE_OP); }
";"			{getString();return(';'); }
("{"|"<%")	{getString();return('{'); }
("}"|"%>")	{getString();return('}'); }
","			{getString();return(','); }
":"			{getString();return(':'); }
"="			{getString();return('='); }
"("			{getString();return('('); }
")"			{getString();return(')'); }
"&"			{getString();return('&'); }
"!"			{getString();return('!'); }
"~"			{getString();return('~'); }
"-"			{getString();return('-'); }
"+"			{getString();return('+'); }
"*"			{getString();return('*'); }
"/"			{getString();return('/'); }
"%"			{getString();return('%'); }
"<"			{getString();return('<'); }
">"			{getString();return('>'); }
"^"			{getString();return('^'); }
"|"			{getString();return('|'); }

[ \t\v\n\f\r]	{}
.			{}			/*	ignore bad characters	*/

<<EOF>>			{return(YYEOF);}	/*	ctrl+z in stdin			*/

%%
void	getInteger(){
	stlval.integer = atoi(yytext);
	pushText(yytext);
}
void	getString(){
	strcpy(stlval.string,yytext);
	pushText(yytext);				// push raw text into temporary buffer
}

void comment()
{
/***********************************/
/* ignore the comment enbraced by  */
//**********************************/
	char c, c1;
	
	while ((c = yyinput()) != 0){
		if( c == '\n')
			stlineno++;
		else if(c == '*'){
			if((c1 = yyinput()) == '/')
				break;
			else
				unput(c1);
		}
	}
}

void stparseClose()
{
	//------------------
	//	close yy_buffer
	//------------------
	st_delete_buffer(YY_CURRENT_BUFFER);
}