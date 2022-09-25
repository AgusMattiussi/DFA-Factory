%{

#include "flex-actions2.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Contextos de Flex. */
%x COMMENT
/* %x STRING */

/* Patrones reutilizables. */
endline			[\n]
whitespace		[ \f\n\r\t\v]
alphanumeric    [a-zA-Z0-9]

%%
/* Contexto COMMENT: No hacer nada con ningun caracter */
"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.									{ ; }
<COMMENT>{endline}							{ ; }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }

/* Contexto STRING: No hacer nada con ningun caracter */
/* TODO: Longitud del string? */
/* "\""                                        { BEGIN(STRING); BeginStringPatternAction(); }
<STRING>.									{ ; }
<STRING>{endline}							{ ; }
<STRING>"\""                                { EndStringPatternAction(); BEGIN(INITIAL); } */

"="                                         { EqualsPatternAction(yytext); }

";"                                         { SemicolonPatternAction(yytext); }

"{"                                         { OpenCurlyBracketsPatternAction(yytext); }

"}"                                         { CloseCurlyBracketsPatternAction(yytext); }

","                                         { CommaPatternAction(yytext); }

{alphanumeric}+                             { VariablePatternAction(yytext, yyleng); }            

"\"{alphanumeric}+\""                       { StringPatternAction(yytext, yyleng); }   

{whitespace}								{ IgnoredPatternAction(yytext, yyleng); }

/* ================================= OPERACIONES ============================== */

"!"                                         { NotPatternAction(yytext); }

"CHECK"                                     { CheckPatterAction(yytext); }

"ADD"                                       { AddPatternAction(yytext); }

"TO"                                        { ToPatternAction(yytext); }

"REM"                                       { RemPatternAction(yytext); }

"FROM"                                      { FromPatternAction(yytext); }

"JOIN"                                      { JoinPatternAction(yytext);}

"PRINT"                                     { PrintPatternAction(yytext); }

/* ================================= TIPOS DE DATO ============================== */

"sym"                                       { return SYM; }
"sym[]"                                     { return SYM_ARRAY; }
"sta"                                       { return STA; }
"sta[]"                                     { return STA_ARRAY; }
"trn"                                       { return TRN; }
"trn[]"                                     { return TRN_ARRAY; }
"dfa"                                       { return DFA; }

%%

