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
    //GLushort* indices;
    const GLushort indices[14] = {
        //strip 1
        0, 3, 1, 4, 2, 5, 
        5, 3, // degen triangle
        3, 6, 4, 7, 5, 8
    };
    float maxCoord; 
    int nVertices, nIndices, N;

    Mesh(int N_, float maxCoord_);
    void generateMesh();

    void update();
};

