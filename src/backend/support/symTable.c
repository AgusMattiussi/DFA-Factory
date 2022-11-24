#include "symTable.h"

//TODO: Limpiar mallocs despues de ejecutar

symbolTable * initSymbolTable(){
    return calloc(1, sizeof(symbolTable));
}

static void freeEntry(entry * firstEntry){
    if (firstEntry == NULL)
           return;
    freeEntry(firstEntry->next);
    free(firstEntry->variableName);
    free(firstEntry);
}

void freeList(symbolTable * st) {
    freeEntry(st->first);
    free(st);
}

static entry * addRec(entry * first, char * variableName, DataType type, int * flag) {
    int c;
    if ( first==NULL || ( c = strcmp(first->variableName, variableName)) > 0) {
        entry * aux = malloc(sizeof(entry));
        aux->variableName = variableName;
        aux->dataType = type;
        aux->value = NULL;
        aux->next = first;
        *flag = 1;
        return aux;
    }
    if ( c < 0) {
        first->next = addRec(first->next, variableName, type, flag);
    }
    return first;
}


int addEntry(symbolTable * st, char * variableName, DataType type) {
    int added = 0;
    st->first = addRec(st->first, variableName, type, &added);
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

bool exists(symbolTable * st, char * variableName, DataType type){
    entry * found = getEntry(st, variableName);
    return found != NULL && found->dataType == type;
}

void setEntryValue(entry * entry, void * value){
    entry->value = value;
} 

int setValue(symbolTable * st, char * variableName, void * value){
    entry * found = getEntry(st, variableName);
    if(found == NULL)
        return -1;
    //FIXME: por alguna razon found->value != NULL no funciona cuando el value es NULL
    // if (found->dataType != DFA_DT && found->value != NULL);
    // {
    //     return -1;
    // }
    
    found->value = value;
    return 0;
}

static char * getType(DataType type){
    switch (type){
    case DFA_DT:
        return "dfa";
    case TRANSITION_DT:
        return "transition";
    case STATE_DT:
        return "state";
    case SYMBOL_DT:
        return "symbol";
    case SYM_ARRAY_DT:
        return "sym_array";
    case STATE_ARRAY_DT:
        return "state_array";
    case TRN_ARRAY_DT:
        return "trn_array";
    default:
        return "unknown";
    }
}

void printTable(symbolTable * st){
    printf("|\tVariable\t | \t  Type  \t|\n");
    printf("|-----------------------------------------------|\n");

    entry * aux = st->first;
    for (int i = 0; i < st->size; i++){
        printf("|\t%12s\t | \t%12s\t|\n", aux->variableName, getType(aux->dataType));
        aux = aux->next;
    }

}

