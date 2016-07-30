#include "camera.h"
#include "tool.h"

const float OrthoSize = 10.0f;
const float PitchClampOffset = 1.0f;
const float ZoomTimeInterval = 0.6f;
const float CameraZNear = 0.2f;
const float CameraZFar = 18.0f;

Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 worldUp, float aspectRatio,
	float fov, float nearDist, float farDist)
	: pos(pos), view(glm::normalize(target - pos)), worldUp(worldUp), aspectRatio(aspectRatio),
	fov(fov), nearDist(nearDist), farDist(farDist)
{
	yaw = glm::degrees(atanf(view.x / view.z)) + 180;
	pitch = glm::degrees(acosf(view.y));

	zoomAction.setCurveShape(LinearCurve);
	zoomAction.setTimeInterval(ZoomTimeInterval);

	updateCameraVectors();
}

void Camera::logic(float deltaTime)
{
	if (zoomAction.isRunning())
	{
		zoomAction.logic(deltaTime);
		pos.z = zoomAction.getValue();
	}

	Tool::clamp(pos.z, CameraZNear, CameraZFar);

	updateCameraVectors();

	O = glm::ortho(-OrthoSize, OrthoSize, -OrthoSize, OrthoSize, nearDist, farDist);
	V = glm::lookAt(pos, pos + view, up);
	P = glm::perspective(fov, aspectRatio, nearDist, farDist);

	VP = P * V;
}

void Camera::scroll(int numSteps)
{
	zoomAction.setBaseValue(pos.z);
	zoomAction.setIncrementValue(-numSteps * 0.4f);
	zoomAction.start();
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

void Camera::updateCameraVectors()
{
	Tool::clamp(pitch, PitchClampOffset, 180 - PitchClampOffset);

	glm::vec3 rawView;

	rawView.x = sin(glm::radians(yaw)) * sin(glm::radians(pitch));
	rawView.y = cos(glm::radians(pitch));
	rawView.z = cos(glm::radians(yaw)) * sin(glm::radians(pitch));

	view = glm::normalize(rawView);
	right = glm::normalize(glm::cross(view, worldUp));
	up = glm::normalize(glm::cross(right, view));
}