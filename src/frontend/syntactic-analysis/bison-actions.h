#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"

/**
 * Se definen las acciones a ejecutar sobre cada regla de producción de la
 * gramática. El objetivo de cada acción debe ser el de construir el nodo
 * adecuado que almacene la información requerida en el árbol de sintaxis
 * abstracta (i.e., el AST).
 */

// Programa.
int ProgramGrammarAction(const int value);

// Expresión.
int AdditionExpressionGrammarAction(const int leftValue, const int rightValue);
int SubtractionExpressionGrammarAction(const int leftValue, const int rightValue);
int MultiplicationExpressionGrammarAction(const int leftValue, const int rightValue);
int DivisionExpressionGrammarAction(const int leftValue, const int rightValue);
int FactorExpressionGrammarAction(const int value);

// Factores.
int ExpressionFactorGrammarAction(const int value);
int ConstantFactorGrammarAction(const int value);

// Constantes.
int IntegerConstantGrammarAction(const int value);

void DummyProgramGrammarAction();
void DummyGrammarAction();

/* ============================== DFA-Factory ================================= */

Program * DfaProgramGrammarAction(DecOrExec * decOrExec);

VarOrString * VariableGrammarAction(Variable * variable);

VarOrString * StringGrammarAction(String * string);

Transition * TransitionGrammarAction(VarOrString * stateFrom, VarOrString* stateTo, VarOrString * symbol);

TransitionArr * TrnArrWithNext(TransitionOrVar * trnOrVar, TransitionArr * next);

TransitionArr * TrnArrNoNext(TransitionOrVar * trnOrVar);

TrnArrValue * TrnArrValueGrammarAction(TransitionArr * trnArr);

Array * ArrayNoNextGrammarAction(VarOrString * varOrStr);

Array * ArrayWithNextGrammarAction(VarOrString * varOrStr, Array * next);

SymOrStaArrValue * SymstaArrValuegGrammarAction(Array * array);

SymOrState * SymbolGrammarAction();

SymOrState * StateGrammarAction();

SymOrStaArr * SymOrStaArrGrammarAction(SymOrState * symOrState);

DfaValue * DfaValueGrammarAction(Variable * states, Variable * symbols, Variable * startState, Variable * endStates, Variable * transitions);

// TODO: agregar prototipos de las DeclarationGrammarActions

AddOperand * AddOperandTransitionGrammarAction(Transition * transition);

AddOperand * AddOperandVarOrStringGrammarAction(VarOrString * varOrString);

TransitionOrVar * TOVVariableGrammarAction(Variable * variable);

TransitionOrVar * TOVTransitionGrammarAction(Transition * transition);

Add * ExistingDFAAddGrammarAction(Variable * resultDFA, AddOperand * leftOperand, Variable * rightOperand);

Add * NotExistingDFAAddGrammarAction(Variable * resultDFA, AddOperand * leftOperand, Variable * rightOperand);

Rem * ExistingDFARemGrammarAction(Variable * resultDFA, VarOrString * varOrStr, Variable * from);

Rem * NotExistingDFARemGrammarAction(Variable * resultDFA, VarOrString * varOrStr, Variable * from);

Check * CheckSymStaArrValueGrammarAction(Variable * variable, SymOrStaArrValue * symOrStaArrValue);

Check * CheckVariableGrammarAction(Variable * variable, Variable * rightVariable);

Complement * ExistingDFAComplementGrammarAction(Variable * variable, Variable * notVariable);

Complement * NotExistingDFAComplementGrammarAction(Variable * variable, Variable * notVariable);

Join * ExistingDFAJoinGrammarAction(Variable * resultDFA, Variable * leftDFA, Variable * rightDFA, TransitionOrVar * tov);

Join * NotExistingDFAJoinGrammarAction(Variable * resultDFA, Variable * leftDFA, Variable * rightDFA, TransitionOrVar * tov);

Print * PrintGrammarAction(String * string);

Exec * ExecAddGrammarAction(Add * add);

Exec * ExecRemGrammarAction(Rem * rem);

Exec * ExecCheckGrammarAction(Check * check); 

Exec * ExecComplementGrammarAction(Complement * complement);

Exec * ExecJoinGrammarAction(Join * join);

Exec * ExecPrintGrammarAction(Print * print); 

DecOrExec * FirstDecGrammarAction(Declaration * dec, DecOrExec * next);

DecOrExec * FirstExecGrammarAction(Exec * exec, DecOrExec * next);

DecOrExec * DecOrExecLamdaGrammarAction();

#endif
