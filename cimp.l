%{
    #include "cimp.tab.h"
    #define YY_DECL int yylex()

%}

%%
":=" {fprintf(stderr, ".%s.", yytext); return S_AF;}
"skip" {fprintf(stderr, ".%s.", yytext); return S_SK;}
";" {fprintf(stderr, ".%s.", yytext); return S_SE;}
"if" {fprintf(stderr, ".%s.", yytext); return S_IF;}
"then" {fprintf(stderr, ".%s.", yytext); return S_TH;}
"else" {fprintf(stderr, ".%s.", yytext); return S_EL;}
"while" {fprintf(stderr, ".%s.", yytext); return S_WH;}
"do" {fprintf(stderr, ".%s.", yytext); return S_DO;}
"+" {fprintf(stderr, ".%s.", yytext); return S_PL;}
"-" {fprintf(stderr, ".%s.", yytext); return S_MO;}
"*" {fprintf(stderr, ".%s.", yytext); return S_MU;}
"(" {fprintf(stderr, ".%s.", yytext); return P_OPEN;}
")" {fprintf(stderr, ".%s.", yytext); return P_CLOSE;}
[ \t\n\r] { }
[1-9][0-9]* {yylval.ival = atoi(yytext); fprintf(stderr, ".%s.", yytext); return V_INT;}
0 {yylval.ival = 0; fprintf(stderr, ".%s.", yytext); return V_INT;}
[a-zA-Z0-9\_]* {yylval.sval = strdup(yytext); fprintf(stderr, ".%s.", yytext); return V_VAR;}
%%
