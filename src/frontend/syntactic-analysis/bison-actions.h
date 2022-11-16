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

SymOrState * SymbolGrammarAction();

SymOrState * StateGrammarAction();

TransitionOrVar * TOVVariableGrammarAction(Variable * variable);

TransitionOrVar * TOVTransitionGrammarAction(Transition * transition);

VarOrString * VariableGrammarAction(Variable * variable);

VarOrString * StringGrammarAction(String * string);

AddOperand * AddOperandTransitionGrammarAction(Transition * transition);

AddOperand * AddOperandVarOrStringGrammarAction(VarOrString * varOrString);

Exec * ExecAddGrammarAction(Add * add);

Exec * ExecRemGrammarAction(Rem * rem);

Exec * ExecCheckGrammarAction(Check * check); 

Exec * ExecComplementGrammarAction(Complement * complement);

Exec * ExecJoinGrammarAction(Join * join);

Exec * ExecPrintGrammarAction(Print * print); 

Add * ExistingDFAAddGrammarAction(Variable * resultDFA, AddOperand * leftOperand, Variable * rightOperand);

Add * NotExistingDFAAddGrammarAction(Variable * resultDFA, AddOperand * leftOperand, Variable * rightOperand);

Rem * ExistingDFARemGrammarAction(Variable * resultDFA, VarOrString * varOrStr, Variable * from);

Rem * ExistingDFARemGrammarAction(Variable * resultDFA, VarOrString * varOrStr, Variable * from);

Check * CheckSymStaArrValueGrammarAction(Variable * variable, SymOrStaArrValue * symOrStaArrValue);

Check * CheckVariableGrammarAction(Variable * variable, Variable * rightVariable);

Complement * ExistingDFAComplementGrammarAction(Variable * variable, Variable * notVariable);

Complement * NotExistingDFAComplementGrammarAction(Variable * variable, Variable * notVariable);

Join * ExistingDFAJoinGrammarAction(Variable * resultDFA, Variable * leftDFA, Variable * rightDFA, TransitionOrVar * tov);

Join * NotExistingDFAJoinGrammarAction(Variable * resultDFA, Variable * leftDFA, Variable * rightDFA, TransitionOrVar * tov);

Print * PrintGrammarAction(String * string);

DfaValue * DfaValueGrammarAction(Variable * states, Variable * symbols, Variable * startState, Variable * endStates, Variable * transitions);

SymOrStaArrValue * SymstaArrValuegGrammarAction(Array * array);

#endif
