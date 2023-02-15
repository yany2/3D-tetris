add_executable (3D-tetris
	"src/3D-tetris.cpp"
	"src/game/game.ixx" "src/game/game.cpp"
	"src/graphics/graphics.ixx"
		"src/graphics/application.cpp"
		"src/graphics/buffer.cpp"
		"src/graphics/shader.cpp"
		"src/graphics/camera.cpp"
		"src/graphics/texture.cpp"
)

set_property(TARGET 3D-tetris PROPERTY CXX_STANDARD 20)

add_dependencies(3D-tetris glfw glad_gl_core_46)

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
	glad_gl_core_46
)