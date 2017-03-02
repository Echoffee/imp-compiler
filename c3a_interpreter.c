#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c3a_interpreter.h"

variable v_root = NULL;
variable v_current = NULL;
etq_cmd e_root = NULL;
etq_cmd e_current = NULL;
ast_node a_root = NULL;
ast_node a_current_skip = NULL;
int global_stop = 0;
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
//-------------------

etq_cmd add_etq_cmd(char* name, ast_node cmd)
{
	etq_cmd e = (etq_cmd) malloc(sizeof(struct s_etq_cmd));
	e->name = (char*) malloc(sizeof(char) * strlen(name));
	strcpy(e->name, name);
	e->cmd = cmd;
	e->next = NULL;

	if (e_current != NULL)
		e_current->next = e;

	e_current = e;
	if (e_root == NULL)
		e_root = e;

	return e;
}

etq_cmd get_etq_cmd(char* name)
{
	etq_cmd c = e_root;
	while (c != NULL && strcmp(c->name, name))
		c = c->next;

	if (c == NULL)
	{
		fprintf(stderr, "ERROR WHILE GETTING ETQ_CMD **%s**\n", name);
	}

	return c;
}

ast_node get_node_from_etq_cmd(char* name)
{
	etq_cmd c = e_root;
	while (c != NULL && strcmp(c->name, name))
		c = c->next;

	if (c != NULL)
		return c->cmd;
	else
		return NULL;
}

void assign_cmd_to_etq_cmd(char* name, ast_node cmd)
{
	etq_cmd c = e_root;
	while (c != NULL && strcmp(c->name, name))
		c = c->next;

		if (c != NULL)
			c->cmd = cmd;
		else
			fprintf(stderr, "ERROR WHILE ASSIGNING CMD TO ETQ_CMD **%s**\n", name);
			//add_variable(name, value);
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

ast_node ast_create_op_node(int factor, ast_node value)
{
	ast_node a = new_ast_node(1);
	a->category = MEMBER;
	a->item = FACT;
	a->value = factor;
	a->childs[0] = value;

	return a;
}

ast_node ast_create_add_node(ast_node left, ast_node right, char* dest)
{
	ast_node a = new_ast_node(2);
	a->category = OPERATOR;
	a->item = ADD;
	a->childs[0] = left;
	a->childs[1] = right;
	a->var = get_variable(dest);

	return a;
}

ast_node ast_create_sub_node(ast_node left, ast_node right, char* dest)
{
	ast_node a = new_ast_node(2);
	a->category = OPERATOR;
	a->item = SUB;
	a->childs[0] = left;
	a->childs[1] = right;
	a->var = get_variable(dest);

	return a;
}

ast_node ast_create_mult_node(ast_node left, ast_node right, char* dest)
{
	ast_node a = new_ast_node(2);
	a->category = OPERATOR;
	a->item = MULT;
	a->childs[0] = left;
	a->childs[1] = right;
	a->var = get_variable(dest);

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

ast_node ast_create_branch(ast_node left, ast_node right)
{
	ast_node a = new_ast_node(2);
	a->category = BRANCH;
	a->item = NONE;
	a->childs[0] = left;
	a->childs[1] = right;
	if (left->childs[0]->category == EMPTY)
	{
		left->childs[0]->childs[0] = a;
	}	
	return a;
}

ast_node ast_create_label_cmd(char* label, ast_node command)
{
	ast_node a = new_ast_node(1);
	a->category = LABEL;
	a->item = NONE;
	a->childs[0] = command;
	add_etq_cmd(label, command);

	return a;
}

ast_node ast_create_empty_node()
{
	ast_node a = new_ast_node(1);
	a->category = EMPTY;
	a->childs[0] = NULL;
	a_current_skip = a;
	return a;
}

ast_node ast_create_jmp_node(char* etq)
{
	ast_node a = new_ast_node(1);
	a->category = JMP;
	a->item = NONE;
	a->value = 0;
	a->childs[0] = ast_create_goto_node(etq);
	
	return a;
}

ast_node ast_create_cond_jmp_node(ast_node cond, char* etq)
{
	ast_node a = new_ast_node(2);
	a->category = JMPC;
	a->item = NONE;
	a->value = 0;
	a->childs[0] = cond;
	a->childs[1] = ast_create_goto_node(etq);
	
	return a;
}

ast_node ast_create_goto_node(char* etq)
{
	ast_node a = new_ast_node(1);
	a->category = MEMBER;
	a->item = ETQ;
	a->svar = (char*) malloc(strlen(etq) * sizeof(char));
	a->childs[0] = NULL;
	strcpy(a->svar, etq);
	
	return a;
}

ast_node ast_create_stop_node()
{
	ast_node a = new_ast_node(0);
	a->category = STOP;
	a->item = NONE;
	
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
	if (global_stop)
		return;
		
	fprintf(stderr, "NODE %d, %d\n", root->category, root->item);
	switch (root->category) {
		case ROOT:
			ast_execute(root->childs[0]);
		break;

		case EMPTY:
			ast_execute(root->childs[0]->childs[1]);
		break;

		case MEMBER:
			switch (root->item) {
				case VAR:
					root->value = root->var->value;
				break;

				case CONST:
					//:^)
				break;
				
				case ETQ:
					root->childs[0] = get_node_from_etq_cmd(root->svar);
				break;
			}
		break;

		case OPERATOR:
			ast_execute(root->childs[0]);
			ast_execute(root->childs[1]);
			switch (root->item) {
				case ADD:
					root->var->value = root->childs[0]->value + root->childs[1]->value;
				break;

				case SUB:
					root->var->value = root->childs[0]->value - root->childs[1]->value;
				break;

				case MULT:
					root->var->value = root->childs[0]->value * root->childs[1]->value;
				break;

				case AFF:
					root->childs[0]->var->value = root->childs[1]->value;
				break;
			}
		break;
		
		case LABEL:
			ast_execute(root->childs[0]);
		break;
		
		case JMP:
			ast_execute(root->childs[0]);
			ast_execute(root->childs[0]->childs[0]);
		break;

		case JMPC:
			ast_execute(root->childs[0]);
			ast_execute(root->childs[1]);
			if (!root->childs[0]->value)
				ast_execute(root->childs[1]->childs[0]);
		break;
		
		case STOP:
			global_stop = 1;
		return;

		case BRANCH:
			ast_execute(root->childs[0]);
			ast_execute(root->childs[1]);
		break;

		case SINGLE_BLOCK:
			ast_execute(root->childs[0]);
			root->value = root->childs[0]->value;
		break;
	}

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

void display_ast_tree(ast_node root, int stage)
{
	if (root == NULL)
		return;
	
	for (int i = 0; i < stage; i++)
		fprintf(stderr, "|");
	fprintf(stderr, "Node type : %d, %d\n", root->category, root->item );

	if (root->category != EMPTY)
		for (int i = 0; i < root->child_num; i++)
		{
			display_ast_tree(root->childs[i], stage + 1);
		}
	
	for (int i = 0; i < stage; i++)
		fprintf(stderr, "|");

	fprintf(stderr, "__________________\n");
}
