#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/* ===================== DFA Factory AST ============================ */

typedef struct DecOrExec DecOrExec;
typedef struct TransitionArr TransitionArr;
typedef struct Array Array;

/* ===================== Enums ============================ */

typedef enum  {
	DFA_DVT,
	TRANS_DVT,
	TRN_ARRAY_DVT,
	SYMSTA_ARR_DVT,
	STRING_DVT
} DecValueType;

typedef enum  {
	ADD_EVT,
	REM_EVT,
	CHECK_EVT,
	COMPLEMENT_EVT,
	JOIN_EVT,
	PRINT_EVT
} ExecValueType;

typedef enum  {
	DEC_DOET,
	EXEC_DOET
} DecOrExecType;

typedef enum  {
	VAR_VOST,
	STRING_VOST
} VarOrStringType;

typedef enum  {
	TRANSITION_TOVT,
	VAR_TOVT
} TransitionOrVarType;

typedef enum  {
	SYM_SOST,
	STATE_SOST
} SymOrStateType;

typedef enum  {
	SYM_STATE_ARR_SSAOVT,
	VAR_SSAOVT
} SymStateArrOrVarType;

typedef enum  {
	TRANSITION_TVOST,
	VAR_OR_STRING_TVOST
} TransitionVarOrStringType;

/* ===================== Structs ============================ */

/* Busco el tipo de dato en la tabla de simbolos con este nombre*/
typedef struct Variable {
	char * value;
} Variable;

/* La diferencia con 'Variable' es que 'String' tiene comillas*/
typedef struct String {
	char * value;
} String;

typedef struct VarOrString {
	enum VarOrStringType type;
	union vos_value {
		Variable * variable;
		String * string;
	};
} VarOrString;

typedef struct Transition {
	VarOrString * stateFrom;
	VarOrString * stateTo;
	VarOrString * symbol;
} Transition;

typedef struct {
	TransitionOrVarType type;
	union tov_value {
		Transition * transition;
		Variable * variable;
	};
} TransitionOrVar;

struct TransitionArr{
	TransitionOrVar * transitionOrVar;
	TransitionArr * next;
};

typedef struct TrnArrValue {
	TransitionArr * transitionArr;
} TrnArrValue;

struct Array {
	VarOrString * VarOrString;
	Array * next;
};

typedef struct SymOrStaArrValue {
	Array * array;
} SymOrStaArrValue;

typedef struct SymOrState {
	enum SymOrStateType type;
} SymOrState;

typedef struct SymOrStaArr {
	SymOrState * symOrState;
} SymOrStaArr;

typedef struct DfaValue {
	Variable * states;
	Variable * symbols;
	Variable * initial;
	Variable * finalStates;
	Variable * transitions;
} DfaValue;

typedef struct Declaration {
	enum DecValueType type;
	Variable * variable;
	SymOrStaArr * symOrStaArr;
	SymOrState * symOrState;
	union d_value {
		DfaValue * dfa;
		Transition * transition;
		SymOrStaArrValue * symOrStaArrValue;
		TrnArrValue * trnArrayValue;
		String * symOrStateValue;
	};
} Declaration;

typedef struct AddOperand {
	enum TransitionVarOrStringType type;
	union ao_value {
		Transition * transition;
		VarOrString * varOrString;
	};
} AddOperand;

typedef struct Add {
	Variable * variable;
	AddOperand * leftOperand;
	Variable * rightOperand;
} Add;

typedef struct {
	Variable * variable;
	VarOrString * varOrString;
	Variable * from;
} Rem;

typedef struct Check {
	enum SymStateArrOrVarType type;
	Variable * variable;
	union c_value {
		SymOrStaArrValue * symOrStaArrValue;
		Variable * rVariable;
	};
} Check;

typedef struct Complement {
	Variable * variable;
	Variable * notVariable;
} Complement;

typedef struct Join {
	Variable * dfaVariable;
	Variable * leftOperand;
	Variable * rightOperand;
	TransitionOrVar * transitionOrVar;
} Join;

typedef struct Print {
	String * string;
} Print;

typedef struct Exec {
	enum ExecValueType type;
	union e_value {
		Add * add;
		Rem * rem;
		Check * check;
		Complement * complement;
		Join * join;
		Print * print;
	};
} Exec;

struct DecOrExec{
	enum DecOrExecType type;
	union doe_value {
		Declaration * declaration;
		Exec * exec;
	};
	DecOrExec * next;
}; 

typedef struct Program {
	DecOrExec * decOrExec;
} Program; 

#endif
