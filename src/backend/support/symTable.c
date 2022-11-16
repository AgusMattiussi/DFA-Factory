#include "symTable.h"

symbolTable * initSymbolTable(){
    return calloc(1, sizeof(symbolTable));
}

static void freeEntry(entry * firstEntry){
    if (firstEntry == NULL)
           return;
    freeEntry(firstEntry->next);
    free(firstEntry);
}

void freeList(symbolTable * st) {
    freeEntry(st->first);
    free(st);
}

static entry * addRec(entry * first, char * variableName, int * flag) {
    int c;
    if ( first==NULL || ( c = strcmp(first->variableName, variableName)) > 0) {
        entry * aux = malloc(sizeof(entry));
        aux->variableName = variableName;
        aux->next = first;
        *flag = 1;
        return aux;
    }
    if ( c < 0) {
        first->next = addRec(first->next, variableName, flag);
    }
    return first;
}


int addEntry(symbolTable * st, char * variableName, DataType type) {
    int added = 0;
    st->first = addRec(st->first, variableName, &added);
    st->size += added;
    return added;
}


static entry * getEntryRec(entry * first, char * variableName){
    int c;
    if (first == NULL || (c = strcmp(first->variableName, variableName)) > 0 )
        return NULL;
    if(c == 0)
        return first;
    return getEntryRec(first->next, variableName);
}

entry * getEntry(symbolTable * st, char * variableName) {
    return getEntryRec(st->first, variableName);
}

