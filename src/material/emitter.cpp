#pragma once

#include "../core/color.h"
#include "material.h"


class emitter : public material {
public:
    emitter(const color &emit_color, double emit_intensity) : emit_color(emit_color), emit_intensity(emit_intensity) {}

    color emit() const override {
        return emit_color * emit_intensity;
    }

     
    bool scatter(const ray &r_in, const rayHitInfo &ray_hit_info, color &attenuation, ray &scattered) const override {
        return false;
    }



private:
    color emit_color;
    double emit_intensity;
};
