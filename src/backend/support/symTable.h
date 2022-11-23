#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "logger.h"


typedef enum DataType {
    DFA_DT,
    TRANSITION_DT,
    STATE_DT,
    SYMBOL_DT,
    SYM_ARRAY_DT,
    STATE_ARRAY_DT,
    TRN_ARRAY_DT
} DataType;

typedef struct entry {
    char * variableName;
    DataType dataType;
    void * value;  
    struct entry * next;
} entry;

typedef struct symbolTable {
    int size;
    entry * first;
} symbolTable;

typedef struct TransitionValue {
	char * stateFrom;
	char * stateTo;
	char * symbol;
} TransitionValue;

typedef struct ArrayValue {
	char * value;
	struct ArrayValue * next;
} ArrayValue;

typedef struct TrnArrayValue {
	TransitionValue * value;
	struct TrnArrayValue * next;
} TrnArrayValue;

typedef struct automata {
	ArrayValue * symbols;
	ArrayValue * states;
	ArrayValue * finalStates;
	TrnArrayValue * transitions;
	size_t startIdx;
	size_t currentStateIdx;
	size_t symCount;
	size_t staCount;
	size_t ** delta;
} automata;

symbolTable * initSymbolTable();
void freeList(symbolTable * st);
int addEntry(symbolTable * st, char * variableName, DataType type);
entry * getEntry(symbolTable * st, char * variableName);
void printTable(symbolTable * st);
bool exists(symbolTable * st, char * variableName, DataType type);
int setValue(symbolTable * st, char * variableName, void * value);
void setEntryValue(entry * entry, void * value);
#endif