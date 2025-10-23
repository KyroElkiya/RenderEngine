#pragma once

#include "../core/interaction/rayHitInfo.h"
#include <algorithm>
#include <memory>
#include <vector>
#include "../core/aabb.h"

using std::make_shared;
using std::shared_ptr;

class sceneObjects : public sceneObject {
public:
    std::vector<shared_ptr<sceneObject>> objects;

    sceneObjects() {}
    sceneObjects(shared_ptr<sceneObject> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<sceneObject> object) {
        objects.push_back(object);
        bbox = aabb(bbox, object->bounding_box());
    } 

    bool intersect(const ray &r, interval ray_t, rayHitInfo &ray_hit_info) const override {
        rayHitInfo temp_info;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for ( const auto &object : objects ) {
            if (object->intersect(r, interval(ray_t.min, closest_so_far), temp_info)) {
                hit_anything = true;
                closest_so_far = temp_info.t;
                ray_hit_info = temp_info;
            }
        }
    return hit_anything;
    }

    aabb bounding_box() const override { return bbox; }

private:
    aabb bbox;
};

