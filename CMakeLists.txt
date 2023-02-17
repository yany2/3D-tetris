cmake_minimum_required (VERSION 3.12)
set(CMAKE_TOOLCHAIN_FILE extern/vcpkg/scripts/buildsystems/vcpkg.cmake)

project ("3D-tetris")

add_subdirectory("extern/glad/cmake")
glad_add_library(glad_gl_core_46 REPRODUCIBLE API gl:core=4.6)

add_subdirectory ("3D-tetris")
