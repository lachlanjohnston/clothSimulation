#pragma once

#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#include "Mesh.h"

class Renderer {
public:
    GLFWwindow* window;
    Mesh* mesh;

    Renderer(Mesh* mesh_, GLFWwindow* window_);
    void initialize();
    void render();
    void update();

    // buffering
    
    GLuint meshElementBuffer, vertexObjectBuffer, program, vertexShader, fragShader;
    GLint positionLoc, colorLoc, MVP;
};