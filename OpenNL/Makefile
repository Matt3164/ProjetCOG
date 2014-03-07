CFlAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

NL: main.o nl.o 
	g++ -o NL main.o nl.o 


nl.o: nl.c 
	gcc -o nl.o -c nl.c


main.o: main.cpp nl.h
	g++ $(CFLAGS) -o main.o -c main.cpp $(LIBS)

#dodo.o: dodo.cpp
#	g++ $(CFLAGS) $(LIBS) -o dodo.o  dodo.cpp
