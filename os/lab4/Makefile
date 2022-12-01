all: main

main: main.c child.o constants.h
	gcc main.c constants.h -o run -lpthread -lrt

child.o: child.c
	gcc child.c constants.h -o child  -lpthread -lrt
