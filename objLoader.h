#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "vec3.h"
#include "components/mesh.h"
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
                double x, y, z;
                iss >> x >> y >> z;
                vec3 vert = vec3(x, y, z);
                out_mesh->addVertex(vert);
            } else if (type == "f") {
                int i0, i1, i2;
                iss >> i0 >> i1 >> i2;
                ivec3 pts = ivec3(i0 - 1, i1 - 1, i2 - 1);
                out_mesh->addTriangle(pts);
            }
        }

        return true;
    }

};


#endif
