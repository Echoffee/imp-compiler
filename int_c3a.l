%{
    #include "int_c3a.tab.h"
    #define YY_DECL int yylex()

%}

%%
"Af" {fprintf(stderr, ".%s.", yytext);return S_AF;}
"Afc" {fprintf(stderr, ".%s.", yytext);return S_AFC;}
"Sk" {fprintf(stderr, ".%s.", yytext);return S_SK;}
":" {fprintf(stderr, ".%s.", yytext);return S_SE;}
"Pl" {fprintf(stderr, ".%s.", yytext);return S_PL;}
"Mo" {fprintf(stderr, ".%s.", yytext);return S_MO;}
"Mu" {fprintf(stderr, ".%s.", yytext);return S_MU;}
"Jp" {fprintf(stderr, ".%s.", yytext);return S_JP;}
"Jz" {fprintf(stderr, ".%s.", yytext);return S_JZ;}
"St" {fprintf(stderr, ".%s.", yytext);return S_ST;}
"+" {fprintf(stderr, ".%s.", yytext);return O_POS;}
"-" {fprintf(stderr, ".%s.", yytext);return O_NEG;}
"\n" {fprintf(stderr, "\\n\n");return S_EOL;}
[ \t] { }
[1-9][0-9]* {yylval.ival = atoi(yytext); fprintf(stderr, ".%s.", yytext);return V_INT;}
0 {yylval.ival = 0; fprintf(stderr, ".%s.", yytext);return V_INT;}
[a-zA-Z\_][a-zA-Z0-9\_]* {yylval.sval = strdup(yytext); fprintf(stderr, ".%s.", yytext);return V_VAR;}
%%
