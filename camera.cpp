#include "camera.h"

const float OrthoSize = 10.0f;
const float PitchClampOffset = 1.0f;

Camera *Camera::camera = NULL;

Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 worldUp, float aspectRatio,
	float fov, float nearDist, float farDist)
	: pos(pos), view(glm::normalize(target - pos)), worldUp(worldUp), aspectRatio(aspectRatio),
	fov(fov), nearDist(nearDist), farDist(farDist)
{
	yaw = glm::degrees(atanf(view.x / view.z)) + 180;
	pitch = glm::degrees(acosf(view.y));

	updateCameraVectors();
}

void Camera::logic(float deltaTime)
{
	updateCameraVectors();

	O = glm::ortho(-OrthoSize, OrthoSize, -OrthoSize, OrthoSize, nearDist, farDist);
	V = glm::lookAt(pos, pos + view, up);
	P = glm::perspective(fov, aspectRatio, nearDist, farDist);

	VP = P * V;
}

glm::mat4 Camera::getOrthoMatrix()
{
	return O;
}

glm::mat4 Camera::getViewMatrix()
{
	return V;
}

glm::mat4 Camera::getProjMatrix()
{
	return P;
}

glm::mat4 Camera::getViewProjMatrix()
{
	return VP;
}

void Camera::setCamera(glm::vec3 pos, glm::vec3 target, glm::vec3 worldUp, float aspectRatio,
	float fov, float nearDist, float farDist)
{
	camera = new Camera(pos, target, worldUp, aspectRatio, fov, nearDist, farDist);
}

Camera *Camera::getCamera()
{
	return camera;
}

void Camera::destoryCamera()
{
	delete camera;
}

void Camera::updateCameraVectors()
{
	if (pitch > 180 - PitchClampOffset)
		pitch = 180 - PitchClampOffset;
	if (pitch < PitchClampOffset)
		pitch = PitchClampOffset;

	glm::vec3 rawView;

	rawView.x = sin(glm::radians(yaw)) * sin(glm::radians(pitch));
	rawView.y = cos(glm::radians(pitch));
	rawView.z = cos(glm::radians(yaw)) * sin(glm::radians(pitch));

	view = glm::normalize(rawView);
	right = glm::normalize(glm::cross(view, worldUp));
	up = glm::normalize(glm::cross(right, view));
}