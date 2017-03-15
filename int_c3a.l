%{
    #include "int_c3a.tab.h"
    #define YY_DECL int yylex()

%}

%%
"Af" {return S_AF;}
"Afc" {return S_AFC;}
"Sk" {return S_SK;}
":" {return S_SE;}
"Pl" {return S_PL;}
"Mo" {return S_MO;}
"Mu" {return S_MU;}
"Jp" {return S_JP;}
"Jz" {return S_JZ;}
"St" {return S_ST;}
"+" {return O_POS;}
"-" {return O_NEG;}
"\n" {return S_EOL;}
[ \t] { }
[1-9][0-9]* {yylval.ival = atoi(yytext); return V_INT;}
0 {yylval.ival = 0; return V_INT;}
[a-zA-Z\_][a-zA-Z0-9\_]* {yylval.sval = strdup(yytext); return V_VAR;}
%%
