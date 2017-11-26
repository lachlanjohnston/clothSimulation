#include "Mesh.hpp"

Mesh::Mesh(int N_, float maxCoord_)
    : N(N_), maxCoord(maxCoord_), nVertices(N_*N_), 
      dt(0.005f), mass(0.001f) {
        this->vertices = new vertex[nVertices];
        this->times = new float[nVertices];
        this->gravity = Force(0.0, -9.8, 0.0);
        this->wind = Force(0.0, 0.0, -.005);
        kValues.resize(2);
        kValues[0] = 20;
        kValues[1] = 15;
        restLengths.resize(2);

        ignoreVertices = {0, 1, 20, 21, 18, 19, 38, 39};
        //ignoreVertices = {0, 1, N_, N_+1, N_-1, N_-2, 2*N_ - 1, 2*N_ - 2};

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

void Mesh::assign(int i, vec v) {
    vertices[i].x = v[0];
    vertices[i].y = v[1];
    vertices[i].z = v[2];
}

// lots of redundant vector operations here
// could be combined into one proc
void Mesh::constrainDeformation(int i) {
    vec v = vectorize(&vertices[i]);
    GLuint index, springType;

    vec vc = oldPos[i];
    vec d = v - vc;

    if()
    float tolerence = 0.1f; // MOVE TO GLOBAL

    float d_scalar = norm_2(d);
    float deformation = (d_scalar - restLengths[0]) / d_scalar;

    if (deformation > tolerence) {
        // inverse procedure
        v += 0.5 * d_scalar * d;
        vc -= 0.5 * d_scalar * d;
        assign(i, v);
        oldPos[i] = vc;
    }
}

void Mesh::verlet() {
    for (int i = 0; i < nVertices; i++) {
        if (ignoreVertices.count(i) > 0) continue;

        vertex* curVertex = &vertices[i];
        std::vector<vec> connectedMasses;
        vec pos = vectorize(curVertex);
        vec old = oldPos[i];
        oldPos[i] = pos;
        

        // FORCES

        vec totalForce = forces[i];
        totalForce += wind.force;
        totalForce += gravity.force * mass;
        totalForce -= totalForce *  0.001; //damper, bcuz no velocity
        vec a = totalForce / mass; // / mass;

         //td::cout << totalForce[0] << " " << totalForce[1] << " " << totalForce[2] << std::endl;

        // STEPPER

        curVertex->x = (2.0 * pos[0]) - old[0] + (a[0] * dt * dt);
        curVertex->y = (2.0  * pos[1]) - old[1] + (a[1] * dt * dt);
        curVertex->z = (2.0  * pos[2]) - old[2] + (a[2] * dt * dt);

        //debug code
        if(i == 399) {
            std::cout << "accel:" << a[0] << " " << a[1] << " " << a[2] << std::endl;
            std::cout << "pos:" << oldPos[i][0] << " " << oldPos[i][1] << " " << oldPos[i][2] << std::endl;
        }
            //std::cout << curVertex->x << " " << curVertex->y << " " << curVertex->z << std::endl;

        constrainDeformation(i);
    }
}


void Mesh::update() {
    
    for (int i = 0; i < nVertices; i++) {
        if (ignoreVertices.count(i) > 0) continue;

        std::vector<vec> springForces;
        vertex* curVertex = &vertices[i];
        vec pos = vectorize(curVertex);

        vec totalForce = Force(0.0,0.0,0.0).force;

        for (auto nn : NN[i]) { // calculate spring forces
            vertex* nnv = &vertices[nn.first];
            totalForce += springForce(vectorize(nnv), pos, kValues[nn.second], restLengths[nn.second]);
        }

        //std::cout << totalForce[0] << " " << totalForce[1] << " " << totalForce[2] << std::endl;


        forces[i] = totalForce;
    }
    
    verlet();

    return;
}