%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (frontend).
	Program * program;
	DecOrExec * decOrExec;

	SymOrState * symOrState;
	SymOrStaArr * symOrStaArr;
	Transition * transition;
	VarOrString * varOrString;
	SymOrStaArrValue * symstaArrValue;
	Array * arr;
	TrnArrValue * trnArrValue;
	TransitionArr * transitionArr;
	DfaValue * dfaValue;

	Declaration * declaration;
	Exec * exec;
	AddOperand * addOperand;
	Add * add;
	Rem * rem; 
	Check * check;
	Complement * complement; 
	Join * join;
	TransitionOrVar * transitionOrVar;
	Print * print;

	// Terminales.
	token token;

}

// IDs y tipos de los tokens terminales generados desde Flex.
// %token <token> BEGIN_STRING
// %token <token> END_STRING
%token <token> EQUALS
%token <token> NOT
%token <token> SEMICOLON
%token <token> OPEN_CURLY
%token <token> CLOSE_CURLY
%token <token> OPEN_BRACKETS
%token <token> CLOSE_BRACKETS
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
%token <token> STATE
%token <token> TRANSITION
%token <token> DFA

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <decOrExec> decOrExec;
%type <declaration> dec;
%type <symOrState> symOrState;
%type <symOrStaArr> symOrStaArr;
%type <transition> transition;
%type <varOrString> varOrString;
%type <symstaArrValue> symstaArrValue;
%type <arr> arr;
%type <trnArrValue> trnArrValue;
%type <transitionArr> trnArr;
%type <dfaValue> dfaValue;
%type <exec> exec;
%type <add> add;
%type <addOperand> addOperand;
%type <rem> rem; 
%type <check> check;
%type <complement> complement; 
%type <join> join;
%type <transitionOrVar> transitionOrVar;
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

dec: symOrState VARIABLE EQUALS STRING 													{ DummyGrammarAction();  } 
	| TRANSITION VARIABLE EQUALS transition 											{ DummyGrammarAction();  }
	| symOrStaArr VARIABLE EQUALS symstaArrValue 									{ DummyGrammarAction();  }
	| TRANSITION OPEN_BRACKETS CLOSE_BRACKETS VARIABLE EQUALS trnArrValue 			{ DummyGrammarAction();  }
	| DFA VARIABLE EQUALS dfaValue 													{ DummyGrammarAction();  }
	;

symOrState: SYMBOL 																		{ DummyGrammarAction();  } 
	| STATE 																		{ DummyGrammarAction();  }
	;

symOrStaArr: symOrState OPEN_BRACKETS CLOSE_BRACKETS  								{ DummyGrammarAction();  }
	;

transition: OPEN_CURLY varOrString COMMA varOrString COMMA varOrString CLOSE_CURLY					{ DummyGrammarAction();  }
	;

varOrString: VARIABLE 																	{ DummyGrammarAction();  }
	| STRING 																		{ DummyGrammarAction();  }
	;

symstaArrValue: OPEN_CURLY arr CLOSE_CURLY 											{ DummyGrammarAction();  }
	;


arr: varOrString COMMA arr																{ DummyGrammarAction();  }
	| varOrString																		{ DummyGrammarAction();  }
	;  
	
trnArrValue: OPEN_CURLY trnArr CLOSE_CURLY 											{ DummyGrammarAction();  }
	;

trnArr: transitionOrVar COMMA trnArr 														{ DummyGrammarAction();  }														{ DummyGrammarAction();  }
	| transitionOrVar 																	{ DummyGrammarAction();  }
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

add: DFA VARIABLE EQUALS ADD addOperand TO VARIABLE									{ DummyGrammarAction();  }
 	|  VARIABLE EQUALS ADD addOperand TO VARIABLE									{ DummyGrammarAction();  }
 ;

addOperand: transition																{ DummyGrammarAction();  }
	| varOrString																	{ DummyGrammarAction();  }
	;	


rem: DFA VARIABLE EQUALS REM varOrString FROM VARIABLE								{ DummyGrammarAction();  }
	| VARIABLE EQUALS REM varOrString FROM VARIABLE									{ DummyGrammarAction();  }
	;
 

complement: DFA VARIABLE EQUALS NOT VARIABLE 										{ DummyGrammarAction();  }
	| VARIABLE EQUALS NOT VARIABLE 													{ DummyGrammarAction();  }
	;


join: DFA VARIABLE EQUALS VARIABLE JOIN VARIABLE transitionOrVar					{ DummyGrammarAction();  }
	| VARIABLE EQUALS VARIABLE JOIN VARIABLE transitionOrVar						{ DummyGrammarAction();  }

transitionOrVar: VARIABLE															{ DummyGrammarAction();  }
	| transition																	{ DummyGrammarAction();  }
	;

print: PRINT STRING 																{ DummyGrammarAction();  }
	;
%%
