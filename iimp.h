struct s_variable {
	char* name;
	int value;
	struct s_variable* next;
};

typedef struct s_variable* variable;

enum e_node_type { EMPTY, ROOT, MEMBER, OPERATOR, LOOP, BRANCH, SINGLE_BLOCK , JMP};
enum e_node_item { NONE, CONST, VAR, AFF, ADD, SUB, MULT, ITE, WD};

typedef enum e_node_type node_type;
typedef enum e_node_item node_item;

struct s_ast_node {
	node_type category;
	node_item item;
	variable var;	//may be empty
	int value;		//may be empty
	int child_num;
	char* svar;
	int sname;
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
ast_node ast_create_o_node(ast_node left, ast_node right, node_item item);
ast_node ast_create_aff_node(char* name, ast_node value);
ast_node ast_create_ITE_node(ast_node condition, ast_node then_block, ast_node else_block);
ast_node ast_create_WD_node(ast_node condition, ast_node do_block);
ast_node ast_create_branch(ast_node left, ast_node right);
ast_node ast_create_node_from_ep(ast_node content);
ast_node ast_create_node_from_cp(ast_node content);
ast_node ast_create_empty_node();
ast_node ast_create_jmp_node();
void initialize_ast();
void ast_execute(ast_node root);


variable add_variable(char* name, int value);
int get_value_from_variable(char* label);
void assign_value_to_variable(char* label, int value);
char* concat(char* stat1, char* stat2);
void display_env();
void display_ast_tree(ast_node root, int stage);
void create_output_file();
void c3a_write(int etq, char* op, char* arg1, char* arg2, char* dst);

enum e_c3a_op { ZERO, PL, MO, MU, AF, AFC, SK, JP, JZ, ST };
typedef enum e_c3a_op c3a_op;

struct s_c3a_cmd {
	char* etq;
	c3a_op cmd;
	char* arg1;
	char* arg2;
	char* dst;
	struct s_c3a_cmd* next;
};

struct s_var{
	char* name;
	int pos;
	struct s_var* next;
};

typedef struct s_etq_cmd* etq_cmd;
typedef struct s_c3a_cmd* c3a_cmd;
typedef struct s_var* var;

void init_output();
void end_output();
void c3a_execute();
void c3a_add_etq_cmd(char* etq, char** args);
void c3a_add_cmd(char** args);
char** c3a_arg_set(char* cmd, char* arg1, char* arg2, char* dst);
char* invert_value(char* value);
void output_write(char* etq, char* op, char* arg1, char* arg2);