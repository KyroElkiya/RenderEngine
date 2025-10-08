#ifndef SPHERE_H
#define SPHERE_H

#include "../rayHitInfo.h"
#include "../common.h"
#include <unistd.h>
#include "../aabb.h"

class sphere : public sceneObject {
public:
    sphere(const point3 center, double radius, shared_ptr<material> mat) 
        : center(center), radius(std::fmax(0, radius)), mat(mat) {
        auto rvec = vec3(radius, radius, radius);
        bbox = aabb(center - rvec, center + rvec);
    }
    
    // TODO: when I add movement to the spheres, I need to write aabb function for getting bbox of moving spheres as well

    aabb bounding_box() const override { return bbox; }

    bool intersect(const ray &r, interval ray_t, rayHitInfo &ray_hit_info) const override {
        vec3 oc = center - r.origin();
        double a = length2(r.direction());
        double b = dot(r.direction(), oc);
        double c = length2(oc) - radius * radius;
        double discriminant = b*b - a*c;

        if (discriminant < 0)
            return false;

        double sqrtd = std::sqrt(discriminant);

        double root = ( b - sqrtd ) / a;
        if (!ray_t.surrounds(root)) {
            root = (b + sqrtd ) / a;
            if (!ray_t.surrounds(root)) {
                return false;
            }
        }

        ray_hit_info.t = root;
        ray_hit_info.P = r.at(ray_hit_info.t);
        vec3 outward_normal = normalize((ray_hit_info.P - center) / radius);
        ray_hit_info.set_face_normal(r, outward_normal);
        ray_hit_info.mat = mat;

        return true;
    }

private:
    point3 center;
    double radius;
    shared_ptr<material> mat;
    aabb bbox;
};

#endif
