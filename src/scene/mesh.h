#pragma once

#include "../core/math/vec3.h"
#include <vector>
#include "sceneObjects.h"
#include "../material/material.h"
#include "../core/utils.h"
#include "../core/math/ivec3.h"
#include "../core/math/vec2.h"
#include "tri.h"
#include "bvh/bvh.h"
#include "triPrim.h"


class mesh : public sceneObject {
public:

    mesh(std::shared_ptr<material> mat) : mat(mat) {}

    void addVertex   (const vec3 &v)       { vertices.push_back(v); }
    void addTriangle (const tri &triangle) { faces.push_back(triangle); }            
    void addNormals  (const vec3 &normals) { vertexNormals.push_back(normals); }
    void addUVs      (const vec2 &uv)      { uvs.push_back(uv); }

    const std::vector<vec3>&  getVertices() const { return vertices; }
    const std::vector<tri>&   getFaces()    const { return faces; }
    const std::vector<vec3>&  getNormals()  const { return vertexNormals; }
    const std::vector<vec2>&  getUVs()      const { return uvs; } 

    bool intersect(const ray &r, interval ray_t, rayHitInfo &ray_hit_info) const override {
        if (!internal_bvh) return false;
        return internal_bvh->intersect(r, ray_t, ray_hit_info);
    }

void finalize(int max_depth = 32) {
    if (vertices.empty() || faces.empty()) return;
    
    std::vector<std::shared_ptr<sceneObject>> triangles;
    triangles.reserve(faces.size());

    for (const auto &triIndex: faces) {
        const vec3& v0 = vertices[triIndex.vert_indices.x];
        const vec3& v1 = vertices[triIndex.vert_indices.y];
        const vec3& v2 = vertices[triIndex.vert_indices.z];

        const vec3 *n0 = nullptr, *n1 = nullptr, *n2 = nullptr;
        if (triIndex.has_vtx_norms && !vertexNormals.empty()) {
            n0 = &vertexNormals[triIndex.norm_indices.x];
            n1 = &vertexNormals[triIndex.norm_indices.y];
            n2 = &vertexNormals[triIndex.norm_indices.z];
        }
    triangles.push_back(std::make_shared<triPrim>(v0, v1, v2, mat, n0, n1, n2));
    }

    // Build internal bvh
    internal_bvh = std::make_shared<bvh_node>(triangles, 0, triangles.size(), 0, max_depth);

    bbox = internal_bvh->bounding_box();

}

    aabb bounding_box() const override { return bbox; }

    private:
    std::vector<vec3> vertices;
    std::vector<vec3> vertexNormals;
    std::vector<vec2> uvs;
    std::vector<tri> faces;
    std::shared_ptr<material> mat;
    std::shared_ptr<bvh_node> internal_bvh;
    aabb bbox;

};


