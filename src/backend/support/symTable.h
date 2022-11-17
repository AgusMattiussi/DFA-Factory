#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include <stdlib.h>
#include <string.h>

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
    //TODO: Valor de inicializacion?    
    struct entry * next;
} entry;

typedef struct symbolTable {
    int size;
    entry * first;
} symbolTable;

symbolTable * initSymbolTable();
void freeList(symbolTable * st);
int addEntry(symbolTable * st, char * variableName, DataType type);
entry * getEntry(symbolTable * st, char * variableName);

#endif