#include "Renderer.hpp"
#include <iostream>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

Renderer::Renderer(Mesh* mesh_, GLFWwindow* window_) 
    : mesh(mesh_), window(window_) {
        initialize();
    };

void Renderer::initialize() {

    // initialize buffer states, shaders here
    glGenBuffers(1, &meshElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshElementBuffer);
    std::cout << sizeof(mesh->indices) << std::endl;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(GLuint), &mesh->indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vertexObjectBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexObjectBuffer);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertexShader);
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragShader);
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    MVP = glGetUniformLocation(program, "MVP");
    positionLoc= glGetAttribLocation(program, "vPos");
    colorLoc = glGetAttribLocation(program, "vCol");
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 6, (void*) 0);
    glEnableVertexAttribArray(colorLoc);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE,
                         sizeof(float) * 6, (void*) (sizeof(float) * 3));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    return;
}

void Renderer::render() {
    // fetch vertices and update buffers
    return;
}

void Renderer::update() {
    // delegate required frame update tasks

    glBufferData(GL_ARRAY_BUFFER, (mesh->nVertices) * (6 * sizeof(float)), mesh->vertices, GL_DYNAMIC_DRAW);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    float ratio;
    int width, height;
    glm::mat4 m, v, p, mvp;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 Pos(70.0f, 70.0f, 70.f);
    glm::vec3 Target(0.0f, 0.0f, 0.0f);
    glm::vec3 Up(0.0f, 1.0f, 0.0f);

    float fov = 0.785398f;

    m = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)); //note: scale can cause issues with lighting
    v = glm::lookAt(Pos, Target, Up);
    p = glm::perspective(fov,ratio,0.1f,1000.0f);

    mvp = p * v * m;
 
    glUseProgram(program);
    glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(mvp));
    glDrawElements(GL_TRIANGLE_STRIP, mesh->indices.size(), GL_UNSIGNED_INT, NULL);
    glfwSwapBuffers(window);
    glfwPollEvents();

    return;
}