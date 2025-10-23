#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <functional>
#include <cmath>
#include <memory>
#include <limits>


template<typename Func>
void timeFunction(const std::string& name, Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();

    // Execute the function with arguments
    std::invoke(std::forward<Func>(func));

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::clog << "[TIMER] \"" << name << "\" took " 
              << duration.count() << " seconds.\n";
}

using std::make_shared;
using std::shared_ptr;

// Setting up constant values

const double EPSILON = 1e-8;
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Util functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}


