#ifndef AABB_H
#define AABB_H

#include "vec3.h"
#include "interval.h"


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

    aabb(const aabb &box0, const aabb &box1) {
        x = interval(box0.x, box1.x);
        y = interval(box0.y, box1.y);
        z = interval(box0.z, box1.z);
    }

    const interval& axis_interval(int n) const {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }
    
    bool intersect(const ray &r, interval ray_t) const {
        const point3 &ray_orig = r.origin();
        const vec3   &ray_dir  = r.direction();

        for (int axis = 0; axis < 3; axis++) {
            const interval &ax = axis_interval(axis);
            const double adinv = 1.0 / ray_dir[axis];

            auto t0 = (ax.min - ray_orig[axis]) * adinv;
            auto t1 = (ax.max - ray_orig[axis]) * adinv;

            if (t0 < t1) {
                if (t0 > ray_t.min) ray_t.min = t0;
                if (t0 < ray_t.max) ray_t.max = t1;
            } else {
                if (t1 > ray_t.min) ray_t.min = t1;
                if (t1 < ray_t.max) ray_t.max = t0;
            }

            if (ray_t.max <= ray_t.min)
                return false;
        }
        return true;
    }
};

#endif
