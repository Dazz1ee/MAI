CC = gcc

all:
	$(CC) -fPIC -c lib1.c -o lib1.o -lm
	$(CC) -fPIC -c lib2.c -o lib2.o -lm
	$(CC) -shared -o libFirst.so lib1.o -lm
	$(CC) -shared -o libSecond.so lib2.o -lm
	sudo cp libFirst.so /usr/lib/
	sudo cp libSecond.so /usr/lib/
	$(CC) static.c -lFirst -o static.o
	$(CC) dynamic.c -ldl -o dynamic.o



clean:
	rm -f *.o *.so *.out