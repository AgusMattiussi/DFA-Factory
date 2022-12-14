#include "../../backend/support/logger.h"
#include "flex-actions.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Implementación de "flex-actions.h".
 *
 * Cada función debe realizar 2 operaciones básicas para lograr el streaming
 * de tokens hacia Bison:
 *
 *	1) Computar los atributos del token y almacenarlos donde sea conveniente
 *		(en la tabla de símbolos, en "yylval", o en ambos).
 *	2) Retornar el token que identifica el terminal identificado.
 *
 * Bison utilizará el token retornado en el punto (2) para matchear dicho
 * terminal en la gramática. Por otro lado, el valor almacenado en "yylval" es
 * el que Bison proveerá como valor semántico al realizar una reducción
 * (mediante $1, $2, $3, etc.).
 */

void BeginCommentPatternAction() {
	LogDebug("BeginCommentPatternAction.");
}

void EndCommentPatternAction() {
	LogDebug("EndCommentPatternAction.");
}

void BeginStringPatternAction() {
	LogDebug("BeginStringPatternAction.");
}

void EndStringPatternAction() {
	LogDebug("EndStringPatternAction.");
}

token EqualsPatternAction(const char * lexeme){
	LogDebug("EqualsPatternAction: '%s'.", lexeme);
	yylval.token = EQUALS;
	return EQUALS;
}


token NotPatternAction(const char * lexeme){
	LogDebug("NotPatternAction: '%s'.", lexeme);
	yylval.token = NOT;
	return NOT;
}

token SemicolonPatternAction(const char * lexeme){
	LogDebug("SemicolonPatternAction: '%s'.", lexeme);
	yylval.token = SEMICOLON;
	return SEMICOLON;
}

token OpenCurlyBracketsPatternAction(const char * lexeme){
	LogDebug("OpenCurlyBracketsPatternAction: '%s'.", lexeme);
	yylval.token = OPEN_CURLY;
	return OPEN_CURLY;
}

token CloseCurlyBracketsPatternAction(const char * lexeme){
	LogDebug("CloseCurlyBracketsPatternAction: '%s'.", lexeme);
	yylval.token = CLOSE_CURLY;
	return CLOSE_CURLY;
}

token OpenBracketsPatternAction(const char * lexeme){
	LogDebug("OpenBracketsPatternAction: '%s'.", lexeme);
	yylval.token = OPEN_BRACKETS;
	return OPEN_BRACKETS;
}

token CloseBracketsPatternAction(const char * lexeme){
	LogDebug("CloseBracketsPatternAction: '%s'.", lexeme);
	yylval.token = CLOSE_BRACKETS;
	return CLOSE_BRACKETS;
}

token CommaPatternAction(const char * lexeme){
	LogDebug("CommaPatternAction: '%s'.", lexeme);
	yylval.token = COMMA;
	return COMMA;
}

token CheckPatterAction(const char * lexeme){
	LogDebug("CheckPatternAction: '%s'.", lexeme);
	yylval.token = CHECK;
	return CHECK;
}

token AddPatternAction(const char * lexeme){
	LogDebug("AddPatternAction: '%s'.", lexeme);
	yylval.token = ADD;
	return ADD;
}

token ToPatternAction(const char * lexeme){
	LogDebug("ToPatternAction: '%s'.", lexeme);
	yylval.token = TO;
	return TO;
}

token RemPatternAction(const char * lexeme){
	LogDebug("RemPatternAction: '%s'.", lexeme);
	yylval.token = REM;
	return REM;
}

token FromPatternAction(const char * lexeme){
	LogDebug("FromPatternAction: '%s'.", lexeme);
	yylval.token = FROM;
	return FROM;
}

token JoinPatternAction(const char * lexeme){
	LogDebug("JoinPatternAction: '%s'.", lexeme);
	yylval.token = JOIN;
	return JOIN;
}

token PrintPatternAction(const char * lexeme){
	LogDebug("PrintPatternAction: '%s'.", lexeme);
	yylval.token = PRINT;
	return PRINT;
}

token VariablePatternAction(const char * lexeme, const int length){
	LogDebug("VariablePatternAction: '%s' (length = %d).", lexeme, length);
	yylval.variable = malloc(sizeof(Variable));
	yylval.variable->value = calloc(length+1, sizeof(char));
	strcpy(yylval.variable->value, lexeme);
	return VARIABLE;
}

token StringPatternAction(const char * lexeme, const int length){
	LogDebug("StringPatternAction: '%s' (length = %d).", lexeme, length);
	yylval.string = malloc(sizeof(String));
	yylval.string->value = calloc(length+1, sizeof(char));
	strcpy(yylval.string->value, lexeme);
	return STRING;
}

token SymbolPatternAction(const char * lexeme){
	LogDebug("SymbolPatternAction: '%s'.", lexeme);
	yylval.token = SYMBOL;
	return SYMBOL;
}

token StatePatternAction(const char * lexeme){
	LogDebug("StatePatternAction: '%s'.", lexeme);
	yylval.token = STATE;
	return STATE;
}

token TransitionPatternAction(const char * lexeme){
	LogDebug("TransitionPatternAction: '%s'.", lexeme);
	yylval.token = TRANSITION;
	return TRANSITION;
}

token DFAPatternAction(const char * lexeme){
	LogDebug("DfaPatternAction: '%s'.", lexeme);
	yylval.token = DFA;
	return DFA;
}


token UnknownPatternAction(const char * lexeme, const int length) {
	LogDebug("UnknownPatternAction: '%s' (length = %d).", lexeme, length);
	yylval.token = YYUNDEF;
	// Al emitir este token, el compilador aborta la ejecución.
	return YYUNDEF;
}

void IgnoredPatternAction(const char * lexeme, const int length) {
	LogDebug("IgnoredPatternAction: '%s' (length = %d).", lexeme, length);
	// Como no debe hacer nada con el patrón, solo se loguea en consola.
}


