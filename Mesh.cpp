#include "Mesh.hpp"

Mesh::Mesh(int N_, float maxCoord_)
    : N(N_), maxCoord(maxCoord_), nVertices(N_*N_), 
      dt(0.01f), mass(0.001f) {
        this->vertices = new vertex[nVertices];
        this->times = new float[nVertices];
        this->gravity = Force(0.0, -9.8, 0.0);
        this->wind = Force(0.0, 0.0, -8.0);
        restLengths.resize(2);
        kValues.resize(2);
        kValues[0] = 1000;
        kValues[1] = 1;
        generateMesh();
        generateIndices();
    };

void Mesh::generateMesh() {
    float d = ((2.f) * maxCoord) / (N-1);
    float z = 0.f;
    restLengths[0] = d;
    restLengths[1] = std::sqrt(2*d*d); // initial diagonal distance

    float y = maxCoord;

    for (int i = 0; i < N; i++) {
        float x = (-1.f) * maxCoord;
        for(int j = 0; j < N; j++) {
            vertex v = {x, y, z, 1.f, 1.f, 1.f, 0.0f, 0.0f, 1.0f};

            int index = j + (i*N);
            vertices[index] = v;

            std::vector<float> _oldPos;
            _oldPos.push_back(x);
            _oldPos.push_back(y);
            _oldPos.push_back(z);
            _oldPos.resize(3);
            oldPos.push_back(_oldPos);

            Force force(0.0, 0.0, 0.0);
            forces.push_back(force.force);

            std::vector<neighbour> vNN = determineNN(index);
            NN.push_back(vNN);

            x += d;
        }
        y -= d;
    }

    return;
}

std::vector<neighbour> Mesh::determineNN(int index) {
    // Determine the nearest neighbours and the type of spring.
    // 0 - structural (adajescent)
    // 1 - shear (diagonal)

    std::vector<neighbour> vNN;

    /* lets figure out it is a corner first */

    if(index == 0) //top left
        { vNN.push_back(neighbour(index+1,0)); vNN.push_back(neighbour(index+N,0)); vNN.push_back(neighbour(index+N+1,1)); vNN.resize(3); return vNN; }
    if(index == (N-1)) // top right
        { vNN.push_back(neighbour(index-1,0)); vNN.push_back(neighbour(index+N,0)); vNN.push_back(neighbour(index+N-1,1)); vNN.resize(3); return vNN; }
    if(index == (N*N-N)) // bottom left
        { vNN.push_back(neighbour(index+1,0)); vNN.push_back(neighbour(index-N,0)); vNN.push_back(neighbour(index-N+1,1)); vNN.resize(3); return vNN;}
    if(index == (N*N-1)) // bottom right
        { vNN.push_back(neighbour(index-1,0)); vNN.push_back(neighbour(index-N,0)); vNN.push_back(neighbour(index-N-1,1)); vNN.resize(3); return vNN;}

    /* checking top edge of mesh */
    if(index < N) {
        vNN.push_back(neighbour(index-1,0)); vNN.push_back(neighbour(index+1,0)); vNN.push_back(neighbour(index+N,0)); vNN.push_back(neighbour(index+N-1,1)); vNN.push_back(neighbour(index+N+1,1));
        vNN.resize(5); 
        return vNN;
    }

    /* checking left edge of mesh */
    if (index % N == 0) {
        vNN.push_back(neighbour(index+1,0)); vNN.push_back(neighbour(index-N,0)); vNN.push_back(neighbour(index-N+1,1)); vNN.push_back(neighbour(index+N,0)); vNN.push_back(neighbour(index+N+1,1));
        vNN.resize(5); 
        return vNN;
    }

    /* checking right edge of mesh */
    if((index+1) % N == 0) {
        vNN.push_back(neighbour(index-1,0)); vNN.push_back(neighbour(index-N,0)); vNN.push_back(neighbour(index-N-1,1)); vNN.push_back(neighbour(index+N,0)); vNN.push_back(neighbour(index+N-1,1));
        vNN.resize(5); 
        return vNN;
    }

    /* checking bottom edge of mesh */
    if((index >= (N*N-N)) && (index < (N*N))) {
        vNN.push_back(neighbour(index-1,0)); vNN.push_back(neighbour(index+1,0)); vNN.push_back(neighbour(index-N,0)); vNN.push_back(neighbour(index-N-1,1)); vNN.push_back(neighbour(index-N+1,1));
        vNN.resize(5); 
        return vNN;
    }

    vNN.push_back(neighbour(index-1,0)); vNN.push_back(neighbour(index+1,0)); vNN.push_back(neighbour(index-N,0)); vNN.push_back(neighbour(index+N,0));
    vNN.push_back(neighbour(index-N-1,1)); vNN.push_back(neighbour(index-N+1,1)); vNN.push_back(neighbour(index+N-1,1)); vNN.push_back(neighbour(index+N+1,1));
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

    vec force = k * x * n;
    return force;
}

