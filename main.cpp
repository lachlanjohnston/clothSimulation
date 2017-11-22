#include "Include.h"
#include <iostream>

//
// Simulation of a Cloth
// Using a 3D Mesh Model
// ------------------------------------
// Lachlan Johnston & Spencer Bryson
// License: MIT
//
// The following program simulates the motion of a
// cloth when exposed to certain external forces, as
// well as colliding with various objects. The model
// will use a 3D mesh consisting of N masses connected
// by springs.
//

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main() {
	// Begin by creating an OpenGL context
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

    glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	
	int n = 20; float maxDistance = .2f;
	Mesh* mesh = new Mesh(n, maxDistance);
	Renderer* r = new Renderer(mesh, window);

	while (!glfwWindowShouldClose(window)) {
		
		mesh->update(); // update mesh via physics engine
		r->update();    // render mesh

		glfwSwapBuffers(window);
		glfwPollEvents();
    }
    
    return 1;
}
