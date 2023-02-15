#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <stdio.h>

module graphics;

using glm::vec3;
using glm::mat4;
using glm::radians;
using glm::normalize;

using graphics::Camera;

Camera::Camera
(
	float fov, float aspectRatio, vec3 position, float yaw, float pitch, vec3 up)
	: fov(fov), aspectRatio(aspectRatio), yaw(yaw), pitch(pitch), position(position), up(up)
{
	updateFront();
	updateView();
	updateProjection();
	updateProjectionView();
}

glm::mat4 Camera::getProjectionView() const
{
	return projectionView;
}

void Camera::setFov(float fov)
{
	this->fov = fov;
	updateProjection();
}

void Camera::setAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	updateProjection();
}

void Camera::updateProjection(float nearPlane, float farPlane)
{
	projection = glm::perspective(radians(fov), aspectRatio, 0.001f, 1000.0f);
	updateProjectionView();
}

void Camera::updateView()
{
	view = glm::lookAt(position, position + front, up);
	updateProjectionView();
}

void Camera::updateProjectionView()
{
	projectionView = projection * view;
}

void Camera::setYaw(float yaw)
{
	this->yaw = yaw;
	updateFront();
}

void Camera::setPitch(float pitch)
{
	this->pitch = pitch;
	updateFront();
}

void Camera::updateFront()
{
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	front = normalize(vec3
	(
		cos(radians(yaw)) * cos(glm::radians(pitch)),
		sin(radians(pitch)),
		sin(radians(yaw)) * cos(radians(pitch))
	));
	updateView();
}

void Camera::setPosition(vec3 position)
{
	this->position = position;
	updateView();
}

void Camera::move(float x, float y, float z)
{
	this->position += y * up + z * normalize(vec3(front.x, 0, front.z)) + x * normalize(glm::cross(up, front));
	updateView();
}

void Camera::turn(float yaw, float pitch)
{
	this->yaw += yaw;
	this->pitch += pitch;
	updateFront();
}