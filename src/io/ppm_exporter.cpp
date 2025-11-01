#pragma once

#include "../core/color.h"
#include "ppm_exporter.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "pixel.h"

inline void write_ppm(const std::string& filename, int width, int height, const std::vector<std::vector<pixel>>& framebuffer ) {
    std::ofstream out(filename);

    if (!out) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    out << "P3\n" << width << " " << height << "\n255\n";
 
    for (int j = 0; j < height; j++) 
        for (int i = 0; i < width; i++) 
            write_color(out, framebuffer[j][i].rgb);

}
