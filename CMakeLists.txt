cmake_minimum_required(VERSION 3.22)
project(fractals)

set(CMAKE_CXX_STANDARD 23)

add_executable(fractals program/shader.cpp program/shader.h main.cpp )

target_link_libraries(fractals opengl32.lib)
target_link_libraries(fractals glew32.lib)
target_link_libraries(fractals glfw3.lib)
