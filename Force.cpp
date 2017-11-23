#include "Force.hpp"

Force::Force() {}

Force::Force(float x, float y, float z) {
    std::vector<float> temp;
    temp.push_back(x);
    temp.push_back(y);
    temp.push_back(z);

    force = vec(temp);
}

vec Force::operator+(vec applyTo) {
    force += applyTo;

    return force;
}