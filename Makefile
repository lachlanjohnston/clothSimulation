
CC=clang++
FLAGS=-lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo 
VER=-std=c++11

all: clean build run

build: main.o Renderer.o Mesh.o Force.o
	$(CC) $(FLAGS) $(VER) main.o Renderer.o Mesh.o Force.o -o output

main.o: main.cpp
	$(CC) -c $(VER) main.cpp

Renderer.o: Renderer.cpp
	$(CC) -c $(VER) Renderer.cpp

Mesh.o: Mesh.cpp
	$(CC) -c $(VER) Mesh.cpp

Force.o: Force.cpp
	$(CC) -c $(VER) Force.cpp

clean:
	rm -f *.o
	rm -f ./output

run:
	./output
