#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

export module graphics:Application;

import :Shader;
import :Buffer;
import :Camera;
import :Texture;

import game;

using std::cout;

using glm::mat4;
using glm::vec3;
using glm::vec4;
using glm::radians;
using glm::translate;

using game::Game;

bool close = false;

namespace graphics {
	export class Application
	{
	private:
		GLFWwindow* window;
		Shader* shader;
		Buffer* buffer;
		Shader* shader2;
		Buffer* buffer2;
		Texture* texture;
		Camera* camera;
		Game* game;
		float cameraRotation;
		float cameraHeight;
		float cameraDistance;
		static Camera* statCam;

		static void resizeCallback(GLFWwindow* window, int width, int height) {
			statCam->setAspectRatio((float)width / height);
			glViewport(0, 0, width, height);
		}

		static void mouseMoveCallback(GLFWwindow* window, double x, double y) {
			static double prevX = x;
			static double prevY = y;
			statCam->turn((x - prevX) * 0.2, (prevY - y) * 0.2);
			prevX = x;
			prevY = y;
		}

		void handleInput() {
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
				cameraHeight += 0.5;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
				cameraHeight -= 0.5;
			}
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				cameraDistance -= 0.5;
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				cameraDistance += 0.5;
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				cameraRotation += 2;
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				cameraRotation -= 2;
			}
			camera->setPosition(vec3(
				cameraDistance * (-cos(radians(cameraRotation))) - 10,
				cameraHeight,
				cameraDistance * (-sin(radians(cameraRotation))) - 10
			));

