#pragma once

#include "math/vec3.h"
#include "math/interval.h"
#include "../debug/utils.h"

#include <iostream>
#include <cmath>

using color = vec3;

inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}

inline double linear_to_srgb(double linear_component) {
    return (linear_component <= 0.0031308) ? linear_component * 12.92 : 1.055 * std::pow(linear_component, 1.0/2.4) - 0.055;
}

inline color linear_to_srgb(vec3 col) {
    return color(linear_to_srgb(col.x), linear_to_srgb(col.y), linear_to_srgb(col.z));
}

inline color is_nan(vec3& col) {
    return color(std::isfinite(col.x) ? col.x : 1, std::isfinite(col.y) ? col.y : 0, std::isfinite(col.z) ? col.z : 1);
}

inline void write_color(std::ostream& out, const color &pixel_color) {

    //auto r = pixel_color.x;
    //auto g = pixel_color.y;
    //auto b = pixel_color.z;
    
    auto rgb = linear_to_srgb(pixel_color);
    //auto rgb = pixel_color;

    rgb = is_nan(rgb);
    nan_vec3(pixel_color, "Color.h Pixel Color");

    //r = linear_to_srgb(r);
    //g = linear_to_srgb(g);
    //b = linear_to_srgb(b);
    
    //r = std::isfinite(r) ? r : 1;
    //g = std::isfinite(g) ? g : 0;
    //b = std::isfinite(b) ? b : 1;

    static const interval intensity(0.000, 0.999);
    
    

    int red   = int ( 255.999 * intensity.clamp(rgb.x) );
    int green = int ( 255.999 * intensity.clamp(rgb.y) );
    int blue  = int ( 255.999 * intensity.clamp(rgb.z) );

    out << red << " " << green << " " << blue << "\n";
}

