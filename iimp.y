%{
    #include <stdio.h>
	#include "imp_interpreter.h"
    int yylex();
    int yyerror(char* s);
%}
%union{
    int ival;
	char* sval;
}

%token S_AF S_SK S_SE S_IF S_TH S_EL S_WH S_DO S_PL S_MO S_MU
%token P_OPEN P_CLOSE
%token V_INT V_VAR

%type<ival> E
%type<ival> T
%type<ival> F
%type<sval> C
%type<sval> CC
%type<ival> V_INT
%type<sval> V_VAR

%start STRT
%%
STRT	: C {printf("\n*** Done ***\n");}

E		: E S_PL T {$$ = $1 + $3;}
		| E S_MO T {$$ = $1 - $3;}
		| T	{$$ = $1;}
		;

T		: T S_MU F {$$ = $1 * $3;}
		| F {$$ = $1;}
		;

F		: P_OPEN E P_CLOSE {$$ = $2;}
		| V_INT {$$ = $1;}
		| V_VAR {$$ = get_value_from_variable($1);}
		;

C		: V_VAR S_AF E {assign_value_to_variable($1, $3);}
		| S_SK { }
//		| P_OPEN C P_CLOSE {$$ = $2;}
		| S_IF E S_TH CC S_EL CC {if($2) {fprintf(stderr, "if"); $$ = $4; }else{fprintf(stderr, "else"); $$ = $6;};}
//		| S_WH E S_DO C {if($2){$$ = concat($4, yytext);};}
		| C S_SE C
		;

CC		: V_VAR S_AF E
		| S_SK
//		| P_OPEN C P_CLOSE {$$ = $2;}
		| S_IF E S_TH CC S_EL CC
//		| S_WH E S_DO C {if($2){$$ = concat($4, yytext);};}
		| CC S_SE CC
		;
%%

int yyerror(char *s)
{
    fprintf(stderr, "*** ERROR: %s***\n", s);
    return 0;
}

int main()
{
    yyparse();
	display_env();
    return 0;
}
