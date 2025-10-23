#pragma once

#include "../core/interaction/rayHitInfo.h"
#include "../core/math/vec3.h"
#include "../core/color.h"
#include "../core/random.h"

class material {
public:
    virtual ~material() = default;

    virtual bool scatter(const ray &r_in, const rayHitInfo &ray_hit_info, color &attenuation, ray &scattered) const {
        return false;
    }

};

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

class pbr : public material {
public:
    pbr(const color &albedo, double specular, color spec_color, double spec_rough)
        : albedo(albedo), specular(specular < 1 ? specular : 1), spec_color(spec_color), spec_rough(spec_rough) {}

    bool scatter(const ray &r_in, const rayHitInfo &ray_hit_info, color &attenuation, ray &scattered) const override {
        return 0;

        // TODO: impliment specular material habit, add transmission, subsurface and emission <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    }
private:
    color albedo;
    double specular;
    color spec_color;
    double spec_rough;
};


