#include "../../backend/domain-specific/calculator.h"
#include "../../backend/support/logger.h"
#include "bison-actions.h"
#include <stdio.h>
#include <string.h>

/**
 * Implementación de "bison-grammar.h".
 */

/**
* Esta función se ejecuta cada vez que se emite un error de sintaxis.
*/
void yyerror(const char * string) {
	LogError("Mensaje: '%s' debido a '%s' (linea %d).", string, yytext, yylineno);
	LogError("En ASCII es:");
	LogErrorRaw("\t");
	const int length = strlen(yytext);
	for (int i = 0; i < length; ++i) {
		LogErrorRaw("[%d]", yytext[i]);
	}
	LogErrorRaw("\n\n");
}

/**
* Esta acción se corresponde con el no-terminal que representa el símbolo
* inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
* indica que efectivamente el programa de entrada se pudo generar con esta
* gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
*/
int ProgramGrammarAction(const int value) {
	LogDebug("\tProgramGrammarAction(%d)", value);
	/*
	* "state" es una variable global que almacena el estado del compilador,
	* cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
	* es utilizada en la función "main".
	*/
	state.succeed = true;
	/*
	* Por otro lado, "result" contiene el resultado de aplicar el análisis
	* sintáctico mediante Bison, y almacenar el nood raíz del AST construido
	* en esta variable. Para el ejemplo de la calculadora, no hay AST porque
	* la expresión se computa on-the-fly, y es la razón por la cual esta
	* variable es un simple entero, en lugar de un nodo.
	*/
	state.result = value;
	return value;
}

Program * DfaProgramGrammarAction(DecOrExec * decOrExec) {
	LogDebug("\tDfaProgramGrammarAction");
	Program * ret = malloc(sizeof(Program));
	ret->decOrExec = decOrExec;

	state.succeed = true;
	state.program = ret;
	return ret;
}

void DummyProgramGrammarAction(){
	state.succeed = true;
	return;
}

void DummyGrammarAction(){
	return;
}

static void addVariable(Variable * name, DataType type){
	LogDebug("Tratando de agregar una variable (%s)\n", name->value);
	addEntry(state.symbolTable, name->value, type);
}

VarOrString * VariableGrammarAction(Variable * variable) {
	LogDebug("\tVariableGrammarAction: %s", variable->value);
	VarOrString * ret = malloc(sizeof(VarOrString));
	ret->type = VAR_VOST;
	ret->variable = variable;
	return ret;
}

VarOrString * StringGrammarAction(String * string) {
	LogDebug("\tStringGrammarAction: %s", string->value);
	VarOrString * ret = malloc(sizeof(VarOrString));
	ret->type = STRING_VOST;
	ret->string = string;
	return ret;
}

Transition * TransitionGrammarAction(VarOrString * stateFrom, VarOrString* stateTo, VarOrString * symbol) {
	LogDebug("\tTransitionGrammarAction");
	Transition * ret = malloc(sizeof(Transition));
	ret->stateFrom = stateFrom;
	ret->stateTo = stateTo;
	ret->symbol = symbol; 
	return ret;
}

TransitionArr * TrnArrNoNext(TransitionOrVar * trnOrVar) {
	LogDebug("\tTrnArrNoNext");
	TransitionArr * ret = malloc(sizeof(TransitionArr));
	ret->transitionOrVar = trnOrVar; 
	ret->next = NULL;
	return ret;
}

TransitionArr * TrnArrWithNext(TransitionOrVar * trnOrVar, TransitionArr * next) {
	LogDebug("\tTrnArrWithNext");
	TransitionArr * ret = malloc(sizeof(TransitionArr));
	ret->transitionOrVar = trnOrVar; 
	ret->next = next;
	return ret;
}

TrnArrValue * TrnArrValueGrammarAction(TransitionArr * trnArr) {
	LogDebug("\tTrnArrValueGrammarAction");
	TrnArrValue * ret = malloc(sizeof(TrnArrValue));
	ret->transitionArr = trnArr; 
	return ret;
}

