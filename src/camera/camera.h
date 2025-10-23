#pragma once

#include "../core/math/vec3.h"

class camera {
public:
    double vfov = 90;
    point3 center = point3(0, 0, 0);
    point3 lookat = point3(0, 0, -1);
    point3 vup = point3(0, 1, 0);

    double focal_length = 1.0;
    double focus_distance = length(center - lookat);
    double defocus_angle = 0;
};


