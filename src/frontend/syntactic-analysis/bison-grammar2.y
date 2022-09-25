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
	int program;
	int expression;
	int factor;
	int constant;

	// Terminales.
	token token;
	char strval[1024];
	struct trn* transition;
	struct symArray* symbolArray;
	struct staArray* stateArray;
	struct trnArray* transitionArray;
	struct dfa* dfa;
	
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

%token <strval> SYM
%token <symbolArray> SYM_ARRAY
%token <strval> STA
%token <stateArray> STA_ARRAY
%token <transition> TRN
%token <transitionArray> TRN_ARRAY
%token <dfa> DFA

// TODO: Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <expression> expression
%type <factor> factor
%type <constant> constant

// Reglas de asociatividad y precedencia (de menor a mayor).
%left ADD SUB
%left MUL DIV

// El símbolo inicial de la gramatica.
%start program

%%

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

%%


/* EXPRESSION: TRANSITION | ...

TRANSITION: TRN NAME OPEN_CURLY VARORSTR COMMA VARORSTR COMMA VARORSTR CLOSE_CURLY

VARORSTR: VARNAME | STR
TRN: trn */