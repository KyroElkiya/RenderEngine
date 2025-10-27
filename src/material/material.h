#pragma once

#include "../core/interaction/rayHitInfo.h"
#include "../core/math/vec3.h"
#include "../core/color.h"
#include "../core/random.h"

class material {
public:
    virtual ~material() = default;
    
    virtual color emit() const {
        return color(0);
    }

    virtual bool scatter(const ray &r_in, const rayHitInfo &ray_hit_info, color &attenuation, ray &scattered) const {
        return false;
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
    color  albedo;
    double specular;
    color  spec_color;
    double spec_rough;
};


