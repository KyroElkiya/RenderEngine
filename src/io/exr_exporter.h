#pragma once

#include "../core/color.h"
#include <vector>
#include <string>

inline void write_exr(const std::string& filename, int width, int height, const std::vector<std::vector<pixel>>& framebuffer);
