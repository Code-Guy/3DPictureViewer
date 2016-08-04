#ifndef _LIGHT_H
#define _LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct PointLight
{
	glm::vec3 position;
	glm::vec3 target;

	glm::mat4 getProjViewMatrix(float fov = 50.0f, float aspectRatio = 16.0f / 9.0f, float zNear = 0.1f, float zFar = 1000.0f)
	{
		glm::mat4 viewMatrix = glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projMatrix = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);

		return projMatrix * viewMatrix;
	}
};

#endif //_LIGHT_H