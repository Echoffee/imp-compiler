%error-verbose

%{
    #include <stdio.h>
	#include "c3a_interpreter.h"
    int yylex();
    int yyerror(char* s);
%}
%union{
    int ival;
	char* sval;

	struct s_ast_node* node;
}

%token S_AF S_AFC S_SK S_SE S_PL S_MO S_MU S_JP S_JZ S_ST
%token O_POS O_NEG
%token S_EOL
%token V_INT V_VAR

%type<node> C
%type<node> L
%type<sval> ETQ
%type<node> V
%type<node> CMD

%type<ival> V_INT
%type<sval> V_VAR

%start STRT
%%
STRT	: C {/*display_ast_tree($1, 0);*/ ast_execute($1); fprintf(stderr, "\n*** Done ***\n");}

C		: C S_EOL C {$$ = ast_create_branch($1, $3);}
		| L { $$ = $1; }
		;

L		: ETQ S_SE CMD { $$ = ast_create_label_cmd($1, $3);}
		|     S_SE CMD { $$ = $2; }
		;
		
ETQ		: V_VAR { $$ = $1;}
		;

V		: V_INT { $$ = ast_create_node_from_int($1);}
		| V_VAR { $$ = ast_create_node_from_variable($1);}
		| O_POS V { $$ = ast_create_op_node(1, $2); }
		| O_NEG V { $$ = ast_create_op_node(-1, $2); }
		;

CMD		: S_PL S_SE V S_SE V S_SE V_VAR { $$ = ast_create_o_node($3, $5, $7, ADD); }
		| S_MO S_SE V S_SE V S_SE V_VAR { $$ = ast_create_o_node($3, $5, $7, SUB); }
		| S_MU S_SE V S_SE V S_SE V_VAR { $$ = ast_create_o_node($3, $5, $7, MULT); }
		| S_AF S_SE V_VAR S_SE V S_SE { $$ = ast_create_aff_node($3, $5); }
		| S_AFC S_SE V S_SE S_SE V_VAR { $$ = ast_create_aff_node($6, $3); }
		| S_SK S_SE S_SE S_SE { $$ = ast_create_empty_node(); }
		| S_SK S_SE S_SE S_SE V_VAR{ $$ = ast_create_empty_node(); }
		| S_JP S_SE S_SE S_SE ETQ { $$ = ast_create_jmp_node($5); }
		| S_JZ S_SE V S_SE S_SE ETQ { $$ = ast_create_cond_jmp_node($3, $6); }
		| S_ST S_SE S_SE S_SE { $$ = ast_create_stop_node(); }
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
