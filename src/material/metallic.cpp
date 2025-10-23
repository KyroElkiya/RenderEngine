#pragma once

#include "material.h"
#include "../core/color.h"


class metal : public material {
public:
    metal(const color &albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray &r_in, const rayHitInfo &ray_hit_info, color &attenuation, ray &scattered) const override {

        vec3 reflected = reflect(r_in.direction(), ray_hit_info.N);
        reflected = normalize(reflected) + (fuzz * random_unit_vector());
        scattered = ray(ray_hit_info.P, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), ray_hit_info.N) > 0);
    }
private:
    color albedo;
    double fuzz;
};


