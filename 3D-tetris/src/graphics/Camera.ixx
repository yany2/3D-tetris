#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <stdio.h>

export module graphics:Camera;

using glm::vec3;
using glm::mat4;
using glm::radians;
using glm::normalize;
using glm::perspective;
using glm::lookAt;

namespace graphics {
	class Camera {
	private:
		float fov;
		float aspectRatio;

		mat4 projection;
		mat4 view;

		float yaw;
		float pitch;

		vec3 position;
		vec3 front;
		vec3 up;

		mat4 projectionView;

		void updateProjection(float nearPlane = 0.001f, float farPlane = 1000.0f) {
			projection = perspective(radians(fov), aspectRatio, 0.001f, 1000.0f);
			updateProjectionView();
		}

		void updateView() {
			view = lookAt(position, position + front, up);
			updateProjectionView();
		}

		void updateProjectionView() {
			projectionView = projection * view;
		}

		void updateFront() {
			if (pitch > 89.0f) {
				pitch = 89.0f;
			}
			if (pitch < -89.0f) {
				pitch = -89.0f;
			}
			front = normalize(vec3(
				cos(radians(yaw)) * cos(radians(pitch)),
				sin(radians(pitch)),
				sin(radians(yaw)) * cos(radians(pitch))
			));
			updateView();
		}

	public:
		Camera(float fov, float aspectRatio, vec3 position = vec3(-80.0f, 0.0f, 0.0f), float yaw = 0.0f, float pitch = 0.0f, vec3 up = vec3(0.0f, 1.0f, 0.0f)
		) : fov(fov), aspectRatio(aspectRatio), yaw(yaw), pitch(pitch), position(position), up(up) {
			updateFront();
			updateView();
			updateProjection();
			updateProjectionView();
		}

		mat4 getProjectionView() const {
			return projectionView;
		}

		void setFov(float fov) {
			this->fov = fov;
			updateProjection();
		}

		void setAspectRatio(float aspectRatio) {
			this->aspectRatio = aspectRatio;
			updateProjection();
		}

		void setYaw(float yaw) {
			this->yaw = yaw;
			updateFront();
		}

		void setPitch(float pitch) {
			this->pitch = pitch;
			updateFront();
		}

		void setPosition(vec3 position) {
			this->position = position;
			updateView();
		}

		void move(float x, float y, float z) {
			this->position += y * up + z * normalize(vec3(front.x, 0, front.z)) + x * normalize(glm::cross(up, front));
			updateView();
		}

		void turn(float yaw, float pitch) {
			this->yaw += yaw;
			this->pitch += pitch;
			updateFront();
		}
	};
}