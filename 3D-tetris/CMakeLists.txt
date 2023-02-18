find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)
find_package(stb REQUIRED)

add_executable (3D-tetris
	"src/3D-tetris.cpp"
	"src/game/game.ixx"
	"src/graphics/Buffer.ixx"
	"src/graphics/Shader.ixx"
	"src/graphics/Camera.ixx"
	"src/graphics/Texture.ixx"
	"src/graphics/Application.ixx"
	"src/graphics/graphics.ixx"
)

set_property(TARGET 3D-tetris PROPERTY CXX_STANDARD 20)


target_include_directories(3D-tetris PRIVATE
	${CMAKE_SOURCE_DIR}/extern/glm/glm
	${CMAKE_SOURCE_DIR}/extern/stb
)

target_link_libraries(3D-tetris PRIVATE
	glfw
	glad::glad
)


add_custom_target(shaders DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/shaders)
add_custom_command(
	TARGET shaders
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
)

add_custom_target(resources DEPENDS $CMAKE_CURRENT_SOURCE_DIR/resources)
add_custom_command(
	TARGET resources
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
)

add_dependencies(3D-tetris shaders resources)