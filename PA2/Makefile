all: main

main: formula.o mmpa.o main.o
	g++ -g formula.o mmpa.o main.o -o main

formula.o: formula.h formula.cpp
	g++ -g -std=c++11 -pedantic -c formula.cpp

mmpa.o: formula.h mmpa.h mmpa.cpp
	g++ -g -std=c++11 -pedantic -c mmpa.cpp

main.o: formula.h mmpa.h main.cpp
	g++ -g -std=c++11 -pedantic -c main.cpp

clean: 
	rm -rf *.o *.exe main
	# del *.o *.exe main