struct s_variable {
	char* name;
	int value;
	struct s_variable* next;
};

typedef struct s_variable* variable;

void add_variable(char* name, int value);
int get_value_from_variable(char* label);
void assign_value_to_variable(char* label, int value);
char* concat(char* stat1, char* stat2);
void display_env();
