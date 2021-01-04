CC = g++

LIBS = -lboost_regex

teste: 
	$(CC) codigo_compilador.cpp -o compilador $(LIBS)
	./compilador exemplos/teste.cd
	clear
	exemplos/teste

fibonacci: 
	$(CC) codigo_compilador.cpp -o compilador $(LIBS)
	./compilador exemplos/fibonacci.cd
	clear
	exemplos/fibonacci


	

