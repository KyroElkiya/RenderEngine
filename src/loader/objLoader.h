#pragma once

#include "../core/math/vec3.h"
#include "../scene/mesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


class objLoader {
public:
    
    static bool load(const std::string &filepath, shared_ptr<mesh>& out_mesh) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error opening OBJ file:" << filepath << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "v") {
                // Getting Point positions.
                double x, y, z;
                iss >> x >> y >> z;
                out_mesh->addVertex(vec3(x, y, z));
            
            } else if (type == "vt") {
                // Getting UV positions.
                double u, v;
                iss >> u >> v;
                out_mesh->addUVs(vec2(u, v));
            
            } else if (type == "vn") {
                // Getting Normal vectors.
                double n1, n2, n3;
                iss >> n1 >> n2 >> n3;
                out_mesh->addNormals(vec3(n1, n2, n3));

            } else if (type == "f") {
                // Return triangle definitions.
                tri t;
                std::string vert_str;

                int i = 0;
                while (iss >> vert_str && i < 3) {
                    std::istringstream vss(vert_str);
                    std::string v_idx_str, vn_idx_str, vt_idx_str;

                    int v_idx = -1, vt_idx = -1, vn_idx = -1;

                    if (std::getline(vss, v_idx_str, '/')) v_idx = std::stoi(v_idx_str);
                    
                    if (std::getline(vss, vt_idx_str, '/')) {
                        if (!vt_idx_str.empty()) vt_idx = std::stoi(vt_idx_str);
                    }
                    
                    if (std::getline(vss, vn_idx_str, '/')) {
                        if (!vn_idx_str.empty()) vn_idx = std::stoi(vn_idx_str);
                    }

                    int vertex_count = out_mesh->getVertices().size();
                    int uv_count     = out_mesh->getUVs().size();
                    int normal_count = out_mesh->getNormals().size();

                    if (v_idx < 0) v_idx = vertex_count + v_idx;
                    else v_idx -= 1;
                    
                    if (vt_idx < 0) vt_idx = uv_count + vt_idx;
                    else vt_idx -= 1;
                    
                    if (vn_idx < 0) vn_idx = normal_count + vn_idx;
                    else vn_idx -= 1;

                    t.vert_indices[i] = v_idx;
                    t.uv_indices[i] = vt_idx;
                    t.norm_indices[i] = vn_idx;

                    ++i;
                }
                
                if (t.uv_indices   != ivec3(-1)) t.has_uvs = true;
                if (t.norm_indices != ivec3(-1)) t.has_vtx_norms = true;

                out_mesh->addTriangle(t);
            }
        }

        return true;
    }

    

};


