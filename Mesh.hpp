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

typedef runge_kutta_dopri5<vec > stepperType;
// typedef std::vector<float> vec;

class Mesh {
public:
    struct vertex {
        float x, y, z, w;
        float r, g, b;
    };

    vertex* vertices;
    std::vector<GLuint> indices;
    float maxCoord; 
    int nVertices, nIndices, N;

    Mesh(int N_, float maxCoord_);
    void generateMesh();
    void generateIndices();

    // physics 

    std::vector<vec > velocities;
    const float dt = 0.01f;
    const static float mass = 0.01f;
    float* times;
    stepperType stepper;
    Force gravity;

    struct sys {
        vec g; float mass;
        void operator()(const vec &pos, vec &velocity, double /*t*/) {
            // need to implement other forces
            vec force = g * mass;
            velocity += force;
        }
    };
    void update();
};
