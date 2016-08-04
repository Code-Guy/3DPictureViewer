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
	getCameraVectors();
	getMatrixs();
	getFrustum();
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

void Camera::getCameraVectors()
{
	yaw = glm::degrees(atanf(view.x / view.z)) + 180;
	pitch = glm::degrees(acosf(view.y));

	Tool::clamp(pitch, PitchClampOffset, 180 - PitchClampOffset);

	glm::vec3 rawView;

	rawView.x = sin(glm::radians(yaw)) * sin(glm::radians(pitch));
	rawView.y = cos(glm::radians(pitch));
	rawView.z = cos(glm::radians(yaw)) * sin(glm::radians(pitch));

	view = glm::normalize(rawView);
	right = glm::normalize(glm::cross(view, worldUp));
	up = glm::normalize(glm::cross(right, view));
}

void Camera::getMatrixs()
{
	O = glm::ortho(-OrthoSize, OrthoSize, -OrthoSize, OrthoSize, nearDist, farDist);
	V = glm::lookAt(pos, pos + view, up);
	P = glm::perspective(fov, aspectRatio, nearDist, farDist);

	VP = P * V;
}

void Camera::getFrustum()
{
	float t;

	frustum[0][0] = VP[0][3] + VP[0][0];
	frustum[0][1] = VP[1][3] + VP[1][0];
	frustum[0][2] = VP[2][3] + VP[2][0];
	frustum[0][3] = VP[3][3] + VP[3][0];

	t = sqrt(frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2]);
	frustum[0][0] /= t;
	frustum[0][1] /= t;
	frustum[0][2] /= t;
	frustum[0][3] /= t;

	frustum[1][0] = VP[0][3] - VP[0][0];
	frustum[1][1] = VP[1][3] - VP[1][0];
	frustum[1][2] = VP[2][3] - VP[2][0];
	frustum[1][3] = VP[3][3] - VP[3][0];

	t = sqrt(frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2]);
	frustum[1][0] /= t;
	frustum[1][1] /= t;
	frustum[1][2] /= t;
	frustum[1][3] /= t;

	frustum[2][0] = VP[0][3] + VP[0][1];
	frustum[2][1] = VP[1][3] + VP[1][1];
	frustum[2][2] = VP[2][3] + VP[2][1];
	frustum[2][3] = VP[3][3] + VP[3][1];

	t = sqrt(frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2]);
	frustum[2][0] /= t;
	frustum[2][1] /= t;
	frustum[2][2] /= t;
	frustum[2][3] /= t;

	frustum[3][0] = VP[0][3] - VP[0][1];
	frustum[3][1] = VP[1][3] - VP[1][1];
	frustum[3][2] = VP[2][3] - VP[2][1];
	frustum[3][3] = VP[3][3] - VP[3][1];

	t = sqrt(frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2]);
	frustum[3][0] /= t;
	frustum[3][1] /= t;
	frustum[3][2] /= t;
	frustum[3][3] /= t;

	frustum[4][0] = VP[0][3] + VP[0][2];
	frustum[4][1] = VP[1][3] + VP[1][2];
	frustum[4][2] = VP[2][3] + VP[2][2];
	frustum[4][3] = VP[3][3] + VP[3][2];

	t = sqrt(frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2]);
	frustum[4][0] /= t;
	frustum[4][1] /= t;
	frustum[4][2] /= t;
	frustum[4][3] /= t;

	frustum[5][0] = VP[0][3] - VP[0][2];
	frustum[5][1] = VP[1][3] - VP[1][2];
	frustum[5][2] = VP[2][3] - VP[2][2];
	frustum[5][3] = VP[3][3] - VP[3][2];

	t = sqrt(frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2]);
	frustum[5][0] /= t;
	frustum[5][1] /= t;
	frustum[5][2] /= t;
	frustum[5][3] /= t;
}

bool Camera::isVertexInFrustum(glm::vec3 v)
{
	for (int i = 0; i < 6; i++)
	{
		if (frustum[i][0] * v.x + frustum[i][1] * v.y + frustum[i][2] * v.z + frustum[i][3] <= 0)
		{
			return false;
		}
	}
	return true;
}