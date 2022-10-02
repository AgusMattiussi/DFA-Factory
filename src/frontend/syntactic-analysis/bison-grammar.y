%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (frontend).
	int program;
	int decOrExec;

	int symsta;
	int symstaArrType;
	int trnValue;
	int varVal;
	int symstaArrValue;
	int arr;
	int trnArrValue;
	int trnArr;
	int dfaValue;

	int dec;
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
%token <token> SYMBOL
%token <token> SYMBOL_ARRAY
%token <token> STATE
%token <token> STATE_ARRAY
%token <token> TRANSITION
%token <token> TRN_ARRAY
%token <token> DFA

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <decOrExec> decOrExec;
%type <dec> dec;
%type <symsta> symsta;
%type <symstaArrType> symstaArrType;
%type <trnValue> trnValue;
%type <varVal> varVal;
%type <symstaArrValue> symstaArrValue;
%type <arr> arr;
%type <trnArrValue> trnArrValue;
%type <trnArr> trnArr;
%type <dfaValue> dfaValue;
%type <exec> exec;
%type <add> add;
%type <rem> rem; 
%type <check> check;
%type <complement> complement; 
%type <join> join;
%type <print> print;

// El símbolo inicial de la gramatica.
%start program

%%

program: decOrExec				 													{ DummyProgramGrammarAction();  }

/* ==== Declaraciones ==== */

decOrExec: dec SEMICOLON decOrExec 													{ DummyGrammarAction();  }	
	| 	exec SEMICOLON decOrExec 													{ DummyGrammarAction();  }																
	| /* lambda */ 																	{ DummyGrammarAction();  }
	;

dec: symsta VARIABLE EQUALS STRING 													{ DummyGrammarAction();  } 
	| TRANSITION VARIABLE EQUALS trnValue 											{ DummyGrammarAction();  }
	| symstaArrType VARIABLE EQUALS symstaArrValue 									{ DummyGrammarAction();  }
	| TRN_ARRAY VARIABLE EQUALS trnArrValue 										{ DummyGrammarAction();  }
	| DFA VARIABLE EQUALS dfaValue 													{ DummyGrammarAction();  }
	;

symsta: SYMBOL 																		{ DummyGrammarAction();  } 
	| STATE 																		{ DummyGrammarAction();  }
	;

symstaArrType: SYMBOL_ARRAY 														{ DummyGrammarAction();  }
	| STATE_ARRAY 																	{ DummyGrammarAction();  }
	;

trnValue: OPEN_CURLY varVal COMMA varVal COMMA varVal CLOSE_CURLY					{ DummyGrammarAction();  }
	;

varVal: VARIABLE 																	{ DummyGrammarAction();  }
	| STRING 																		{ DummyGrammarAction();  }
	;

symstaArrValue: OPEN_CURLY arr CLOSE_CURLY 											{ DummyGrammarAction();  }
	;

arr: STRING COMMA arr 																{ DummyGrammarAction();  }
	| VARIABLE COMMA arr 															{ DummyGrammarAction();  }
	| STRING  																		{ DummyGrammarAction();  }
	| VARIABLE 																		{ DummyGrammarAction();  }
	;
	
trnArrValue: OPEN_CURLY trnArr CLOSE_CURLY 											{ DummyGrammarAction();  }
	;

trnArr: VARIABLE COMMA trnArr 													{ DummyGrammarAction();  }
	| trnValue COMMA trnArr 														{ DummyGrammarAction();  }
	| trnValue 																	{ DummyGrammarAction();  }
	| VARIABLE 																		{ DummyGrammarAction();  }
	;

dfaValue: OPEN_CURLY VARIABLE COMMA VARIABLE COMMA VARIABLE COMMA VARIABLE COMMA VARIABLE CLOSE_CURLY	{ DummyGrammarAction();  }
	;
// {estados, simbolos, simbolo inicial, finales, transiciones}


/* ==== Prestaciones ==== */

exec: add 																			{ DummyGrammarAction();  }
	| rem 																			{ DummyGrammarAction();  }
	| check 																		{ DummyGrammarAction();  }
	| complement 																	{ DummyGrammarAction();  }
	| join 																			{ DummyGrammarAction();  }
	| print																			{ DummyGrammarAction();  }
	; 																				

check: VARIABLE CHECK symstaArrValue 												{ DummyGrammarAction();  }	
	| VARIABLE CHECK VARIABLE 														{ DummyGrammarAction();  }
	;

add: DFA VARIABLE EQUALS ADD trnValue TO VARIABLE 									{ DummyGrammarAction();  }
	| DFA VARIABLE EQUALS ADD VARIABLE TO VARIABLE									{ DummyGrammarAction();  }
	| DFA VARIABLE EQUALS ADD STRING TO VARIABLE									{ DummyGrammarAction();  }
	| VARIABLE EQUALS ADD trnValue TO VARIABLE 										{ DummyGrammarAction();  }
	| VARIABLE EQUALS ADD VARIABLE TO VARIABLE										{ DummyGrammarAction();  }
	| VARIABLE EQUALS ADD STRING TO VARIABLE										{ DummyGrammarAction();  }								
	;

rem: DFA VARIABLE EQUALS REM STRING FROM VARIABLE 									{ DummyGrammarAction();  }
	| DFA VARIABLE EQUALS REM VARIABLE FROM VARIABLE 								{ DummyGrammarAction();  }
	| VARIABLE EQUALS REM STRING FROM VARIABLE 										{ DummyGrammarAction();  }
	| VARIABLE EQUALS REM VARIABLE FROM VARIABLE 									{ DummyGrammarAction();  }
	;

complement: DFA VARIABLE EQUALS NOT VARIABLE 										{ DummyGrammarAction();  }
	| VARIABLE EQUALS NOT VARIABLE 													{ DummyGrammarAction();  }
	;

join: DFA VARIABLE EQUALS VARIABLE JOIN VARIABLE VARIABLE 							{ DummyGrammarAction();  }
	| DFA VARIABLE EQUALS VARIABLE JOIN VARIABLE trnValue 							{ DummyGrammarAction();  }
	| VARIABLE EQUALS VARIABLE JOIN VARIABLE VARIABLE 								{ DummyGrammarAction();  }
	| VARIABLE EQUALS VARIABLE JOIN VARIABLE trnValue 								{ DummyGrammarAction();  }
	;

print: PRINT STRING 																{ DummyGrammarAction();  }
	;


%%
