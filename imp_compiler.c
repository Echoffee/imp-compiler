#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imp_compiler.h"

variable v_root = NULL;
ast_node a_root = NULL;
ast_node a_current_branch = NULL;
int current_node_name_n = 0;
char* prev_buff_name = NULL;
char* current_buff_name = NULL;
int current_buff_name_n = 0;

void set_node_name()
{
	current_node_name_n++;
}

char* set_var_name()
{
	strcpy(prev_buff_name, current_buff_name);
	int cn = current_buff_name_n * 10;
	current_buff_name[0] = 'V'; 
	current_buff_name[1] = 'A';
	int i = 0;
	do {
		cn = cn / 10;
		i++;
		current_buff_name[1 + i] = '0' + cn % 10;
	}while (cn / 10 > 0);
	current_buff_name[2 + i] = '\0';
	
	current_buff_name_n++;
	
	return current_buff_name;
}

//AST
ast_node new_ast_node(int size)
{
	ast_node a = (ast_node) malloc(sizeof(struct s_ast_node));
	a->category = EMPTY;
	a->item = NONE;
	a->var = NULL;
	a->value = 0;
	a->child_num = size;
	a->childs = (ast_node*) malloc(sizeof(ast_node) * size);
	a->svar = NULL;
	a->sname = current_node_name_n;
	set_node_name();
	return a;
}

ast_node ast_create_node_from_int(char* value)	//To be reviewed
{
	ast_node a = ast_create_node_from_variable(value);
	a->item = CONST;
	return a;
}

ast_node ast_create_node_from_variable(char* name)
{
	ast_node a = new_ast_node(0);
	a->category = MEMBER;
	a->item = VAR;
	a->svar = (char*) malloc(strlen(name) * sizeof(char));
	strcpy(a->svar, name);

	return a;
}

ast_node ast_create_o_node(ast_node left, ast_node right, node_item item)
{
	ast_node a = new_ast_node(2);
	a->category = OPERATOR;
	a->item = item;
	a->childs[0] = left;
	a->childs[1] = right;

	return a;
}

ast_node ast_create_aff_node(char* name, ast_node value)
{
	ast_node a = new_ast_node(2);
	a->category = OPERATOR;
	a->item = AFF;
	a->childs[0] = ast_create_node_from_variable(name);
	a->childs[1] = value;

	return a;
}

ast_node ast_create_ITE_node(ast_node condition, ast_node then_block, ast_node else_block)
{
	ast_node a = new_ast_node(4);
	a->category = LOOP;
	a->item = ITE;
	a->childs[0] = condition;
	a->childs[1] = then_block;
	a->childs[2] = else_block;
	a->childs[3] = NULL;

	return a;
}

ast_node ast_create_WD_node(ast_node condition, ast_node do_block)
{
	fprintf(stderr, "ast_create_WD_node\n");
	ast_node a = new_ast_node(3);
	a->category = LOOP;
	a->item = WD;
	a->childs[0] = condition;
	a->childs[1] = do_block;
	a->childs[2] = NULL;

	return a;
}

ast_node ast_create_branch(ast_node left, ast_node right)
{
	ast_node a = new_ast_node(2);
	a->category = BRANCH;
	a->childs[0] = left;
	a->childs[1] = right;

	return a;
}

ast_node ast_create_empty_node()
{
	ast_node a = new_ast_node(0);
	a->category = EMPTY;
	return a;
}


void initialize_ast()
{
	a_root = (ast_node) malloc(sizeof(struct s_ast_node));
	a_root->category = ROOT;
	a_root->item = NONE;
	a_root->var = NULL;
	a_root->value = 0;
	a_root->child_num = 1;
	a_root->childs = (ast_node*) malloc(sizeof(ast_node));
	current_node_name_n = 0;
}

