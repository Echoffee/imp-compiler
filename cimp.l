%{
    #include "cimp.tab.h"
    #define YY_DECL int yylex()

%}

%%
":=" {return S_AF;}
"skip" {return S_SK;}
";" {return S_SE;}
"if" {return S_IF;}
"then" {return S_TH;}
"else" {return S_EL;}
"while" {return S_WH;}
"do" {return S_DO;}
"+" {return S_PL;}
"-" {return S_MO;}
"*" {return S_MU;}
"(" {return P_OPEN;}
")" {return P_CLOSE;}
[ \t\n] { }
[1-9][0-9]* {yylval.sval = yytext; return V_INT;}
0 {yylval.sval = yytext; return V_INT;}
[a-zA-Z0-9\_]* {yylval.sval = strdup(yytext); return V_VAR;}
%%