			camera->setYaw(cameraRotation);

			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}

			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}

			bool flag = true;
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
				game->moveFalling(-1, true);
				game->moved = true;
				flag = false;
			}
			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
				game->moveFalling(1, true);
				game->moved = true;
				flag = false;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
				if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
					game->rotateFalling(1, false);
				else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
					game->rotateFalling(2, false);
				else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
					game->rotateFalling(3, false);
				else
					game->moveFalling(1, false);
				game->moved = true;
				flag = false;
			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
				if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
					game->rotateFalling(1, true);
				else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
					game->rotateFalling(2, true);
				else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
					game->rotateFalling(3, true);
				else
					game->moveFalling(-1, false);
				game->moved = true;
				flag = false;
			}

			if (flag) {
				game->moved = false;
			}


			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				close = true;
			}
		}

		void renderFrame() {
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			shader->use();
			texture->use(shader);
			for (int i2 = 0; i2 < 30; i2++) {
				int colorCode = i2 + 7;
				shader->setUniform("color", vec4(colorCode % 2, (colorCode / 2) % 2, (colorCode / 4) % 2, 1.0f));
				for (int i1 = 0; i1 < 10; i1++) {
					for (int i3 = 0; i3 < 10; i3++) {
						if (game->gameGrid[i1][i2][i3])
						{
							mat4 model2 = glm::translate(game->getModelMat(), -vec3((float)i1 * 2, (float)i2 * 2, (float)i3 * 2));

							model2 = glm::scale(model2, vec3(1.0, 1.0, 1.0));
							//model2 = glm::rotate(model2, glm::radians(20.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));


							//model2 = glm::rotate(model2, glm::radians(1.0f), glm::vec3(dist(mt), dist(mt), dist(mt)));

							mat4 transform = camera->getProjectionView() * model2;
							shader->setTransform(&transform);
							buffer->draw();
						}
					}
				}
			}

			auto coords = game->falling->getCoords();
			for (auto coord : coords) {
				mat4 model2 = translate(game->getModelMat(), coord.getTranslateVec3());
				model2 = glm::scale(model2, vec3(1.0, 1.0, 1.0));
				mat4 transform = camera->getProjectionView() * model2;
				shader->setTransform(&transform);
				buffer->draw();
			}


			int i = 0;
			bool flag2 = true;
			while (flag2) {
				i++;
				for (auto coord : coords) {
					int x = coord.x;
					int y = coord.y + i;
					int z = coord.z;

					if (y > 29 || game->gameGrid[x][y][z]) {
						flag2 = false;
					}
				}
			}

			i--;
			for (auto coord : coords) {
				mat4 model2 = translate(game->getModelMat(), coord.getTranslateVec3() + vec3(0, -2 * i, 0));
				model2 = glm::scale(model2, vec3(1.0, 1.0, 1.0));
				mat4 transform = camera->getProjectionView() * model2;
				shader->setTransform(&transform);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				buffer->draw();
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}


			shader2->use();
			mat4 scale = glm::scale(game->getModelMat(), vec3(2, 2, 2));
			mat4 transform = translate(scale, vec3(-9.5, -29.5, -9.5));
			transform = camera->getProjectionView() * transform;
			shader2->setTransform(&transform);
			buffer2->draw(GL_LINES);

			glfwSwapBuffers(window);
			if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
				if (!game->pushed) {
					while (!game->update());
					game->pushed = true;
				}
			}
			else {
				game->pushed = false;
			}
		}

	public:
		Application(
			const char* title, const char* textureLocation, const char* vertexShaderSourceLocation, const char* fragmentShaderSourceLocation,
			float* vertex, unsigned int vertexc, unsigned char* attrib, unsigned int attrc, unsigned int* index, unsigned int indexc
		) {
			this->game = new Game();

			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			int width = 800;
			int height = 600;
			GLFWmonitor* monitor = nullptr;

			//fullscreen
			/*
			monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			width = mode->width;
			height = mode->height;
			*/

			window = glfwCreateWindow(width, height, title, monitor, nullptr);
			if (!window) {
				cout << "Failed to create GLFW window\n";
			}

			glfwMakeContextCurrent(window);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				cout << "Failed to initialize GLAD\n";
			}

			glfwSetFramebufferSizeCallback(window, resizeCallback);
			glfwSetCursorPosCallback(window, mouseMoveCallback);


			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			shader = new Shader(vertexShaderSourceLocation, fragmentShaderSourceLocation);
			buffer = new Buffer(vertex, vertexc, attrib, attrc, index, indexc);


			{
				float* vertex2 = new float[44 * 3];
				for (int i = 0; i < 11; i++) {
					vertex2[i * 12 + 0] = i;
					vertex2[i * 12 + 1] = 0;
					vertex2[i * 12 + 2] = 0;

					vertex2[i * 12 + 3] = i;
					vertex2[i * 12 + 4] = 0;
					vertex2[i * 12 + 5] = 10;

					vertex2[i * 12 + 6] = 0;
					vertex2[i * 12 + 7] = 0;
					vertex2[i * 12 + 8] = i;

					vertex2[i * 12 + 9] = 10;
					vertex2[i * 12 + 10] = 0;
					vertex2[i * 12 + 11] = i;
				}
				unsigned int* index2 = new unsigned int[44];
				for (int i1 = 0; i1 < 44; i1++) {
					index2[i1] = i1;
				}
				unsigned char attrib[] = { 3 };
				buffer2 = new Buffer(vertex2, 44 * 3, attrib, 1, index2, 44);
				shader2 = new Shader("shaders/vertex2.txt", "shaders/fragment2.txt");
			}

			texture = new Texture(textureLocation);
			statCam = camera = new Camera(60.0f, (float)width / height);
			camera->setPitch(-40);
			cameraHeight = 30;
			cameraRotation = 0;
			cameraDistance = 50;
			//glfwSwapInterval(0);
			glEnable(GL_DEPTH_TEST);
		}

		~Application() {
			delete game;
			delete camera;
			delete buffer;
			delete shader;
			delete texture;
			glfwTerminate();
		}

		void drawFrame() {
			handleInput();
			renderFrame();
		}

		bool shouldClose() const {
			return glfwWindowShouldClose(window) || close;
		}

		int updateGame() {
			return game->update();
		}
	};

	Camera* Application::statCam;
}
