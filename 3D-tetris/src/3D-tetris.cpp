#include <iostream>
#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace std;

import graphics;
import game;

int main()
{
	std::srand(std::time(NULL));
	float vertex[] =
	{
		-1.0f,	-1.0f,	-1.0f,		0.0f,	0.0f,	0.0f,	0.0f, 0.0f,
		-1.0f,	-1.0f,	-1.0f,		0.0f,	0.0f,	0.0f,	1.0f, 1.0f,
		1.0f,	-1.0f,	-1.0f,		0.0f,	0.0f,	0.0f,	0.0f, 1.0f,
		1.0f,	-1.0f,	-1.0f,		0.0f,	0.0f,	0.0f,	1.0f, 0.0f,
		-1.0f,	1.0f,	-1.0f,		0.0f,	0.0f,	0.0f,	0.0f, 1.0f,
		-1.0f,	1.0f,	-1.0f,		0.0f,	0.0f,	0.0f,	1.0f, 0.0f,
		1.0f,	1.0f,	-1.0f,		0.0f,	0.0f,	0.0f,	0.0f, 0.0f,
		1.0f,	1.0f,	-1.0f,		0.0f,	0.0f,	0.0f,	1.0f, 1.0f,

		-1.0f,	-1.0f,	1.0f,		0.0f,	0.0f,	0.0f,	0.0f, 1.0f,
		1.0f,	-1.0f,	1.0f,		0.0f,	0.0f,	0.0f,	1.0f, 1.0f,
		-1.0f,	1.0f,	1.0f,		0.0f,	0.0f,	0.0f,	0.0f, 0.0f,
		1.0f,	1.0f,	1.0f,		0.0f,	0.0f,	0.0f,	1.0f, 0.0f
	};

	unsigned char a[] = { 3, 3, 2 };

	unsigned int index[] =
	{
		0, 7, 3,
		0, 7, 4,
		9, 10, 8,
		9, 10, 11,
		7, 10, 11,
		7, 10, 5,
		3, 8, 9,
		3, 8, 0,
		2, 11, 6,
		2, 11, 9,
		1, 10, 8,
		1, 10, 4
	};

	Game* game = new Game();


	graphics::Application app
	(
		"OpenGL",
		"shaders/vertex.txt",
		"shaders/fragment.txt",
		vertex, 12 * 8,
		a, 3,
		index, 3 * 2 * 6,
		"resources/textures/texture.jpg",
		game
	);



	long time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	long seconds = time / 3000;

	while (!app.shouldClose())
	{
		if (app.drawFrame())
		{
			if (game->update() == 1) break;
		}

		glfwPollEvents();

		long newtime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		long newseconds = newtime / 3000;
		if (newseconds != seconds)
		{
			if (game->update() == 1) break;
			seconds = newseconds;
		}
	}
	std::cout << game->score << std::endl;
	delete game;

	return 0;
}


