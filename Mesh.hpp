#pragma once

#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <boost/numeric/odeint.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "Force.hpp"

using namespace boost::numeric::odeint;
using namespace boost::numeric::ublas;

typedef boost::numeric::ublas::vector<float, std::vector<float> > vec;

typedef runge_kutta_dopri5<vec> stepperType;
// typedef std::vector<float> vec;

class Mesh {
public:
    struct vertex {
        float x, y, z;
        float r, g, b;
    };

    vertex* vertices;
    std::vector<GLuint> indices;
    float maxCoord; 
    int nVertices, nIndices, N;

    Mesh(int N_, float maxCoord_);
    void generateMesh();
    void generateIndices();
    std::vector<GLuint> determineNN(int index);

    // physics 

    std::vector<vec > velocities;
    std::vector<std::vector<GLuint> > NN;
    float dt;
    float mass;
    float restLength;
    float* times;
    stepperType stepper;
    Force gravity;

    vec vectorize(vertex* v);
    float normalize(vec v, float norm);
    vec squareVector(vec v);
        
    void update();
};

