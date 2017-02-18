#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imp_interpreter.h"

variable v_v_root = NULL;
variable v_current = NULL;
ast_node a_root = NULL;
//PRE_AST
void add_variable(char* name, int value)
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
}

var get_variable(char* name)
{
	variable c = v_root;
	while (c != NULL && strcmp(c->name, label))
		c = c->next;

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
	a->category = NULL;
	a->item = NULL;
	a->var = NULL;
	a->value = NULL;
	a->child_num = size;
	a->childs = (ast_node*) malloc(sizeof(ast_node) * size);
	return ast_node;
}

ast_node ast_create_node_from_int(int value)
{
	ast_node a = new_ast_node(0);
	a->category = MEMBER;
	a->item = CONST;
	a->value = value;

	return a;
}

ast_node ast_create_node_from_variable(char* name)
{
	ast_node a = new_ast_node(0);
	a->category = MEMBER;
	a->item = VAR;
	a->var = get_variable(name);

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

void initialize_ast()
{
	a_root = (ast_node) malloc(sizeof(struct s_ast_node));
	a_root->category = ROOT;
	a_root->item = NULL;
	a_root->var = NULL;
	a_root->value = NULL;
	a_root->child_num = 1;
	a_root->childs = (ast_node*) malloc(sizeof(ast_node));
}

//UTIL
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
