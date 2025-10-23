#pragma once

using namespace std;

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


