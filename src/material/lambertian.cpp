#pragma once

#include "material.h"
#include "../core/color.h"


class lambertian : public material {
public:
    lambertian(const color &albedo) : albedo(albedo) {}

    bool scatter(const ray &r_in, const rayHitInfo &ray_hit_info, color &attenuation, ray &scattered) const override {
    auto scatter_direction = ray_hit_info.N + random_unit_vector();

    if (scatter_direction.near_zero())
        scatter_direction = ray_hit_info.N;

    scattered = ray(ray_hit_info.P, scatter_direction);
    attenuation = albedo;

    return true;
    }
private:
    color albedo;
};


