#include "Include.hpp"
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

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;

 //n must be AT LEAST 3!!!
int n = 20; float maxDistance = 1.0f;
Mesh* mesh;
Renderer* r;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

     if (key == GLFW_KEY_W && action == GLFW_PRESS)
        mesh->toggleWind = !mesh->toggleWind;

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        r->toggleFrame = !r->toggleFrame;

    if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        r->moveCamera(-3);
    if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        r->moveCamera(3);
    if(key == GLFW_KEY_UP && action == GLFW_PRESS)
        r->moveCamera(-1);
    if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        r->moveCamera(1);
        
        
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
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Simple cloth", NULL, NULL);


    std::cout << "W: Toggle wind force" << std::endl;
    std::cout << "F: Toggle wire frame" << std::endl;
    std::cout << "Arrow keys: Move (naive) camera" << std::endl;

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    mesh = new Mesh(n, maxDistance);
	r = new Renderer(mesh, window);
    

	while (!glfwWindowShouldClose(window)) {
		mesh->update(); // update mesh via physics engine
		r->update();    // render mesh
    }

    // for (int i = 0; i < 10; i++) {
    //     mesh->update();
    //     r->update();
    // }
	glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    return 0;
}
