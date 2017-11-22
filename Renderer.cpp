#include "Renderer.h"

Renderer::Renderer(Mesh* mesh_, GLFWwindow* window_) 
    : mesh(mesh_), window(window_) {
        initialize();
    };

void Renderer::initialize() {
    // initialize buffer states, shaders here
    glGenBuffers(1, &meshElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh->indices), mesh->indices, GL_STATIC_DRAW);

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

    //std::cout << "initialized mesh" << std::endl;
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
    mat4x4 m, p, mvp;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    mat4x4_identity(m);
    mat4x4_rotate_Z(m, m, (float) glfwGetTime());
    mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    mat4x4_mul(mvp, p, m);
    glUseProgram(program);
    glUniformMatrix4fv(MVP, 1, GL_FALSE, (const GLfloat*) mvp);
    glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_SHORT, NULL);
    // glfwSwapBuffers(window);
    // glfwPollEvents();

    return;
}