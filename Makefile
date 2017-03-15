all: int_imp int_c3a cmp_imp cmp_c3a iimp

int_imp.tab.c int_imp.tab.h:	int_imp.y
	bison -d int_imp.y

int_imp.lex.yy.c: int_imp.l int_imp.tab.h
	flex -o int_imp.lex.yy.c int_imp.l

int_imp: int_imp.lex.yy.c int_imp.tab.c int_imp.tab.h imp_interpreter.c
	gcc -o int_imp int_imp.tab.c int_imp.lex.yy.c imp_interpreter.c -lfl

int_c3a.tab.c int_c3a.tab.h:	int_c3a.y
	bison -d int_c3a.y

int_c3a.lex.yy.c: int_c3a.l int_c3a.tab.h
	flex -o int_c3a.lex.yy.c int_c3a.l

int_c3a : int_c3a.lex.yy.c int_c3a.tab.c int_c3a.tab.h c3a_interpreter.c
	gcc -o int_c3a int_c3a.tab.c int_c3a.lex.yy.c c3a_interpreter.c -lfl

cmp_imp.lex.yy.c: cmp_imp.l cmp_imp.tab.h
		flex -o cmp_imp.lex.yy.c cmp_imp.l

cmp_imp.tab.c cmp_imp.tab.h:	cmp_imp.y
		bison -d cmp_imp.y

cmp_imp: cmp_imp.lex.yy.c cmp_imp.tab.c cmp_imp.tab.h imp_compiler.c
		gcc -o cmp_imp cmp_imp.tab.c cmp_imp.lex.yy.c imp_compiler.c -lfl

cmp_c3a.lex.yy.c: cmp_c3a.l cmp_c3a.tab.h
		flex -o cmp_c3a.lex.yy.c cmp_c3a.l
		
cmp_c3a.tab.c cmp_c3a.tab.h:	cmp_c3a.y
		bison -d cmp_c3a.y
		
cmp_c3a: cmp_c3a.lex.yy.c cmp_c3a.tab.c cmp_c3a.tab.h c3a_compiler.c
		gcc -o cmp_c3a cmp_c3a.tab.c cmp_c3a.lex.yy.c c3a_compiler.c -lfl
		
iimp.lex.yy.c: iimp.l iimp.tab.h
		flex -o iimp.lex.yy.c iimp.l
		
iimp.tab.c iimp.tab.h:	iimp.y
		bison -d iimp.y
		
iimp: iimp.lex.yy.c iimp.tab.c iimp.tab.h iimp.c
		gcc -o iimp iimp.tab.c iimp.lex.yy.c iimp.c -lfl
		

clean:
	rm int_imp int_c3a cmp_imp cmp_c3a iimp *.tab.c *.lex.yy.c *.tab.h *.output 
