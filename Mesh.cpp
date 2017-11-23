#include "Mesh.hpp"

Mesh::Mesh(int N_, float maxCoord_)
    : N(N_), maxCoord(maxCoord_), nVertices(N_*N_)  {
        this->vertices = new vertex[nVertices];
        this->times = new float[nVertices];
        this->gravity = Force(0.0, -9.8, 0.0);
        generateMesh();
        generateIndices();
    };

void Mesh::generateMesh() {
    float d = ((2.f) * maxCoord) / (N-1);
    float z = 0.f;
    float w = 1.0f;

    float y = maxCoord;

    for (int i = 0; i < N; i++) {
        float x = (-1.f) * maxCoord;
        for(int j = 0; j < N; j++) {
            vertex v = {x, y, z, w, 1.f, 1.f, 1.f};
            vertices[j + (i*N)] = v;
            times[j + (i*N)] = 0;

            std::vector<float> velocityv;
            velocityv.resize(3);
            velocityv.push_back(0);
            velocityv.push_back(0);
            velocityv.push_back(0);

            vec velocity(velocityv);
            velocities.push_back(velocity);
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
    for (int i = 0; i < nVertices; i++) {
        vertex* curVertex = &vertices[i];
        // std::vector<float> position;
        // std::vector<float> velocity = velocities[i];
        float t = times[i];

        std::vector<float> positionv;
        

        positionv.resize(3);
        positionv[0] = curVertex->x;
        positionv[1] = curVertex->y;
        positionv[2] = curVertex->z;

        vec position(positionv);

        sys s;
        s.g = gravity.force;
        s.mass = mass;
        stepper.do_step(s, position, t, dt);

        curVertex->x = position[0];
        curVertex->y = position[1];
        curVertex->z = position[2];
    }

    return;
}