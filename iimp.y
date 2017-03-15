%error-verbose

%{
    #include <stdio.h>
	#include "iimp.h"
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

%left S_PL S_MO

%type<node> E
%type<node> T
%type<node> F
%type<node> N
%type<ival> V
%type<node> C
%type<node> CC

%type<ival> V_INT
%type<sval> V_VAR

%start STRT
%%
STRT	: C {/*display_ast_tree($1, 0);*/ast_execute($1); fprintf(stderr, "\n*** IMP > C3A Done ***\n");}
		;

E		: E S_PL T {$$ = ast_create_o_node($1, $3, ADD);}
		| E S_MO T {$$ = ast_create_o_node($1, $3, SUB);}
		| T	{$$ = $1;}
		;

T		: T S_MU F {$$ = ast_create_o_node($1, $3, MULT);}
		| F {$$ = $1;}
		;

F		: P_OPEN E P_CLOSE {$$ = ast_create_node_from_ep($2);}
		| N {$$ = $1 ;}
		| V_VAR {$$ = ast_create_node_from_variable($1);}
		;

N		: V {$$ = ast_create_node_from_int($1); }
		;
		
V		: S_MO V {$$ = -1 * $2; }
		| S_PL V {$$ = $2; }
		| V_INT {$$ = $1; }
		;
		
C		: C S_SE C {$$ = ast_create_branch($1, $3);}
		| S_SK { $$ = ast_create_empty_node();}
		| V_VAR S_AF E {$$ = ast_create_aff_node($1, $3); }
		| P_OPEN C P_CLOSE {$$ = ast_create_node_from_cp($2);}
		| S_IF E S_TH CC S_EL CC {$$ = ast_create_ITE_node($2, $4, $6);}
		| S_WH E S_DO CC {$$ = ast_create_WD_node($2, $4);}
		;
		
CC		: S_SK { $$ = ast_create_empty_node();}
		| V_VAR S_AF E {$$ = ast_create_aff_node($1, $3); }
		| P_OPEN C P_CLOSE {$$ = ast_create_node_from_cp($2);}
		| S_IF E S_TH CC S_EL CC {$$ = ast_create_ITE_node($2, $4, $6);}
		| S_WH E S_DO CC {$$ = ast_create_WD_node($2, $4);}
		;
%%

int yyerror(char *s)
{
    fprintf(stderr, "*** ERROR: %s***\n", s);
    return 0;
}

int main()
{
	initialize_ast();
    yyparse();
	c3a_add_etq_cmd("END", c3a_arg_set("ST", "", "", ""));
	init_output();
	c3a_execute();
	end_output();
	fprintf(stderr, "\n*** C3A > Y86 Done ***\n");
    return 0;
}
