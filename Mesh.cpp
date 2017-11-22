#include "Mesh.h"
#include <iostream>

Mesh::Mesh(int N_, float maxCoord_)
    : N(N_), maxCoord(maxCoord_), nVertices(N_*N_)  {
        this->vertices = new vertex[nVertices];
        generateMesh();
        generateIndices();
    };

void Mesh::generateMesh() {
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

void Mesh::generateIndices() {
    int size = (2*(N*N) - 4) / 2; int i = 0;

    // std::vector<GLushort> indices;
    int addedIndices = 0;

    while (size > (N-2)) {
        indices.push_back(i);
        indices.push_back(i + N);
        addedIndices += 2;
        size -= 1; 

        if (addedIndices % (2*N) == 0 && size > (N-2)) {
            indices.push_back(i + N);
            indices.push_back(i + 1);
        } 

        i++;
    }
}

void Mesh::update() {
    // physics here, must complete before rendering
    // for (int i = 0; i < nVertices; i++)
    //     vertices[i].y -= .0098;
    return;
}