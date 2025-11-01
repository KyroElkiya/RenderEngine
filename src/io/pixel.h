#pragma once

#include "../core/color.h"
#include "../core/math/vec3.h"

struct pixel {
    color rgb;
    double hit;
    vec3 N;
    double depth;
    vec3 P;
};
