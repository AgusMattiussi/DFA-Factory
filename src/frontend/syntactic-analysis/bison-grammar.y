%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	/*
	Program program;
	Expression expression;
	Factor factor;
	Constant constant;
	...
	*/
	// TODO: No-terminales (frontend).
	// int program;
	// int expression;
	// int factor;
	// int constant;
	int program;

	int decList;
	int dec;
	int symsta;
	int symstaArrType;
	int trnValue;
	int varVal;
	int symstaArrValue;
	int symArr;
	int staArr;
	int trnArrValue;
	int trnArr;
	int dfaValue;

	int execList;
	int exec;
	int add;
	int rem; 
	int check;
	int complement; 
	int join;
	int print;

	// Terminales.
	token token;
	
}

// IDs y tipos de los tokens terminales generados desde Flex.
/* 
%token <token> ADD
%token <token> SUB
%token <token> MUL
%token <token> DIV
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <integer> INTEGER 
*/

%token <token> EQUALS
%token <token> NOT
%token <token> SEMICOLON
%token <token> OPEN_CURLY
%token <token> CLOSE_CURLY
%token <token> COMMA
%token <token> CHECK
%token <token> ADD
%token <token> TO
%token <token> REM
%token <token> FROM
%token <token> JOIN
%token <token> PRINT
%token <token> VARIABLE
%token <token> STRING
%token <token> SYM
%token <token> SYM_ARRAY
%token <token> STA
%token <token> STA_ARRAY
%token <token> TRN
%token <token> TRN_ARRAY
%token <token> DFA

// TODO: Tipos de dato para los no-terminales generados desde Bison.
/* %type <program> program
%type <expression> expression
%type <factor> factor
%type <constant> constant */
%type <program> program
%type <decList> decList;
%type <dec> dec;
%type <symsta> symsta;
%type <symstaArrType> symstaArrType;
%type <trnValue> trnValue;
%type <varVal> varVal;
%type <symstaArrValue> symstaArrValue;
%type <symArr> symArr;
%type <staArr> staArr;
%type <trnArrValue> trnArrValue;
%type <trnArr> trnArr;
%type <dfaValue> dfaValue;
%type <execList> execList;
%type <exec> exec;
%type <add> add;
%type <rem> rem; 
%type <check> check;
%type <complement> complement; 
%type <join> join;
%type <print> print;

// Reglas de asociatividad y precedencia (de menor a mayor).
/* %left ADD SUB
%left MUL DIV */

// El símbolo inicial de la gramatica.
%start program

/* %%

program: expression													{ $$ = ProgramGrammarAction($1); }
	;

expression: expression[left] ADD expression[right]					{ $$ = AdditionExpressionGrammarAction($left, $right); }
	| expression[left] SUB expression[right]						{ $$ = SubtractionExpressionGrammarAction($left, $right); }
	| expression[left] MUL expression[right]						{ $$ = MultiplicationExpressionGrammarAction($left, $right); }
	| expression[left] DIV expression[right]						{ $$ = DivisionExpressionGrammarAction($left, $right); }
	| factor														{ $$ = FactorExpressionGrammarAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorGrammarAction($2); }
	| constant														{ $$ = ConstantFactorGrammarAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantGrammarAction($1); }
	;

%% */

%%

program: decList execList		 													{ }

/* ==== Declaraciones ==== */

decList: dec SEMICOLON decList 														{ }
	| execList 																		{ }
	| /* lambda */ 																	{ }
	;

dec: symsta VARIABLE EQUALS STRING 													{ } 
	| TRN VARIABLE EQUALS trnValue 													{ }
	| symstaArrType VARIABLE EQUALS symstaArrValue 									{ }
	| TRN_ARRAY VARIABLE EQUALS trnArrValue 										{ }
	| DFA VARIABLE EQUALS dfaValue 													{ }
	;

symsta: SYM 																		{ } 
	| STA 																			{ }
	;

symstaArrType: SYM_ARRAY 															{ }
	| STA_ARRAY 																	{ }
	;

trnValue: OPEN_CURLY varVal COMMA varVal COMMA varVal CLOSE_CURLY					{ }
	;

varVal: VARIABLE 																	{ }
	| STRING 																		{ }
	;

symstaArrValue: OPEN_CURLY symArr CLOSE_CURLY 										{ }
	| OPEN_CURLY staArr CLOSE_CURLY 												{ }
	;

symArr: STRING COMMA symArr 														{ }
	| VARIABLE COMMA symArr 														{ }
	| STRING  																		{ }
	| VARIABLE 																		{ }
	;

staArr: STRING COMMA staArr 														{ }
	| VARIABLE COMMA staArr 														{ }
	| STRING  																		{ }
	| VARIABLE 																		{ }
	;
	
trnArrValue: OPEN_CURLY trnArr CLOSE_CURLY 											{ }
	;

trnArr: TRN COMMA trnArr 															{ }
	| VARIABLE COMMA trnArr 														{ }
	| TRN 																			{ }
	| VARIABLE 																		{ }
	;

dfaValue: OPEN_CURLY VARIABLE COMMA VARIABLE COMMA VARIABLE COMMA VARIABLE COMMA VARIABLE CLOSE_CURLY	{ }
	;
// {estados, simbolos, simbolo inicial, finales, transiciones}


/* ==== Prestaciones ==== */

execList: exec SEMICOLON execList 													{ }
	| decList 																		{ }
	| /* lambda */ 																	{ }
	;

exec: add | rem | check | complement | join | print 								{ }

check: VARIABLE CHECK symstaArrValue 												{ }
	| VARIABLE CHECK VARIABLE 														{ }
	;

add: DFA VARIABLE EQUALS ADD trnValue TO VARIABLE 									{ }
	| DFA VARIABLE EQUALS ADD VARIABLE TO VARIABLE									{ }
	| DFA VARIABLE EQUALS ADD STRING TO VARIABLE									{ }
	| VARIABLE EQUALS ADD trnValue TO VARIABLE 										{ }
	| VARIABLE EQUALS ADD VARIABLE TO VARIABLE										{ }
	| VARIABLE EQUALS ADD STRING TO VARIABLE										{ }								
	;

rem: DFA VARIABLE EQUALS REM STRING FROM VARIABLE 									{ }
	| DFA VARIABLE EQUALS REM VARIABLE FROM VARIABLE 								{ }
	| VARIABLE EQUALS REM STRING FROM VARIABLE 										{ }
	| VARIABLE EQUALS REM VARIABLE FROM VARIABLE 									{ }
	;

complement: DFA VARIABLE EQUALS NOT VARIABLE 										{ }
	| VARIABLE EQUALS NOT VARIABLE 													{ }
	;

join: DFA VARIABLE EQUALS VARIABLE JOIN VARIABLE VARIABLE 							{ }
	| DFA VARIABLE EQUALS VARIABLE JOIN VARIABLE trnValue 							{ }
	| VARIABLE EQUALS VARIABLE JOIN VARIABLE VARIABLE 								{ }
	| VARIABLE EQUALS VARIABLE JOIN VARIABLE trnValue 								{ }
	;

print: PRINT STRING 																{ }
	;


%%


/* EXPRESSION: TRANSITION | ...

TRANSITION: TRN NAME OPEN_CURLY VARORSTR COMMA VARORSTR COMMA VARORSTR CLOSE_CURLY

VARORSTR: VARNAME | STR
TRN: trn */