void ast_execute(ast_node root)
{
	int etq = -1;
	
	switch (root->category) {
		case ROOT:
			ast_execute(root->childs[0]);
		break;

		case EMPTY:
			output_write(root->sname, "Sk", "", "", "");
		break;

		case MEMBER:
			switch (root->item) {
				case VAR:
					//root->value = root->var->value;
				break;

				case CONST:
					output_write(root->sname, "Afc", root->svar, "", "_CONST");
					//:^)
				break;
			}
		break;

		case OPERATOR:
			ast_execute(root->childs[1]);
			ast_execute(root->childs[0]);
			switch (root->item) {
				case ADD:
					//root->value = root->childs[0]->value + root->childs[1]->value;
					if (root->childs[1]->item = CONST)
						output_write(-1, "Pl", root->childs[0]->svar, "_CONST", "_TEMP");
					else
						output_write(root->sname, "Pl", root->childs[0]->svar, "_TEMP", "_TEMP");
				break;

				case SUB:
					//root->value = root->childs[0]->value - root->childs[1]->value;
					if (root->childs[1]->item = CONST)
						output_write(-1, "Mo", root->childs[0]->svar, "_CONST", "_TEMP");
					else
						output_write(root->sname, "Mo", root->childs[0]->svar, "_TEMP", "_TEMP");
				break;

				case MULT:
					//root->value = root->childs[0]->value * root->childs[1]->value;
					if (root->childs[1]->item = CONST)
						output_write(-1, "Mu", root->childs[0]->svar, "_CONST", "_TEMP");
					else
						output_write(root->sname, "Mu", root->childs[0]->svar, "_TEMP", "_TEMP");
				break;

				case AFF:
					//root->childs[0]->var->value = root->childs[1]->value;
					if (root->childs[1]->item = CONST)
						output_write(-1, "Af", root->childs[0]->svar, "_CONST", "");
					else
						output_write(root->sname, "Af", root->childs[0]->svar, "_TEMP", "");
					
				break;
			}
		break;

		case LOOP:
			output_write(root->sname, "Sk", "", "", "");
			switch (root->item) {
				case ITE:
				{
					if (root->childs[2]->item == AFF)
						etq = root->childs[2]->childs[1]->sname;
					else
						etq = root->childs[2]->sname;
						
					ast_node after = ast_create_empty_node();
					output_write(etq, "Jz", root->childs[0]->svar, "", "@");
					ast_execute(root->childs[1]);
					output_write(after->sname, "Jp", "", "", "@");
					ast_execute(root->childs[2]);
					output_write(after->sname, "Sk", "", "", "");
				}
				break;

				case WD:
				{
					ast_node after = ast_create_empty_node();
					output_write(after->sname, "Jz", root->childs[0]->svar, "", "@");
					ast_execute(root->childs[1]);
					output_write(root->sname, "Jp", "", "", "@");
					output_write(after->sname, "Sk", "", "", "");
				}
				break;
			}
		break;

		case BRANCH:
			a_current_branch = root;
			ast_execute(root->childs[0]);
			ast_execute(root->childs[1]);
		break;
		
		case JMP:
			//if (root->childs[0]->item == AFF)
				etq = root->childs[0]->childs[1]->sname;
			//	else
			//etq = root->childs[0]->sname;
			output_write(etq, "Jp", "", "", "@");
			break;

		case SINGLE_BLOCK:
			ast_execute(root->childs[0]);
			root->value = root->childs[0]->value;
		break;
	}

}

ast_node ast_create_node_from_ep(ast_node content)		
{		
 	ast_node a = new_ast_node(1);		
 	a->category = SINGLE_BLOCK;		
 	a->childs[0] = content;		
	a->svar = (char*) malloc(sizeof(char) * 6);
	strcpy(a->svar, "_TEMP");
 		
 	return a;		
}		
 		
ast_node ast_create_node_from_cp(ast_node content)		
{		
 	ast_node a = new_ast_node(1);		
 	a->category = SINGLE_BLOCK;		
 	a->childs[0] = content;		
	a->svar = (char*) malloc(sizeof(char) * 6);
	strcpy(a->svar, "_TEMP");
 
  	return a;		
}

//UTIL

void output_write(int etq, char* op, char* arg1, char* arg2, char* dst)
{
	if (etq < 0)
		fprintf(stdout, "\t:%s\t:%s\t:%s\t:%s\n", op, arg1, arg2, dst);
	else
	{
		if (strlen(dst) > 0 && dst[0] == '@')
			fprintf(stdout, "\t:%s\t:%s\t:%s\t:ET%d\n", op, arg1, arg2, etq);
		else
			fprintf(stdout, "ET%d\t:%s\t:%s\t:%s\t:%s\n", etq, op, arg1, arg2, dst);
	}

}

void display_env()
{
	
	fprintf(stderr, "*** ENV ***\n");
	while(v_root != NULL)
	{
		printf("Var %s : %d\n", v_root->name, v_root->value);
		v_root = v_root->next;
	}

	fprintf(stderr, "*** END ***\n");
}

void display_ast_tree(ast_node root, int stage)
{
	if (root == NULL)
		return;
	
	for (int i = 0; i < stage; i++)
		fprintf(stderr, "|");
	fprintf(stderr, "Node type : %d, %d\n", root->category, root->item );

	for (int i = 0; i < root->child_num; i++)
	{
		display_ast_tree(root->childs[i], stage + 1);
	}
	for (int i = 0; i < stage; i++)
		fprintf(stderr, "|");

	fprintf(stderr, "__________________\n");
}
