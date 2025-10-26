#pragma once

#include "../core/math/ivec3.h"
#include "../core/math/vec3.h"
#include "../core/math/vec2.h"

struct tri {
    ivec3 vert_indices = ivec3(-1);
    ivec3 uv_indices = ivec3(-1);
    ivec3 norm_indices = ivec3(-1);
    bool has_uvs = false;
    bool has_vtx_norms = false;
};


