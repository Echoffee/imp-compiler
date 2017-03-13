#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c3a_compiler.h"


c3a_cmd c_root = NULL;
c3a_cmd c_current = NULL;
var v_root = NULL;
var v_current = NULL;
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
	
	if (v_root == NULL)
		v_root = v;
		
	if (v_current != NULL)
		v_current->next = v;
	
	v_current = v;
	return v;
}

int get_var_pos(char* var_name)
{
	var v = v_root;
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
			case EMPTY:
				// :thinking:
			break;
			
			case PL:
			
			break;
			
			case MO:
			
			break;
			
			case MU:
			
			break;
			
			case AF:
			{
				char v1[16];
				sprintf(v1, "%d(%%edx)", get_var_pos(c_root->arg1));
				char v2[16];
				sprintf(v2, "%d(%%edx)", get_var_pos(c_root->arg2));
				output_write(c_root->etq, "mrmovl", v1, "%%eax");
				output_write("", "rmmovl", "%%eax", v2);
				
			}
			break;
			
			case AFC:
				output_write(c_root->etq, "irmovl", "4(%%edx)", "%%eax");
				output_write("", "rmmovl", "%%eax", "8(%%edx)");
			break;
			
			case SK:
				output_write(c_root->etq, "nop", "", "");
			break;
			
			case JP:
			
			break;
			
			case JZ:
			
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

