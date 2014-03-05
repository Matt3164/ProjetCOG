NL: main.o nl.o
	g++ -o NL main.o nl.o


nl.o: nl.c 
	gcc -o nl.o -c nl.c -W -Wall -pedantic


main.o: main.cpp nl.h
	g++ -o main.o -c main.cpp -W -Wall -pedantic
