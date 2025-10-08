#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <limits>
#include <stdio.h>

using std::make_shared;
using std::shared_ptr;

// Setting up constant values

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Util functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    // Returns a random double between 0 and 1.
    return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random double between given min and max.
    return min + (max-min) * random_double();
}

inline int random_int(int min, int max) {
    return int(random_double(min, max + 1));
}

// Common headers

#include "interval.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"


#endif
