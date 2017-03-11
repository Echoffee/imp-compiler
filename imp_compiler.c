#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imp_compiler.h"

variable v_root = NULL;
variable v_current = NULL;
ast_node a_root = NULL;
ast_node a_current_branch = NULL;
etq_cmd e_root = NULL;
etq_cmd e_current = NULL;
FILE* output = NULL;
char* current_name = NULL;
int current_name_n = NULL;
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

void set_name()
{
	int cn = current_name_n * 10;
	current_name[0] = 'E'; 
	current_name[1] = 'T';
	int i = 0;
	do {
		cn = cn / 10;
		i++;
		current_name[1 + i] = '0' + cn % 10;
	}while (cn / 10 > 0);
	current_name[2 + i] = '\0';
	
	current_name_n++;
}

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

etq_cmd get_etq_cmd_from_ast(ast_node n)
{
	etq_cmd c = e_root;
	while (c != NULL && c == n)
		c = c->next;

	if (c == NULL)
	{
		fprintf(stderr, "ERROR WHILE GETTING ETQ_CMD");
	}

	return c;
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
	a->sname = (char*) malloc (sizeof(char) * 8);
	strcpy(a->sname, current_name);
	set_name();
	return a;
}

ast_node ast_create_node_from_int(char* value)	//To be reviewed
{
	ast_node a = ast_create_node_from_variable(value);
	fprintf(stderr, "********%s********\n", a->sname);
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

ast_node ast_create_add_node(ast_node left, ast_node right)
{
	ast_node a = new_ast_node(2);
	a->category = OPERATOR;
	a->item = ADD;
	a->childs[0] = left;
	a->childs[1] = right;
	a->svar = (char*) malloc(sizeof(char) * 6);
	strcpy(a->svar, "_TEMP");

	return a;
}

ast_node ast_create_sub_node(ast_node left, ast_node right)
{
	ast_node a = new_ast_node(2);
	a->category = OPERATOR;
	a->item = SUB;
	a->childs[0] = left;
	a->childs[1] = right;
	a->svar = (char*) malloc(sizeof(char) * 6);
	strcpy(a->svar, "_TEMP");

	return a;
}

ast_node ast_create_mult_node(ast_node left, ast_node right)
{
	ast_node a = new_ast_node(2);
	a->category = OPERATOR;
	a->item = MULT;
	a->childs[0] = left;
	a->childs[1] = right;
	a->svar = (char*) malloc(sizeof(char) * 6);
	strcpy(a->svar, "_TEMP");

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

ast_node ast_create_jmp_node(ast_node dst)
{
	ast_node a = new_ast_node(1);
	a->category = JMP;
	a->childs[0] = dst;
	
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
	current_name = (char*) malloc(sizeof(char) * 8);
	current_name_n = 0;
	strcpy(current_name, "ET0");
	
}

void ast_execute(ast_node root)
{
	char* etq = NULL;
	
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
					output_write(root->sname, "Afc", root->svar, "", "_TEMP");
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
					output_write(root->sname, "Pl", root->childs[0]->svar, "_TEMP", "_TEMP");
				break;

				case SUB:
					//root->value = root->childs[0]->value - root->childs[1]->value;
					output_write(root->sname, "Mo", root->childs[0]->svar, "_TEMP", "_TEMP");
				break;

				case MULT:
					//root->value = root->childs[0]->value * root->childs[1]->value;
					output_write(root->sname, "Mu", root->childs[0]->svar, "_TEMP", "_TEMP");
				break;

				case AFF:
					//root->childs[0]->var->value = root->childs[1]->value;
					if (root->childs[1]->item = CONST)
						output_write("", "Af", root->childs[0]->svar, "_TEMP", "");
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
					output_write("", "Jz", root->childs[0]->svar, "", etq);
					ast_execute(root->childs[1]);
					output_write("", "Jp", "", "", after->sname);
					ast_execute(root->childs[2]);
					output_write(after->sname, "Sk", "", "", "");
				}
				break;
//////////////////////////////////////////////
				case WD:
				{
					ast_node after = ast_create_empty_node();
					output_write("", "Jz", root->childs[0]->svar, "", after->sname);
					ast_execute(root->childs[1]);
					output_write("", "Jp", "", "", root->sname);
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
			output_write(root->sname, "Jp", "", "", etq);
			break;

		case SINGLE_BLOCK:
			ast_execute(root->childs[0]);
			root->value = root->childs[0]->value;
		break;
	}

}

//UTIL
void create_output_file()
{
	output = fopen("output.c3a", "w");
}

void output_write(char* etq, char* op, char* arg1, char* arg2, char* dst)
{
	fprintf(stdout, "%s\t:%s\t:%s\t:%s\t:%s\n", etq, op, arg1, arg2, dst);
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
