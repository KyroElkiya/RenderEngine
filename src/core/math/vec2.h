#pragma once

using namespace std;

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