vec squareVector(vec v) { 
    for (auto i : v) i *= i; 
    return v;
}

// lots of redundant vector operations here
// could be combined into one proc
void Mesh::constrainDeformation(int i) {
    vec v = vectorize(&vertices[i]);
    GLuint index, springType;

    for (auto nn : NN[i]) {
        index = nn.first;
        springType = nn.second;
        vec vc = vectorize(&vertices[index]);
        vec d = v - vc;

        float tolerence = 0.1f; // MOVE TO GLOBAL

        float d_scalar = norm_2(d);
        float deformation = (d_scalar - restLengths[springType]) / d_scalar;

        if (deformation > tolerence) {
            // inverse procedure
            v += 0.5 * d_scalar * d;
            vc -= 0.5 * d_scalar * d;
        }
    }
}

void Mesh::verlet() {
    for (int i = N; i < nVertices; i++) {
        vertex* curVertex = &vertices[i];
        std::vector<vec> connectedMasses;
        vec pos = vectorize(curVertex);
        vec old = oldPos[i];
        oldPos[i] = pos;
        vec totalForce = forces[i];
        vec a = totalForce; // / mass;

        // std::cout << totalForce[0] << " " << totalForce[1] << " " << totalForce[2] << std::endl;
        curVertex->x = (2.00 * pos[0]) - old[0] + (a[0] * dt * dt);
        curVertex->y = (2.00 * pos[1]) - old[1] + (a[1] * dt * dt);
        curVertex->z = (2.00 * pos[2]) - old[2] + (a[2] * dt * dt);

        //debug code
        // if(i == 399) {
        //     std::cout << "accel:" << a[0] << " " << a[1] << " " << a[2] << std::endl;
        //     std::cout << "pos:" << oldPos[i][0] << " " << oldPos[i][1] << " " << oldPos[i][2] << std::endl;
        // }
            //std::cout << curVertex->x << " " << curVertex->y << " " << curVertex->z << std::endl;

        constrainDeformation(i);
    }
}


void Mesh::update() {
    
    for (int i = N; i < nVertices; i++) {
        std::vector<vec> springForces;
        vertex* curVertex = &vertices[i];
        vec pos = vectorize(curVertex);

        vec totalForce = Force(0.0,0.0,0.0).force;

        for (auto nn : NN[i]) { // calculate spring forces
            vertex* nnv = &vertices[nn.first];
            totalForce += springForce(vectorize(nnv), pos, kValues[nn.second], restLengths[nn.second]);
        }

        //std::cout << totalForce[0] << " " << totalForce[1] << " " << totalForce[2] << std::endl;

        if (!(cross(gravity.force, Force(0.0, 0.0, -1.0).force) == 0))
            totalForce += gravity.force * mass;
        totalForce += wind.force;
        totalForce -= totalForce * 0.001; //damper, bcuz no veloci=ty

        forces[i] = totalForce;
    }
    
    verlet();

    return;
}