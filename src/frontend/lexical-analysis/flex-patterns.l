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
/* TODO: Ser mas permisivo con los caracteres para simbolos y estados */
alphanumeric    [a-zA-Z0-9]
CHECK           "CHECK"
ADD             "ADD"
TO              "TO"
REM             "REM"
FROM            "FROM"
JOIN            "JOIN"
PRINT           "PRINT"
sym             "sym"
symArray        "sym[]"
sta             "sta"
staArray        "sta[]"
trn             "trn"
trnArray        "trn[]"
dfa             "dfa"

%%
"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.									{ ; }
<COMMENT>{endline}							{ ; }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }

"="                                         { return EqualsPatternAction(yytext); }

";"                                         { return SemicolonPatternAction(yytext); }

"{"                                         { return OpenCurlyBracketsPatternAction(yytext); }

"}"                                         { return CloseCurlyBracketsPatternAction(yytext); }

","                                         { return CommaPatternAction(yytext); }

{whitespace}								{ IgnoredPatternAction(yytext, yyleng); }

"!"                                         { return NotPatternAction(yytext); }

{CHECK}                                     { return CheckPatterAction(yytext); }

{ADD}                                       { return AddPatternAction(yytext); }

{TO}                                        { return ToPatternAction(yytext); }

{REM}                                       { return RemPatternAction(yytext); }

{FROM}                                      { return FromPatternAction(yytext); }

{JOIN}                                      { return JoinPatternAction(yytext);}

{PRINT}                                     { return PrintPatternAction(yytext); }

{sym}                                       { return SymbolPatternAction(yytext); }

{symArray}                                  { return SymbolArrayPatternAction(yytext); }

{sta}                                       { return StatePatternAction(yytext); }

{staArray}                                  { return StateArrayPatternAction(yytext); }

{trn}                                       { return TransitionPatternAction(yytext); }

{trnArray}                                  { return TransitionArrayPatternAction(yytext); }

{dfa}                                       { return DFAPatternAction(yytext); }

\"{alphanumeric}+\"                         { return StringPatternAction(yytext, yyleng); } 

{alphanumeric}+                             { return VariablePatternAction(yytext, yyleng); }            

.                                           { return UnknownPatternAction(yytext, yyleng); }

%%

