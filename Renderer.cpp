#include "Renderer.hpp"
#include <iostream>
#include <stdio.h>

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

    GLint status, infoLogLength;
    GLchar* strInfoLog;

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);

        strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(vertexShader, infoLogLength, NULL, strInfoLog);

        fprintf(stderr, "Compilation error in vertex shader: %s\n", strInfoLog);
        delete[] strInfoLog;
    }

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &infoLogLength);

        strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(fragShader, infoLogLength, NULL, strInfoLog);

        fprintf(stderr, "Compilation error in frag shader: %s\n", strInfoLog);
        delete[] strInfoLog;
    }

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    // Check the program
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0 ){
		std::vector<char> programErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(program, infoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
	}

    MVP = glGetUniformLocation(program, "MVP");
    model = glGetUniformLocation(program, "model");
    lightPosLoc = glGetUniformLocation(program, "light.position");
    lightIntenLoc = glGetUniformLocation(program, "light.intensities");
    positionLoc= glGetAttribLocation(program, "vPos");
    colorLoc = glGetAttribLocation(program, "vCol");
    normLoc = glGetAttribLocation(program, "vNorm");

    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 9, (void*) 0);
    glEnableVertexAttribArray(colorLoc);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE,
                         sizeof(float) * 9, (void*) (sizeof(float) * 3));
    glEnableVertexAttribArray(normLoc);
    glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE,
                         sizeof(float) * 9, (void*) (sizeof(float) * 6));


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    return;
}

void Renderer::render() {
    // fetch vertices and update buffers
    return;
}

void Renderer::update() {
    // delegate required frame update tasks

    glBufferData(GL_ARRAY_BUFFER, (mesh->nVertices) * (9 * sizeof(float)), mesh->vertices, GL_DYNAMIC_DRAW);
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    float ratio;
    int width, height;
    glm::mat4 m, v, p, mvp;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 Pos(3.0f, 3.0f, 3.f);
    glm::vec3 Target(0.0f, 0.0f, 0.0f);
    glm::vec3 Up(0.0f, 1.0f, 0.0f);

    float fov = 0.785398f;

    //m = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)); //note: scale can cause issues with lighting
    m = glm::mat4(1.0f);
    v = glm::lookAt(Pos, Target, Up);
    p = glm::perspective(fov,ratio,0.1f,100.0f);

    mvp = p * v * m;
 
    glUseProgram(program);
    glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(m));
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(gLight.position));
    glUniform3fv(lightIntenLoc, 1, glm::value_ptr(gLight.intensities));
    glDrawElements(GL_TRIANGLE_STRIP, mesh->indices.size(), GL_UNSIGNED_INT, NULL);
    glfwSwapBuffers(window);
    glfwPollEvents();

    return;
}