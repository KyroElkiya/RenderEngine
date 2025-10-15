#ifndef MESH_H
#define MESH_H

#include "../vec3.h"
#include <vector>
#include "../sceneObjects.h"
#include "materials.h"

struct ivec3 {
    int x, y, z;

    ivec3() : x(0), y(0), z(0) {}
    ivec3(int v) : x(v), y(v), z(v) {}
    ivec3(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}

    bool operator==(const ivec3 &v1) {
        return x == v1.x && y == v1.y && z == v1.z;
    }

};


class mesh : public sceneObject {
public:

    mesh(shared_ptr<material> mat) : mat(mat) {}

    aabb bounding_box() const override { return bbox; }

    void addVertex(const  vec3 &v) { vertices.push_back(v); }
    void addTriangle(const ivec3 &tri) { triangleIndices.push_back(tri); }            
    //void addNormals  (const  vec3 &normals) { vertexNormals.push_back(normals); }

    const std::vector<vec3>&  getVertices() const { return vertices; }
    const std::vector<ivec3>& getIndices()  const { return triangleIndices; }
    //const std::vector<vec3>&  getNormals()  const { return vertexNormals; }

    bool intersect(const ray &r, interval ray_t, rayHitInfo &ray_hit_info) const override {
        
        float closestT = std::numeric_limits<double>::max();
        bool intersected = false;
 
        for (const auto& triIndex : triangleIndices) {
            const vec3& v0 = vertices[triIndex.x];
            const vec3& v1 = vertices[triIndex.y];
            const vec3& v2 = vertices[triIndex.z];

            vec3 edge0 = v1 - v0;
            vec3 edge1 = v2 - v0;

            vec3 dir = normalize(r.direction()); 
            vec3 p = cross(dir, edge1);
            double det = dot(edge0, p);
        
            if (det < 1e-8) 
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
            ray_hit_info.N = normalize(cross(edge0, edge1));
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
    std::vector<ivec3> triangleIndices;
    //std::vector<vec3> vertexNormals;
    shared_ptr<material> mat;
    aabb bbox;

};


#endif
