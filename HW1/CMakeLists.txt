cmake_minimum_required(VERSION 3.24)
project(Graph1)

set(CMAKE_CXX_STANDARD 11)

file(COPY ${CMAKE_SOURCE_DIR}/inputs DESTINATION ${CMAKE_BINARY_DIR})

add_executable(Graph1 raytracer.cpp Ray3D.cpp Ray3D.h Vec3D.cpp Vec3D.h helpers.cpp helpers.h parser.cpp parser.h tinyxml2.cpp tinyxml2.h ppm.cpp ppm.h types.h BVH.cpp BVH.h)
