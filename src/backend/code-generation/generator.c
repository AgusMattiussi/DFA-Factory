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
	LogDebug("%d", toReturn);
	return toReturn;
}

int DecOrExecCode(DecOrExec * decOrExec){
	LogDebug("DecOrExecCode");
	if(decOrExec->type == DEC_DOET)
		return DecCode(decOrExec->declaration);
	else	// type == EXEC_DOET
		return ExecCode(decOrExec->exec);
}


int DecCode(Declaration * dec){
	LogDebug("DecCode");
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

//TODO: Devolver -1?
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
int DfaValueCode(Variable * variable, DfaValue * dfaValue) {
	LogDebug("DfaValueCode");
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

	entry * finalStatesEntry = getEntry(st, dfaValue->finalStates->value);
	this->finalStates = (ArrayValue *) finalStatesEntry->value;
	
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
		if(strcmp(currState->value, (char *) initial->value) == 0){
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
			this->delta[i][j] = this->staCount;
		}		
	}
	
	TrnArrayValue * currentTrn = this->transitions;

	while (currentTrn != NULL) {
		
		int idxStaFrom = getIndex(this->states, currentTrn->value->stateFrom);

		int idxStaTo = getIndex(this->states, currentTrn->value->stateTo);

		int idxSym = getIndex(this->symbols, currentTrn->value->symbol);
		
		if (this->delta[idxStaFrom][idxSym] == this->staCount){
			this->delta[idxStaFrom][idxSym] = idxStaTo;
		}
		else {
			return -1;
		}
		currentTrn = currentTrn->next;
	}
	
	return setValue(st, variable->value, this);
}

static char * getTransitionParam(VarOrString * vos, DataType type) {
	if(vos->type == VAR_VOST){
		entry * stateFrom = getEntry(st, vos->variable->value);
		if(stateFrom == NULL || stateFrom->dataType != type){
			LogDebug(" NO ENCONTRE LA ENTRY");
			return NULL;
		}
		return (char *) stateFrom->value;
	} else {
		return vos->string->value;
	}
}

