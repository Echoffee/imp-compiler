all: iimp ic3a cimp

iimp.tab.c iimp.tab.h:	iimp.y
	bison -d iimp.y

iimp.lex.yy.c: iimp.l iimp.tab.h
	flex -o iimp.lex.yy.c iimp.l

iimp: iimp.lex.yy.c iimp.tab.c iimp.tab.h imp_interpreter.c
	gcc -o iimp iimp.tab.c iimp.lex.yy.c imp_interpreter.c -lfl

ic3a.tab.c ic3a.tab.h:	ic3a.y
	bison -d ic3a.y

ic3a.lex.yy.c: ic3a.l ic3a.tab.h
	flex -o ic3a.lex.yy.c ic3a.l

ic3a : ic3a.lex.yy.c ic3a.tab.c ic3a.tab.h c3a_interpreter.c
	gcc -o ic3a ic3a.tab.c ic3a.lex.yy.c c3a_interpreter.c -lfl

cimp.lex.yy.c: cimp.l cimp.tab.h
		flex -o cimp.lex.yy.c cimp.l

cimp.tab.c cimp.tab.h:	cimp.y
		bison -d cimp.y

cimp: cimp.lex.yy.c cimp.tab.c cimp.tab.h imp_compiler.c
		gcc -o cimp cimp.tab.c cimp.lex.yy.c imp_compiler.c -lfl

clean:
	rm iimp iimp.tab.c iimp.lex.yy.c iimp.tab.h ic3a ic3a.tab.c ic3a.lex.yy.c ic3a.tab.h cimp 
