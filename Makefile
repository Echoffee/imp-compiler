all: iimp ic3a

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

clean:
	rm iimp iimp.tab.c iimp.lex.yy.c iimp.tab.h ic3a ic3a.tab.c ic3a.lex.yy.c ic3a.tab.h
