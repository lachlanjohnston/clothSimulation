#pragma once

#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <math.h>
#include <boost/numeric/odeint.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "Force.hpp"

using namespace boost::numeric::odeint;
using namespace boost::numeric::ublas;

typedef boost::numeric::ublas::vector<float, std::vector<float> > vec;

typedef runge_kutta_dopri5<vec> stepperType;
typedef std::vector<vec> stateType;
typedef std::pair<GLuint, GLuint> neighbour; // this type is the pair we want
// typedef std::vector<float> vec;

class Mesh {
public:
    struct vertex {
        float x, y, z;
        float r, g, b;
        float n1, n2, n3;
    };

    vertex* vertices;
    std::set<GLuint> ignoreVertices;
    std::vector<GLuint> indices;
    float maxCoord; 
    int nVertices, nIndices, N;

    Mesh(int N_, float maxCoord_);
    void generateMesh();
    void generateIndices();
    std::vector<neighbour> determineNN(int index);
    void verlet();
    void assign(int i, vec v);

    // physics 

    std::vector<vec > velocities;
    std::vector<vec > oldPos;
    std::vector<vec > forces;


    std::vector<std::vector<neighbour> > NN; // note change to index:type mapping
    std::vector<float> restLengths; // 0: structural spring, 1: shear, 2: maybe later...
    std::vector<float> kValues;
    float dt;
    float mass;
    float* times;
    Force gravity, wind;

    vec vectorize(vertex* v);
    float normalize(vec v, float norm);
    vec squareVector(vec v);

    void constrainDeformation(int i); // beta
        
    void update();
};