Array * ArrayNoNextGrammarAction(VarOrString * varOrStr) {
	LogDebug("\tArrayNoNextGrammarAction");
	Array * ret = malloc(sizeof(Array));
	ret->VarOrString = varOrStr;
	ret->next = NULL; 
	return ret;
}

Array * ArrayWithNextGrammarAction(VarOrString * varOrStr, Array * next) {
	LogDebug("\tArrayWithNextGrammarAction");
	Array * ret = malloc(sizeof(Array));
	ret->VarOrString = varOrStr;
	ret->next = next; 
	return ret;
}

SymOrStaArrValue * SymstaArrValuegGrammarAction(Array * array) {
	LogDebug("\tSymstaArrValuegGrammarAction");
	SymOrStaArrValue * ret = malloc(sizeof(SymOrStaArrValue));
	ret->array = array;
	return ret;
}

SymOrState * SymbolGrammarAction(){
	LogDebug("\tSymbolGrammarAction");
	SymOrState * ret = malloc(sizeof(SymOrState));
	ret->type = SYM_SOST;
	return ret;
}

SymOrState * StateGrammarAction() {
	LogDebug("\tStateGrammarAction");
	SymOrState * ret = malloc(sizeof(SymOrState));
	ret->type = STATE_SOST;
	return ret;
}

SymOrStaArr * SymOrStaArrGrammarAction(SymOrState * symOrState) {
	LogDebug("\tSymOrStaArrGrammarAction");
	SymOrStaArr * ret = malloc(sizeof(SymOrStaArr));
	ret->symOrState = symOrState;
	return ret;	
}

DfaValue * DfaValueGrammarAction(Variable * states, Variable * symbols, Variable * startState, Variable * endStates, Variable * transitions) {
	LogDebug("\tDfaValueGrammarAction");
	DfaValue * ret = malloc(sizeof(DfaValue));
	ret->states = states;
	ret->symbols = symbols;
	ret->initial = startState;
	ret->finalStates = endStates;
	ret->transitions = transitions;
	return ret;
}

Declaration * DecSymOrStaGrammarAction(SymOrState * symOrState, Variable * variable, String * value){
	LogDebug("\tDecSymOrStaGrammarAction");
	Declaration * ret = malloc(sizeof(Declaration));
	ret->type = STRING_DVT;
	ret->variable = variable;
	ret->symOrState = symOrState;
	ret->symOrStateValue = value;

	DataType type = symOrState->type == SYM_SOST ? SYMBOL_DT : STATE_DT;
	addVariable(variable, type);

	return ret;
}

Declaration * DecTransitionGrammarAction(Variable * variable, Transition * value){
	LogDebug("\tDecTransitionGrammarAction");
	Declaration * ret = malloc(sizeof(Declaration));
	ret->type = TRANS_DVT;
	ret->variable = variable;
	ret->transition = value;

	addVariable(variable, TRANSITION_DT);
	return ret;
}

Declaration * DecSymOrStaArrGrammarAction(SymOrStaArr * symOrStaArr, Variable * variable, SymOrStaArrValue * value){
	LogDebug("\tDecSymOrStaArrGrammarActionv");
	Declaration * ret = malloc(sizeof(Declaration));
	ret->type = SYMSTA_ARR_DVT;
	ret->variable = variable;
	ret->symOrStaArr = symOrStaArr;
	ret->symOrStaArrValue = value;

	DataType type = symOrStaArr->symOrState->type == SYM_SOST ? SYM_ARRAY_DT : STATE_ARRAY_DT;
	addVariable(variable, type);
	return ret;
}

Declaration * DecTransitionArrGrammarAction(Variable * variable, TrnArrValue * value){
	LogDebug("\tDecTransitionArrGrammarAction");
	Declaration * ret = malloc(sizeof(Declaration));
	ret->type = TRN_ARRAY_DVT;
	ret->variable = variable;
	ret->trnArrayValue = value;

	addVariable(variable, TRN_ARRAY_DT);
	return ret;
}

