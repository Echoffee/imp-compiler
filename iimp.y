%{
    #include <stdio.h>
    int yylex();
    int yyerror(char* s);
%}
%union{
    int ival;
}

%token S_AF S_SK S_SE S_IF S_TH S_EL S_WH S_DO S_PL S_MO S_MU
%token P_OPEN P_CLOSE
%token V_INT V_VAR

%type<ival> E
%type<ival> T
%type<ival> F
%type<ival> V_INT

%start STRT
%%
STRT	: E {printf("\n*** Done ***\n");}

E		: E S_PL T {$$ = $1 + $3;}
		| E S_MO T {$$ = $1 - $3;}
		| T	{$$ = $1;}
		;

T		: T S_MU F {$$ = $1 * $3;}
		| F {$$ = $1;}
		;

F		: P_OPEN E P_CLOSE {$$ = $2;}
		| V_INT {$$ = $1;}
//		| V_VAR {$$ = $1;}
		;

C		: V S_AF E
		| S_SK
		| P_OPEN C P_CLOSE
		| S_IF E S_TH C S_EL C
		| S_WH E S_DO C
		| C S_SE C
		;
%%
