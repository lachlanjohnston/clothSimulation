#pragma once

#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <boost/numeric/odeint.hpp>
#include <boost/numeric/ublas/vector.hpp>
using namespace boost::numeric::odeint;
using namespace boost::numeric::ublas;

typedef runge_kutta_dopri5<std::vector<float> > stepperType;

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

    // physics 

    std::vector<vector<float> > velocity;
    const float dt = 0.01f;
    float mass = 0.01f;
    float gravity = 9.8f;
    float* times;
    stepperType stepper;
    static void step(std::vector<float> pos, vector<float> dposdt, const double /* t */) ;

    void update();
};

