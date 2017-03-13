%error-verbose

%{
    #include <stdio.h>
	#include "c3a_compiler.h"
    int yylex();
    int yyerror(char* s);
%}
%union{
    int ival;
	char* sval;
	char** asval;

	struct s_ast_node* node;
}

%token S_AF S_AFC S_SK S_SE S_PL S_MO S_MU S_JP S_JZ S_ST
%token O_POS O_NEG
%token S_EOL
%token V_INT V_VAR

%type<sval> V_INT
%type<sval> V_VAR
%type<asval> CMD
%type<sval> V

%start STRT
%%
STRT	: C {init_output(); c3a_execute(); end_output(); fprintf(stderr, "\n*** Done ***\n");}

C		: C S_EOL C { }
		| L { }
		;

L		: V_VAR S_SE CMD { c3a_add_etq_cmd($1, $3);}
		|     S_SE CMD { c3a_add_cmd($2); }
		;

V		: V_INT { $$ = $1;}
		| V_VAR { $$ = $1;}
		| O_POS V { $$ = $2; }
		| O_NEG V { $$ = invert_value($2); }
		;

CMD		: S_PL S_SE V S_SE V S_SE V_VAR { $$ = c3a_arg_set("PL", $3, $5, $7); }
		| S_MO S_SE V S_SE V S_SE V_VAR { $$ = c3a_arg_set("MO", $3, $5, $7); }
		| S_MU S_SE V S_SE V S_SE V_VAR { $$ = c3a_arg_set("MU", $3, $5, $7); }
		| S_AF S_SE V_VAR S_SE V S_SE { $$ = c3a_arg_set("AF", $3, $5, ""); }
		| S_AFC S_SE V S_SE S_SE V_VAR { $$ = c3a_arg_set("AFC", $6, $3, ""); }
		| S_SK S_SE S_SE S_SE { $$ = c3a_arg_set("SK", "", "", ""); }
		| S_SK S_SE S_SE S_SE V_VAR{ $$ = c3a_arg_set("SK", "", "", ""); }
		| S_JP S_SE S_SE S_SE V_VAR { $$ = c3a_arg_set("JP", $5, "", ""); }
		| S_JZ S_SE V S_SE S_SE V_VAR { $$ = c3a_arg_set("JZ", $3, $6, ""); }
		| S_ST S_SE S_SE S_SE { $$ = c3a_arg_set("ST", "", "", ""); }
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
    return 0;
}
