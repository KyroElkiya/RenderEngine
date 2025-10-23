#pragma once

#include "../utils.h"
#include "../random.h"
#include <cmath>

using namespace std;

class vec3 {
public:
    double x, y, z;

    vec3() : x(0), y(0), z(0) {}
    vec3(double v) : x(v), y(v), z(v) {}
    vec3(double xx, double yy, double zz) : x(xx), y(yy), z(zz) {}

    vec3 operator = (const vec3& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
    
    double& operator[](int i) {
        switch(i) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("vec3 index out of range");
        }
    }

    const double& operator[](int i) const {
        switch(i) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("vec3 index out of range");
        }
    }

    // Negate
    vec3 operator - () const {
        return vec3(-x, -y, -z);
    }
    // Math operators vec3 and vec3
    vec3 operator + (const vec3 &v) const {
        return vec3 (x + v.x, y + v.y, z + v.z);
    }
    
    vec3 operator += (const vec3 &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    vec3 operator - (const vec3 &v) const {
        return vec3 (x - v.x, y - v.y, z - v.z);
    }

    vec3 operator -= (const vec3 &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    vec3 operator * (const vec3 &v) const {
        return vec3 (x * v.x, y * v.y, z * v.z);
    }

    vec3 operator *= (const vec3 &v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    vec3 operator / (const vec3 &v) const {
        return vec3 (x / v.x, y / v.y, z / v.z);
    }

    vec3 operator /= (const vec3 &v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }
    // Math operators vec3 and double
    vec3 operator + (const double f) const {
        return vec3 (x + f, y + f, z + f);
    }

    vec3 operator += (const double f) {
        x += f;
        y += f;
        z += f;
        return *this;
    }

    vec3 operator - (const double f) const {
        return vec3 (x - f, y - f, z - f);
    }

    vec3 operator -= (const double f) {
        x -= f;
        y -= f;
        z -= f;
        return *this;
    }
    vec3 operator * (const double f) const {
        return vec3 (x * f, y * f, z * f);
    }

    vec3 operator *= (const double f) {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }
    vec3 operator / (const double f) const {
        return vec3 (x / f, y / f, z / f);
    }
    
    vec3 operator /= (const double f) {
        x /= f;
        y /= f;
        z /= f;
        return *this;
    }
    
    // useful functions

    bool near_zero() const {
        auto s = EPSILON;
        return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
    }

    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

};

// Alias settings

using point3 = vec3;
using col3 = vec3;

// Vector util functions

vec3 operator + (double f,const vec3 v) {
    return vec3 (v.x + f, v.y + f, v.z + f);
}

vec3 operator - (double f, const vec3 v) {
    return vec3 (v.x - f, v.y - f, v.z - f);
}

vec3 operator * (double f, const vec3 v) {
    return vec3 (v.x * f, v.y * f, v.z * f);
}

vec3 operator / (double f, const vec3 v) {
    return vec3 (v.x / f, v.y / f, v.z / f); 
}

inline vec3 cross(vec3 &v1, vec3 &v2) {
    return vec3(
            v1.y * v2.z - v1.z * v2.y,
            -(v1.x * v2.z - v1.z * v2.x),
            v1.x * v2.y - v1.y * v2.x 
            );
}

inline double dot(const vec3 &v1, const vec3 &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline double length2(const vec3 &v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline double length(const vec3 &v) {
    return sqrt(length2(v));
}

inline vec3 normalize(const vec3 &v) {
    return v / length(v);
}

inline vec3 random_unit_vector() {
    while (true) {
        auto p = vec3::random(-1, 1);
        auto lensq = length2(p);
        if (1e-160 < lensq && lensq <= 1) return p / sqrt(lensq);
    }
}

inline vec3 random_on_hemisphere(const vec3 &normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (length2(p) < 1)
            return p;
    }
}

inline vec3 reflect(const vec3 v, const vec3 n) {
    return v - 2*dot(v, n) * n;
}

inline vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat) {
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - length2(r_out_perp))) * n;
    return r_out_perp + r_out_parallel;
}

