enum e_c3a_op { EMPTY, PL, MO, MU, AF, AFC, SK, JP, JZ, ST };
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
void display_env();
