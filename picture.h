#ifndef _PICTURE_H
#define _PICTURE_H

#include "texture.h"
#include "pictureshader.h"
#include "shadowmapshader.h"

#include <glm/glm.hpp>
   
#define POS_VB 0
#define TEXCOORD_VB 1
#define INDEX_VB 2 

#define POS_LOCATION 0
#define TEXCOORD_LOCATION 1

class Picture
{
public:
	Picture(std::string fileName);
	~Picture();

	void setSize(float size);
	void setPosition(glm::vec3 pos);
	void setRotation(glm::vec3 rot);

	void addSize(float size);
	void addPosition(glm::vec3 pos);
	void addRotation(glm::vec3 rot);

	void renderPass(PictureShader *pictureShader);
	void shadowMapPass(ShadowMapShader *shadowMapShader);

private:
	Texture *texture;

	GLuint vao;
	GLuint buffers[3];

	glm::vec3 pos[4];
	glm::vec2 texCoord[4];
	int indices[6];

	float size;
	glm::vec3 translation;
	glm::vec3 rotation;

	glm::mat4 getModelMatrix(glm::vec3 t, glm::vec3 r, float s);
};

#endif //_PICTURE_H