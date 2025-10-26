#pragma once

#include "../core/math/vec3.h"
#include <string>
#include <ostream>


inline void print(std::string message) {
    std::cout << message << std::endl;

}

inline void nan_vec3(vec3 v0, std::string object) {
    if (!std::isfinite(v0.x) || !std::isfinite(v0.y) || !std::isfinite(v0.z))
        std::cout << "NaN detected at: " << object << std::endl;
}
