#pragma once

#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#include "Mesh.hpp"

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

class Renderer {
public:
    GLFWwindow* window;
    Mesh* mesh;

    Renderer(Mesh* mesh_, GLFWwindow* window_);
    void initialize();
    void render();
    void update();
    // buffering
    
    GLuint meshElementBuffer, vertexObjectBuffer, program, vertexShader, fragShader, vao;
    GLint positionLoc, colorLoc, normLoc, lightPosLoc, lightIntenLoc, MVP, model;

    int elementSize;

    struct Light {
        glm::vec3 position;
        glm::vec3 intensities; //a.k.a. the color of the light
    };

    Light gLight = { glm::vec3(5.f,-10.f,25.0f), glm::vec3(0.5f,0.5f,0.5f) };

    const char* vertex_shader_text =
        "uniform mat4 MVP;\n"
        "attribute vec3 vCol;\n"
        "attribute vec3 vPos;\n"
        "attribute vec3 vNorm;\n"
        "varying vec3 fCol;\n"
        "varying vec3 fPos;\n"
        "varying vec3 fNorm;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = MVP * vec4(vPos, 1.0);\n"
        "    fCol = vCol;\n"
        "    fPos = vPos;\n"
        "    fNorm = vNorm;\n"
        "}\n";

    // consider moving normal mat calc outside of shader and create a uniform for it    
    const char* fragment_shader_text =
        "uniform mat4 model;\n"
        "uniform struct Light {\n"
        "   vec3 position;\n"
        "   vec3 intensities;\n" //a.k.a the color of the light
        "} light;\n"
        "varying vec3 fCol;\n"
        "varying vec3 fPos;\n"
        "varying vec3 fNorm;\n"
        "void main()\n"
        "{\n"
        //"    mat3 normMat = transpose(inverse(mat3(model[0].xyz, model[1].xyz, model[2].xyz)));\n"
        //"    vec3 normal = normalize(normMat * fNorm);\n"
        "    vec3 normal = normalize(mat3(model[0].xyz, model[1].xyz, model[2].xyz) * fNorm);\n"
        "    vec3 pos = vec3(model * vec4(fPos, 1));\n"
        "    vec3 dist = light.position - pos;\n"
        "    float brightness = dot(normal, dist) / (length(dist) * length(normal));\n"
        "    brightness = clamp(brightness, 0.0, 1.0);\n"
        //"    gl_FragColor = vec4(brightness * fCol, 1.0);\n"
        "    gl_FragColor = vec4(brightness * light.intensities * fCol, 1.0);\n"
        //"    gl_FragColor = vec4(fCol, 1.0);\n"
        "}\n";
};

