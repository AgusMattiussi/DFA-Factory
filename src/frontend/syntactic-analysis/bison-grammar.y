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
	String * string;
	Variable * variable;
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
%token <variable> VARIABLE
%token <string> STRING
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

program: decOrExec				 													{ $$ = DfaProgramGrammarAction($1);  }

/* ==== Declaraciones ==== */

decOrExec: dec[declaration] SEMICOLON decOrExec[next]								{ $$ = FirstDecGrammarAction($declaration, $next);  }	
	| 	exec[execution] SEMICOLON decOrExec[next] 									{ $$ = FirstExecGrammarAction($execution, $next);  }																
	| /* lambda */ 																	{ $$ = DecOrExecLamdaGrammarAction();  }
	;

dec: symOrState VARIABLE EQUALS STRING												{ DummyGrammarAction();  } 
	| TRANSITION VARIABLE EQUALS transition 										{ DummyGrammarAction();  }
	| symOrStaArr VARIABLE EQUALS symstaArrValue 									{ DummyGrammarAction();  }
	| TRANSITION OPEN_BRACKETS CLOSE_BRACKETS VARIABLE EQUALS trnArrValue 			{ DummyGrammarAction();  }
	| DFA VARIABLE EQUALS dfaValue 													{ DummyGrammarAction();  }
	;

symOrState: SYMBOL 																	{ $$ = SymbolGrammarAction();  } 
	| STATE 																		{ $$ = StateGrammarAction();  }
	;

symOrStaArr: symOrState OPEN_BRACKETS CLOSE_BRACKETS  								{ $$ = SymOrStaArrGrammarAction($1);  }
	;

transition: OPEN_CURLY varOrString[stateFrom]
	 COMMA varOrString[stateTo] COMMA varOrString[symbol] CLOSE_CURLY				{ $$ = TransitionGrammarAction($stateFrom, $stateTo, $symbol);  }
	;

varOrString: VARIABLE 																{ $$ = VariableGrammarAction($1);  }
	| STRING 																		{ $$ = StringGrammarAction($1);  }
	;

symstaArrValue: OPEN_CURLY arr[array] CLOSE_CURLY 									{ $$ = SymstaArrValuegGrammarAction($array);  }
	;


arr: varOrString[vos] COMMA arr[next]												{ $$ = ArrayWithNextGrammarAction($vos, $next);  }
	| varOrString																	{ $$ = ArrayNoNextGrammarAction($1);  }
	;  
	
trnArrValue: OPEN_CURLY trnArr[transitionArray] CLOSE_CURLY 						{ $$ = TrnArrValueGrammarAction($transitionArray);  }
	;

trnArr: transitionOrVar[trnOrVar] COMMA trnArr[next] 								{ $$ = TrnArrWithNext($trnOrVar, $next);  }
	| transitionOrVar 																{ $$ = TrnArrNoNext($1);  }
	;


dfaValue: OPEN_CURLY VARIABLE[states] COMMA VARIABLE[symbols] 
		COMMA VARIABLE[startState] COMMA VARIABLE[endStates] COMMA
		VARIABLE[trns] CLOSE_CURLY													{ $$ = DfaValueGrammarAction($states, $symbols, $startState, $endStates, $trns); }
	;
// {estados, simbolos, estado inicial, finales, transiciones}


/* ==== Prestaciones ==== */

exec: add 																			{ $$ = ExecAddGrammarAction($1);  }
	| rem 																			{ $$ = ExecRemGrammarAction($1);  }
	| check 																		{ $$ = ExecCheckGrammarAction($1);  }
	| complement 																	{ $$ = ExecComplementGrammarAction($1);  }
	| join 																			{ $$ = ExecJoinGrammarAction($1); }
	| print																			{ $$ = ExecPrintGrammarAction($1);  }
	; 																				

check: VARIABLE[variable] CHECK symstaArrValue[ssav] 								{ $$ = CheckSymStaArrValueGrammarAction($variable, $ssav);  }	
	| VARIABLE[variable] CHECK VARIABLE[rightOperand] 								{ $$ = CheckVariableGrammarAction($variable, $rightOperand);  }
	;


add: DFA VARIABLE[resultDFA] EQUALS ADD addOperand[leftOperand] TO VARIABLE[rightOperand]			{ $$ = NotExistingDFAAddGrammarAction($resultDFA, $leftOperand, $rightOperand);  }
 	|  VARIABLE[resultDFA] EQUALS ADD addOperand[leftOperand] TO VARIABLE[rightOperand]	 			{ $$ = ExistingDFAAddGrammarAction($resultDFA, $leftOperand, $rightOperand);  }
 	;

addOperand: transition																{ $$ = AddOperandTransitionGrammarAction($1);  }
	| varOrString																	{ $$ = AddOperandVarOrStringGrammarAction($1);  }
	;	


rem: DFA VARIABLE[resultDFA] EQUALS REM varOrString[varOrStr]
	 FROM VARIABLE[from]															{ $$ = NotExistingDFARemGrammarAction($resultDFA, $varOrStr, $from);  }
	| VARIABLE[resultDFA] EQUALS REM varOrString[varOrStr]
	 FROM VARIABLE[from]															{ $$ = ExistingDFARemGrammarAction($resultDFA, $varOrStr, $from);  }
	;
 

complement: DFA VARIABLE[variable] EQUALS NOT VARIABLE[notVariable]  				{ $$ = NotExistingDFAComplementGrammarAction($variable, $notVariable);  }
	| VARIABLE[variable] EQUALS NOT VARIABLE[notVariable] 							{ $$ = ExistingDFAComplementGrammarAction($variable, $notVariable);  }
	;


join: DFA VARIABLE[resultDFA] EQUALS VARIABLE[leftDFA] JOIN VARIABLE[rightDFA] transitionOrVar[tov]				{ $$ = NotExistingDFAJoinGrammarAction($resultDFA, $leftDFA, $rightDFA, $tov);  }
	| VARIABLE[resultDFA] EQUALS VARIABLE[leftDFA] JOIN VARIABLE[rightDFA] transitionOrVar[tov]					{ $$ = ExistingDFAJoinGrammarAction($resultDFA, $leftDFA, $rightDFA, $tov);  }
	;

transitionOrVar: VARIABLE															{ $$ = TOVVariableGrammarAction($1);  }
	| transition																	{ $$ = TOVTransitionGrammarAction($1);  }
	;

print: PRINT STRING 																{ $$ = PrintGrammarAction($2);  }
	;
%%
