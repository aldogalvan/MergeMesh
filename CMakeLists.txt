cmake_minimum_required(VERSION 3.21)
project(MergeMesh)

set(CMAKE_CXX_STANDARD 14)
include_directories(libigl-main/include)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
add_subdirectory(tetgen-master)
add_subdirectory(libigl-main)

#LibIGL
#list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#option(LIBIGL_WITH_OPENGL   "Use OpenGL"    ON)
#option(LIBIGL_WITH_OPENGL_GLFW   "Use GLFW"    ON)
#include(libigl)

#include necessary libraries
#set(OpenGL_GL_PREFERENCE GLVND)
#find_package(OpenGL REQUIRED)
#find_package(glfw3 3.3 REQUIRED)

add_executable(MergeMesh main.cpp)

target_link_libraries(MergeMesh Eigen3::Eigen tet igl::core )

