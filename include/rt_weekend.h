#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

using std::make_shared;
using std::shared_ptr;

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.;
}

inline float random_float() {
    static std::uniform_real_distribution<float> distribution(0., 1.);
    static std::mt19937 generator;
    return distribution(generator);
}

inline float random_float(float min, float max) {
    // returns a random float in [min, max)
    return min + (max - min) * random_float();
}

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"
