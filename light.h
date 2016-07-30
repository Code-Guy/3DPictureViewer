#ifndef _LIGHT_H
#define _LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct DirectionLight
{
	glm::vec3 position;
	glm::vec3 target;

	glm::mat4 getOrthoViewMatrix(float left = -10.0f, float right = 10.0f, float bottom = -10.0f, float top = 10.0f, 
		float zNear = -10.0f, float zFar = 10.0f)
	{
		glm::mat4 viewMatrix = glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 orthoMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);

		return orthoMatrix * viewMatrix;
	}
};

#endif //_LIGHT_H