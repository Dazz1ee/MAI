all: main

main: main.c child.o
	gcc main.c -o run

child.o: child.c
	gcc child.c -o child

clean:
	rm -rf *.o hello