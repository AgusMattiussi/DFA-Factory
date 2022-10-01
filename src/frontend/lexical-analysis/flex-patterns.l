%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Contextos de Flex. */
%x COMMENT

/* Patrones reutilizables. */
endline			[\n]
whitespace		[ \f\n\r\t\v]
alphanumeric    [a-zA-Z0-9]

%%
"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.									{ ; }
<COMMENT>{endline}							{ ; }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }

"="                                         { EqualsPatternAction(yytext); }

";"                                         { SemicolonPatternAction(yytext); }

"{"                                         { OpenCurlyBracketsPatternAction(yytext); }

"}"                                         { CloseCurlyBracketsPatternAction(yytext); }

","                                         { CommaPatternAction(yytext); }

{alphanumeric}+                             { VariablePatternAction(yytext, yyleng); }            

"\"{alphanumeric}+\""                     { StringPatternAction(yytext, yyleng); }   

{whitespace}								{ IgnoredPatternAction(yytext, yyleng); }

"!"                                         { NotPatternAction(yytext); }

"CHECK"                                     { CheckPatterAction(yytext); }

"ADD"                                       { AddPatternAction(yytext); }

"TO"                                        { ToPatternAction(yytext); }

"REM"                                       { RemPatternAction(yytext); }

"FROM"                                      { FromPatternAction(yytext); }

"JOIN"                                      { JoinPatternAction(yytext);}

"PRINT"                                     { PrintPatternAction(yytext); }

"sym"                                       { SymbolPatternAction(yytext); }

"sym[]"                                     { SymbolArrayPatternAction(yytext); }

"sta"                                       { StatePatternAction(yytext); }

"sta[]"                                     { StateArrayPatternAction(yytext); }

"trn"                                       { TransitionPatternAction(yytext); }

"trn[]"                                     { TransitionArrayPatternAction(yytext); }

"dfa"                                       { DFAPatternAction(yytext); }

%%

