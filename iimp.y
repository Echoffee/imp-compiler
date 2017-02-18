%{
    #include <stdio.h>
	#include "imp_interpreter.h"
    int yylex();
    int yyerror(char* s);
%}
%union{
    int ival;
	char* sval;

	struct s_ast_node* node;
}

%token S_AF S_SK S_SE S_IF S_TH S_EL S_WH S_DO S_PL S_MO S_MU
%token P_OPEN P_CLOSE
%token V_INT V_VAR

%type<node> E
%type<node> T
%type<node> F
%type<node> C

%type<ival> V_INT
%type<sval> V_VAR

%start STRT
%%
STRT	: C {ast_execute($1); printf("\n*** Done ***\n");}

E		: E S_PL T {$$ = ast_create_add_node($1, $3);}
		| E S_MO T {$$ = ast_create_sub_node($1, $3);}
		| T	{$$ = $1;}
		;

T		: T S_MU F {$$ = ast_create_mult_node($1, $3);}
		| F {$$ = $1;}
		;

F		: P_OPEN E P_CLOSE {$$ = ast_create_node_from_ep($2);}
		| V_INT {$$ = ast_create_node_from_int($1);}
		| V_VAR {$$ = ast_create_node_from_variable($1);}
		;

C		: V_VAR S_AF E {$$ = ast_create_aff_node($1, $3);}
		| S_SK { }
		| P_OPEN C P_CLOSE {$$ = ast_create_node_from_cp($2);}
		| S_IF E S_TH C S_EL C {ast_create_ITE_node($2, $4, $6);}
		| S_WH E S_DO C {ast_create_WD_node($2, $4);}
		| C S_SE C {ast_create_branch($1, $3);}
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
