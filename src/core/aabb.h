#pragma once

#include "math/vec3.h"
#include "math/interval.h"
#include "ray.h"

class aabb {
public:
    interval x, y, z;

    aabb() {} // Default AABB is empty since intervals are empty by default

    aabb(const interval &x, const interval &y, const interval &z) : x(x), y(y), z(z) {}

    aabb(const point3 &a, const point3 &b) {
        // Treat input points and extreme points of bounding box, so min and max of both is the bounding box itself
        
        x = (a.x <= b.x) ? interval(a.x, b.x) : interval(b.x, a.x);
        y = (a.y <= b.y) ? interval(a.y, b.y) : interval(b.y, a.y);
        z = (a.z <= b.z) ? interval(a.z, b.z) : interval(b.z, a.z);   
    }

    //aabb(const aabb &box0, const aabb &box1) {
    //    x = interval(box0.x, box1.x);
    //    y = interval(box0.y, box1.y);
    //   z = interval(box0.z, box1.z);
    //}

    aabb(const aabb& box0, const aabb& box1) {
        x = interval(
            std::min(box0.x.min, box1.x.min),
            std::max(box0.x.max, box1.x.max)
        );
        y = interval(
            std::min(box0.y.min, box1.y.min),
            std::max(box0.y.max, box1.y.max)
        );
        z = interval(
            std::min(box0.z.min, box1.z.min),
            std::max(box0.z.max, box1.z.max)
        );
    }

    const interval& axis_interval(int n) const {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }
    
    int longest_axis() const {
        double dx = x.max - x.min;
        double dy = y.max - y.min;
        double dz = z.max - z.min;

        if (dx > dy && dx > dz)
            return 0;
        else if (dy > dz)
            return 1;
        else
            return 2;
    }

    point3 centroid() const {
        return point3(0.5 * (x.min + x.max), 0.5 * (y.min + y.max), 0.5 * (z.min + z.max));
    }

    bool intersect(const ray &r, interval ray_t) const {
        const point3 &ray_orig = r.origin();
        const vec3   &ray_dir  = r.direction();

        for (int axis = 0; axis < 3; axis++) {
            const interval &ax = axis_interval(axis);
            const double adinv = (std::abs(ray_dir[axis]) > 1e-8) ? 1.0 / ray_dir[axis]: std::numeric_limits<double>::infinity();

            auto t0 = (ax.min - ray_orig[axis]) * adinv;
            auto t1 = (ax.max - ray_orig[axis]) * adinv;
            
            if (adinv < 0.0)
                std::swap(t0, t1);

            ray_t.min = std::max(t0, ray_t.min);
            ray_t.max = std::min(t1, ray_t.max);

            if (ray_t.max <= ray_t.min)
                return false;
        }
        return true;
    }
};