Declaration * DecDfaGrammarAction(Variable * variable, DfaValue * value){
	LogDebug("\tDecDfaGrammarAction");
	Declaration * ret = malloc(sizeof(Declaration));
	ret->type = DFA_DVT;
	ret->variable = variable;
	ret->dfa = value;

	addVariable(variable, DFA_DT);
	return ret;
} 

AddOperand * AddOperandTransitionGrammarAction(Transition * transition) {
	LogDebug("\tAddOperandTransitionGrammarAction");
	AddOperand * ret = malloc(sizeof(AddOperand));
	ret->type = TRANSITION_TVOST;
	ret->transition = transition;
	return ret;
}

AddOperand * AddOperandVarOrStringGrammarAction(VarOrString * varOrString) {
	LogDebug("\tAddOperandVarOrStringGrammarAction");
	AddOperand * ret = malloc(sizeof(AddOperand));
	ret->type = VAR_OR_STRING_TVOST;
	ret->varOrString = varOrString;
	return ret;
}

TransitionOrVar * TOVVariableGrammarAction(Variable * variable){
	LogDebug("\tTOVVariableGrammarAction");
	TransitionOrVar * ret = malloc(sizeof(TransitionOrVar));
	ret->type = VAR_TOVT;
	ret->variable = variable;
	return ret;
}


TransitionOrVar * TOVTransitionGrammarAction(Transition * transition){
	LogDebug("\tTOVTransitionGrammarAction");
	TransitionOrVar * ret = malloc(sizeof(TransitionOrVar));
	ret->type = TRANSITION_TOVT;
	ret->transition = transition;
	return ret;
}

Add * ExistingDFAAddGrammarAction(Variable * resultDFA, AddOperand * leftOperand, Variable * rightOperand) {
	LogDebug("\tExistingDFAAddGrammarAction");
	Add * ret = malloc(sizeof(Add));
	ret->variable = resultDFA;
	ret->leftOperand = leftOperand;
	ret->rightOperand = rightOperand;
	return ret;
}

Add * NotExistingDFAAddGrammarAction(Variable * resultDFA, AddOperand * leftOperand, Variable * rightOperand) {
	LogDebug("\tNotExistingDFAAddGrammarAction");
	addVariable(resultDFA, DFA_DT);
	return ExistingDFAAddGrammarAction(resultDFA, leftOperand, rightOperand);
}

Rem * ExistingDFARemGrammarAction(Variable * resultDFA, VarOrString * varOrStr, Variable * from) {
	LogDebug("\tExistingDFARemGrammarAction");
	Rem * ret = malloc(sizeof(Rem));
	ret->variable = resultDFA;
	ret->varOrString = varOrStr;
	ret->from = from;
	return ret;
}

Rem * NotExistingDFARemGrammarAction(Variable * resultDFA, VarOrString * varOrStr, Variable * from) {
	LogDebug("\tNotExistingDFARemGrammarAction");
	addVariable(resultDFA, DFA_DT);
	return ExistingDFARemGrammarAction(resultDFA, varOrStr, from);
}

Check * CheckSymStaArrValueGrammarAction(Variable * variable, SymOrStaArrValue * symOrStaArrValue){
	LogDebug("\tCheckSymStaArrValueGrammarAction");
	Check * ret = malloc(sizeof(Check));
	ret->type = SYM_STATE_ARR_SSAOVT;
	ret->variable = variable;
	ret->symOrStaArrValue = symOrStaArrValue;
	return ret;
}

Check * CheckVariableGrammarAction(Variable * variable, Variable * rightVariable){
	LogDebug("\tCheckVariableGrammarAction");
	Check * ret = malloc(sizeof(Check));
	ret->type = VAR_SSAOVT;
	ret->variable = variable;
	ret->rVariable = rightVariable;
	return ret;
}

