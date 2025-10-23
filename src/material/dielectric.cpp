#pragma once

#include "material.h"
#include "../core/color.h"

class dieletric : public material {
public:
    dieletric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray &r_in, const rayHitInfo &ray_hit_info, color &attenuation, ray &scattered) const override {
        
        attenuation = color(1.0, 1.0, 1.0);
        double ri = ray_hit_info.front_face ? (1.0/refraction_index) : refraction_index;
        vec3 normalized_direction = normalize(r_in.direction());
        double cos_theta = std::fmin(dot(-normalized_direction, ray_hit_info.N), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, ri) > random_double())
            direction = reflect(normalized_direction, ray_hit_info.N);
        else 
            direction = refract(normalized_direction, ray_hit_info.N, ri);

        scattered = ray(ray_hit_info.P, direction);
        return true;
    }
private:
    double refraction_index;

    static double reflectance(double cosine, double refraction_index) {
        // Use Schlicks approximation for reflectance.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1 - cosine), 5);
    }
};


