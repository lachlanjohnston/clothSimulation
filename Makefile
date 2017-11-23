
CC=clang++
FLAGS=-lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -std=c++11

all: clean build run

build: main.o Renderer.o Mesh.o Force.o
	$(CC) $(FLAGS) main.o Renderer.o Mesh.o Force.o -o output

main.o: main.cpp
	$(CC) -c  main.cpp

Renderer.o: Renderer.cpp
	$(CC) -c Renderer.cpp

Mesh.o: Mesh.cpp
	$(CC) -c Mesh.cpp

Force.o: Force.cpp
	$(CC) -c Force.cpp

clean:
	rm -f *.o
	rm -f ./output

run:
	./output
