 all: main

main: main.cpp convector.o generator.o combiner.o
	g++ main.cpp -o run -lpthread -lrt

convector.o: convector.cpp
	g++ convector.cpp -o convector -lpthread -lrt

generator.o: generator.cpp
	g++ generator.cpp -o generator -lpthread -lrt

combiner.o: combiner.cpp
	g++ combiner.cpp -o combiner -lpthread -lrt
