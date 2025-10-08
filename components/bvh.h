#ifndef BVH_H
#define BVH_H

#include "../aabb.h"
#include "../sceneObjects.h"
#include "../rayHitInfo.h"
#include <memory>
#include <algorithm>

class bvh_node : public sceneObject {
public:
    bvh_node(sceneObjects scene) : bvh_node(scene.objects, 0, scene.objects.size()) {
        // Apparently theres some subtlety with C++ here? The constructor (without span indicies) creates
        // implicit copy of the sceneObjects, which we will modify, the lifetime of the copied scene only
        // extends until this constructor exits, which is ok apparently because we only need to persist
        // the resulting Bounding Volume Hierarchy.
    }

    bvh_node(std::vector<std::shared_ptr<sceneObject>> &objects, size_t start, size_t end) {
    int axis = random_int(0, 2);

    auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    } else if (object_span == 2) {
        left = objects[start];
        right = objects[start + 1];
    } else {
        std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

        auto mid = start + object_span / 2;
        left = make_shared<bvh_node>(objects, start, mid);
        right = make_shared<bvh_node>(objects, mid, end);
    }
    bbox = aabb(left->bounding_box(), right->bounding_box());

    }

    bool intersect(const ray &r, interval ray_t, rayHitInfo &ray_hit_info) const override {
        if (!bbox.intersect(r, ray_t))
            return false;

        bool intersect_left = left->intersect(r, ray_t, ray_hit_info);
        bool intersect_right = right->intersect(r, 
                interval(ray_t.min, intersect_left ? ray_hit_info.t : ray_t.max), ray_hit_info);

        return intersect_left || intersect_right;
    }
    
    aabb bounding_box() const override { return bbox; }


private:
    shared_ptr<sceneObject> left;
    shared_ptr<sceneObject> right;
    aabb bbox;
    
    static bool box_compare(const shared_ptr<sceneObject> a, const shared_ptr<sceneObject> b, int axis_index) {
    auto a_axis_interval = a->bounding_box().axis_interval(axis_index);
    auto b_axis_interval = b->bounding_box().axis_interval(axis_index);
    return a_axis_interval.min < b_axis_interval.min;
    }

    static bool box_x_compare (const shared_ptr<sceneObject> a, const shared_ptr<sceneObject> b) {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare (const shared_ptr<sceneObject> a, const shared_ptr<sceneObject> b) {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare (const shared_ptr<sceneObject> a, const shared_ptr<sceneObject> b) {
        return box_compare(a, b, 2);
    }

};


#endif
