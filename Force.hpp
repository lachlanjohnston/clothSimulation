#include <boost/numeric/ublas/vector.hpp>

typedef boost::numeric::ublas::vector<float, std::vector<float> > vec;

class Force{
public:
    vec force;

    Force();
    Force(float x, float y, float z);
    // return new accel vector
    vec operator+(vec applyTo);
};