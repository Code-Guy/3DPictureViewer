#ifndef _CAMERA_H
#define _CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

class Camera
{
public:
	Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 worldUp, float aspectRatio,
		float fov = 45.0f, float nearDist = 0.1f, float farDist = 1000.0f);

	glm::mat4 getOrthoMatrix();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjMatrix();
	glm::mat4 getViewProjMatrix();

	void castRay(glm::ivec4 viewport, int x, int y, glm::vec3 &p, glm::vec3 &d);

private:
	glm::mat4 O;
	glm::mat4 V;
	glm::mat4 P;
	glm::mat4 VP;

	glm::vec3 pos;
	glm::vec3 view;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float fov;
	float aspectRatio;
	float nearDist;
	float farDist;

	float yaw;
	float pitch;

	float frustum[6][4];

	void getCameraVectors();
	void getMatrixs();
	void getFrustum();

	bool isVertexInFrustum(glm::vec3 v);
};

#endif //_CAMERA_H