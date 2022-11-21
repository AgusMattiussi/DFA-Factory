#include "../support/logger.h"
#include "generator.h"


/**
 * Implementación de "generator.h".
 */
static symbolTable * st;

/* typedef enum DataType {
    DFA_DT,
    TRANSITION_DT,
    STATE_DT,
    SYMBOL_DT,
    SYM_ARRAY_DT,
    STATE_ARRAY_DT,
    TRN_ARRAY_DT
} DataType; */

int Generator(Program * program, symbolTable * symbolTable) {
	st = symbolTable;
	int toReturn = 0;
	DecOrExec * current = program->decOrExec;
	while(current != NULL && toReturn == 0){
		LogDebug(" En el while\n");
		toReturn = DecOrExecCode(current);
		current = current->next;
	}
	LogDebug("%d\n", toReturn);
	return toReturn;
}

int DecOrExecCode(DecOrExec * decOrExec){
	LogDebug("DecOrExecCode\n");
	if(decOrExec->type == DEC_DOET)
		return DecCode(decOrExec->declaration);
	else	// type == EXEC_DOET
		return ExecCode(decOrExec->exec);
}


int DecCode(Declaration * dec){
	LogDebug("DecCode\n");
	switch (dec->type){
		case DFA_DVT:
			return DfaValueCode(dec->variable, dec->dfa);
			break;
		case TRANS_DVT:
			return TransitionCode(dec->variable, dec->transition);
			break;
		case TRN_ARRAY_DVT:
			return TrnArrayCode(dec->variable, dec->trnArrayValue);
			break;
		case SYMSTA_ARR_DVT:
			return SymStaArrayCode(dec->variable, dec->symOrStaArr, dec->symOrStaArrValue);
			break;
		case STRING_DVT:
			return SymStaCode(dec->variable, dec->symOrState, dec->symOrStateValue);
			break;
	}

	return 0;
}

static bool validateDfaVariables(DfaValue * dfaValue){
	if(!exists(st, dfaValue->initial->value, STATE_DT))
		return false;
	if(!exists(st, dfaValue->states->value, STATE_ARRAY_DT))
		return false;
	if(!exists(st, dfaValue->finalStates->value, STATE_ARRAY_DT))
		return false;
	if(!exists(st, dfaValue->symbols->value, SYM_ARRAY_DT))
		return false;
	if(!exists(st, dfaValue->transitions->value, TRN_ARRAY_DT))
		return false;
	return true;
}

int DfaValueCode(Variable * variable, DfaValue * dfaValue){
	if (!validateDfaVariables(dfaValue)) {
		return -1;
	}

	// TODO:
	return 0;
}


static char * getTransitionParam(VarOrString * vos, DataType type){
	LogDebug("getTransitionParam\n");
	if(vos->type == VAR_VOST){
		entry * stateFrom = getEntry(st, vos->variable->value);
		if(stateFrom == NULL || stateFrom->dataType != type){
			LogDebug(" NO ENCONTRE LA ENTRY\n");
			return NULL;
		}
		return (char *) stateFrom->value;
	} else {
		return vos->string->value;
	}
}

int TransitionCode(Variable * variable, Transition * trnValue){
	LogDebug("TransitionCode\n");
	TransitionValue * value = malloc(sizeof(TransitionValue));

	value->stateFrom = getTransitionParam(trnValue->stateFrom, STATE_DT);
	if(value->stateFrom == NULL){
		LogDebug("FALLE BIEN\n");
		return -1;
	}

	value->stateTo = getTransitionParam(trnValue->stateTo, STATE_DT);
	if(value->stateTo == NULL)
		return -1;

	value->symbol = getTransitionParam(trnValue->symbol, SYMBOL_DT);
	if(value->symbol == NULL)
		return -1;

	return setValue(st, variable->value, value);
}

int TrnArrayCode(Variable * variable, TrnArrValue * trnArrValue){
	return 0;
}

int SymStaArrayCode(Variable * variable, SymOrStaArr * symOrStaArr, SymOrStaArrValue * symOrStaArrValue) {
	return 0;
}

int SymStaCode(Variable * variable, SymOrState * symOrSta, String * symOrStaValue) {
	return setValue(st, variable->value, symOrStaValue->value);
}

/* typedef enum  {
	DFA_DVT,
	TRANS_DVT,
	TRN_ARRAY_DVT,
	SYMSTA_ARR_DVT,
	STRING_DVT
} DecValueType;
 */
int ExecCode(Exec * exec) {
	return 0;
}