#pragma once

#include <string>
#include <vector>
#include "../core/color.h"

inline void write_ppm(const std::string& filename, int width, int height, const std::vector<std::vector<color>>& framebuffer);
