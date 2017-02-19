struct s_variable {
	char* name;
	int value;
	struct s_variable* next;
};

typedef struct s_variable* variable;

enum e_node_type { EMPTY, ROOT, MEMBER, OPERATOR, LOOP, BRANCH, SINGLE_BLOCK, LABEL};
enum e_node_item { NONE, CONST, VAR, FACT, AFF, ADD, SUB, MULT, ITE, WD};

typedef enum e_node_type node_type;
typedef enum e_node_item node_item;

struct s_ast_node {
	node_type category;
	node_item item;
	variable var;	//may be empty
	int value;		//may be empty
	int child_num;
	struct s_ast_node** childs;
};

typedef struct s_ast_node* ast_node;

struct s_etq_cmd {
	char* name;
	ast_node cmd;
	struct s_etq_cmd* next;
};

typedef struct s_etq_cmd* etq_cmd;
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

ast_node new_ast_node(int size);
ast_node ast_create_node_from_int(int value);
ast_node ast_create_node_from_variable(char* name);
ast_node ast_create_op_node(int factor, ast_node value);
ast_node ast_create_add_node(ast_node left, ast_node right, ast_node dest);
ast_node ast_create_sub_node(ast_node left, ast_node right, ast_node dest);
ast_node ast_create_mult_node(ast_node left, ast_node right, ast_node dest);
ast_node ast_create_aff_node(char* name, ast_node value);
ast_node ast_create_branch(ast_node left, ast_node right);
ast_node ast_create_label_cmd(ast_node label, ast_node command);
ast_node ast_create_empty_node();
void initialize_ast();
void ast_execute(ast_node root);


variable add_variable(char* name, int value);
int get_value_from_variable(char* label);
void assign_value_to_variable(char* label, int value);
char* concat(char* stat1, char* stat2);
void display_env();
void display_ast_tree(ast_node root, int stage);
