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
    glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 6, (void*) 0);
    glEnableVertexAttribArray(colorLoc);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE,
                         sizeof(float) * 6, (void*) (sizeof(float) * 3));

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    return;
}

void Renderer::render() {
    // fetch vertices and update buffers
    return;
}

void Renderer::update() {
    // delegate required frame update tasks

    glBufferData(GL_ARRAY_BUFFER, (mesh->nVertices) * (6 * sizeof(float)), mesh->vertices, GL_STATIC_DRAW);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    float ratio;
    int width, height;
    glm::mat4 m, v, p, mvp;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);      // Left=-2, Right=2, Bottom=-2, Top=2, Near=5, Far=9
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();

    // mat4x4_identity(m);
    //mat4x4_rotate_Z(m, m, (float) glfwGetTime());
    // mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    // mat4x4_mul(mvp, p, m);


    glm::vec3 Pos(1.0f, 1.0f, -1.f);
    glm::vec3 Target(0.0f, 0.0f, 0.0f);
    glm::vec3 Up(0.0, 1.0f, 0.0f);

    float fov = 0.785398f;

    m = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    v = glm::lookAt(Pos, Target, Up);
    p = glm::perspective(fov,ratio,0.1f,100.0f);

    mvp = p * v * m;


    glUseProgram(program);
    glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(mvp));
    glDrawElements(GL_TRIANGLE_STRIP, mesh->indices.size(), GL_UNSIGNED_INT, NULL);
    glfwSwapBuffers(window);
    glfwPollEvents();

    return;
}