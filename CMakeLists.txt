cmake_minimum_required(VERSION 3.22)
project(fractals)

set(CMAKE_CXX_STANDARD 23)

find_package(GLEW REQUIRED)
find_package(OpenGL)

add_executable(fractals program/shader.cpp program/shader.h main.cpp )

target_link_libraries(fractals OpenGL::OpenGL)
target_link_libraries(fractals GLEW::GLEW)
target_link_libraries(fractals glfw)
