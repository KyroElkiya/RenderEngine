#pragma once

#include "../core/math/vec3.h"
#include <vector>
#include "sceneObjects.h"
#include "../material/material.h"
#include "../core/utils.h"

struct ivec3 {
    int x, y, z;

    ivec3() : x(0), y(0), z(0) {}
    ivec3(int v) : x(v), y(v), z(v) {}
    ivec3(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}

    int& operator[](int i) {
        switch(i) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("ivec3 index out of range");
        }
    }

    const int& operator[](int i) const {
        switch(i) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("ivec3 index out of range");
        }
    }

    bool operator!=(const ivec3 &v1) {
        return x != v1.x && y != v1.y && z != v1.z;
    }

    bool operator==(const ivec3 &v1) {
        return x == v1.x && y == v1.y && z == v1.z;
    }

};

struct vec2 {
    double u, v;

    vec2() : u(0), v(0) {}
    vec2(double x) : u(x), v(x) {}
    vec2(double x, double y) : u(x), v(y) {}
    
    double& operator[](int i) {
        switch(i) {
            case 0: return u;
            case 1: return v;
            default: throw std::out_of_range("vec2 index out of range");
        }
    }

    const double& operator[](int i) const {
        switch(i) {
            case 0: return u;
            case 1: return v;
            default: throw std::out_of_range("vec2 index out of range");
        }
    }


    bool operator==(const vec2 &v1) {
        return u == v1.u && v == v1.v;
    }
};


struct tri {
    ivec3 vert_indices = ivec3(-1);
    ivec3 uv_indices = ivec3(-1);
    ivec3 norm_indices = ivec3(-1);
    bool has_uvs = false;
    bool has_vtx_norms = false;
};


class mesh : public sceneObject {
public:

    mesh(shared_ptr<material> mat) : mat(mat) {}

    aabb bounding_box() const override { return bbox; }

    void addVertex   (const vec3 &v)       { vertices.push_back(v); }
    void addTriangle (const tri &triangle) { faces.push_back(triangle); }            
    void addNormals  (const vec3 &normals) { vertexNormals.push_back(normals); }
    void addUVs      (const vec2 &uv)      { uvs.push_back(uv); }

    const std::vector<vec3>&  getVertices() const { return vertices; }
    const std::vector<tri>&   getFaces()    const { return faces; }
    const std::vector<vec3>&  getNormals()  const { return vertexNormals; }
    const std::vector<vec2>&  getUVs()      const { return uvs; } 

    bool intersect(const ray &r, interval ray_t, rayHitInfo &ray_hit_info) const override {
        
        float closestT = std::numeric_limits<double>::max();
        bool intersected = false;
 
        for (const auto& triIndex : faces) {
            
            // TODO: Build in safeguard for if the below attributes do not exist. <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

            const vec3& v0 = vertices[triIndex.vert_indices.x];
            const vec3& v1 = vertices[triIndex.vert_indices.y];
            const vec3& v2 = vertices[triIndex.vert_indices.z];
            
            if (triIndex.has_uvs) {
                const vec2& uv0 = uvs[triIndex.uv_indices.x];
                const vec2& uv1 = uvs[triIndex.uv_indices.y];
                const vec2& uv2 = uvs[triIndex.uv_indices.z];
            }

            vec3 edge0 = v1 - v0;
            vec3 edge1 = v2 - v0;

            vec3 dir = normalize(r.direction()); 
            vec3 p = cross(dir, edge1);
            double det = dot(edge0, p);
        
            if (det < EPSILON)
                continue;

            double inv_det = 1/det;

            vec3 t = r.origin() - v0;
            double u = dot(t, p) * inv_det;
        
            if (u < 0 || u > 1)
                continue;

            vec3 q = cross(t, edge0);
            double v = dot(dir, q) * inv_det;
        
            if (v < 0 || u + v > 1)
                continue;
    
            double dist = dot(edge1, q) * inv_det;

            if (dist < 0)
                continue;

            closestT = dist;
            ray_hit_info.t = dist;
            ray_hit_info.P = r.at(dist);
            
            if (triIndex.has_vtx_norms) {
                const vec3& n0 = vertexNormals[triIndex.norm_indices.x];
                const vec3& n1 = vertexNormals[triIndex.norm_indices.y];
                const vec3& n2 = vertexNormals[triIndex.norm_indices.z];

                ray_hit_info.N = normalize(n0 * (1 - u - v) + n1 * u + n2 * v);
            } else {
                ray_hit_info.N = normalize(cross(edge0, edge1));
            }
            ray_hit_info.set_face_normal(r, ray_hit_info.N);
            ray_hit_info.mat = mat;

            intersected = true;
        }
        return intersected;
    }

void finalize() {
    if (vertices.empty()) return;

    point3 min = vertices[0];
    point3 max = vertices[0];

    for (const auto& v : vertices) {
        min = vec3(fmin(min.x, v.x), fmin(min.y, v.y), fmin(min.z, v.z));
        max = vec3(fmax(max.x, v.x), fmax(max.y, v.y), fmax(max.z, v.z));
    }

    bbox = aabb(min, max);
}

private:
    std::vector<vec3> vertices;
    std::vector<vec3> vertexNormals;
    std::vector<vec2> uvs;
    std::vector<tri> faces;
    shared_ptr<material> mat;
    aabb bbox;

};


