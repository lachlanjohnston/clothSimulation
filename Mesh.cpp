#include "Mesh.hpp"

Mesh::Mesh(int N_, float maxCoord_)
    : N(N_), maxCoord(maxCoord_), nVertices(N_*N_), 
      dt(0.01f), mass(1.0f) {
        this->vertices = new vertex[nVertices];
        this->times = new float[nVertices];
        this->gravity = Force(0.0, 0.0, -0.1);
        generateMesh();
        generateIndices();
    };

void Mesh::generateMesh() {
    float d = ((2.f) * maxCoord) / (N-1);
    float z = 0.f;
    restLength = d;

    float y = maxCoord;

    for (int i = 0; i < N; i++) {
        float x = (-1.f) * maxCoord;
        for(int j = 0; j < N; j++) {
            vertex v = {x, y, z, 1.f, 1.f, 1.f, 0.0f, 0.0f, 1.0f};

            int index = j + (i*N);
            vertices[index] = v;
            times[index] = 0;

            std::vector<float> velocityv;
            velocityv.resize(3);
            velocityv.push_back(0);
            velocityv.push_back(0);
            velocityv.push_back(0);

            vec velocity(velocityv);
            velocities.push_back(velocity);

            std::vector<GLuint> vNN = determineNN(index);
            NN.push_back(vNN);

            x += d;
        }
        y -= d;
    }

    return;
}

std::vector<GLuint> Mesh::determineNN(int index) {
    //ugly if statements incoming....

    std::vector<GLuint> vNN;

    /* lets figure out it is a corner first */

    if(index == 0) //top left
        { vNN.push_back(index+1); vNN.push_back(index+N); vNN.push_back(index+N+1); vNN.resize(3); return vNN; }
    if(index == (N-1)) // top right
        { vNN.push_back(index-1); vNN.push_back(index+N); vNN.push_back(index+N-1); vNN.resize(3); return vNN; }
    if(index == (N*N-N)) // bottom left
        { vNN.push_back(index+1); vNN.push_back(index-N); vNN.push_back(index-N+1); vNN.resize(3); return vNN;}
    if(index == (N*N-1)) // bottom right
        { vNN.push_back(index-1); vNN.push_back(index-N); vNN.push_back(index-N-1); vNN.resize(3); return vNN;}

    /* checking top edge of mesh */
    if(index < N) {
        vNN.push_back(index-1); vNN.push_back(index+1); vNN.push_back(index+N); vNN.push_back(index+N-1); vNN.push_back(index+N+1);
        vNN.resize(5); 
        return vNN;
    }

    /* checking left edge of mesh */
    if (index % N == 0) {
        vNN.push_back(index+1); vNN.push_back(index-N); vNN.push_back(index-N+1); vNN.push_back(index+N); vNN.push_back(index+N+1);
        vNN.resize(5); 
        return vNN;
    }

    /* checking right edge of mesh */
    if((index+1) % N == 0) {
        vNN.push_back(index-1); vNN.push_back(index-N); vNN.push_back(index-N-1); vNN.push_back(index+N); vNN.push_back(index+N-1);
        vNN.resize(5); 
        return vNN;
    }

    /* checking bottom edge of mesh */
    if((index >= (N*N-N)) && (index < (N*N))) {
        vNN.push_back(index-1); vNN.push_back(index+1); vNN.push_back(index-N); vNN.push_back(index-N-1); vNN.push_back(index-N+1);
        vNN.resize(5); 
        return vNN;
    }

    vNN.push_back(index-1); vNN.push_back(index+1); vNN.push_back(index-N); vNN.push_back(index+N);
    vNN.push_back(index-N-1); vNN.push_back(index-N+1); vNN.push_back(index+N-1); vNN.push_back(index+N+1);
    vNN.resize(8);
    
    return vNN;
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

vec Mesh::vectorize(vertex* v) {
    std::vector<float> positionv;

    positionv.resize(3);
    positionv[0] = v->x;
    positionv[1] = v->y;
    positionv[2] = v->z;

    vec position(positionv);

    return position;
}

vec normalize(vec v, float norm) { return v / norm; }

vec springForce(vec v1, vec v2, float k, float restLength) {
    vec d = v1 - v2;
    float norm = norm_2(d);
    float x = norm - restLength;
    vec n = normalize(d, norm);

    return k * x * n;
}

vec squareVector(vec v) { 
    for (auto i : v) i *= i; 
    return v;
}

struct sys {
    vec g; float mass, restLength, k;
    std::vector<vec>  NN;
    void operator()(const vec &pos, vec &velocity, double /*t*/) {
        // need to implement other forces
        // vec force = g * mass;
        // velocity = force;
        std::vector<vec> springForces;

        for (auto i : NN) // calculate spring forces
            springForces.push_back(springForce(i, pos, k, restLength));
        
        for (auto force : springForces)
            velocity += force; // apply forces

        vec squareVelocity = velocity;

        velocity += g * mass - 0.1 * squareVector(squareVelocity); // apply gravity & bogodrag
    }
};

void Mesh::update() {
    // physics here, must complete before rendering

    for (int i = 20; i < nVertices; i++) {
        vertex* curVertex = &vertices[i];
        float t = times[i];

        std::vector<vec> connectedMasses;
        vec position = vectorize(curVertex);
        for (auto j : NN[i]) {
            vertex* n = &vertices[j];
            connectedMasses.push_back(vectorize(n));
        }

        sys s;
        s.g = gravity.force;
        s.mass = mass;
        s.restLength = restLength;
        s.k = 6;
        s.NN = connectedMasses;
        stepper.do_step(s, position, t, dt);

        curVertex->x = position[0];
        curVertex->y = position[1];
        curVertex->z = position[2];
    }

    

    return;
}