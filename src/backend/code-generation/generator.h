#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../semantic-analysis/abstract-syntax-tree.h"
#include "../support/symTable.h"

int Generator(Program * result, symbolTable * symbolTable);
int DecOrExecCode(DecOrExec * decOrExec);
int DecCode(Declaration * dec);
int DfaValueCode(Variable * variable, DfaValue * dfaValue);
int TransitionCode(Variable * variable, Transition * trnValue);
int TrnArrayCode(Variable * variable, TrnArrValue * trnArrValue);
int SymStaArrayCode(Variable * variable, SymOrStaArr * symOrStaAr, SymOrStaArrValue * symOrStaArrValue);
int SymStaCode(Variable * variable, SymOrState * symOrSta, String * symOrStaValue);
int ExecCode(Exec * exec);
int AddCode(Add * add);
int RemoveCode(Rem * rem);
int CheckCode(Check * check);
int ComplementCode(Complement * complement);
int JoinCode(Join * join);
int PrintCode(Print * toPrint);

#endif
