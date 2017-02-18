all: iimp

iimp.tab.c iimp.tab.h:	iimp.y
	bison -d iimp.y

lex.yy.c: iimp.l iimp.tab.h
	flex iimp.l

iimp: lex.yy.c iimp.tab.c iimp.tab.h imp_interpreter.c
	gcc -o iimp iimp.tab.c lex.yy.c imp_interpreter.c -lfl

clean:
	rm iimp iimp.tab.c lex.yy.c iimp.tab.h
