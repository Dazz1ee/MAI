 all: main

main: main.cpp child.o
	g++ main.cpp -o run -lpthread -lrt

child.o: child.cpp
	g++ child.cpp -o child -lpthread -lrt
