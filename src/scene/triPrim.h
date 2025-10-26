#pragma once


#include "../core/math/vec3.h"
#include "../core/aabb.h"
#include "../core/interaction/rayHitInfo.h"
#include "../material/material.h"
#include "sceneObjects.h"
#include <memory>


class triPrim : public sceneObject {
public:
    triPrim(const vec3& v0, const vec3& v1, const vec3& v2, std::shared_ptr<material> mat,
            const vec3* n0 = nullptr, const vec3* n1 = nullptr, const vec3* n2 = nullptr) 
        : v0(v0), v1(v1), v2(v2), mat(mat), n0(n0), n1(n1), n2(n2) 
    {
        point3 min(
                std::min({v0.x, v1.x, v2.x}),
                std::min({v0.y, v1.y, v2.y}),
                std::min({v0.z, v1.z, v2.z})
                );

        point3 max(
                std::max({v0.x, v1.x, v2.x}),
                std::max({v0.y, v1.y, v2.y}),
                std::max({v0.z, v1.z, v2.z})
                );

        fix_bbox_degenerate(min, max);
        bbox = aabb(min, max);

    }

    bool intersect(const ray& r, interval ray_t, rayHitInfo& hit) const override {
        const double EPS = 1e-8;
        vec3 edge0 = v1 - v0;
        vec3 edge1 = v2 - v0;
        vec3 dir = r.direction();
        vec3 p = cross(dir, edge1);
        double det = dot(edge0, p);

        if (fabs(det) < EPS)
            return false;

        double inv_det = 1.0 / det;
        vec3 t = r.origin() - v0;
        double u = dot(t, p) * inv_det;
        if (u < 0.0 || u > 1.0) return false;

        vec3 q = cross(t, edge0);
        double v = dot(dir, q) * inv_det;
        if (v < 0.0 || u + v > 1.0) return false;

        double dist = dot(edge1, q) * inv_det;
        if (!ray_t.contains(dist)) return false;

        hit.t = dist;
        hit.P = r.at(dist);
        vec3 norm;
        // Compute normal
        if (n0 && n1 && n2) {
            norm = (*n0) * (1 - u - v) + (*n1) * u + (*n2) * v;
        } else {
            norm = cross(edge0, edge1);
        }
        
        double len2 = length2(norm);
        if (len2 < 1e-16 || !std::isfinite(len2)) {
            norm = vec3(0, 1, 0);
        } else {
            norm /= std::sqrt(len2);
        }

        hit.N = norm;

        hit.set_face_normal(r, hit.N);
        hit.mat = mat;

        return true;
    }

    aabb bounding_box() const override { return bbox; }

private:
    vec3 v0, v1, v2;
    const vec3* n0;
    const vec3* n1;
    const vec3* n2;
    std::shared_ptr<material> mat;
    aabb bbox;
};

