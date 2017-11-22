
CC=clang++
FLAGS=-lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -std=c++11

all: build run

build: main.o Renderer.o Mesh.o
	$(CC) $(FLAGS) main.o Renderer.o Mesh.o -o output

main.o: main.cpp
	$(CC) -c  main.cpp

Renderer.o: Renderer.cpp
	$(CC) -c Renderer.cpp

mesh.o: Mesh.cpp
	$(CC) -c Mesh.cpp

clean:
	rm -f *.o
	rm -f ./output

run:
	./output