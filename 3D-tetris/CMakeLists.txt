# CMakeList.txt : CMake project for 3D-tetris, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (3D-tetris "3D-tetris.cpp" "3D-tetris.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET 3D-tetris PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
