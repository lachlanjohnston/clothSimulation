#pragma once

#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>


class Mesh {
public:
    struct vertex {
        float x, y, z;
        float r, g, b;
    };

    vertex* vertices;
    GLushort* indices;
    float maxCoord; 
    int nVertices, nIndices, N;

    Mesh(int N_, float maxCoord_);
    void generateMesh();

    void update();
};

