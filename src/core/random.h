#pragma once

#include <random>
#include <cstdlib>

inline double random_double() {
    // Returns a random double between 0 and 1.
    //return std::rand() / (RAND_MAX + 1.0);
    
    thread_local static std::mt19937 generator(std::random_device{}());
    thread_local static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // Returns a random double between given min and max.
    //return min + (max-min) * random_double();

    thread_local static std::mt19937 generator(std::random_device{}());

    std::uniform_real_distribution<double> dist(min, max);
    return dist(generator);
}

inline int random_int(int min, int max) {
    //return int(random_double(min, max + 1));

    thread_local static std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(generator);
}


