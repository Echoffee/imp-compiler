struct s_variable {
	char* name;
	int value;
	struct s_variable* next;
};

typedef struct s_variable* variable;

enum node_type { ROOT, MEMBER, OPERATOR, LOOP, BRANCH };
enum node_item { CONST, VAR, AFF, ADD, SUB, MULT, ITE, WD};

struct s_ast_node {
	node_type category;
	node_item item;
	variable var;	//may be empty
	int value;		//may be empty
	int child_num;
	struct s_ast_node* childs;
};

typedef struct s_ast_node* ast_node;

/*
	ast_node childs :
		- ROOT : n, basically the main()
		- MEMBER : none, is a variable or a constant
		- OPERATOR : 0 : left member; 1 : right member
		- LOOP :
			- ITE : 0 : condition; 1 : then block; 2 : else block
			- WD : 0 : condition; 1 : do block
		- BRANCH : like ROOT but yeah no
*/

void add_variable(char* name, int value);
int get_value_from_variable(char* label);
void assign_value_to_variable(char* label, int value);
char* concat(char* stat1, char* stat2);
void display_env();
