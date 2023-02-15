#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

export module graphics;

import game;

export namespace graphics
{
	export class Shader
	{
	private:
		unsigned int program;
	public:
		Shader(const char* vertexShaderSourceLocation, const char* fragmentShaderSourceLocation);
		~Shader();
		void use() const;
		void setUniform(const char* name, int value) const;
		void setUniform(const char* name, glm::vec4 value) const;
		void setTransform(glm::mat4* transform) const;
	};

	export class Buffer
	{
	private:
		unsigned int vao;
		unsigned int vbo;
		unsigned int ebo;
		unsigned int size;
		unsigned int indexc;
	public:
		Buffer
		(
			float* vertex,
			unsigned int vertexc,
			unsigned char* attrib,
			unsigned int attrc,
			unsigned int* index,
			unsigned int indexc
		);
		~Buffer();
		void draw(unsigned int drawMode = GL_TRIANGLES) const;
	};

	export class Texture
	{
	private:
		unsigned int texture;
	public:
		Texture(const char* textureLocation);
		~Texture();
		void use(Shader* shader) const;
	};

	class Camera
	{
	private:
		float fov;
		float aspectRatio;

		glm::mat4 projection;
		glm::mat4 view;

		float yaw;
		float pitch;

		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 up;

		glm::mat4 projectionView;

		void updateProjection(float nearPlane = 0.001f, float farPlane = 1000.0f);
		void updateView();
		void updateProjectionView();
		void updateFront();
	public:
		Camera
		(
			float fov,
			float aspectRatio,
			glm::vec3 position = glm::vec3(-80.0f, 0.0f, 0.0f),
			float yaw = 0.0f,
			float pitch = 0.0f,
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)
		);

		glm::mat4 getProjectionView() const;
		void setFov(float fov);
		void setAspectRatio(float aspectRatio);
		void setYaw(float yaw);
		void setPitch(float pitch);
		void setPosition(glm::vec3 position);
		void move(float c, float y, float z);
		void turn(float yaw, float pitch);
	};

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
		static void resizeCallback(GLFWwindow* window, int width, int height);
		static void mouseMoveCallback(GLFWwindow* window, double x, double y);
	public:
		Application
		(
			const char* title,
			const char* vertexShaderSourceLocation,
			const char* fragmentShaderSourceLocation,
			float* vertex,
			unsigned int vertexc,
			unsigned char* attrib,
			unsigned int attrc,
			unsigned int* index,
			unsigned int indexc,
			const char* textureLocation,
			Game* game
		);
		~Application();
		bool drawFrame();
		bool shouldClose() const;
	};
}