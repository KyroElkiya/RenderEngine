cmake_minimum_required(VERSION 3.15)
project(Raytracer LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenEXR REQUIRED)

add_executable(Raytracer
    src/main.cpp
)

target_link_libraries(Raytracer PRIVATE OpenEXR::OpenEXR)
