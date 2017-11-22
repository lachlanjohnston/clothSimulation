#include "Mesh.h"
#include <iostream>

Mesh::Mesh(int N_, float maxCoord_)
    : N(N_), maxCoord(maxCoord_), nVertices(N_*N_)  {
        this->vertices = new vertex[nVertices];
        generateMesh();
    };

void Mesh::generateMesh() {
    std::cout << "here1" << std::endl;
    float d = ((2.f) * maxCoord) / (N-1);
    float z = 0.f;

    float y = maxCoord;

    for (int i = 0; i < N; i++) {
        float x = (-1.f) * maxCoord;
        for(int j = 0; j < N; j++) {
            vertex v = {x, y, z, 1.f, 1.f, 1.f};
            vertices[j + (i*N)] = v;
            x += d;
        }
        y -= d;
    }
    return;
}

void Mesh::update() {
    // physics here, must complete before rendering
    return;
}