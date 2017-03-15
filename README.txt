##########################################################
#      Projet de compilation : compilateur IMP > Y86     #
##########################################################

Groupe (seul) : Adrien HALNAUT (A4)

L'exécution du makefile sans argument génère tous les exécutables nécessaires : 
	- exs/int_imp
	- exs/int_c3a
	- exs/cmp_imp
	- exs/cmp_c3a
	- iimp

Tous les exécutables prennent un fichier au format imp ou c3a en entrée et affichent le résultat
sur la sortie d'erreur pour les messages d'états et sur la sortie standard pour donner l'environnement
ou la compilation correspondante.

Question 1 : Interpréteur IMP (exs/int_imp)
	Lexèmes (flex) : 				int_imp.l
	Grammaire (bison) : 			int_imp.y
	Programme principal (AST) : 	imp_interpreter.c + imp_interpreter.h
	Retourne l'environnement (les variables) sur stdout
	
Question 2 : Interpréteur C3A (exs/int_c3a)
	Lexèmes (flex) : 				int_c3a.l
	Grammaire (bison) : 			int_c3a.y
	Programme principal (AST) : 	c3a_interpreter.c + c3a_interpreter.h
	Retourne l'environnement (les variables) sur stdout
	
Question 3 : Compilateur IMP > C3A (exs/cmp_imp)
	Lexèmes (flex) : 				cmp_imp.l
	Grammaire (bison) : 			cmp_imp.y
	Programme principal (AST) : 	imp_compiler.c + imp_compiler.h
	Retourne le programme IMP correspondant en C3A sur stdout
	
Question 4 : Compilateur C3A > Y86 (exs/cmp_c3a)
	Lexèmes (flex) : 				cmp_c3a.l
	Grammaire (bison) : 			cmp_c3a.y
	Programme principal (Liste) : 	c3a_compiler.c + c3a_compiler.h
	Retourne le programme C3A correspondant en Y86 sur stdout
	
Question 5 / Synthèse : Compilateur IMP > Y86 (iimp)
	Lexèmes (flex) : 					iimp.l
	Grammaire (bison) :					iimp.y
	Programme principal (AST + Liste) : iimp.c + iimp.h
	Retourne le programme IMP correspondant en Y86 sur stdout
	
	Le code de iimp est une sorte de fusion des fichiers des questions 3 et 4
	
Une sorte d'arbre AST peut être affiché en décommentant les appels à display_ast_tree(), mais 
l'affichage n'est pas très agréable.

Lors de la préparation de mon rendu, la commande make invoquait de temps en temps des commandes
non spécifiées dans les règles données (yacc et mv -f). Ces commandes écrasaient le fichier iimp.c avec du code
généré par Bison, rendant donc la compilation impossible. 
J'ai pu retrouver un comportement normal qu'en ayant utilisé un backup grâce à Git.
Si un comportement semblable vous arrive, veuillez me contacter par mail pour que je vous donne
un lien vers mon dépôt afin d'avoir un Makefile sain.