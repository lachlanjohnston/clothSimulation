#include "Mesh.h"

Mesh::Mesh(int N_, float maxCoord_)
    : N(N_), maxCoord(maxCoord_), nVertices(N_*N_)  {
        generateMesh();
    };

void Mesh::generateMesh() {
    float d = ((2.f) * this->maxCoord) / (N-1);
    float z = 0.f;

    float y = this->maxCoord;
    for (int i = 0; i < N; i++) {
        float x = (-1.f) * this->maxCoord;
        for(int j = 0; j < N; j++) {
            vertex v = {x, y, z, 1.f, 1.f, 1.f};
            this->vertices[j + (i*N)] = v;
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