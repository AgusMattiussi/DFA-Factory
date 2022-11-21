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

static bool validateDfaVariables(DfaValue *dfaValue)
{
	if (!exists(st, dfaValue->initial->value, STATE_DT))
	{
		LogDebug("%s no es un state",  dfaValue->initial->value);
		return false;
	}
	if (!exists(st, dfaValue->states->value, STATE_ARRAY_DT))
	{
		LogDebug("%s no es un states array", dfaValue->states->value);
		return false;
	}
	if (!exists(st, dfaValue->finalStates->value, STATE_ARRAY_DT))
	{
		LogDebug("%s no es un states array", dfaValue->finalStates->value);
		return false;
	}
	if (!exists(st, dfaValue->symbols->value, SYM_ARRAY_DT))
	{
		LogDebug("%s no es un sym array", dfaValue->symbols->value);
		return false;
	}
	if (!exists(st, dfaValue->transitions->value, TRN_ARRAY_DT))
	{
		LogDebug("%s no es un trn array", dfaValue->transitions->value);
		return false;
	}
	return true;
}

int DfaValueCode(Variable * variable, DfaValue * dfaValue){
	LogDebug("DfaValueCode\n");
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
	if(value->stateFrom == NULL)
		return -1;
		
	value->stateTo = getTransitionParam(trnValue->stateTo, STATE_DT);
	if(value->stateTo == NULL)
		return -1;

	value->symbol = getTransitionParam(trnValue->symbol, SYMBOL_DT);
	if(value->symbol == NULL)
		return -1;

	return setValue(st, variable->value, value);
}

TrnArrayValue * AddTransitionArrayValue(TrnArrayValue * arr, TransitionValue * value) {
	if (arr == NULL)
	{
		TrnArrayValue *aux = malloc(sizeof(TrnArrayValue));
		aux->value = value;
		aux->next = NULL;
		return aux;
	}
	else
	{
		arr->next = AddTransitionArrayValue(arr->next, value);
	}
	return arr;
}

int TrnArrayCode(Variable *variable, TrnArrValue *trnArrValue){
	LogDebug("TrnArrayCode\n");
	TrnArrayValue * value = NULL;
	TransitionArr *current = trnArrValue->transitionArr;
	entry *entry;
	TrnArrayValue *aux = value;
	while (current != NULL)
	{
		if (current->transitionOrVar->type == VAR_TOVT)
		{
			if (!exists(st, current->transitionOrVar->variable->value, TRANSITION_DT))
			{
				return -1;
			}
			entry = getEntry(st, current->transitionOrVar->variable->value);
			aux = AddTransitionArrayValue(aux, (TransitionValue *)entry->value);
		}
		else
		{
			TransitionValue * auxTrn = malloc(sizeof(TransitionValue));

			auxTrn->stateFrom = getTransitionParam(current->transitionOrVar->transition->stateFrom, STATE_DT);
			if (auxTrn->stateFrom == NULL)
				return -1;

			auxTrn->stateTo = getTransitionParam(current->transitionOrVar->transition->stateTo, STATE_DT);
			if (auxTrn->stateTo == NULL)
				return -1;

			auxTrn->symbol = getTransitionParam(current->transitionOrVar->transition->symbol, SYMBOL_DT);
			if (auxTrn->symbol == NULL)
				return -1;

			aux = AddTransitionArrayValue(aux,auxTrn);
		}
		aux = aux->next;
		current = current->next;
	}
	return setValue(st, variable->value, value);
}

static ArrayValue *AddArrayValue(ArrayValue *arr, char *value)
{
	if (arr == NULL)
	{
		ArrayValue *aux = malloc(sizeof(ArrayValue));
		aux->value = calloc(strlen(value), sizeof(char));
		strcpy(aux->value, value);
		aux->next = NULL;
		return aux;
	}
	else
	{
		arr->next = AddArrayValue(arr->next, value);
	}
	return arr;
}

int SymStaArrayCode(Variable * variable, SymOrStaArr * symOrStaArr, SymOrStaArrValue * symOrStaArrValue) {
	LogDebug("SymStaArrayCode\n");
	ArrayValue * value = NULL;
	Array * current = symOrStaArrValue->array;
	entry * entry;
	ArrayValue * aux = value;
	while (current != NULL)
	{
		if (current->VarOrString->type == VAR_VOST)
		{
			if (symOrStaArr->symOrState->type == SYM_SOST)
			{
				if (!exists(st, current->VarOrString->variable->value, SYMBOL_DT))
				{
					return -1;
				}
			}
			else
			{
				if (!exists(st, current->VarOrString->variable->value, STATE_DT))
				{
					return -1;
				}
			}
			entry = getEntry(st,current->VarOrString->variable->value);
			aux = AddArrayValue(aux, (char *)entry->value );
		}
		else
		{
			aux = AddArrayValue(aux, current->VarOrString->string->value);
		}
		aux = aux->next;
		current = current->next;
	}	
	return setValue(st,variable->value,value);
}

int SymStaCode(Variable * variable, SymOrState * symOrSta, String * symOrStaValue) {
	LogDebug("SymStaCode\n");
	return setValue(st, variable->value, symOrStaValue->value);
}

int ExecCode(Exec * exec) {
	return 0;
}