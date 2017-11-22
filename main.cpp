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

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main() {
	// Begin by creating an OpenGL context
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

	int n = 10; float maxDistance = .8f;
	Mesh* mesh = new Mesh(n, maxDistance);
	Renderer* r = new Renderer(mesh, window);

	while (!glfwWindowShouldClose(window)) {
		
		mesh->update(); // update mesh via physics engine
		r->update();    // render mesh
    }

	glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    return 0;
}