int TransitionCode(Variable * variable, Transition * trnValue) {
	LogDebug("TransitionCode");
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

int TrnArrayCode(Variable *variable, TrnArrValue *trnArrValue) {
	LogDebug("TrnArrayCode");
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

static void AddArrayValue(ArrayValue ** head, char *value) {
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
	LogDebug("SymStaArrayCode");
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
	LogDebug("SymStaCode");
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
	LogDebug("PrintCode");
	fprintf(stdout, "# %s\n", toPrint->string->value);
	return 0;
}

static bool isInFinalState(automata * automata){
	ArrayValue * final = automata->finalStates;

	//TODO: Modularizar
	ArrayValue * current = automata->states;
	for (size_t i = 0; i < automata->currentStateIdx; i++, current = current->next);

	while (final != NULL){
		if(strcmp(final->value, current->value) == 0)
			return true;
	}
	return false;
}

static printCheckOutput(char * dfaName, ArrayValue * word, bool result){
	ArrayValue * symbol = word;
	printf("%s-{", dfaName);
	while (symbol != NULL){
		printf("%s", symbol->value);
		if(symbol->next != NULL)
			putchar(',');
		symbol = symbol->next;
	}
	printf("}:%s\n", result ? "true" : "false");

}

int CheckCode(Check * check){
	LogDebug("CheckCode");
	
	if (!exists(st, check->variable->value, DFA_DT)){
		LogDebug("El DFA no existe");
		return -1;
	}
	entry * dfa = getEntry(st, check->variable->value);
	automata * myAuto = (automata *)dfa->value;
	ArrayValue * word = NULL;
	
	if(check->type == VAR_SSAOVT){
		LogDebug("Es variable");
		entry * wordEntry = getEntry(st, check->rVariable->value);
		if(wordEntry == NULL){
			LogDebug("La variable %s(sym[]) no esta en la tabla", check->rVariable->value);
			return -1;
		}
		word = (ArrayValue *) wordEntry->value;
	} else {
		LogDebug("Es array de simbolos");
		Array * aux = check->symOrStaArrValue->array;
		while (aux != NULL){
			if(aux->VarOrString->type != VAR_VOST)
				AddArrayValue(&word, aux->VarOrString->string->value);
			else
				return -1;
			aux = aux->next;
		}
	} 
	
	ArrayValue * current = word;
	int symIdx = 0;
	// TODO: Responsabilidad de automata.c?
	while (current != NULL){
		symIdx = getIndex(myAuto->symbols, current->value);
		
		//printf("De %zd a ", myAuto->currentStateIdx);

		// No existe la transicion delta[estado][simbolo]
		if(myAuto->currentStateIdx == myAuto->staCount ){
			//printf("ERROR\n");
			LogDebug("No hay transicion del estado %d con %s", myAuto->currentStateIdx, current->value);
			break;
		}
		
		//LogDebug("El idx del simbolo es %d", symIdx);
		//LogDebug("currentStateIdx: %zd", myAuto->currentStateIdx);
		// El simbolo no existe en el automata
		if(symIdx == myAuto->symCount){
			//printf("ERROR\n");
			LogDebug("El simbolo %s no esta en el automata", current->value);
			break;
		}

		
		myAuto->currentStateIdx = myAuto->delta[myAuto->currentStateIdx][symIdx];
		//printf("%zd con %s\n", myAuto->currentStateIdx, current->value);
		current = current->next;
	}
	//TODO: Borrar
	if(current != NULL){
		LogDebug("Current no es null (no termino la palabra)");
	} else if(!isInFinalState(myAuto)){
		LogDebug("El automata no esta en estado final");
	}
	
	printCheckOutput(dfa->variableName, word,(current == NULL && isInFinalState(myAuto)));
		
	return 0;
}

static ArrayValue * copyArrayValue(ArrayValue * old, int * count) {
	ArrayValue * aux = old;
	ArrayValue * new = NULL;
	while (aux != NULL) {
		AddArrayValue(&new,aux->value);
		aux = aux->next;
		(*count)++;
	}
	return new;
}

static TrnArrayValue * copyTrnArrayValue(TrnArrayValue * old) {
	TrnArrayValue * aux = old;
	TrnArrayValue * new = NULL;
	while (aux != NULL)
	{
		AddTransitionArrayValue(&new,aux->value);
		aux = aux->next;
	}
	return new;
}

static void freeArrayValue(ArrayValue * array) {
	if (array == NULL)
		return;
	freeArrayValue(array->next);
	free(array->value);
	free(array);
}

static void freeTrnArrayValue(TrnArrayValue * array) {
	if (array == NULL)
		return;
	freeTrnArrayValue(array->next);
	// free(array->value->stateFrom);
	// LogDebug("Freed stateFrom");
	// free(array->value->stateTo);
	// LogDebug("Freed stateTo");
	// free(array->value->symbol);
	// LogDebug("Freed symbol");
	// free(array->value);
	// LogDebug("Freed value");
	free(array);
}

static void freeAutomata(automata * toFree) {
	freeArrayValue(toFree->states);
	freeArrayValue(toFree->symbols);
	freeArrayValue(toFree->finalStates);
	freeTrnArrayValue(toFree->transitions);
	for (size_t i = 0; i < toFree->staCount; i++)
		free(toFree->delta[i]);
	free(toFree->delta);
	free(toFree);
}

int AddCode(Add * add){
	LogDebug("AddCode");
	if (!exists(st, add->rightOperand->value, DFA_DT))
	{
		return -1;
	}
	entry * rightOperandEntry = getEntry(st, add->rightOperand->value);
	int freeRightOperand = 1;
	if (strcmp(add->variable->value,add->rightOperand->value) != 0)
	{
		freeRightOperand = 0;
	}
	entry * value = getEntry(st, add->variable->value);
	automata * rightOperand = (automata*) rightOperandEntry->value;
	automata * newDfa = calloc(1, sizeof(automata));
	// hacer copia de todo excepto delta
	ArrayValue * aux = rightOperand->symbols;
	int count = 0;
	ArrayValue * newSym = copyArrayValue(aux,&count);
	newDfa->symbols = newSym;
	newDfa->symCount = count;

	aux = rightOperand->finalStates;
	count = 0;
	ArrayValue * newFinalStates = copyArrayValue(aux,&count);
	newDfa->finalStates = newFinalStates;

	aux = rightOperand->states;
	count = 0;
	ArrayValue * newStates = copyArrayValue(aux,&count);
	newDfa->states = newStates;
	newDfa->staCount = count;

	newDfa->startIdx = rightOperand->startIdx;
	newDfa->currentStateIdx = newDfa->startIdx;

	TrnArrayValue * trnAux = rightOperand->transitions;
	newDfa->transitions = copyTrnArrayValue(trnAux);

	if (add->leftOperand->type == VAR_OR_STRING_TVOST) // si hay una variable/string la agrego
	{
		if (add->leftOperand->varOrString->type == VAR_VOST)
		{
			if (!exists(st, add->leftOperand->varOrString->variable->value, STATE_DT) && !exists(st, add->leftOperand->varOrString->variable->value, TRANSITION_DT))
			{
				LogDebug("No existe la leftOperand");
				return -1;
			}
			else
			{
				entry * new = getEntry(st, add->leftOperand->varOrString->variable->value);
				if (new->dataType == STATE_DT)
				{
					AddArrayValue(&newDfa->states, (char*)newStates->value);
					newDfa->staCount++;
				}
				else if (new->dataType == TRANSITION_DT)
				{	
					AddTransitionArrayValue(&newDfa->transitions, (TransitionValue*) new->value);
				}
			}
		}
		else
		{
			AddArrayValue(&newDfa->states, add->leftOperand->varOrString->string->value);
			newDfa->staCount++;
		}
	}
	else if(add->leftOperand->type == TRANSITION_TVOST) // si hay una trn nueva la agrego
	{
		TransitionValue *auxTrn = malloc(sizeof(TransitionValue));

		auxTrn->stateFrom = getTransitionParam(add->leftOperand->transition->stateFrom, STATE_DT);
		if (auxTrn->stateFrom == NULL)
			return -1;

		auxTrn->stateTo = getTransitionParam(add->leftOperand->transition->stateTo, STATE_DT);
		if (auxTrn->stateTo == NULL)
			return -1;

		auxTrn->symbol = getTransitionParam(add->leftOperand->transition->symbol, SYMBOL_DT);
		if (auxTrn->symbol == NULL)
			return -1;

		AddTransitionArrayValue(&newDfa->transitions, auxTrn);
	}
	// genero delta 
	newDfa->delta = malloc(newDfa->staCount * sizeof(size_t*));
	for (int i = 0; i < newDfa->staCount; i++)
        newDfa->delta[i] = malloc(newDfa->symCount * sizeof(size_t));

	for (int i = 0; i < newDfa->staCount; i++){
		for (int j = 0; j < newDfa->symCount; j++) {
			newDfa->delta[i][j] = newDfa->staCount;
		}		
	}	

	TrnArrayValue * currentTrn = newDfa->transitions;
	while (currentTrn != NULL) {

		int idxStaFrom = getIndex(newDfa->states, currentTrn->value->stateFrom);

		int idxStaTo = getIndex(newDfa->states, currentTrn->value->stateTo);

		int idxSym = getIndex(newDfa->symbols, currentTrn->value->symbol);

		if (newDfa->staCount==idxStaFrom || newDfa->staCount==idxStaTo || newDfa->symCount==idxSym)
		{
			LogDebug("No es psible en este automata la trn");
			return -1;
		}
		
		if (newDfa->delta[idxStaFrom][idxSym] == newDfa->staCount){
			newDfa->delta[idxStaFrom][idxSym] = idxStaTo;
		}
		else {
			return -1;
		}
		currentTrn = currentTrn->next;
	}

	if (freeRightOperand)
	{
		freeAutomata(rightOperand);
	}
	

	return setValue(st,value->variableName,newDfa);
}

static int remFromList(automata * myAuto, ArrayValue * list, char * state) {
    ArrayValue * current = list;
    if(strcmp(current->value, state) == 0) { //remuevo el primero
        myAuto->states = NULL;
        free(current);
        return 0;
    } //el primero no es

	if(current->next == NULL)
		return -1;

	ArrayValue * nextState = current->next;
    while(current != NULL) {
        if(strcmp(nextState->value, state) == 0) {
            current->next = nextState->next;
            free(nextState);
            return 0;
        }
        current = current->next;
        if(nextState != NULL)
            nextState = nextState->next;
    }
    return -1;
}

int RemoveCode(Rem * rem){
    LogDebug("RemCode");
    
    if (!exists(st, rem->from->value, DFA_DT)){
        LogDebug("El DFA no existe");
        return -1;
    } 
    
    entry * dfa = getEntry(st, rem->from->value);
    automata * myAuto = (automata *)dfa->value;

    char * state;
    VarOrString * varOrStr = rem->varOrString;


    if(varOrStr->type == VAR_VOST) {
        if(!exists(st, varOrStr->variable->value, STATE_DT)) {
            return -1;
        }
        entry * stateEntry = getEntry(st, varOrStr->variable->value);
        state = (char *)stateEntry->value;
    } else { //type == STRING_VOST
        state = varOrStr->string->value;
    } 
	
	int idx = getIndex(myAuto->states, state);
	int rem1 = -1, rem2 = -1;

    rem1 = remFromList(myAuto, myAuto->states, state);
    rem2 = remFromList(myAuto, myAuto->finalStates, state);

    if(rem1 == -1)
        LogDebug("No se encontró el estado a borrar");

	for(int i=0; i<myAuto->staCount; i++) {
        for(int j=0; j<myAuto->symCount; j++) {
            if(i == idx || myAuto->delta[i][j] == idx || myAuto->delta[i][j] == myAuto->staCount) {
                myAuto->delta[i][j] = myAuto->staCount - 1;
            } else if(idx < myAuto->delta[i][j]) {
				myAuto->delta[i][j]--;
			}
        }
    }
	
	if(rem1 == 0)
		myAuto->staCount--;
    
    TrnArrayValue * currentTrn = myAuto->transitions;
    TrnArrayValue * nextTrn = currentTrn->next;
	TrnArrayValue * aux = NULL;
	
	while((strcmp(currentTrn->value->stateFrom, state) == 0) || (strcmp(currentTrn->value->stateTo, state) == 0)) {
		myAuto->transitions = nextTrn;
		aux = currentTrn->next;
		nextTrn = nextTrn->next;	
		free(currentTrn);
		currentTrn = aux;				
	}

	currentTrn = myAuto->transitions;
	nextTrn = currentTrn->next;
	while(currentTrn != NULL) {
            if((strcmp(currentTrn->value->stateFrom, state) == 0) || (strcmp(currentTrn->value->stateTo, state) == 0))  {
                currentTrn->next = nextTrn->next;
                free(nextTrn);
            }
            currentTrn = currentTrn->next;
            if(nextTrn != NULL)
                nextTrn = nextTrn->next;
    }

	if(!exists(st, rem->variable->value, DFA_DT))
		addEntry(st, rem->variable->value, DFA_DT);

    return setValue(st, rem->variable->value, myAuto);
}

static bool isFinal(char * state, ArrayValue * finalStates){
	ArrayValue * aux = finalStates;
	while(aux != NULL){
		if(strcmp(state, aux->value))
			return true;
		aux = aux->next;
	}
	return false;
}

int ComplementCode(Complement * complement) {
    LogDebug("ComplementCode");
	if(!exists(st, complement->notVariable->value, DFA_DT))
		return -1;

	if (!exists(st, complement->variable->value, DFA_DT))
		addEntry(st, complement->variable->value, DFA_DT);
	
	entry * entry = getEntry(st, complement->notVariable->value);
	automata * originalDfa = (automata *) entry->value;
	ArrayValue * originalFinalStates = originalDfa->finalStates;
	ArrayValue * newFinalStates = NULL;

	ArrayValue * current = originalDfa->states;

	while(current != NULL){
		if(!isFinal(current->value, originalFinalStates)){
			AddArrayValue(&newFinalStates, current->value);
		}
		current = current->next;
	}

	originalDfa->finalStates = newFinalStates;

	return setValue(st, complement->variable->value, originalDfa);
}

int JoinCode(Join * join){
	return 0;
}