Complement * ExistingDFAComplementGrammarAction(Variable * variable, Variable * notVariable) {
	LogDebug("\tExistingDFAComplementGrammarAction");
	Complement * ret = malloc(sizeof(Complement));
	ret->variable = variable;
	ret->notVariable = notVariable;
	return ret;
}

Complement * NotExistingDFAComplementGrammarAction(Variable * variable, Variable * notVariable) {
	LogDebug("\tNotExistingDFAComplementGrammarAction");
	addVariable(variable, DFA_DT);
	return ExistingDFAComplementGrammarAction(variable, notVariable);
}

Join * ExistingDFAJoinGrammarAction(Variable * resultDFA, Variable * leftDFA, Variable * rightDFA, TransitionOrVar * tov) {
	LogDebug("\tExistingDFAJoinGrammarAction");
	Join * ret = malloc(sizeof(Join));
	ret->dfaVariable = resultDFA;
	ret->leftOperand = leftDFA;
	ret->rightOperand = rightDFA;
	ret->transitionOrVar = tov;
	return ret;
}

Join * NotExistingDFAJoinGrammarAction(Variable * resultDFA, Variable * leftDFA, Variable * rightDFA, TransitionOrVar * tov) {
	LogDebug("\tNotExistingDFAJoinGrammarAction");
	addVariable(resultDFA, DFA_DT);
	return ExistingDFAJoinGrammarAction(resultDFA, leftDFA, rightDFA, tov);
}

Print * PrintGrammarAction(String * string){
	LogDebug("\tPrintGrammarAction");
	Print * ret = malloc(sizeof(Print));
	ret->string = string;
	return ret;
}

Exec * ExecAddGrammarAction(Add * add) {
	LogDebug("\tExecAddGrammarAction");
	Exec * ret = malloc(sizeof(Exec));
	ret->type = ADD_EVT;
	ret->add = add;
	return ret;
}

Exec * ExecRemGrammarAction(Rem * rem) {
	LogDebug("\tExecRemGrammarAction");
	Exec * ret = malloc(sizeof(Exec));
	ret->type = REM_EVT;
	ret->rem = rem;
	return ret;
}

Exec * ExecCheckGrammarAction(Check * check) {
	LogDebug("\tExecCheckGrammarAction");
	Exec * ret = malloc(sizeof(Exec));
	ret->type = CHECK_EVT;
	ret->check = check;
	return ret;
}

Exec * ExecComplementGrammarAction(Complement * complement) {
	LogDebug("\tExecComplementGrammarAction");
	Exec * ret = malloc(sizeof(Exec));
	ret->type = COMPLEMENT_EVT;
	ret->complement = complement;
	return ret;
}

Exec * ExecJoinGrammarAction(Join * join) {
	LogDebug("\tExecJoinGrammarAction");
	Exec * ret = malloc(sizeof(Exec));
	ret->type = JOIN_EVT;
	ret->join = join;
	return ret;
}

Exec * ExecPrintGrammarAction(Print * print) {
	LogDebug("\tExecPrintGrammarAction");
	Exec * ret = malloc(sizeof(Exec));
	ret->type = PRINT_EVT;
	ret->print = print;
	return ret;
}

DecOrExec * FirstDecGrammarAction(Declaration * dec, DecOrExec * next) {
	LogDebug("\tFirstDecGrammarAction");
	DecOrExec * ret = malloc(sizeof(DecOrExec));
	ret->declaration = dec;
	ret->type = DEC_DOET;
	ret->next = next;
	return ret;
}

DecOrExec * FirstExecGrammarAction(Exec * exec, DecOrExec * next) {
	LogDebug("\tFirstExecGrammarAction");
	DecOrExec * ret = malloc(sizeof(DecOrExec));
	ret->exec = exec;
	ret->type = EXEC_DOET;
	ret->next = next;
	return ret;
}

DecOrExec * DecOrExecLamdaGrammarAction() {
	LogDebug("\tDecOrExecLamdaGrammarAction");
	return NULL;
}