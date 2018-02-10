%{
#define _cplusplus					//	compile as C++ code , rather default setting : compile as C code
									//	add /TP option for VC compiler
									
#include<STC_PROC.h>			// collection functions , were packed outside , be much easier to test and maintain. Hsien , 2012.06.11
#include<STC_ERRHDL.h>
extern int				stlex();		// stlex() was established externally
extern int				stlineno;
extern char*			sttext	;

void	yyerror(const char* s);

#include	<STC_ASM_COMMONS.h>		// for accessing error handler

#define LY_ERR_SYNTAX	0x01

%}


%union{
	//---------
	//	Native Types
	//---------
	int			integer		;
	float		_float		;
	char		string[32]	;
	//---------
	//	Compound Types
	//---------
#include<PLC_IRC.h>
	IRC_OPERAND	operandElem	;
	IRC_UNIT	outputElem	;
}

/***********************/
/* OPERAND definition */
/***********************/
%token	<string>	FRAMENAME  
%token	<integer>	INTEGER	
%token	<_float>	FLOAT

%type	<operandElem>	constant variable 
%type	<outputElem>	operand

/***********************/
/* OPERATOR definition */
/***********************/
%token	<string> LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP RIGHT_OP LEFT_OP
%token	<string> '~' '!' '-' '*' '/' '+' '&' '|' '=' ';' '%' '>' '<' '^'

/***********************/
/* PROGCTRL definition */
/***********************/
%token IF ELSE WHILE GOTO END
%token YYEOF

%start st_program

%%

constant
	:	INTEGER			{$$.type = CONSTANT; $$.integer = $1		;}
	|	FLOAT			{$$.type = CONSTANT; $$._float	= $1		;}
	;
	
variable
	:	FRAMENAME INTEGER '.'		INTEGER	{if(stcErrorHandler(refErrorHdl,MOD_VAR,varProcess($1,_BOOLEAN	,$2,$4	,&$$),popText(),stlineno) == STC_FATAL)	YYABORT;}
	|	FRAMENAME INTEGER ':'		INTEGER	{if(stcErrorHandler(refErrorHdl,MOD_VAR,varProcess($1,_INT08	,$2,$4	,&$$),popText(),stlineno) == STC_FATAL)	YYABORT;}
	|	FRAMENAME INTEGER 					{if(stcErrorHandler(refErrorHdl,MOD_VAR,varProcess($1,_INT16	,$2,0	,&$$),popText(),stlineno) == STC_FATAL)	YYABORT;}
	|	FRAMENAME INTEGER FRAMENAME	INTEGER	{if(stcErrorHandler(refErrorHdl,MOD_VAR,varProcess($1,_INT32	,$2,0	,&$$),popText(),stlineno) == STC_FATAL)	YYABORT;}
	;
	
operand
	:	variable				{$$.type = _OPERAND;$$.operandData = $1;}
	|	constant				{$$.type = _OPERAND;$$.operandData = $1;}
	;

primary_expression
	: operand					{if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&($1),generation),popText(),stlineno) == STC_FATAL) YYABORT;}
	| '(' expression ')'		
	;

postfix_expression
	: primary_expression
	| postfix_expression '(' argument_expression_list ')'		{/* function , not valid so far */}
	;

argument_expression_list
	: primary_expression
	| argument_expression_list ',' primary_expression
	;

unary_expression
	: postfix_expression
	| '~' unary_expression				{/* bitwise inversion*/}
	| '!' unary_expression				{/* logic inversion */				resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($1,&outputElement),$1,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$1,stlineno) == STC_FATAL) YYABORT;}
	| '-' unary_expression				{/* negation special operation */	resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,negationProcess(),$1,stlineno) == STC_FATAL)  YYABORT;}
	;
	

multiplicative_expression
	: unary_expression										
	| multiplicative_expression '*' unary_expression		{resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	| multiplicative_expression '/' unary_expression		{resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	| multiplicative_expression '%' unary_expression		{resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression		{resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	| additive_expression '-' multiplicative_expression		{resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	;

shift_expression
	: additive_expression
	| shift_expression LEFT_OP additive_expression			{resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	| shift_expression RIGHT_OP additive_expression			{resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	;

relational_expression
	: shift_expression
	| relational_expression '<' shift_expression	{resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	| relational_expression '>' shift_expression	{resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	| relational_expression LE_OP shift_expression	{resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	| relational_expression GE_OP shift_expression	{resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression	{resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	| equality_expression NE_OP relational_expression	{resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression			{resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression		{resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression	{resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression {resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression	{resetText();if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL)  YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	;

expression						
	: logical_or_expression				
	| operand '=' expression	{if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&($1),generation),popText(),stlineno) == STC_FATAL) YYABORT;if(stcErrorHandler(refErrorHdl,MOD_VAR,opProcess($2,&outputElement),$2,stlineno) == STC_FATAL) YYABORT; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),$2,stlineno) == STC_FATAL) YYABORT;}
	;							

ifcondition
	: IF	'(' expression ')'	{generation = CHILD;	/* gateway changing  , Hsien  ,2013.02.04*/}

statement
	: compound_statement
	| expression_statement							
	| ifcondition statement END					{if(stcErrorHandler(refErrorHdl,MOD_PCDPM,branchProcess(&outputQueue,_COND_JUMP	,1)	,popText(),stlineno)	== STC_FATAL) YYABORT;}
	| ifcondition statement ELSE statement	END	{if(stcErrorHandler(refErrorHdl,MOD_PCDPM,branchProcess(&outputQueue,_JUMP		,1)	,popText(),stlineno)	== STC_FATAL) YYABORT;}
	| WHILE '(' expression ')' statement				
	| GOTO INTEGER ';'
	;

compound_statement
	: '{' '}'
	| '{' statement_list '}' 
	;

statement_list
	: statement					{/**/}		
	| statement_list statement	{/**/}
	;

st_program
	: statement_list YYEOF	{/*outputElement.type = _PROGCTRL ; outputElement.progCtrlData.ctrlCmd = _PROG_END; outputElement.progCtrlData.insIndex = 0 ; pcdpmWriteData(&outputElement,sizeof(IRC_UNIT),generation);*/YYACCEPT; }	
	;

expression_statement
	: ';'						
	| expression ';'			{outputElement.type = _PROGCTRL ; outputElement.progCtrlData.ctrlCmd = _EXPR_END; outputElement.progCtrlData.insIndex = 0 ; if(stcErrorHandler(refErrorHdl,MOD_PCDPM,pcdpmWriteData(&outputQueue,&outputElement,generation),popText(),stlineno) == STC_FATAL) YYABORT;}
	;

%%

void	yyerror(const char* s){
	//----------------------------------
	//	extern errorStack
	//	output error to external device
	//		would be called in yyparse() when syntax error occurs 
	//----------------------------------
	stcErrorHandler(refErrorHdl,MOD_LY
	,LY_ERR_SYNTAX
	,sttext/*popText()*//*stlval.string*/
	,stlineno);				// not well-organized yet , Hsien,2012.10.09
};