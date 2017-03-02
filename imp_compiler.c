#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imp_compiler.h"

variable v_root = NULL;
variable v_current = NULL;
ast_node a_root = NULL;
FILE* output = NULL;
//PRE_AST
variable add_variable(char* name, int value)
{
	variable v = (variable) malloc(sizeof(struct s_variable));
	v->name = (char*) malloc(sizeof(char) * strlen(name));
	strcpy(v->name, name);
	v->value = value;
	v->next = NULL;

	if (v_current != NULL)
		v_current->next = v;

	v_current = v;
	if (v_root == NULL)
		v_root = v;

	return v;
}

variable get_variable(char* name)
{
	variable c = v_root;
	while (c != NULL && strcmp(c->name, name))
		c = c->next;

	if (c == NULL)
		return add_variable(name, 0);

	return c;
}

int get_value_from_variable(char* label)
{
	variable c = v_root;
	while (c != NULL && strcmp(c->name, label))
		c = c->next;

	if (c != NULL)
		return c->value;
	else
		return 0;
}

void assign_value_to_variable(char* label, int value)
{
	variable c = v_root;
	while (c != NULL && strcmp(c->name, label))
		c = c->next;

		if (c != NULL)
			c->value = value;
		else
			add_variable(label, value);
}

char* concat(char* stat1, char* stat2)
{
	char* result = (char*) malloc(sizeof(char) * (2 + strlen(stat1) + strlen(stat2)));
	int i = 0;
	while (i < strlen(stat1))
	{
		result[i] = stat1[i];
		i++;
	}

	result[i] = ';';
	i = 0;
	while (i < strlen(stat2))
	{
		result[strlen(stat1) + i] = stat2[i];
		i++;
	}

	result[strlen(stat1) + i] = '\0';
	return result;
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

	return a;
}

ast_node ast_create_node_from_int(char* value)
{
	return(ast_create_node_from_variable(value));
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

ast_node ast_create_add_node(ast_node left, ast_node right)
{
	ast_node a = new_ast_node(2);
	a->category = OPERATOR;
	a->item = ADD;
	a->childs[0] = left;
	a->childs[1] = right;

	return a;
}

ast_node ast_create_sub_node(ast_node left, ast_node right)
{
	ast_node a = new_ast_node(2);
	a->category = OPERATOR;
	a->item = SUB;
	a->childs[0] = left;
	a->childs[1] = right;

	return a;
}

ast_node ast_create_mult_node(ast_node left, ast_node right)
{
	ast_node a = new_ast_node(2);
	a->category = OPERATOR;
	a->item = MULT;
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
	ast_node a = new_ast_node(3);
	a->category = LOOP;
	a->item = ITE;
	a->childs[0] = condition;
	a->childs[1] = then_block;
	a->childs[2] = else_block;

	return a;
}

ast_node ast_create_WD_node(ast_node condition, ast_node do_block)
{
	ast_node a = new_ast_node(2);
	a->category = LOOP;
	a->item = WD;
	a->childs[0] = condition;
	a->childs[1] = do_block;

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

ast_node ast_create_node_from_ep(ast_node content)
{
	ast_node a = new_ast_node(1);
	a->category = SINGLE_BLOCK;
	a->childs[0] = content;

	return a;
}

ast_node ast_create_node_from_cp(ast_node content)
{
	ast_node a = new_ast_node(1);
	a->category = SINGLE_BLOCK;
	a->childs[0] = content;

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
}

void ast_execute(ast_node root)
{
	switch (root->category) {
		case ROOT:
			ast_execute(root->childs[0]);
		break;

		case EMPTY:
			output_write("", "Sk", "", "", "");
		break;

		case MEMBER:
			switch (root->item) {
				case VAR:
					//root->value = root->var->value;
				break;

				case CONST:
					//:^)
				break;
			}
		break;

		case OPERATOR:
			ast_execute(root->childs[0]);
			ast_execute(root->childs[1]);
			switch (root->item) {
				case ADD:
					//root->value = root->childs[0]->value + root->childs[1]->value;
					output_write("", "Pl", root->childs[0]->svar, root->childs[1]->svar, root->childs[2]->svar);
				break;

				case SUB:
					//root->value = root->childs[0]->value - root->childs[1]->value;
					output_write("", "Mo", root->childs[0]->svar, root->childs[1]->svar, root->childs[2]->svar);
				break;

				case MULT:
					//root->value = root->childs[0]->value * root->childs[1]->value;
					output_write("", "Mu", root->childs[0]->svar, root->childs[1]->svar, root->childs[2]->svar);
				break;

				case AFF:
					//root->childs[0]->var->value = root->childs[1]->value;
					output_write("", "Af", root->childs[0]->svar, root->childs[1]->svar, "");
				break;
			}
		break;

		/*case LOOP:
			switch (root->item) {
				case ITE:
					ast_execute(root->childs[0]);
					if (root->childs[0]->value)
						ast_execute(root->childs[1]);
					else
						ast_execute(root->childs[2]);
				break;

				case WD:
					ast_execute(root->childs[0]);
					while(root->childs[0]->value)
					{
						ast_execute(root->childs[1]);
						ast_execute(root->childs[0]);
					}
				break;
			}
		break;

		case BRANCH:
			ast_execute(root->childs[0]);
			ast_execute(root->childs[1]);
		break;

		case SINGLE_BLOCK:
			ast_execute(root->childs[0]);
			root->value = root->childs[0]->value;
		break;*/
	}

}

//UTIL
void create_output_file()
{
	output = fopen("output.c3a", "w");
}

void output_write(char* etq, char* op, char* arg1, char* arg2, char* dst)
{
	fprintf(stderr, "%s\t:%s\t:%s\t:%s\t:%s\n", etq, op, arg1, arg2, dst);
}

void display_env()
{
	printf("*** ENV ***\n");
	while(v_root != NULL)
	{
		printf("Var %s : %d\n", v_root->name, v_root->value);
		v_root = v_root->next;
	}

	printf("*** END ***\n");
}

void display_ast_tree(ast_node root, int stage)
{
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
