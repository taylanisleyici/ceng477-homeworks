cmake_minimum_required(VERSION 3.24)
project(HW2)

set(CMAKE_CXX_STANDARD 11)

file(COPY ${CMAKE_SOURCE_DIR}/inputs_outputs DESTINATION ${CMAKE_BINARY_DIR})

add_executable(HW2 Camera.cpp Camera.h Color.cpp Color.h Helpers.cpp Helpers.h Main.cpp Matrix4.cpp Matrix4.h Mesh.cpp Mesh.h Rotation.cpp Rotation.h Scene.cpp Scene.h Scaling.cpp Scaling.h tinyxml2.cpp tinyxml2.h Translation.cpp Translation.h Vec3.cpp Vec3.h Vec4.cpp Vec4.h Transformation.cpp Transformation.h)
