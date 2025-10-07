#ifndef CAMERA_H
#define CAMERA_H

#include "../vec3.h"

class camera {
public:
    point3 center = point3(0.0, 0.0, 0.0);
    double focal_length = 1.0;
};

#endif
