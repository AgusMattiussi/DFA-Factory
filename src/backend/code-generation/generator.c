#include "../support/logger.h"
#include "generator.h"


/**
 * Implementación de "generator.h".
 */
static symbolTable * st;

int Generator(Program * program, symbolTable * symbolTable) {
	st = symbolTable;
	int toReturn = 0;
	DecOrExec * current = program->decOrExec;
	while(current != NULL && toReturn == 0){
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

	return -1;
}

static bool validateDfaVariables(DfaValue *dfaValue) {
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


/**
 * 1) Recorro y cuento simbolos -> asigno symcount
 * 2) Recorro y cuento estados -> asigno stacount y startidx
 * 3) inicializo delta de symCount*staCount
 * 4) Recorro transiciones asigno delta[stateFrom][sym] = stateTo
 * 
**/

static int getIndex(ArrayValue * first, char * value){
	int index = 0;
	ArrayValue * aux = first;
	while(aux != NULL) {
		if(strcmp(aux->value, value) == 0){
			return index;
		}
		index++;
		aux = aux->next;
	}
	return index;
}

// TODO: LIBERAR TODO!!!!!!!
// TODO: Despues de los CHECKS resetear valores del DFA
int DfaValueCode(Variable * variable, DfaValue * dfaValue){
	LogDebug("DfaValueCode\n");
	if (!validateDfaVariables(dfaValue)) {
		return -1;
	}

	/* Inicializa variables en 0 */
	automata * this = calloc(1, sizeof(automata)); 

	/* Inicializamos las variables */
	entry * symbolsEntry = getEntry(st, dfaValue->symbols->value);
	this->symbols = (ArrayValue *) symbolsEntry->value;
	entry * statesEntry = getEntry(st, dfaValue->states->value);
	this->states = (ArrayValue *) statesEntry->value;
	entry * transitionsEntry = getEntry(st, dfaValue->transitions->value);
	this->transitions = (TrnArrayValue *) transitionsEntry->value;

	ArrayValue * currSym = this->symbols;
	while (currSym != NULL){
		this->symCount++;
		currSym = currSym->next;
	}

	ArrayValue * currState = this->states;
	entry * initial = getEntry(st, dfaValue->initial->value);
	while (currState != NULL){
		/* Indice del estado inicial*/
		if(strcmp(currState->value, (char *) initial->value)){
			this->startIdx = this->staCount;
		}
		this->staCount++;
		currState = currState->next;
	}
	/* Siempre empieza en el estado inicial */
	this->currentStateIdx = this->startIdx;

	this->delta = malloc(this->staCount * sizeof(size_t*));
	for (int i = 0; i < this->staCount; i++)
        this->delta[i] = malloc(this->symCount * sizeof(size_t));

	/* Usamos la cantidad de estados + 1 (un numero de estado inalcanzable) 
		para saber si esta posicion del array "esta vacia" */
	for (int i = 0; i < this->staCount; i++){
		for (int j = 0; j < this->symCount; j++) {
			this->delta[i][j] = this->staCount + 1;
		}		
	}
	
	TrnArrayValue * currentTrn = this->transitions;

	while (currentTrn != NULL) {
		
		int idxStaFrom = getIndex(this->states, currentTrn->value->stateFrom);

		int idxStaTo = getIndex(this->states, currentTrn->value->stateTo);

		int idxSym = getIndex(this->symbols, currentTrn->value->symbol);
		
		if (this->delta[idxStaFrom][idxSym] == this->staCount + 1){
			this->delta[idxStaFrom][idxSym] = idxStaTo;
		}
		else {
			return -1; //TODO: codigo error no es DFA
		}
		currentTrn = currentTrn->next;
	}
	
	return setValue(st, variable->value, this);
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

static void AddTransitionArrayValue(TrnArrayValue ** head, TransitionValue * value) {
	TrnArrayValue * newNode = malloc(sizeof(TrnArrayValue));
	newNode->value = value;
	newNode->next = NULL;
	
	if (*head == NULL) {
		*head = newNode;
	}
	else {
		TrnArrayValue *lastNode = *head;

        while(lastNode->next != NULL) {
            lastNode = lastNode->next;
        }

        lastNode->next = newNode;
	}
}

int TrnArrayCode(Variable *variable, TrnArrValue *trnArrValue){
	LogDebug("TrnArrayCode\n");
	TrnArrayValue * value = NULL;
	TransitionArr *current = trnArrValue->transitionArr;
	entry *entry;
	while (current != NULL)
	{
		if (current->transitionOrVar->type == VAR_TOVT)
		{
			if (!exists(st, current->transitionOrVar->variable->value, TRANSITION_DT))
			{
				return -1;
			}
			entry = getEntry(st, current->transitionOrVar->variable->value);
			AddTransitionArrayValue(&value, (TransitionValue *)entry->value);
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

			AddTransitionArrayValue(&value,auxTrn);
		}
		current = current->next;
	}

	return setValue(st, variable->value, value);
}

static void AddArrayValue(ArrayValue ** head, char *value)
{
	ArrayValue * newNode = malloc(sizeof(ArrayValue));
	newNode->value = calloc(strlen(value)+1, sizeof(char));
	strcpy(newNode->value, value);
	newNode->next = NULL;
	
	if (*head == NULL) {
		*head = newNode;
	}
	else {
		ArrayValue *lastNode = *head;

        while(lastNode->next != NULL) {
            lastNode = lastNode->next;
        }

        lastNode->next = newNode;
	}
}

int SymStaArrayCode(Variable * variable, SymOrStaArr * symOrStaArr, SymOrStaArrValue * symOrStaArrValue) {
	LogDebug("SymStaArrayCode\n");
	ArrayValue * value = NULL;
	Array * current = symOrStaArrValue->array;
	entry * entry;
	while (current != NULL)
	{
		if (current->VarOrString->type == VAR_VOST) {
			if (symOrStaArr->symOrState->type == SYM_SOST) {
				if (!exists(st, current->VarOrString->variable->value, SYMBOL_DT)) {
					return -1;
				}
			}
			else {
				if (!exists(st, current->VarOrString->variable->value, STATE_DT)){
					return -1;
				}
			}

			entry = getEntry(st, current->VarOrString->variable->value);

			AddArrayValue(&value, (char *)entry->value);
		}
		else {
			AddArrayValue(&value, current->VarOrString->string->value);
		}
		current = current->next;
	}
		
	return setValue(st, variable->value, value);
}

int SymStaCode(Variable * variable, SymOrState * symOrSta, String * symOrStaValue) {
	LogDebug("SymStaCode\n");
	return setValue(st, variable->value, symOrStaValue->value);
}


int ExecCode(Exec * exec) {
	switch (exec->type) {
		case ADD_EVT:
			return AddCode(exec->add);
		case REM_EVT:
			return RemoveCode(exec->rem);
		case CHECK_EVT:
			return CheckCode(exec->check);
		case COMPLEMENT_EVT:
			return ComplementCode(exec->complement);
		case JOIN_EVT:
			return JoinCode(exec->join);
		case PRINT_EVT:
			return PrintCode(exec->print);
	}

	return -1;
}

//TODO: Considerar el -f
int PrintCode(Print * toPrint){
	fprintf(stdout, "# %s\n", toPrint->string->value);
	return 0;
}

int AddCode(Add * add){
	return 0;
}

int RemoveCode(Rem * rem){
	return 0;
}

int CheckCode(Check * check){
	return 0;
}

int ComplementCode(Complement * complement){
	return 0;
}

int JoinCode(Join * join){
	return 0;
}