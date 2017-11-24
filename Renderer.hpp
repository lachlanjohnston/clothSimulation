#pragma once

#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#include "Mesh.hpp"

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

    int elementSize;

    const char* vertex_shader_text =
        "uniform mat4 MVP;\n"
        "attribute vec3 vCol;\n"
        "attribute vec3 vPos;\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = MVP * vec4(vPos, 1.0);\n"
        "    color = vCol;\n"
        "}\n";
    const char* fragment_shader_text =
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(color, 1.0);\n"
        "}\n";
};

