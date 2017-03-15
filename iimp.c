#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iimp.h"

variable v_root = NULL;
ast_node a_root = NULL;
int current_node_name_n = 0;
int current_var_name_n = 0;

void set_node_name()
{
	current_node_name_n++;
}

void set_var_name()
{
	current_var_name_n++;
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

ast_node ast_create_node_from_int(int value)
{
	char n[5];
	sprintf(n, "V%d", current_var_name_n);
	ast_node a = ast_create_node_from_variable(n);
	a->value = value;
	a->item = CONST;
	set_var_name();	
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
	char n[8];
	
	switch (root->category) {
		case ROOT:
			ast_execute(root->childs[0]);
		break;

		case EMPTY:
			c3a_write(root->sname, "SK", "", "", "");
		break;

		case MEMBER:
			switch (root->item) {
				case VAR:
					//root->value = root->var->value;
				break;

				case CONST:
				sprintf(n, "%d", root->value);
					c3a_write(root->sname, "AFC", n, "", root->svar);
					//:^)
				break;
			}
		break;

		case OPERATOR:
		{
			
			ast_execute(root->childs[1]);
			ast_execute(root->childs[0]);
			root->svar = (char*) malloc(sizeof(char) * 5);
			sprintf(root->svar, "V%d", current_var_name_n);
			set_var_name();
			switch (root->item) {
				case ADD:
					//root->value = root->childs[0]->value + root->childs[1]->value;
					if (root->childs[1]->item = CONST)
						c3a_write(-1, "PL", root->childs[0]->svar, root->childs[1]->svar, root->svar);
					else
						c3a_write(root->sname, "PL", root->childs[0]->svar, root->childs[1]->svar, root->svar);
				break;

				case SUB:
					//root->value = root->childs[0]->value - root->childs[1]->value;
					if (root->childs[1]->item = CONST)
						c3a_write(-1, "MO", root->childs[0]->svar, root->childs[1]->svar, root->svar);
					else
						c3a_write(root->sname, "MO", root->childs[0]->svar, root->childs[1]->svar, root->svar);
				break;

				case MULT:
					//root->value = root->childs[0]->value * root->childs[1]->value;
					if (root->childs[1]->item = CONST)
						c3a_write(-1, "MU", root->childs[0]->svar, root->childs[1]->svar, root->svar);
					else
						c3a_write(root->sname, "MU", root->childs[0]->svar, root->childs[1]->svar, root->svar);
				break;

				case AFF:
					//root->childs[0]->var->value = root->childs[1]->value;
					if (root->childs[1]->item = CONST)
						c3a_write(-1, "AF", root->childs[0]->svar, root->childs[1]->svar, "");
					else
						c3a_write(root->sname, "AF", root->childs[0]->svar, root->childs[1]->svar, "");
					
				break;
			}
			
		}
			
		break;

		case LOOP:
			c3a_write(root->sname, "SK", "", "", "");
			switch (root->item) {
				case ITE:
				{
					ast_execute(root->childs[0]);
					if (root->childs[2]->item == AFF)
						etq = root->childs[2]->childs[1]->sname;
					else
						etq = root->childs[2]->sname;
					
					ast_node after = ast_create_empty_node();
					c3a_write(etq, "JZ", root->childs[0]->svar, "", "@");
					ast_execute(root->childs[1]);
					c3a_write(after->sname, "JP", "", "", "@");
					c3a_write(etq, "SK", "", "", "");
					ast_execute(root->childs[2]);
					c3a_write(after->sname, "SK", "", "", "");
				}
				break;

				case WD:
				{
					ast_execute(root->childs[0]);
					ast_node after = ast_create_empty_node();
					c3a_write(after->sname, "JZ", root->childs[0]->svar, "", "@");
					ast_execute(root->childs[1]);
					c3a_write(root->sname, "JP", "", "", "@");
					c3a_write(after->sname, "SK", "", "", "");
				}
				break;
			}
		break;

		case BRANCH:
			ast_execute(root->childs[0]);
			ast_execute(root->childs[1]);
		break;
		
		case JMP:
			etq = root->childs[0]->childs[1]->sname;
			c3a_write(etq, "JP", "", "", "@");
		break;

		case SINGLE_BLOCK:
			ast_execute(root->childs[0]);
			root->value = root->childs[0]->value;
			root->svar = root->childs[0]->svar;
		break;
	}

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

//C3A
c3a_cmd c_root = NULL;
c3a_cmd c_current = NULL;
var var_root = NULL;
var var_current = NULL;
int v_pos = 4;


void new_c3a_cmd(char* etq, char* cmd, char* arg1, char* arg2, char* dst)
{
	c3a_cmd a = (c3a_cmd) malloc(sizeof(struct s_c3a_cmd));
	a->etq = (char*) malloc(sizeof(char) * 12);
	strcpy(a->etq, etq);
	c3a_op op = EMPTY;
	{
		if (!strcmp(cmd, "PL"))
			op = PL;
		if (!strcmp(cmd, "MO"))
			op = MO;
		if (!strcmp(cmd, "MU"))
			op = MU;
		if (!strcmp(cmd, "AF"))
			op = AF;
		if (!strcmp(cmd, "AF"))
			op = AF;
		if (!strcmp(cmd, "AFC"))
			op = AFC;
		if (!strcmp(cmd, "SK"))
			op = SK;
		if (!strcmp(cmd, "JP"))
			op = JP;
		if (!strcmp(cmd, "JZ"))
			op = JZ;
		if (!strcmp(cmd, "ST"))
			op = ST;
	}
	
	a->cmd = op;
	a->arg1 = (char*) malloc(sizeof(char) * 12);
	strcpy(a->arg1, arg1);
	a->arg2 = (char*) malloc(sizeof(char) * 12);
	strcpy(a->arg2, arg2);
	a->dst = (char*) malloc(sizeof(char) * 12);
	strcpy(a->dst, dst);
	a->next = NULL;
	
	if (c_root == NULL)
		c_root = a;
	
	if (c_current != NULL)
		c_current->next = a;
	
	c_current = a;
}

var new_var(char* name)
{
	var v = (var) malloc(sizeof(struct s_var));
	v->name = (char*) malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(v->name, name);
	v->pos = v_pos;
	v_pos += 4;
	v->next = NULL;
	
	if (var_root == NULL)
		var_root = v;
		
	if (var_current != NULL)
		var_current->next = v;
	
	var_current = v;
	return v;
}

int get_var_pos(char* var_name)
{
	var v = var_root;
	while(v != NULL && strcmp(var_name, v->name))
		v = v->next;
		
	if (v == NULL)
		v = new_var(var_name);
		
	return v->pos;
}

void c3a_add_etq_cmd(char* etq, char** args)
{
	new_c3a_cmd(etq, args[0], args[1], args[2], args[3]);
}

void c3a_add_cmd(char** args)
{
	new_c3a_cmd("", args[0], args[1], args[2], args[3]);
}

char** c3a_arg_set(char* cmd, char* arg1, char* arg2, char* dst)
{
	char** s = (char**) malloc(sizeof(char*) * 4);
	s[0] = cmd;
	s[1] = arg1;
	s[2] = arg2;
	s[3] = dst;
	
	return s;
}

char* invert_value(char* value)
{
	char* s = (char*) malloc(sizeof(char) * strlen(value) + 2);
	s[0] = '-';
	for (int i = 0; i < strlen(value) + 1; i++)
		s[i + 1] = value[i];
		
	return s;
}
//-------------------


void init_output()
{
	printf("                  .pos      0\n");
	printf("INIT      :irmovl Data,     %%edx\n");
	printf("           irmovl 256,      %%eax\n");
	printf("           addl   %%edx,     %%eax\n");
	printf("           rrmovl %%eax,     %%esp\n");
	printf("           rrmovl %%eax,     %%ebp\n");
}

void end_output()
{
	printf("MUL       :nop\n");
	printf("           mrmovl 4(%%esp),  %%eax\n");
	printf("           mrmovl 8(%%esp),  %%ebx\n");
	printf("           andl   %%eax,     %%eax\n");
	printf("           je     END\n");
	printf("SIGN      :nop\n");
	printf("           jg     MULPLUS\n");
	printf("           irmovl 0,        %%ecx\n");
	printf("           subl   %%eax,     %%ecx\n");
	printf("           rrmovl %%ecx,     %%eax\n");
	printf("           rmmovl %%eax,     4(%%esp)\n");
	printf("           irmovl 0,        %%ecx\n");
	printf("           subl   %%ebx,     %%ecx\n");
	printf("           rrmovl %%ecx,     %%ebx\n");
	printf("           rmmovl %%ebx,     8(%%esp)\n");
	printf("MULPLUS   :nop\n");
	printf("           mrmovl 4(%%esp),  %%eax\n");
	printf("           andl   %%eax,     %%eax\n");
	printf("           je     END\n");
	printf("           irmovl 1,        %%esi\n");
	printf("           subl   %%esi,     %%eax\n");
	printf("           mrmovl 8(%%esp),  %%ebx\n");
	printf("           pushl  %%ebx\n");
	printf("           pushl  %%eax\n");
	printf("           call   MULPLUS\n");
	printf("           popl   %%eax\n");
	printf("           popl   %%eax\n");
	printf("           mrmovl 0(%%edx),  %%eax\n");
	printf("           mrmovl 8(%%esp),  %%ebx\n");
	printf("           addl   %%ebx,     %%eax\n");
	printf("           rmmovl %%eax,     0(%%edx)\n");
	printf("           ret\n");
	printf("END       :irmovl 0,        %%eax\n");
	printf("           rmmovl %%eax,     0(%%edx)\n");
	printf("           ret\n");
	printf("                  .align    8\n");
	printf("Data      :");
}

void c3a_execute()
{
	while (c_root != NULL)
	{
		switch (c_root->cmd) {
			case ZERO:
				// :thinking:
			break;
			
			case PL:
			{
				char v1[16];
				sprintf(v1, "%d(%%edx)", get_var_pos(c_root->arg1));
				char v2[16];
				sprintf(v2, "%d(%%edx)", get_var_pos(c_root->arg2));
				char v3[16];
				sprintf(v3, "%d(%%edx)", get_var_pos(c_root->dst));
				output_write(c_root->etq, "mrmovl", v1, "%eax");
				output_write("", "mrmovl", v2, "%ebx");
				output_write("", "addl", "%eax", "%ebx");
				output_write("", "rmmovl", "%ebx", v3);
			}
			break;
			
			case MO:
			{
				char v1[16];
				sprintf(v1, "%d(%%edx)", get_var_pos(c_root->arg1));
				char v2[16];
				sprintf(v2, "%d(%%edx)", get_var_pos(c_root->arg2));
				char v3[16];
				sprintf(v3, "%d(%%edx)", get_var_pos(c_root->dst));
				output_write(c_root->etq, "mrmovl", v1, "%ebx");
				output_write("", "mrmovl", v2, "%eax");
				output_write("", "subl", "%eax", "%ebx");
				output_write("", "rmmovl", "%ebx", v3);
			}
			break;
			
			case MU:
			{
				char v1[16];
				sprintf(v1, "%d(%%edx)", get_var_pos(c_root->arg1));
				char v2[16];
				sprintf(v2, "%d(%%edx)", get_var_pos(c_root->arg2));
				char v3[16];
				sprintf(v3, "%d(%%edx)", get_var_pos(c_root->dst));
				output_write(c_root->etq, "mrmovl", v1, "%eax");
				output_write("", "mrmovl", v2, "%ebx");
				output_write("", "pushl", "%eax", "");
				output_write("", "pushl", "%ebx", "");
				output_write("", "call", "MUL", "");
				output_write("", "popl", "%eax", "");
				output_write("", "popl", "%ebx", "");
				output_write("", "mrmovl", "0(%edx)", "%eax");
				output_write("", "rmmovl", "%eax", v3);
			}
			break;
			
			case AF:
			{
				char v1[16];
				sprintf(v1, "%d(%%edx)", get_var_pos(c_root->arg2));
				char v2[16];
				sprintf(v2, "%d(%%edx)", get_var_pos(c_root->arg1));
				output_write(c_root->etq, "mrmovl", v1, "%eax");
				output_write("", "rmmovl", "%eax", v2);
				
			}
			break;
			
			case AFC:
			{
				char v1[16];
				sprintf(v1, "%d(%%edx)", get_var_pos(c_root->dst));
				output_write(c_root->etq, "irmovl", c_root->arg1, "%eax");
				output_write("", "rmmovl", "%eax", v1);
			}
			break;
			
			case SK:
				output_write(c_root->etq, "nop", "", "");
			break;
			
			case JP:
				output_write(c_root->etq, "jmp", c_root->dst, "");
			break;
			
			case JZ:
			{
				char v1[16];
				sprintf(v1, "%d(%%edx)", get_var_pos(c_root->arg1));
				output_write(c_root->etq, "mrmovl", v1, "%eax");
				output_write("", "andl", "%eax", "%eax");
				output_write("", "je", c_root->dst, "");
			}
			break;
			
			case ST:
				output_write(c_root->etq, "halt", "", "");
			break;
			
		}
		
		c_root = c_root->next;
	}
}

//UTIL
void output_write(char* etq, char* op, char* arg1, char* arg2)
{
	if (strlen(etq) > 0)
		fprintf(stdout, "%s\t:%s\t%s,\t%s\n", etq, op, arg1, arg2);
	else
		fprintf(stdout, "\t %s\t%s,\t%s\n", op, arg1, arg2);
}

void c3a_write(int etq, char* op, char* arg1, char* arg2, char* dst)
{
	if (etq < 0)
		c3a_add_cmd(c3a_arg_set(op, arg1, arg2, dst));
	else
	{
		char etqm[16];
		sprintf(etqm, "ET%d", etq);
		if (strlen(dst) > 0 && dst[0] == '@')
			c3a_add_cmd(c3a_arg_set(op, arg1, arg2, etqm));
		else
			c3a_add_etq_cmd(etqm, c3a_arg_set(op, arg1, arg2, dst));
	}

}