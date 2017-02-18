#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imp_interpreter.h"

variable root = NULL;
variable current = NULL;

void add_variable(char* name, int value)
{
	variable v = (variable) malloc(sizeof(struct s_variable));
	v->name = (char*) malloc(sizeof(char) * strlen(name));
	strcpy(v->name, name);
	v->value = value;
	v->next = NULL;

	if (current != NULL)
		current->next = v;

	current = v;
	if (root == NULL)
		root = v;
}

int get_value_from_variable(char* label)
{
	variable c = root;
	while (c != NULL && strcmp(c->name, label))
		c = c->next;

	if (c != NULL)
		return c->value;
	else
		return 0;
}

void assign_value_to_variable(char* label, int value)
{
	variable c = root;
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

void display_env()
{
	printf("*** ENV ***\n");
	while(root != NULL)
	{
		printf("Var %s : %d\n", root->name, root->value);
		root = root->next;
	}

	printf("*** END ***\n");
}
