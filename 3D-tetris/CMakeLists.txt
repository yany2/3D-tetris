add_executable (3D-tetris "src/3D-tetris.cpp" )

set_property(TARGET 3D-tetris PROPERTY CXX_STANDARD 20)

add_dependencies(3D-tetris glfw)

target_include_directories(3D-tetris PRIVATE
	${CMAKE_SOURCE_DIR}/extern/glfw/include/GLFW
	${CMAKE_SOURCE_DIR}/extern/glm/glm
	${CMAKE_SOURCE_DIR}/extern/stb
)

target_link_directories(3D-tetris PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}/extern/glfw/src
)

target_link_libraries(3D-tetris PRIVATE
	glfw
)