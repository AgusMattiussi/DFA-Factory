#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"

//FIXME: Valor de 1024 esta bien?
// typedef char state[1024];
// typedef char symbol[1024];

// struct trn {
//     state fromState;
//     state toState;
//     symbol consumed;
// };

// //FIXME: valor 100 esta bien?
// struct symArray{
//     int size;
//     symbol symbols[100];
// };

// struct staArray{
//     int size;
//     state states[100];
// };

// struct trnArray{
//     int size;
//     struct trn transitions[100];
// };

// //FIXME: Punteros?
// struct dfa{
//     struct staArray states;
//     struct symArray alphabet;
//     state initialState;
//     struct staArray finalStates;
//     struct trnArray delta;
// };

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

#endif
