cmake_minimum_required(VERSION 3.10)

project(raytracer)

set(CMAKE_CXX_STANDARD 11)

file(COPY ${CMAKE_SOURCE_DIR}/inputs DESTINATION ${CMAKE_BINARY_DIR})

# Define the source files for the individual targets
set(SOURCE_FILES_PARSER parser.cpp parser.h)
set(SOURCE_FILES_PPM ppm.cpp ppm.h)
set(SOURCE_FILES_TINYXML2 tinyxml2.cpp tinyxml2.h)
set(SOURCE_FILES_RAYTRACER raytracer.cpp)
set(SOURCE_FILES_MYVEC3D Vec3D.cpp Vec3D.h)
set(SOURCE_FILES_HELPER helpers.cpp helpers.h)
set(SOURCE_FILES_RAY3D Ray3D.cpp Ray3D.h)


# Create individual library targets for each source file
add_library(parser_lib ${SOURCE_FILES_PARSER})
add_library(ppm_lib ${SOURCE_FILES_PPM})
add_library(tinyxml2_lib ${SOURCE_FILES_TINYXML2})
add_library(myvec3d_lib ${SOURCE_FILES_MYVEC3D})
add_library(helper_lib ${SOURCE_FILES_HELPER})
add_library(ray_lib ${SOURCE_FILES_RAY3D})
# Link the libraries to raytracer
add_executable(raytracer ${SOURCE_FILES_RAYTRACER})

# Specify the dependencies
target_link_libraries(raytracer PRIVATE myvec3d_lib parser_lib ppm_lib tinyxml2_lib helper_lib ray_lib)

# Set optimization options for raytracer
target_compile_options(raytracer PRIVATE)

